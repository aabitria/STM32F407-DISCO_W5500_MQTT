################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../W5500/socket.c \
../W5500/w5500_spi.c \
../W5500/wizchip_conf.c 

OBJS += \
./W5500/socket.o \
./W5500/w5500_spi.o \
./W5500/wizchip_conf.o 

C_DEPS += \
./W5500/socket.d \
./W5500/w5500_spi.d \
./W5500/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
W5500/%.o W5500/%.su W5500/%.cyclo: ../W5500/%.c W5500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../MQTTC -I../MQTTC/MQTTPacket -I../W5500 -I../W5500/W5500 -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-W5500

clean-W5500:
	-$(RM) ./W5500/socket.cyclo ./W5500/socket.d ./W5500/socket.o ./W5500/socket.su ./W5500/w5500_spi.cyclo ./W5500/w5500_spi.d ./W5500/w5500_spi.o ./W5500/w5500_spi.su ./W5500/wizchip_conf.cyclo ./W5500/wizchip_conf.d ./W5500/wizchip_conf.o ./W5500/wizchip_conf.su

.PHONY: clean-W5500

