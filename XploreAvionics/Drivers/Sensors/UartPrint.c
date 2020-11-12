#include "UartPrint.h"

void printToUart(uint8_t* data, int size){
	HAL_UART_Transmit(&huart3, data, size, 1000);
}
