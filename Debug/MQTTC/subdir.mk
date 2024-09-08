################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MQTTC/MQTTClient.c \
../MQTTC/mqtt_interface.c 

OBJS += \
./MQTTC/MQTTClient.o \
./MQTTC/mqtt_interface.o 

C_DEPS += \
./MQTTC/MQTTClient.d \
./MQTTC/mqtt_interface.d 


# Each subdirectory must supply rules for building sources it contributes
MQTTC/%.o MQTTC/%.su MQTTC/%.cyclo: ../MQTTC/%.c MQTTC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../MQTTC -I../MQTTC/MQTTPacket -I../W5500 -I../W5500/W5500 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MQTTC

clean-MQTTC:
	-$(RM) ./MQTTC/MQTTClient.cyclo ./MQTTC/MQTTClient.d ./MQTTC/MQTTClient.o ./MQTTC/MQTTClient.su ./MQTTC/mqtt_interface.cyclo ./MQTTC/mqtt_interface.d ./MQTTC/mqtt_interface.o ./MQTTC/mqtt_interface.su

.PHONY: clean-MQTTC

