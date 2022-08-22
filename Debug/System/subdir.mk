################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../System/System.cpp \
../System/Telemetry.cpp \
../System/Thread.cpp 

OBJS += \
./System/System.o \
./System/Telemetry.o \
./System/Thread.o 

CPP_DEPS += \
./System/System.d \
./System/Telemetry.d \
./System/Thread.d 


# Each subdirectory must supply rules for building sources it contributes
System/%.o System/%.su: ../System/%.cpp System/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/ADC1115/Core/Src" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/ADC1115/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/BNO055/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/HX711/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/Stemma/Core/inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x/Platform/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/Sensors" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src/Build" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src/Protocol" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/BNO055" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/ADC1115" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/Stemma" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers" -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System

clean-System:
	-$(RM) ./System/System.d ./System/System.o ./System/System.su ./System/Telemetry.d ./System/Telemetry.o ./System/Telemetry.su ./System/Thread.d ./System/Thread.o ./System/Thread.su

.PHONY: clean-System
