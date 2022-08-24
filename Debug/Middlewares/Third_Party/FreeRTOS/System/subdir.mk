################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Middlewares/Third_Party/FreeRTOS/System/System.cpp \
../Middlewares/Third_Party/FreeRTOS/System/Telemetry.cpp \
../Middlewares/Third_Party/FreeRTOS/System/Thread.cpp 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/System/System.o \
./Middlewares/Third_Party/FreeRTOS/System/Telemetry.o \
./Middlewares/Third_Party/FreeRTOS/System/Thread.o 

CPP_DEPS += \
./Middlewares/Third_Party/FreeRTOS/System/System.d \
./Middlewares/Third_Party/FreeRTOS/System/Telemetry.d \
./Middlewares/Third_Party/FreeRTOS/System/Thread.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/System/%.o Middlewares/Third_Party/FreeRTOS/System/%.su: ../Middlewares/Third_Party/FreeRTOS/System/%.cpp Middlewares/Third_Party/FreeRTOS/System/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/BNO055/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/HX711/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/Stemma/Core/inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Platform/Inc" -O2 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-System

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-System:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS/System/System.d ./Middlewares/Third_Party/FreeRTOS/System/System.o ./Middlewares/Third_Party/FreeRTOS/System/System.su ./Middlewares/Third_Party/FreeRTOS/System/Telemetry.d ./Middlewares/Third_Party/FreeRTOS/System/Telemetry.o ./Middlewares/Third_Party/FreeRTOS/System/Telemetry.su ./Middlewares/Third_Party/FreeRTOS/System/Thread.d ./Middlewares/Third_Party/FreeRTOS/System/Thread.o ./Middlewares/Third_Party/FreeRTOS/System/Thread.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-System

