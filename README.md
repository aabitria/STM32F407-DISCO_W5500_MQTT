# STM32F4 MQTT over W5500 Ethernet

This is just a project that explores MQTT connectivity over Ethernet, not just Ethernet but using W5500 chips that has Ethernet and TCP/IP stack in it.

This project uses the following hardware:
- STM32F407-DISCO board
- W5500 board
- PC/laptop for MQTT broker and client
- wifi router with LAN ports
- Android phone with MyMQTT.
- Ethernet cable

This project uses ioDriver library from Wiznet (manufacturer of W5500) to accomplish all PHY, TCP/IP, DHCP, and socket connections.  Since its IP connection is DHCP, there is less need to specify the static IP address; just rely on the DHCP server to give us the IP addr.  It also uses PAHO Embedded-C MQTT Client library to implement the MQTT block that sits above W5500 TCP/IP.

In the host PC, mosquitto is the MQTT broker being used.  For now, its IP address is fixed and hard-coded into the MCU.  The STM32F4, the phone, and the MQTTX application within the PC are the clients.  The STM32F4 subscribes to certain topics, which MQTTX publishes to.  There are application handlers in STM32F4 firmware code that will respond whenever there are new messages on the subscribed topics.


# STM32F407-DISCO to W5500 Board Pinout

STM32F407	W5500	COLOR			PURPOSE
3V			3.3V	RED
GND			GND		BROWN/BLACK
PA1			SCS		ORANGE			CHIP SELECT
PA4			RST		VIOLET			CHIP RESET
PA5			SCLK	YELLOW			SPI CLK
PA6			MISO	BLUE			SPI MISO
PA7			MOSI	GREEN			SPI MOSI

PA2					WHITE			BOARD UART TX
PA3					GREEN			BOARD UART RX