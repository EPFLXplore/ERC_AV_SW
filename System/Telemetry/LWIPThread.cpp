/*
 * LWIPThread.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: AV Team 2020
 */

#include "LWIPThread.h"

#include "Telemetry.h"
#include "Lang/Operators.h"
#include "Debug/Debug.h"

#include "lwip.h"

#include <new>


static struct netif gnetif; // global network interface
static void onStatusUpdate(struct netif *netif);


static LWIPClientIO* client;


LWIPThread::LWIPThread(const char* ip, const uint16_t port) : Thread("Telemetry", 2048) {
	client = new LWIPClientIO(ip, port);
}

LWIPThread::~LWIPThread() {
	delete client;
}

void LWIPThread::init() {
	osDelay(50); // Time to get the shell loaded by Cortex M4

	println("Initializing LWIP...");

	tcpip_init(nullptr, nullptr);

	/* IP addresses initialization with DHCP (IPv4) */
	ip4_addr local_ip;
	ip4_addr netmask;
	ip4_addr gateway;

	IP4_ADDR(&local_ip, 192, 168, 1, 10);
	IP4_ADDR(&netmask, 255, 255, 0, 0);
	IP4_ADDR(&gateway, 255, 255, 255, 255);

	/* add the network interface (IPv4/IPv6) with RTOS */
	netif_add(&gnetif, &local_ip, &netmask, &gateway, NULL, &ethernetif_init, &tcpip_input);

	/* Registers the default network interface */
	netif_set_default(&gnetif);

	onStatusUpdate(&gnetif);

	netif_set_link_callback(&gnetif, onStatusUpdate);

	osThreadDef(EthLink, ethernet_link_thread, osPriorityNormal, 0, 1024);
	osThreadCreate(osThread(EthLink), &gnetif);
}

void onStatusUpdate(struct netif *netif) {
	if (netif_is_link_up(netif)) {
		/* When the netif is fully configured this function must be called */
		netif_set_up(netif);
		console.printf("[Telemetry] Link is up\r\n");
		int32_t error = client->connectClient();

		if(error != 0) {
			console.printf("[Telemetry] Cannot connect to server with error code %d\r\n", error);
		}
	} else {
		/* When the netif link is down this function must be called */
		netif_set_down(netif);
		console.printf("[Telemetry] Link is down\r\n");
		client->disconnectClient();
	}
}

void LWIPThread::loop() {
	client->update(); // Handle reception
	telemetryDriver.flush(client); // Handle transmission
}
