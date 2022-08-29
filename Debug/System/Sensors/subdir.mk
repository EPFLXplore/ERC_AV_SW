################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../System/Sensors/HX711_thread.cpp \
../System/Sensors/IMU.cpp \
../System/Sensors/INA219_thread.cpp \
../System/Sensors/Prober.cpp \
../System/Sensors/Stemma_thread.cpp \
../System/Sensors/TOF_thread.cpp \
../System/Sensors/Voltmeter_thread.cpp 

OBJS += \
./System/Sensors/HX711_thread.o \
./System/Sensors/IMU.o \
./System/Sensors/INA219_thread.o \
./System/Sensors/Prober.o \
./System/Sensors/Stemma_thread.o \
./System/Sensors/TOF_thread.o \
./System/Sensors/Voltmeter_thread.o 

CPP_DEPS += \
./System/Sensors/HX711_thread.d \
./System/Sensors/IMU.d \
./System/Sensors/INA219_thread.d \
./System/Sensors/Prober.d \
./System/Sensors/Stemma_thread.d \
./System/Sensors/TOF_thread.d \
./System/Sensors/Voltmeter_thread.d 


# Each subdirectory must supply rules for building sources it contributes
System/Sensors/%.o System/Sensors/%.su: ../System/Sensors/%.cpp System/Sensors/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/ADC1115/Core/Src" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/ADC1115/Core/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/BNO055/Core/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/HX711/Core/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/Stemma/Core/inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/VL53L1x/Core/Inc" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/VL53L1x/Platform/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/Sensors" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/RoCo/Src/Build" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/RoCo/Src" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/RoCo/Src/Protocol" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/BNO055" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/ADC1115" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/Stemma" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/VL53L1x" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/System/Peripherals" -I"C:/Users/Bryan Kheirallah/Desktop/aa/AV_workspace/Drivers/INA219/Core/Src" -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System-2f-Sensors

clean-System-2f-Sensors:
	-$(RM) ./System/Sensors/HX711_thread.d ./System/Sensors/HX711_thread.o ./System/Sensors/HX711_thread.su ./System/Sensors/IMU.d ./System/Sensors/IMU.o ./System/Sensors/IMU.su ./System/Sensors/INA219_thread.d ./System/Sensors/INA219_thread.o ./System/Sensors/INA219_thread.su ./System/Sensors/Prober.d ./System/Sensors/Prober.o ./System/Sensors/Prober.su ./System/Sensors/Stemma_thread.d ./System/Sensors/Stemma_thread.o ./System/Sensors/Stemma_thread.su ./System/Sensors/TOF_thread.d ./System/Sensors/TOF_thread.o ./System/Sensors/TOF_thread.su ./System/Sensors/Voltmeter_thread.d ./System/Sensors/Voltmeter_thread.o ./System/Sensors/Voltmeter_thread.su

.PHONY: clean-System-2f-Sensors

