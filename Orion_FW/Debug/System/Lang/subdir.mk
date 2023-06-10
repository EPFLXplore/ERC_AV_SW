################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../System/Lang/Operators.cpp 

OBJS += \
./System/Lang/Operators.o 

CPP_DEPS += \
./System/Lang/Operators.d 


# Each subdirectory must supply rules for building sources it contributes
System/Lang/%.o System/Lang/%.su System/Lang/%.cyclo: ../System/Lang/%.cpp System/Lang/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/BRoCo" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Sensors" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/ADC1115/Core/Inc" -I../Core/Inc -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/BMP280/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/BRoCo/Src/Build" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615/Core" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/ALLINONE/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Src" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Config" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System-2f-Lang

clean-System-2f-Lang:
	-$(RM) ./System/Lang/Operators.cyclo ./System/Lang/Operators.d ./System/Lang/Operators.o ./System/Lang/Operators.su

.PHONY: clean-System-2f-Lang

