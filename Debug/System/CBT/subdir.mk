################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../System/CBT/Main.cpp \
../System/CBT/UDevDriver.cpp 

OBJS += \
./System/CBT/Main.o \
./System/CBT/UDevDriver.o 

CPP_DEPS += \
./System/CBT/Main.d \
./System/CBT/UDevDriver.d 


# Each subdirectory must supply rules for building sources it contributes
System/CBT/%.o System/CBT/%.su: ../System/CBT/%.cpp System/CBT/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/BNO055/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/HX711/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/Stemma/Core/inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Platform/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Lang" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src/Build" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src/Protocol" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Sensors" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Misc" -I../Core/ThreadSafe -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System-2f-CBT

clean-System-2f-CBT:
	-$(RM) ./System/CBT/Main.d ./System/CBT/Main.o ./System/CBT/Main.su ./System/CBT/UDevDriver.d ./System/CBT/UDevDriver.o ./System/CBT/UDevDriver.su

.PHONY: clean-System-2f-CBT

