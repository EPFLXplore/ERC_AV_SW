################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bmp280.c \
../Core/Src/main.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c 

OBJS += \
./Core/Src/bmp280.o \
./Core/Src/main.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o 

C_DEPS += \
./Core/Src/bmp280.d \
./Core/Src/main.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/bmp280.o: ../Core/Src/bmp280.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/bmp280.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32h7xx_hal_msp.o: ../Core/Src/stm32h7xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32h7xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32h7xx_it.o: ../Core/Src/stm32h7xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32h7xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/syscalls.o: ../Core/Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/sysmem.o: ../Core/Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I../Core/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -IC:/Users/jthom/STM32Cube/Repository/STM32Cube_FW_H7_V1.8.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

