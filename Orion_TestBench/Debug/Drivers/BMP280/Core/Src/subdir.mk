################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/BMP280/Core/Src/BMP280.cpp \
../Drivers/BMP280/Core/Src/BMP280_compensation.cpp \
../Drivers/BMP280/Core/Src/BMP280_hardware.cpp 

OBJS += \
./Drivers/BMP280/Core/Src/BMP280.o \
./Drivers/BMP280/Core/Src/BMP280_compensation.o \
./Drivers/BMP280/Core/Src/BMP280_hardware.o 

CPP_DEPS += \
./Drivers/BMP280/Core/Src/BMP280.d \
./Drivers/BMP280/Core/Src/BMP280_compensation.d \
./Drivers/BMP280/Core/Src/BMP280_hardware.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BMP280/Core/Src/%.o Drivers/BMP280/Core/Src/%.su: ../Drivers/BMP280/Core/Src/%.cpp Drivers/BMP280/Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_SW_workspace/Orion_TestBench/Drivers/ADC1115/Core/Inc" -I"/Orion_TestBench/Drivers/BMP280/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_SW_workspace/Orion_TestBench/Drivers/BMP280/Core/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BMP280-2f-Core-2f-Src

clean-Drivers-2f-BMP280-2f-Core-2f-Src:
	-$(RM) ./Drivers/BMP280/Core/Src/BMP280.d ./Drivers/BMP280/Core/Src/BMP280.o ./Drivers/BMP280/Core/Src/BMP280.su ./Drivers/BMP280/Core/Src/BMP280_compensation.d ./Drivers/BMP280/Core/Src/BMP280_compensation.o ./Drivers/BMP280/Core/Src/BMP280_compensation.su ./Drivers/BMP280/Core/Src/BMP280_hardware.d ./Drivers/BMP280/Core/Src/BMP280_hardware.o ./Drivers/BMP280/Core/Src/BMP280_hardware.su

.PHONY: clean-Drivers-2f-BMP280-2f-Core-2f-Src

