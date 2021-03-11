################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Users/jthom/git/XploreAvionics/System/System.cpp \
C:/Users/jthom/git/XploreAvionics/System/Telemetry.cpp \
C:/Users/jthom/git/XploreAvionics/System/Thread.cpp 

OBJS += \
./System/System.o \
./System/Telemetry.o \
./System/Thread.o 

CPP_DEPS += \
./System/System.d \
./System/Telemetry.d \
./System/Thread.d 


# Each subdirectory must supply rules for building sources it contributes
System/System.o: C:/Users/jthom/git/XploreAvionics/System/System.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/System.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Telemetry.o: C:/Users/jthom/git/XploreAvionics/System/Telemetry.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Telemetry.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Thread.o: C:/Users/jthom/git/XploreAvionics/System/Thread.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Thread.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

