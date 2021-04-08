################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/arion/git/XploreAvionics/System/Sensors/ADC16.cpp \
/Users/arion/git/XploreAvionics/System/Sensors/ADC24.cpp \
/Users/arion/git/XploreAvionics/System/Sensors/Barometer.cpp \
/Users/arion/git/XploreAvionics/System/Sensors/IMU.cpp \
/Users/arion/git/XploreAvionics/System/Sensors/Prober.cpp 

OBJS += \
./System/Sensors/ADC16.o \
./System/Sensors/ADC24.o \
./System/Sensors/Barometer.o \
./System/Sensors/IMU.o \
./System/Sensors/Prober.o 

CPP_DEPS += \
./System/Sensors/ADC16.d \
./System/Sensors/ADC24.d \
./System/Sensors/Barometer.d \
./System/Sensors/IMU.d \
./System/Sensors/Prober.d 


# Each subdirectory must supply rules for building sources it contributes
System/Sensors/ADC16.o: /Users/arion/git/XploreAvionics/System/Sensors/ADC16.cpp System/Sensors/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Sensors/ADC16.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Sensors/ADC24.o: /Users/arion/git/XploreAvionics/System/Sensors/ADC24.cpp System/Sensors/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Sensors/ADC24.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Sensors/Barometer.o: /Users/arion/git/XploreAvionics/System/Sensors/Barometer.cpp System/Sensors/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Sensors/Barometer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Sensors/IMU.o: /Users/arion/git/XploreAvionics/System/Sensors/IMU.cpp System/Sensors/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Sensors/IMU.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Sensors/Prober.o: /Users/arion/git/XploreAvionics/System/Sensors/Prober.cpp System/Sensors/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DBUILD_FOR_AVIONICS_CM4 -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Sensors/Prober.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

