#include "UartPrint.h"

void printToUart(UART_HandleTypeDef* huart, uint8_t* data, int size){
	HAL_UART_Transmit(huart, data, size, 1000);
}
