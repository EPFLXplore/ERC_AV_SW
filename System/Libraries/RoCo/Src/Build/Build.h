/*
 * Build.h
 *
 *  Created on: 3 May 2020
 *      Author: Arion
 */

#ifndef BUILD_BUILD_H_
#define BUILD_BUILD_H_


#define PROTOCOL_21W3



#if defined(BUILD_FOR_CONTROL_STATION)
	#define BUILD_WITH_NETWORK_CLIENT_IO
	#define BUILD_WITH_NETWORK_BUS
#elif defined(BUILD_FOR_NAVIGATION)
	#define BUILD_WITH_NETWORK_SERVER_IO
	#define BUILD_WITH_NETWORK_BUS
#elif defined(BUILD_FOR_AVIONICS_CM7)
	#define BUILD_WITH_LWIP_CLIENT_IO
	#define BUILD_WITH_BUFFERED_IO
	#define BUILD_WITH_NETWORK_BUS
#elif defined(BUILD_FOR_AVIONICS_CM4)
	#define BUILD_WITH_BUFFERED_IO
	#define BUILD_WITH_NETWORK_BUS
#elif defined(BUILD_FOR_TESTING)
	#define BUILD_WITH_NETWORK_SERVER_IO
	#define BUILD_WITH_NETWORK_CLIENT_IO
	#define BUILD_WITH_NETWORK_BUS
	#define BUILD_WITH_LOOPBACK_BUS
#else
	#error "Please specify a build target"
#endif


#endif /* BUILD_BUILD_H_ */