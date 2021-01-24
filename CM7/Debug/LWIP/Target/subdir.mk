################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../LWIP/Target/ethernetif.c 

C_DEPS += \
./LWIP/Target/ethernetif.d 

OBJS += \
./LWIP/Target/ethernetif.o 


# Each subdirectory must supply rules for building sources it contributes
LWIP/Target/ethernetif.o: ../LWIP/Target/ethernetif.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM7 -DDEBUG -DSTM32H745xx -c -I../LWIP/App -I../LWIP/Target -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/LWIP/system -I../../Drivers/CMSIS/Include -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/LWIP/src/include -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Middlewares/Third_Party/FatFs/src -I../../System -I../../Drivers/BSP/Components/lan8742 -I../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../Middlewares/Third_Party/LwIP/system/arch -I../../Middlewares/Third_Party/LwIP/system -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../Middlewares/Third_Party/LwIP/src/include/netif -I../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"LWIP/Target/ethernetif.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

