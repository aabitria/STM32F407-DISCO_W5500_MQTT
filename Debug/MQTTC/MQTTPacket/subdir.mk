################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MQTTC/MQTTPacket/MQTTConnectClient.c \
../MQTTC/MQTTPacket/MQTTConnectServer.c \
../MQTTC/MQTTPacket/MQTTDeserializePublish.c \
../MQTTC/MQTTPacket/MQTTFormat.c \
../MQTTC/MQTTPacket/MQTTPacket.c \
../MQTTC/MQTTPacket/MQTTSerializePublish.c \
../MQTTC/MQTTPacket/MQTTSubscribeClient.c \
../MQTTC/MQTTPacket/MQTTSubscribeServer.c \
../MQTTC/MQTTPacket/MQTTUnsubscribeClient.c \
../MQTTC/MQTTPacket/MQTTUnsubscribeServer.c 

OBJS += \
./MQTTC/MQTTPacket/MQTTConnectClient.o \
./MQTTC/MQTTPacket/MQTTConnectServer.o \
./MQTTC/MQTTPacket/MQTTDeserializePublish.o \
./MQTTC/MQTTPacket/MQTTFormat.o \
./MQTTC/MQTTPacket/MQTTPacket.o \
./MQTTC/MQTTPacket/MQTTSerializePublish.o \
./MQTTC/MQTTPacket/MQTTSubscribeClient.o \
./MQTTC/MQTTPacket/MQTTSubscribeServer.o \
./MQTTC/MQTTPacket/MQTTUnsubscribeClient.o \
./MQTTC/MQTTPacket/MQTTUnsubscribeServer.o 

C_DEPS += \
./MQTTC/MQTTPacket/MQTTConnectClient.d \
./MQTTC/MQTTPacket/MQTTConnectServer.d \
./MQTTC/MQTTPacket/MQTTDeserializePublish.d \
./MQTTC/MQTTPacket/MQTTFormat.d \
./MQTTC/MQTTPacket/MQTTPacket.d \
./MQTTC/MQTTPacket/MQTTSerializePublish.d \
./MQTTC/MQTTPacket/MQTTSubscribeClient.d \
./MQTTC/MQTTPacket/MQTTSubscribeServer.d \
./MQTTC/MQTTPacket/MQTTUnsubscribeClient.d \
./MQTTC/MQTTPacket/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
MQTTC/MQTTPacket/%.o MQTTC/MQTTPacket/%.su MQTTC/MQTTPacket/%.cyclo: ../MQTTC/MQTTPacket/%.c MQTTC/MQTTPacket/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../MQTTC -I../MQTTC/MQTTPacket -I../W5500 -I../W5500/W5500 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MQTTC-2f-MQTTPacket

clean-MQTTC-2f-MQTTPacket:
	-$(RM) ./MQTTC/MQTTPacket/MQTTConnectClient.cyclo ./MQTTC/MQTTPacket/MQTTConnectClient.d ./MQTTC/MQTTPacket/MQTTConnectClient.o ./MQTTC/MQTTPacket/MQTTConnectClient.su ./MQTTC/MQTTPacket/MQTTConnectServer.cyclo ./MQTTC/MQTTPacket/MQTTConnectServer.d ./MQTTC/MQTTPacket/MQTTConnectServer.o ./MQTTC/MQTTPacket/MQTTConnectServer.su ./MQTTC/MQTTPacket/MQTTDeserializePublish.cyclo ./MQTTC/MQTTPacket/MQTTDeserializePublish.d ./MQTTC/MQTTPacket/MQTTDeserializePublish.o ./MQTTC/MQTTPacket/MQTTDeserializePublish.su ./MQTTC/MQTTPacket/MQTTFormat.cyclo ./MQTTC/MQTTPacket/MQTTFormat.d ./MQTTC/MQTTPacket/MQTTFormat.o ./MQTTC/MQTTPacket/MQTTFormat.su ./MQTTC/MQTTPacket/MQTTPacket.cyclo ./MQTTC/MQTTPacket/MQTTPacket.d ./MQTTC/MQTTPacket/MQTTPacket.o ./MQTTC/MQTTPacket/MQTTPacket.su ./MQTTC/MQTTPacket/MQTTSerializePublish.cyclo ./MQTTC/MQTTPacket/MQTTSerializePublish.d ./MQTTC/MQTTPacket/MQTTSerializePublish.o ./MQTTC/MQTTPacket/MQTTSerializePublish.su ./MQTTC/MQTTPacket/MQTTSubscribeClient.cyclo ./MQTTC/MQTTPacket/MQTTSubscribeClient.d ./MQTTC/MQTTPacket/MQTTSubscribeClient.o ./MQTTC/MQTTPacket/MQTTSubscribeClient.su ./MQTTC/MQTTPacket/MQTTSubscribeServer.cyclo ./MQTTC/MQTTPacket/MQTTSubscribeServer.d ./MQTTC/MQTTPacket/MQTTSubscribeServer.o ./MQTTC/MQTTPacket/MQTTSubscribeServer.su ./MQTTC/MQTTPacket/MQTTUnsubscribeClient.cyclo ./MQTTC/MQTTPacket/MQTTUnsubscribeClient.d ./MQTTC/MQTTPacket/MQTTUnsubscribeClient.o ./MQTTC/MQTTPacket/MQTTUnsubscribeClient.su ./MQTTC/MQTTPacket/MQTTUnsubscribeServer.cyclo ./MQTTC/MQTTPacket/MQTTUnsubscribeServer.d ./MQTTC/MQTTPacket/MQTTUnsubscribeServer.o ./MQTTC/MQTTPacket/MQTTUnsubscribeServer.su

.PHONY: clean-MQTTC-2f-MQTTPacket

