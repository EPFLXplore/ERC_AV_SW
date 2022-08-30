/*
 * Build.h
 *
 *  Created on: 3 May 2020
 *      Author: Arion
 */

#ifndef BUILD_BUILD_H_
#define BUILD_BUILD_H_


//#define PROTOCOL_20W18
#define PROTOCOL_22W69

//#define BUILD_FOR_HANDLING_DEVICE
//#define BUILD_FOR_NAVIGATION
//#define BUILD_FOR_SCIENCE_A
#define BUILD_FOR_SCIENCE_B

#define BUILD_WITH_NETWORK_BUS

//#if defined(BUILD_FOR_CONTROL_STATION)
////	#define BUILD_WITH_NETWORK_CLIENT_IO
//	#define BUILD_WITH_NETWORK_BUS
//#elif defined(BUILD_FOR_NAVIGATION)
////	#define BUILD_WITH_NETWORK_SERVER_IO
//	#define BUILD_WIT H_NETWORK_BUS
//#elif defined(BUILD_FOR_AVIONICS)
//#elif defined(BUILD_FOR_HANDLING_DEVICE)
//#elif defined(BUILD_FOR_TESTING)
////	#define BUILD_WITH_NETWORK_SERVER_IO
////	#define BUILD_WITH_NETWORK_CLIENT_IO
//	#define BUILD_WITH_NETWORK_BUS
////	#define BUILD_WITH_LOOPBACK_BUS
//#elif defined(BUILD_WITH_STMUART)
//	#define CBT
//	#define BUILD_WITH_NETWORK_BUS
//#else
//	#error "Please specify a build target"
//#endif


#endif /* BUILD_BUILD_H_ */
