#include "w5500_spi.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

static uint8_t w5500_spi_read_write (uint8_t data)
{
	/* Transmit a byte, and receive a byte in return (full-duplex) */

	// Wait till TX FIFO has a free slot
	while ((hspi1.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

	*(__IO uint8_t *)&hspi1.Instance->DR = data;

	// Now wait till data arrives
	while ((hspi1.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);

	return (*(__IO uint8_t *)&hspi1.Instance->DR);
}
/* TODO: convert this to HAL */

static void w5500_spi_chip_select (void)
{
	HAL_GPIO_WritePin(SPI1_NCS_GPIO_Port, SPI1_NCS_Pin, GPIO_PIN_RESET);
}

static void w5500_spi_chip_deselect (void)
{
	HAL_GPIO_WritePin(SPI1_NCS_GPIO_Port, SPI1_NCS_Pin, GPIO_PIN_SET);
}

static uint8_t w5500_spi_read (void)
{
	/* send a dummy byte; only care for returned byte */
	uint8_t ret;
	ret = w5500_spi_read_write(0x0);
	return ret;
}

static void w5500_spi_write (uint8_t write_data)
{
	w5500_spi_read_write(write_data);
}

/* Burst functions */

static void w5500_spi_read_burst (uint8_t *buf, uint16_t len)
{
	for (uint16_t i = 0; i < len; i++) {
		*buf = w5500_spi_read_write(0x0);
		buf++;
	}
}

static void w5500_spi_write_burst (uint8_t *buf, uint16_t len)
{
	for (uint16_t i = 0; i < len; i++) {
		w5500_spi_read_write(*buf);
		buf++;
	}
}

static void w5500_spi_io_init (void)
{
	// re-initialize GPIO for SPI1 CS??
	// just a placeholder
}

void w5500_spi_init(void)
{
	//uint8_t tmp;
	uint8_t memsize[2][8] = {
								{2, 2, 2, 2, 2, 2, 2, 2,},
								{2, 2, 2, 2, 2, 2, 2, 2,}
							};
	/* 8 is for max independent sockets, 2 arrays is for socket tx/rx buffer size */

	w5500_spi_io_init();

	w5500_spi_chip_deselect();

	/* Reset the W5500 via a pulse on RST line */
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
	//tmp = 0xFF;
	//while (tmp--);
	HAL_Delay(2);
	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);

	reg_wizchip_cs_cbfunc(w5500_spi_chip_select, w5500_spi_chip_deselect);
	reg_wizchip_spi_cbfunc(w5500_spi_read, w5500_spi_write);
	reg_wizchip_spiburst_cbfunc(w5500_spi_read_burst, w5500_spi_write_burst);

	/* WIZCHIP Initialize IOCTL */
	if (ctlwizchip(CW_INIT_WIZCHIP, (void *)memsize) == -1) {
		while(1);
	}

	/* ctlwizchip can also be used for configuring PHY and getting PHY status */
}
