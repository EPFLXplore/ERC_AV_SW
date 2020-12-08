################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/arion/git/XploreAvionics/System/Libraries/bmp280.c \
/Users/arion/git/XploreAvionics/System/Libraries/bno055.c \
/Users/arion/git/XploreAvionics/System/Libraries/bno055_stm32.c 

CPP_SRCS += \
/Users/arion/git/XploreAvionics/System/Libraries/UartPrint.cpp 

C_DEPS += \
./System/Libraries/bmp280.d \
./System/Libraries/bno055.d \
./System/Libraries/bno055_stm32.d 

OBJS += \
./System/Libraries/UartPrint.o \
./System/Libraries/bmp280.o \
./System/Libraries/bno055.o \
./System/Libraries/bno055_stm32.o 

CPP_DEPS += \
./System/Libraries/UartPrint.d 


# Each subdirectory must supply rules for building sources it contributes
System/Libraries/UartPrint.o: /Users/arion/git/XploreAvionics/System/Libraries/UartPrint.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I/Users/arion/git/XploreAvionics/System -I/Users/arion/git/XploreAvionics/CM4/Core/Inc -I/Users/arion/git/XploreAvionics/CM4/FATFS/Target -I/Users/arion/git/XploreAvionics/CM4/FATFS/App -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FatFs/src -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/include -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Libraries/UartPrint.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Libraries/bmp280.o: /Users/arion/git/XploreAvionics/System/Libraries/bmp280.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I/Users/arion/git/XploreAvionics/System -I/Users/arion/git/XploreAvionics/CM4/Core/Inc -I/Users/arion/git/XploreAvionics/CM4/FATFS/Target -I/Users/arion/git/XploreAvionics/CM4/FATFS/App -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FatFs/src -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/include -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"System/Libraries/bmp280.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Libraries/bno055.o: /Users/arion/git/XploreAvionics/System/Libraries/bno055.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I/Users/arion/git/XploreAvionics/System -I/Users/arion/git/XploreAvionics/CM4/Core/Inc -I/Users/arion/git/XploreAvionics/CM4/FATFS/Target -I/Users/arion/git/XploreAvionics/CM4/FATFS/App -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FatFs/src -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/include -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"System/Libraries/bno055.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
System/Libraries/bno055_stm32.o: /Users/arion/git/XploreAvionics/System/Libraries/bno055_stm32.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I/Users/arion/git/XploreAvionics/System -I/Users/arion/git/XploreAvionics/CM4/Core/Inc -I/Users/arion/git/XploreAvionics/CM4/FATFS/Target -I/Users/arion/git/XploreAvionics/CM4/FATFS/App -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FatFs/src -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/include -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"System/Libraries/bno055_stm32.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

