################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VL53L1x/Core/src/vl53l1_api.c \
../Drivers/VL53L1x/Core/src/vl53l1_api_calibration.c \
../Drivers/VL53L1x/Core/src/vl53l1_api_core.c \
../Drivers/VL53L1x/Core/src/vl53l1_api_debug.c \
../Drivers/VL53L1x/Core/src/vl53l1_api_preset_modes.c \
../Drivers/VL53L1x/Core/src/vl53l1_api_strings.c \
../Drivers/VL53L1x/Core/src/vl53l1_core.c \
../Drivers/VL53L1x/Core/src/vl53l1_core_support.c \
../Drivers/VL53L1x/Core/src/vl53l1_error_strings.c \
../Drivers/VL53L1x/Core/src/vl53l1_register_funcs.c \
../Drivers/VL53L1x/Core/src/vl53l1_silicon_core.c \
../Drivers/VL53L1x/Core/src/vl53l1_wait.c 

C_DEPS += \
./Drivers/VL53L1x/Core/src/vl53l1_api.d \
./Drivers/VL53L1x/Core/src/vl53l1_api_calibration.d \
./Drivers/VL53L1x/Core/src/vl53l1_api_core.d \
./Drivers/VL53L1x/Core/src/vl53l1_api_debug.d \
./Drivers/VL53L1x/Core/src/vl53l1_api_preset_modes.d \
./Drivers/VL53L1x/Core/src/vl53l1_api_strings.d \
./Drivers/VL53L1x/Core/src/vl53l1_core.d \
./Drivers/VL53L1x/Core/src/vl53l1_core_support.d \
./Drivers/VL53L1x/Core/src/vl53l1_error_strings.d \
./Drivers/VL53L1x/Core/src/vl53l1_register_funcs.d \
./Drivers/VL53L1x/Core/src/vl53l1_silicon_core.d \
./Drivers/VL53L1x/Core/src/vl53l1_wait.d 

OBJS += \
./Drivers/VL53L1x/Core/src/vl53l1_api.o \
./Drivers/VL53L1x/Core/src/vl53l1_api_calibration.o \
./Drivers/VL53L1x/Core/src/vl53l1_api_core.o \
./Drivers/VL53L1x/Core/src/vl53l1_api_debug.o \
./Drivers/VL53L1x/Core/src/vl53l1_api_preset_modes.o \
./Drivers/VL53L1x/Core/src/vl53l1_api_strings.o \
./Drivers/VL53L1x/Core/src/vl53l1_core.o \
./Drivers/VL53L1x/Core/src/vl53l1_core_support.o \
./Drivers/VL53L1x/Core/src/vl53l1_error_strings.o \
./Drivers/VL53L1x/Core/src/vl53l1_register_funcs.o \
./Drivers/VL53L1x/Core/src/vl53l1_silicon_core.o \
./Drivers/VL53L1x/Core/src/vl53l1_wait.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VL53L1x/Core/src/%.o Drivers/VL53L1x/Core/src/%.su: ../Drivers/VL53L1x/Core/src/%.c Drivers/VL53L1x/Core/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I/home/avm/STM32Cube/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Inc -I/home/avm/STM32Cube/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/home/avm/STM32Cube/Repository/STM32Cube_FW_H7_V1.10.0/Middlewares/Third_Party/FreeRTOS/Source/include -I/home/avm/STM32Cube/Repository/STM32Cube_FW_H7_V1.10.0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I/home/avm/STM32Cube/Repository/STM32Cube_FW_H7_V1.10.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/home/avm/STM32Cube/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/home/avm/STM32Cube/Repository/STM32Cube_FW_H7_V1.10.0/Drivers/CMSIS/Include -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/Drivers/ADC1115/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/Drivers/BNO055/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/Drivers/HX711/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/Drivers/Stemma/Core/inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/Drivers/U087/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/Drivers/VL53L1x/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/Drivers/VL53L1x/Platform/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/System/RoCo" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS/System/RoCo/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-VL53L1x-2f-Core-2f-src

clean-Drivers-2f-VL53L1x-2f-Core-2f-src:
	-$(RM) ./Drivers/VL53L1x/Core/src/vl53l1_api.d ./Drivers/VL53L1x/Core/src/vl53l1_api.o ./Drivers/VL53L1x/Core/src/vl53l1_api.su ./Drivers/VL53L1x/Core/src/vl53l1_api_calibration.d ./Drivers/VL53L1x/Core/src/vl53l1_api_calibration.o ./Drivers/VL53L1x/Core/src/vl53l1_api_calibration.su ./Drivers/VL53L1x/Core/src/vl53l1_api_core.d ./Drivers/VL53L1x/Core/src/vl53l1_api_core.o ./Drivers/VL53L1x/Core/src/vl53l1_api_core.su ./Drivers/VL53L1x/Core/src/vl53l1_api_debug.d ./Drivers/VL53L1x/Core/src/vl53l1_api_debug.o ./Drivers/VL53L1x/Core/src/vl53l1_api_debug.su ./Drivers/VL53L1x/Core/src/vl53l1_api_preset_modes.d ./Drivers/VL53L1x/Core/src/vl53l1_api_preset_modes.o ./Drivers/VL53L1x/Core/src/vl53l1_api_preset_modes.su ./Drivers/VL53L1x/Core/src/vl53l1_api_strings.d ./Drivers/VL53L1x/Core/src/vl53l1_api_strings.o ./Drivers/VL53L1x/Core/src/vl53l1_api_strings.su ./Drivers/VL53L1x/Core/src/vl53l1_core.d ./Drivers/VL53L1x/Core/src/vl53l1_core.o ./Drivers/VL53L1x/Core/src/vl53l1_core.su ./Drivers/VL53L1x/Core/src/vl53l1_core_support.d ./Drivers/VL53L1x/Core/src/vl53l1_core_support.o ./Drivers/VL53L1x/Core/src/vl53l1_core_support.su ./Drivers/VL53L1x/Core/src/vl53l1_error_strings.d ./Drivers/VL53L1x/Core/src/vl53l1_error_strings.o ./Drivers/VL53L1x/Core/src/vl53l1_error_strings.su ./Drivers/VL53L1x/Core/src/vl53l1_register_funcs.d ./Drivers/VL53L1x/Core/src/vl53l1_register_funcs.o ./Drivers/VL53L1x/Core/src/vl53l1_register_funcs.su ./Drivers/VL53L1x/Core/src/vl53l1_silicon_core.d ./Drivers/VL53L1x/Core/src/vl53l1_silicon_core.o ./Drivers/VL53L1x/Core/src/vl53l1_silicon_core.su ./Drivers/VL53L1x/Core/src/vl53l1_wait.d ./Drivers/VL53L1x/Core/src/vl53l1_wait.o ./Drivers/VL53L1x/Core/src/vl53l1_wait.su

.PHONY: clean-Drivers-2f-VL53L1x-2f-Core-2f-src

