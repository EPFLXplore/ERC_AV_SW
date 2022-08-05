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
#include "Thread.h"

#define BUILD_WITH_STMUART

#ifdef BUILD_WITH_STMUART
#include "stm32h7xx_hal.h"

#define UART_BUFFER_SIZE    128                 // SHOULDN'T BE BIGGER THAN 255 (RELATED TO MESSAGEBUS), OTHERWISE BIG ISSUES
#define THREAD_STACK_SIZE   1024                // DON'T CHANGE IF NOT NECESSARY TO DO SO
#define NB_UART_PORTS       8                   // CHANGE ONLY IF NEEDED

class STMUARTDriver: public IODriver {
    public:
        STMUARTDriver(UART_HandleTypeDef* huart); // Constructor
        STMUARTDriver(USART_HandleTypeDef* huart); // Constructor
        virtual ~STMUARTDriver(); // Destructor
        uint8_t* getBuffer();
        uint8_t get_sender_id(UART_HandleTypeDef* huart);
        uint8_t get_sender_id(USART_HandleTypeDef* huart);

        void receive(const std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> &receiver) override;
        void transmit(uint8_t* buffer, uint32_t length) override;

        void receiveUART(uint8_t sender_id, uint8_t* buffer, uint32_t length);

        void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);
        void HAL_USART_RxCpltCallback(USART_HandleTypeDef* husart);

    private:
        UART_HandleTypeDef* huart;
        USART_HandleTypeDef* husart;

        // Needed for the HAL_UART_RxCpltCallback to access STMUARTDriver's class attributes (i.e. receiver_func & buffer)
        USART_TypeDef* mapper[NB_UART_PORTS] = {USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8};

        uint8_t* buffer;
        std::function<void (uint8_t sender_id, uint8_t* buffer, uint32_t length)> receiver_func; // User-defined callback function

        class STMUARTReceptionThread : Thread {
        public:
            STMUARTReceptionThread(std::function<void ()> tickFunc) : Thread("UARTReceptionThread", THREAD_STACK_SIZE), tickFunc(tickFunc) {}
            void loop() { tickFunc(); };

        private:
            std::function<void ()> tickFunc;
        };

        STMUARTReceptionThread* recvThread;
};


#endif /* BUILD_WITH_STMUART */

#endif /* LIBRARIES_ROCO_SRC_STMUARTDRIVER_H_ */
