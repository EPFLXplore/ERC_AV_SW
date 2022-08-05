/**
 * @file STMUARTDriver.cpp
 * @author Mounir Raki (mounir.raki@epfl.ch)
 * @brief
 * @version 1.0
 * @date 2022-03-29
 *
 * UART Driver for the STM32 boards
 *
 */

#include "STMUARTDriver.h"
#include "Lang/Operators.h"

#include <cstring>
#include <inttypes.h>

#ifdef BUILD_WITH_STMUART


/**
 * @brief Construct a new STMUARTDriver::STMUARTDriver object
 *
 * @param huart the UART port to initialize
 */
STMUARTDriver::STMUARTDriver(UART_HandleTypeDef* huart): huart(huart), recvThread(nullptr)
{
    this->buffer = (uint8_t*) pvPortMalloc(UART_BUFFER_SIZE);
    if(buffer == nullptr){
//        console.printf("[RoCo] [STMUARTDriverInit] Unable to allocate DMA buffer for MCU#%" PRIu32 "\r\n", get_sender_id(huart));
    }

    // Initialize the Receive_DMA to initially receive the header of the first packet (on 1 byte -> size from 0 to 255)
    if(HAL_UART_Receive_DMA(huart, this->buffer, UART_BUFFER_SIZE) != HAL_OK)
    {
//        console.printf("[RoCo] [STMUARTDriverInit] Unable to initialize UART in receive mode for MCU#%" PRIu32 "\r\n", get_sender_id(huart));
    }
}

STMUARTDriver::STMUARTDriver(USART_HandleTypeDef* husart): husart(husart), recvThread(nullptr)
{
    this->buffer = (uint8_t*) pvPortMalloc(UART_BUFFER_SIZE);
    if(buffer == nullptr){
//        console.printf("[RoCo] [STMUARTDriverInit] Unable to allocate DMA buffer for MCU#%" PRIu32 "\r\n", get_sender_id(huart));
    }

    // Initialize the Receive_DMA to initially receive the header of the first packet (on 1 byte -> size from 0 to 255)
    if(HAL_USART_Receive_DMA(husart, this->buffer, UART_BUFFER_SIZE) != HAL_OK)
    {
//        console.printf("[RoCo] [STMUARTDriverInit] Unable to initialize UART in receive mode for MCU#%" PRIu32 "\r\n", get_sender_id(huart));
    }
}

void STMUARTDriver::receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver)
{
    this->receiver_func = receiver;
}

void STMUARTDriver::transmit(uint8_t* buffer, uint32_t length)
{
    if(HAL_UART_Transmit(huart, buffer, length, HAL_MAX_DELAY) != HAL_OK){
//        console.printf("[RoCo] [STMUARTDriverTransmit] Transmission failed for MCU#%" PRIu32 "\r\n", get_sender_id(huart));
    }
}


/**
 * @brief Destroy the STMUARTDriver::STMUARTDriver object
 *
 */
STMUARTDriver::~STMUARTDriver() {
    vPortFree(buffer);
    delete this->recvThread;
}

/**
 * @brief Getter to the reference of the buffer
 *
 * @return uint8_t* the reference to the buffer
 */
uint8_t* STMUARTDriver::getBuffer()
{
	return this->buffer;
}


/**
 * @brief Function handling the call to the user-defined callback routine
 *
 * @param sender_id the ID of the MCU
 * @param buffer the buffer to provide to the user-defined callback function
 * @param length the size of the data in the buffer to provide
 */
void STMUARTDriver::receiveUART(uint8_t sender_id, uint8_t* buffer, uint32_t length)
{
	this->receiver_func(sender_id, buffer, length);
}


/* STM32 UART DMA CALLBACK */
void STMUARTDriver::HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    // Call the receive function to handle received data properly
	uint8_t sender_id = this->get_sender_id(huart);
	this->receiveUART(sender_id, this->getBuffer(), UART_BUFFER_SIZE);
    if(HAL_UART_Receive_DMA(huart, this->getBuffer(), UART_BUFFER_SIZE) != HAL_OK){
//        console.printf("[RoCo] [STMUARTDriverRxCallback] Unable to initialize UART in receive mode for MCU#%" PRIu32 "\r\n", sender_id);
    }
}

void STMUARTDriver::HAL_USART_RxCpltCallback(USART_HandleTypeDef* husart)
{
    // Call the receive function to handle received data properly
	uint8_t sender_id = this->get_sender_id(husart);
	this->receiveUART(sender_id, this->getBuffer(), UART_BUFFER_SIZE);
    if(HAL_USART_Receive_DMA(husart, this->getBuffer(), UART_BUFFER_SIZE) != HAL_OK){
//        console.printf("[RoCo] [STMUARTDriverRxCallback] Unable to initialize UART in receive mode for MCU#%" PRIu32 "\r\n", sender_id);
    }
}

/**
 * @brief Get the sender id from the USART port ID
 *
 * @param huart the USART port to get
 * @return uint8_t the sender_id
 */
uint8_t STMUARTDriver::get_sender_id(UART_HandleTypeDef* huart)
{
    for(int i = 0; i < NB_UART_PORTS; ++i){
        if(this->mapper[i] == huart->Instance){
            return i+1;
        }
    }
    return 0;
}


uint8_t STMUARTDriver::get_sender_id(USART_HandleTypeDef* husart)
{
    for(int i = 0; i < NB_UART_PORTS; ++i){
        if(this->mapper[i] == husart->Instance){
            return i+1;
        }
    }
    return 0;
}

#endif /* BUILD_WITH_STMUART */
