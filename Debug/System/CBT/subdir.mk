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
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x/Platform/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/HX711" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/HX711/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/BNO055/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/Stemma/Core/inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/U087/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/ADC1115/Core/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/Sensors" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src/Build" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src/Protocol" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System-2f-CBT

clean-System-2f-CBT:
	-$(RM) ./System/CBT/Main.d ./System/CBT/Main.o ./System/CBT/Main.su ./System/CBT/UDevDriver.d ./System/CBT/UDevDriver.o ./System/CBT/UDevDriver.su

.PHONY: clean-System-2f-CBT

