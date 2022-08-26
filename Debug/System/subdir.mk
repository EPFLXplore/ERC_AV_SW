################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../System/Handlers.cpp \
../System/System.cpp \
../System/Telemetry.cpp \
../System/Thread.cpp 

OBJS += \
./System/Handlers.o \
./System/System.o \
./System/Telemetry.o \
./System/Thread.o 

CPP_DEPS += \
./System/Handlers.d \
./System/System.d \
./System/Telemetry.d \
./System/Thread.d 


# Each subdirectory must supply rules for building sources it contributes
System/%.o System/%.su: ../System/%.cpp System/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115/Core/Src" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/BNO055/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/HX711/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/Stemma/Core/inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Platform/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Sensors" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src/Build" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src/Protocol" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/BNO055" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/Stemma" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Peripherals" -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System

clean-System:
	-$(RM) ./System/Handlers.d ./System/Handlers.o ./System/Handlers.su ./System/System.d ./System/System.o ./System/System.su ./System/Telemetry.d ./System/Telemetry.o ./System/Telemetry.su ./System/Thread.d ./System/Thread.o ./System/Thread.su

.PHONY: clean-System

