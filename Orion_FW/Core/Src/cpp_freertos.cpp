/* Includes ------------------------------------------------------------------*/
#include "cpp_freertos.h"
#include "System.h"
void cpp_freertos_init(void)
{
	setupTelemetry();
	initCortex();
}
