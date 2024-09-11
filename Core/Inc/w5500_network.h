/*
 * w5500_network.h
 *
 *  Created on: Sep 11, 2024
 *      Author: Lenovo310
 */

#ifndef _W5500_NETWORK_H_
#define _W5500_NETWORK_H_

#include <stdio.h>
#include "w5500_spi.h"
#include "wizchip_conf.h"
#include "dhcp.h"

void w5500_network_dhcp_loop (void);
int w5500_network_is_connected (void);
void w5500_network_dhcp_init (void);
void w5500_network_phy_conf (void);

#endif /* INC_W5500_NETWORK_H_ */
