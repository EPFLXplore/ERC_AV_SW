################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/HX711/Core/Src/hx711.c 

C_DEPS += \
./Drivers/HX711/Core/Src/hx711.d 

OBJS += \
./Drivers/HX711/Core/Src/hx711.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/HX711/Core/Src/%.o Drivers/HX711/Core/Src/%.su: ../Drivers/HX711/Core/Src/%.c Drivers/HX711/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/Drivers/ADC1115/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/Drivers/BNO055/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/Drivers/HX711/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/Drivers/Stemma/Core/inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/Drivers/U087/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/Drivers/VL53L1x/Core/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/Drivers/VL53L1x/Platform/Inc" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/System" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/System/RoCo/Src" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/System/Lang" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/System/RoCo/Src/Build" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/System/RoCo/Src/Protocol" -I"/home/avm/STM32CubeIDE/workspace_1.10.1/EPFL_XPLORE_AVIONICS_MCU_ROCOROS_REMIX/System/Sensors" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-HX711-2f-Core-2f-Src

clean-Drivers-2f-HX711-2f-Core-2f-Src:
	-$(RM) ./Drivers/HX711/Core/Src/hx711.d ./Drivers/HX711/Core/Src/hx711.o ./Drivers/HX711/Core/Src/hx711.su

.PHONY: clean-Drivers-2f-HX711-2f-Core-2f-Src

