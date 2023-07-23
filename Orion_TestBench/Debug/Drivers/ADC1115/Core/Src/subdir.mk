################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ADC1115/Core/Src/ADS1113.c 

C_DEPS += \
./Drivers/ADC1115/Core/Src/ADS1113.d 

OBJS += \
./Drivers/ADC1115/Core/Src/ADS1113.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ADC1115/Core/Src/%.o Drivers/ADC1115/Core/Src/%.su: ../Drivers/ADC1115/Core/Src/%.c Drivers/ADC1115/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/41786/STM32CubeIDE/workspace_1.7.0/AV_SW_workspace/Orion_TestBench/Drivers/ADC1115/Core/Inc" -I"/Orion_TestBench/Drivers/BMP280/Core/Inc" -I"C:/Users/41786/STM32CubeIDE/workspace_1.7.0/AV_SW_workspace/Orion_TestBench/Drivers/BMP280/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-ADC1115-2f-Core-2f-Src

clean-Drivers-2f-ADC1115-2f-Core-2f-Src:
	-$(RM) ./Drivers/ADC1115/Core/Src/ADS1113.d ./Drivers/ADC1115/Core/Src/ADS1113.o ./Drivers/ADC1115/Core/Src/ADS1113.su

.PHONY: clean-Drivers-2f-ADC1115-2f-Core-2f-Src

