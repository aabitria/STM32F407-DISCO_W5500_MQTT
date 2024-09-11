/*
 * w5500_network.c
 *
 *  Created on: Sep 11, 2024
 *      Author: Lenovo310
 */
#include "w5500_network.h"

#define DHCP_BUFF_SIZE      	(2048)
#define SOCKET_APP				(1)
#define SOCKET_DHCP      		(7)

wiz_NetInfo netinfo = {
	.mac = {0x80, 0x34, 0x28, 0x74, 0xA5, 0xCB},
	.ip = {192, 168, 254, 230},
	.sn = {255, 255, 254, 0},
	.gw = {192, 168, 254, 254},
	.dns = {8, 8, 8, 8},
	.dhcp = NETINFO_STATIC,
};


uint8_t dhcp_buffer[DHCP_BUFF_SIZE];

static int w5500_network_ip_init_flag = 0;


static void w5500_network_display_configuration(void)
{
	wiz_NetInfo info;

	ctlnetwork(CN_GET_NETINFO, (void *)&info);

	printf("\r\n");
	printf(" MAC         : %02X:%02X:%02X:%02X:%02X:%02X\r\n", info.mac[0], info.mac[1], info.mac[2], info.mac[3], info.mac[4], info.mac[5]);
	printf(" IP          : %d.%d.%d.%d\r\n", info.ip[0], info.ip[1], info.ip[2], info.ip[3]);
	printf(" Subnet Mask : %d.%d.%d.%d\r\n", info.sn[0], info.sn[1], info.sn[2], info.sn[3]);
	printf(" Gateway     : %d.%d.%d.%d\r\n", info.gw[0], info.gw[1], info.gw[2], info.gw[3]);
	printf("====================================================================================================\r\n\n");
}

static void w5500_network_phy_status_check (void)
{
	uint8_t temp;

	printf("\n\rChecking cable presence...");

	do {
		ctlwizchip(CW_GET_PHYLINK, (void *)&temp);

		if (temp == PHY_LINK_OFF) {
			printf("\n\rNo Cable Connected...");
			HAL_Delay(1000);
		}
	} while(temp == PHY_LINK_OFF);

	// If it gets out of the loop, then connection succeeded.
	printf("\n\rEthernet Cable is now connected!");
}

static void w5500_network_phy_print_conf (void)
{
	wiz_PhyConf phyconf;

	ctlwizchip(CW_GET_PHYCONF, (void*) &phyconf);

	if (phyconf.by == PHY_CONFBY_HW) {
		printf("\n\rPHY Configured by Hardware Pins");
	} else {
		printf("\n\rPHY Configured by Registers");
	}

	if (phyconf.mode == PHY_MODE_AUTONEGO) {
		printf("\n\rAutonegotiation Enabled");
	} else {
		printf("\n\rAutonegotiation NOT Enabled");
	}

	if (phyconf.duplex == PHY_DUPLEX_FULL) {
		printf("\n\rDuplex Mode: Full");
	} else {
		printf("\n\rDuplex Mode: Half");
	}

	if (phyconf.speed == PHY_SPEED_10) {
		printf("\n\rSpeed: 10Mbps");
	} else {
		printf("\n\rSpeed: 100Mbps");
	}
}


static void w5500_network_on_dhcp_ip_conflict (void)
{
	while(1);
}

static void w5500_network_on_dhcp_ip_assigned (void)
{
	printf("IP obtained: \r\n");

	getIPfromDHCP(netinfo.ip);
	getGWfromDHCP(netinfo.gw);
	getSNfromDHCP(netinfo.sn);
	getDNSfromDHCP(netinfo.dns);

	netinfo.dhcp = NETINFO_DHCP;

	ctlnetwork(CN_SET_NETINFO, (void *)&netinfo);

	w5500_network_display_configuration();

	w5500_network_ip_init_flag = 1;
}

int w5500_network_is_connected (void)
{
	return w5500_network_ip_init_flag;
}

void w5500_network_dhcp_loop (void)
{
	//uint8_t ret;

	DHCP_run();

	//ret = DHCP_run();
	//if (ret != DHCP_IP_LEASED) {
	//	printf("\r\nFailed to get DHCP IP...");
	//	while(1);
	//}
}

void w5500_network_dhcp_init (void)
{
	DHCP_init(SOCKET_DHCP, dhcp_buffer);
	reg_dhcp_cbfunc(w5500_network_on_dhcp_ip_assigned,
					w5500_network_on_dhcp_ip_assigned,
					w5500_network_on_dhcp_ip_conflict);
}

void w5500_network_phy_conf (void)
{
	wiz_PhyConf phyconf;

	phyconf.by = PHY_CONFBY_SW;
	phyconf.duplex = PHY_DUPLEX_FULL;
	phyconf.speed = PHY_SPEED_10;
	phyconf.mode = PHY_MODE_AUTONEGO;

	ctlwizchip(CW_SET_PHYCONF, (void *)&phyconf);

	w5500_network_phy_status_check();
	w5500_network_phy_print_conf();
}



