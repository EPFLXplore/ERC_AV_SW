################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/dma.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_hal_timebase_tim.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32h7xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

CPP_SRCS += \
../Core/Src/freertos.cpp \
../Core/Src/main.cpp 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/dma.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_hal_timebase_tim.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32h7xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 

OBJS += \
./Core/Src/adc.o \
./Core/Src/dma.o \
./Core/Src/freertos.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_hal_timebase_tim.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32h7xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

CPP_DEPS += \
./Core/Src/freertos.d \
./Core/Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/41786/Documents/AV_workspace/Drivers/VL53L1x/Core/Inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/VL53L1x/Platform/Inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/HX711" -I"C:/Users/41786/Documents/AV_workspace/Drivers/HX711/Core/Inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/BNO055/Core/Inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/Stemma/Core/inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/ADC1115/Core/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/41786/Documents/AV_workspace/System/Sensors" -I"C:/Users/41786/Documents/AV_workspace/System" -I"C:/Users/41786/Documents/AV_workspace/System/RoCo/Src/Build" -I"C:/Users/41786/Documents/AV_workspace/System/RoCo/Src" -I"C:/Users/41786/Documents/AV_workspace/System/RoCo/Src/Protocol" -I"C:/Users/41786/Documents/AV_workspace/System/Peripherals" -I"C:/Users/41786/Documents/AV_workspace/Drivers/INA219/Core/Src" -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I"C:/Users/41786/Documents/AV_workspace/Drivers/ADC1115/Core/Src" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/41786/Documents/AV_workspace/Drivers/ADC1115/Core/Inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/BNO055/Core/Inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/HX711/Core/Inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/Stemma/Core/inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/VL53L1x/Core/Inc" -I"C:/Users/41786/Documents/AV_workspace/Drivers/VL53L1x/Platform/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"C:/Users/41786/Documents/AV_workspace/System/Sensors" -I"C:/Users/41786/Documents/AV_workspace/System" -I"C:/Users/41786/Documents/AV_workspace/System/RoCo/Src/Build" -I"C:/Users/41786/Documents/AV_workspace/System/RoCo/Src" -I"C:/Users/41786/Documents/AV_workspace/System/RoCo/Src/Protocol" -I"C:/Users/41786/Documents/AV_workspace/Drivers/BNO055" -I"C:/Users/41786/Documents/AV_workspace/Drivers/ADC1115" -I"C:/Users/41786/Documents/AV_workspace/Drivers/Stemma" -I"C:/Users/41786/Documents/AV_workspace/Drivers/VL53L1x" -I"C:/Users/41786/Documents/AV_workspace/Drivers" -I"C:/Users/41786/Documents/AV_workspace/System/Peripherals" -I"C:/Users/41786/Documents/AV_workspace/Drivers/INA219/Core/Src" -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32h7xx_hal_msp.d ./Core/Src/stm32h7xx_hal_msp.o ./Core/Src/stm32h7xx_hal_msp.su ./Core/Src/stm32h7xx_hal_timebase_tim.d ./Core/Src/stm32h7xx_hal_timebase_tim.o ./Core/Src/stm32h7xx_hal_timebase_tim.su ./Core/Src/stm32h7xx_it.d ./Core/Src/stm32h7xx_it.o ./Core/Src/stm32h7xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32h7xx.d ./Core/Src/system_stm32h7xx.o ./Core/Src/system_stm32h7xx.su ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

