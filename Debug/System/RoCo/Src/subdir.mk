################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../System/RoCo/Src/IOBus.cpp \
../System/RoCo/Src/MessageBus.cpp \
../System/RoCo/Src/NetworkBus.cpp \
../System/RoCo/Src/STMUARTDriver.cpp 

OBJS += \
./System/RoCo/Src/IOBus.o \
./System/RoCo/Src/MessageBus.o \
./System/RoCo/Src/NetworkBus.o \
./System/RoCo/Src/STMUARTDriver.o 

CPP_DEPS += \
./System/RoCo/Src/IOBus.d \
./System/RoCo/Src/MessageBus.d \
./System/RoCo/Src/NetworkBus.d \
./System/RoCo/Src/STMUARTDriver.d 


# Each subdirectory must supply rules for building sources it contributes
System/RoCo/Src/%.o System/RoCo/Src/%.su: ../System/RoCo/Src/%.cpp System/RoCo/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115/Core/Src" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/BNO055/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/HX711/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/Stemma/Core/inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Platform/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Sensors" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src/Build" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src/Protocol" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/BNO055" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/Stemma" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Peripherals" -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System-2f-RoCo-2f-Src

clean-System-2f-RoCo-2f-Src:
	-$(RM) ./System/RoCo/Src/IOBus.d ./System/RoCo/Src/IOBus.o ./System/RoCo/Src/IOBus.su ./System/RoCo/Src/MessageBus.d ./System/RoCo/Src/MessageBus.o ./System/RoCo/Src/MessageBus.su ./System/RoCo/Src/NetworkBus.d ./System/RoCo/Src/NetworkBus.o ./System/RoCo/Src/NetworkBus.su ./System/RoCo/Src/STMUARTDriver.d ./System/RoCo/Src/STMUARTDriver.o ./System/RoCo/Src/STMUARTDriver.su

.PHONY: clean-System-2f-RoCo-2f-Src

