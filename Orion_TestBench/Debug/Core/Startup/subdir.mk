################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32h750vbtx.s 

S_DEPS += \
./Core/Startup/startup_stm32h750vbtx.d 

OBJS += \
./Core/Startup/startup_stm32h750vbtx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Users/vince/STM32CubeIDE/workspace_1.13.0/AV_SW_workspace/Orion_TestBench/Drivers/ADC1115/Core/Inc" -I"C:/Users/vince/STM32CubeIDE/workspace_1.13.0/AV_SW_workspace/Orion_TestBench/Drivers/BMP280/Core/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32h750vbtx.d ./Core/Startup/startup_stm32h750vbtx.o

.PHONY: clean-Core-2f-Startup

