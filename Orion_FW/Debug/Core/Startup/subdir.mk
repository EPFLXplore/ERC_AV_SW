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
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/BRoCo" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Sensors" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/ADC1115/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/BMP280/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/BRoCo/Src/Build" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615/Core" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/ALLINONE/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Src" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Config" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32h750vbtx.d ./Core/Startup/startup_stm32h750vbtx.o

.PHONY: clean-Core-2f-Startup

