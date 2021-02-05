################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/arion/git/XploreAvionics/System/Misc/WatchdogThread.cpp 

OBJS += \
./System/Misc/WatchdogThread.o 

CPP_DEPS += \
./System/Misc/WatchdogThread.d 


# Each subdirectory must supply rules for building sources it contributes
System/Misc/WatchdogThread.o: /Users/arion/git/XploreAvionics/System/Misc/WatchdogThread.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DBUILD_FOR_AVIONICS_CM7 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I../LWIP/App -I../../Middlewares/Third_Party/LWIP/system -I../LWIP/Target -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/LWIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/lan8742 -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../Middlewares/Third_Party/LwIP/system/arch -I../../Middlewares/Third_Party/LwIP/system -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../Middlewares/Third_Party/LwIP/src/include/netif -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"System/Misc/WatchdogThread.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

