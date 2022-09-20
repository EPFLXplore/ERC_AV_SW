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
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/VL53L1x/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/VL53L1x/Platform/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/HX711" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/HX711/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/BNO055/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/Stemma/Core/inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/ADC1115/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/Sensors" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/RoCo/Src/Build" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/RoCo/Src" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/RoCo/Src/Protocol" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/BNO055" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/ADC1115" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/Stemma" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/VL53L1x" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/ADC1115/Core/Src" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/ADC1115/Core" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/Peripherals" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/INA219/Core/Src" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32h750vbtx.d ./Core/Startup/startup_stm32h750vbtx.o

.PHONY: clean-Core-2f-Startup

