/*
 * LWIPThread.cpp
 *
 *  Created on: Jan 23, 2021
 *      Author: arion
 */

#include "LWIPThread.h"

#include "Debug/Debug.h"

#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "ethernetif.h"

static void ethernet_link_status_updated(struct netif *netif);
/* ETH Variables initialization ----------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
/* USER CODE BEGIN OS_THREAD_ATTR_CMSIS_RTOS_V2 */
#define INTERFACE_THREAD_STACK_SIZE ( 1024 )


LWIPThread::LWIPThread(const char* ip, const uint16_t port) : Thread("Client thread"), ip(ip), port(port) {

}

void LWIPThread::init() {
	osDelay(50); // Time to get the shell loaded by Cortex M4

	console.printf("Initializing LWIP... ");

	/* Initilialize the LwIP stack with RTOS */
	tcpip_init( NULL, NULL );

	/* IP addresses initialization with DHCP (IPv4) */
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;

	/* add the network interface (IPv4/IPv6) with RTOS */

	netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
	osDelay(1000);

	/* Registers the default network interface */
	netif_set_default(&gnetif);

	if (netif_is_link_up(&gnetif))
	{
	/* When the netif is fully configured this function must be called */
	netif_set_up(&gnetif);
	}
	else
	{
	/* When the netif link is down this function must be called */
	netif_set_down(&gnetif);
	}

	/* Set the link callback function, this function is called on change of link status*/
	netif_set_link_callback(&gnetif, ethernet_link_status_updated);

	/* Create the Ethernet link handler thread */
	/* USER CODE BEGIN H7_OS_THREAD_NEW_CMSIS_RTOS_V2 */
	osThreadAttr_t attr;
	attr.name = "EthLink";
	attr.stack_size = INTERFACE_THREAD_STACK_SIZE;
	attr.priority = osPriorityBelowNormal;
	osThreadNew(ethernet_link_thread, &gnetif, &attr);
	/* USER CODE END H7_OS_THREAD_NEW_CMSIS_RTOS_V2 */

	/* Start DHCP negotiation for a network interface (IPv4) */
	dhcp_start(&gnetif);

	console.printf("done\n");
}

static void ethernet_link_status_updated(struct netif *netif) {
	console.printf("hello world");
}

void LWIPThread::loop() {

}
