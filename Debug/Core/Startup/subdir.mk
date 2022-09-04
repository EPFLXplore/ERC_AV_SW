################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
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
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/VL53L1x/Core/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/VL53L1x/Platform/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/HX711" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/HX711/Core/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/BNO055/Core/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/Stemma/Core/inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/ADC1115/Core/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/Sensors" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/RoCo/Src/Build" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/RoCo/Src" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/RoCo/Src/Protocol" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/BNO055" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/ADC1115" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/Stemma" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/VL53L1x" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/ADC1115/Core/Src" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/ADC1115/Core" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/Peripherals" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/INA219/Core/Src" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32h750vbtx.d ./Core/Startup/startup_stm32h750vbtx.o

.PHONY: clean-Core-2f-Startup

