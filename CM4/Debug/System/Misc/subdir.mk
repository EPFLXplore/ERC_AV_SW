################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Users/jthom/git/XploreAvionics/System/Misc/WatchdogThread.cpp 

OBJS += \
./System/Misc/WatchdogThread.o 

CPP_DEPS += \
./System/Misc/WatchdogThread.d 


# Each subdirectory must supply rules for building sources it contributes
System/Misc/WatchdogThread.o: C:/Users/jthom/git/XploreAvionics/System/Misc/WatchdogThread.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Misc/WatchdogThread.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

