/*
 * LWIPThread.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: AV Team 2020
 */

#include "LWIPThread.h"

#include "Debug/Debug.h"

#include "lwip.h"


static struct netif gnetif; // global network interface
static void onStatusUpdate(struct netif *netif);

LWIPThread::LWIPThread(const char* ip, const uint16_t port) : Thread("Client thread"), ip(ip), port(port) {

}

void LWIPThread::init() {
	osDelay(50); // Time to get the shell loaded by Cortex M4

	console.printf("Initializing LWIP...\n");

	tcpip_init( NULL, NULL );

	/* IP addresses initialization with DHCP (IPv4) */
	local_ip = 0;
	netmask = 0;
	gateway = 0;

	/* add the network interface (IPv4/IPv6) with RTOS */
	netif_add(&gnetif, (ip4_addr*) &local_ip, (ip4_addr*) &netmask, (ip4_addr*) &gateway, NULL, &ethernetif_init, &tcpip_input);

	/* Registers the default network interface */
	netif_set_default(&gnetif);

	onStatusUpdate(&gnetif);

	netif_set_link_callback(&gnetif, onStatusUpdate);

	osThreadDef(EthLink, ethernet_link_thread, osPriorityNormal, 0, 1024);
	osThreadCreate (osThread(EthLink), &gnetif);

	dhcp_start(&gnetif);

}

void onStatusUpdate(struct netif *netif) {
	if (netif_is_link_up(netif)) {
		/* When the netif is fully configured this function must be called */
		netif_set_up(netif);
		console.printf("Link is up\n");
	} else {
		/* When the netif link is down this function must be called */
		netif_set_down(netif);
		console.printf("Link is down\n");
	}
}

void LWIPThread::loop() {

}
