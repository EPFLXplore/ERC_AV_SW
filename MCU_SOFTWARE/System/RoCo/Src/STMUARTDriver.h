/**
 * @file STMUARTDriver.h
 * @author Mounir Raki (mounir.raki@epfl.ch)
 * @brief
 * @version 1.0
 * @date 2022-03-29
 *
 * Header of the UART driver for the STM32 boards
 *
 */
//TODO: (MAYBE) USE MULTITON PATTERN TO MAKE SURE EVERY UART INSTANCE ACCESSES THE SAME INSTANCIATE EVERY UART INTERFACE
#ifndef LIBRARIES_ROCO_SRC_STMUARTDRIVER_H_
#define LIBRARIES_ROCO_SRC_STMUARTDRIVER_H_

#include "Build/Build.h"
#include "IODriver.h"

#include "stm32h7xx_hal.h"

#include "Thread.h"

#define UART_BUFFER_SIZE    255		            // SHOULDN'T BE BIGGER THAN 255 (RELATED TO MESSAGEBUS), OTHERWISE BIG ISSUES
#define NB_UART_PORTS       8                   // CHANGE ONLY IF NEEDED

class STMUARTDriver: public IODriver, public Thread {
    public:
        STMUARTDriver(UART_HandleTypeDef* huart); // Constructor
        virtual ~STMUARTDriver(); // Destructor
        uint8_t* getBuffer();
        xSemaphoreHandle getSemaphore();
        UART_HandleTypeDef* getHuart();
        uint8_t getSenderID(UART_HandleTypeDef* huart);

        void init();
        void loop();

        void receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) override;
        void transmit(uint8_t* buffer, uint32_t length) override;

        void receiveUART(uint8_t sender_id, uint8_t* buffer, uint32_t length);

    private:
        UART_HandleTypeDef* huart;
        uint32_t last_dma_index;

        // Needed for the HAL_UART_RxCpltCallback to access STMUARTDriver's class attributes (i.e. receiver_func & buffer)
        USART_TypeDef* mapper[NB_UART_PORTS] = {USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8};

        uint8_t* buffer;
        std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> receiver_func; // User-defined callback function

    	xSemaphoreHandle semaphore;
};

#endif /* LIBRARIES_ROCO_SRC_STMUARTDRIVER_H_ */
