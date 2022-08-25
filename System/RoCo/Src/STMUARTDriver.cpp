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
#include "Debug/Debug.h"
#include "Lang/Operators.h"
#include "usart.h"
#include <cstring>
#include <inttypes.h>

//#ifdef BUILD_WITH_STMUART


/**
 * @brief Construct a new STMUARTDriver::STMUARTDriver object
 *
 * @param huart the UART port to initialize
 */



STMUARTDriver::STMUARTDriver(UART_HandleTypeDef* huart): Thread("STMUARTDriver", osPriorityHigh), huart(huart), last_dma_index(0) {

	this->buffer = (uint8_t*) pvPortMalloc(UART_BUFFER_SIZE);

    if(buffer == nullptr){
//        console.printf("[RoCo] [STMUARTDriverInit] Unable to allocate DMA buffer for MCU#%" PRIu32 "\r\n", getSenderID(huart));
    }

    this->semaphore = xSemaphoreCreateCounting(16, 0);

    if(semaphore == nullptr) {
//        console.printf("[RoCo] [STMUARTDriverInit] Unable to allocate semaphore for MCU#%" PRIu32 "\r\n", getSenderID(huart));
    }

//    osDelay(5);
}

STMUARTDriver::~STMUARTDriver() {
    vPortFree(buffer);
}

void STMUARTDriver::init() {
	this->last_dma_index = 0;

	__HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);

	HAL_StatusTypeDef temp = HAL_UARTEx_ReceiveToIdle_DMA(huart, buffer, UART_BUFFER_SIZE);
	if(temp != HAL_OK) {
//        console.printf("[RoCo] [STMUARTDriverInit] Unable to initialize UART in receive mode for MCU#%" PRIu32 "\r\n", getSenderID(huart));
    }
}

void STMUARTDriver::loop() {
	if(xSemaphoreTake(semaphore, portMAX_DELAY)) {
		volatile uint32_t end_dma_index = UART_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);

		volatile uint8_t sender = getSenderID(huart);

		if(end_dma_index < last_dma_index) { // Finish buffer
			receiveUART(sender, buffer + last_dma_index, UART_BUFFER_SIZE - last_dma_index);
			this->last_dma_index = 0;
		}

		if(end_dma_index > last_dma_index) {
			receiveUART(sender, buffer + last_dma_index, end_dma_index - last_dma_index);
			this->last_dma_index = end_dma_index;
		}
	}
}

void STMUARTDriver::receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) {
    this->receiver_func = receiver;
}

void STMUARTDriver::transmit(uint8_t* buffer, uint32_t length) {
    if(HAL_UART_Transmit(huart, buffer, length, portMAX_DELAY) != HAL_OK){
//        console.printf("[RoCo] [STMUARTDriverTransmit] Transmission failed for MCU#%" PRIu32 "\r\n", getSenderID(huart));
    }
}


/**
 * @brief Getter to the reference of the buffer
 *
 * @return uint8_t* the reference to the buffer
 */
uint8_t* STMUARTDriver::getBuffer() {
	return this->buffer;
}


xSemaphoreHandle STMUARTDriver::getSemaphore() {
	return this->semaphore;
}

UART_HandleTypeDef* STMUARTDriver::getHuart() {
	return this->huart;
}

/**
 * @brief Function handling the call to the user-defined callback routine
 *
 * @param sender_id the ID of the MCU
 * @param buffer the buffer to provide to the user-defined callback function
 * @param length the size of the data in the buffer to provide
 */
void STMUARTDriver::receiveUART(uint8_t sender_id, uint8_t* buffer, uint32_t length) {
	this->receiver_func(sender_id, buffer, length);
}






//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
//	xSemaphoreGiveFromISR(instance->getSemaphore(), nullptr);
//}




/**
 * @brief Get the sender id from the USART port ID
 *
 * @param huart the USART port to get
 * @return uint8_t the sender_id
 */
uint8_t STMUARTDriver::getSenderID(UART_HandleTypeDef* huart) {
    for(int i = 0; i < NB_UART_PORTS; ++i){
        if(this->mapper[i] == huart->Instance){
            return i+1;
        }
    }
    return 0;
}

//#endif /* BUILD_WITH_STMUART */
