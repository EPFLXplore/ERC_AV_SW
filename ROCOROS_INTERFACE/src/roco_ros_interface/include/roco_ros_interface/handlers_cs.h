/* DESCRIPTION

This file contains all the handle functions for the different packets received
from RoCo.

Probably have to use boost:bind to be able to pass publisher objects to
handlers.
*/

#include "ros/ros.h"
// add the different types od data needed
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/UInt32MultiArray.h"
#include "std_msgs/Float32.h"
#include "std_msgs/UInt32.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Bool.h"

#include <sstream>
#include <string>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <thread>
#include <cstring>

#include "RoCo.h"

/* MULTI ARRAY
 are made of a
 layout  // specification of data layout
 data   // array of data
 */

//----------callbacks ----------

// receive data from ROS and send to RoCo

void reset_power_supply_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  std::cout<<msg->data<<std::endl;
  Reset_PowerSupplyPacket packet;
  packet.reset = msg->data;
  sender->send<Reset_PowerSupplyPacket>(&packet);
  std::cout<<packet.reset<<" sent!"<<std::endl;
}

void switch_avionics_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  Switch_AvionicsPacket packet;
  packet.on = msg->data;
  sender->send<Switch_AvionicsPacket>(&packet);
}

void switch_raman_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  Switch_RamanPacket packet;
  packet.on = msg->data;
  sender->send<Switch_RamanPacket>(&packet);
}

void switch_jetson_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  Switch_JetsonPacket packet;
  packet.on = msg->data;
  sender->send<Switch_JetsonPacket>(&packet);
}

void switch_lidar_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  Switch_LidarPacket packet;
  packet.on = msg->data;
  sender->send<Switch_LidarPacket>(&packet);
}

void switch_ethernet_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  Switch_EthernetPacket packet;
  packet.on = msg->data;
  sender->send<Switch_EthernetPacket>(&packet);
}

void switch_LED_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  sc_LED_packet packet;
  packet.on = msg->data;
  std::cout << "LED SENT" << std::endl;
  sender->send<sc_LED_packet>(&packet);
}

void switch_trap_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  sc_trap_packet packet;
  packet.open = msg->data;
    std::cout << "TRAP SENT!" << std::endl;
  sender->send<sc_trap_packet>(&packet);
}

void switch_caching_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  sc_caching_packet packet;
  packet.open = msg->data;
  std::cout << "CACHING SENT" << std::endl;
  sender->send<sc_caching_packet>(&packet);
}

void hd_voltmeter_motor_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  hd_voltmeter_motor_packet packet;
  packet.extended = msg->data;
  sender->send<hd_voltmeter_motor_packet>(&packet);
}

void hd_voltmeter_tare_callback(const boost::shared_ptr<std_msgs::Bool const> msg, NetworkBus* sender)
{
  hd_voltmeter_tare_packet packet;
  packet.tare = msg->data;
  std::cout << "GOING THROUGH THE TARE CALLBACK" << std::endl;
  sender->send<hd_voltmeter_tare_packet>(&packet);
}


//----------handlers----------

// receive data from RoCo and send to ROS

void handle_IMU(uint8_t sender_id, avionics_IMU_packet* packet, void* ros_publisher)
{
  sensor_msgs::Imu msg;
  //Clear array
	// msg.data.clear();
  msg.angular_velocity.x = packet->angular_velocity[0];
  msg.angular_velocity.y = packet->angular_velocity[1];
  msg.angular_velocity.z = packet->angular_velocity[2];

  msg.linear_acceleration.x = packet->acceleration[0];
  msg.linear_acceleration.y = packet->acceleration[1];
  msg.linear_acceleration.z = packet->acceleration[2];

  msg.orientation.w = packet->orientation[0];
  msg.orientation.x = packet->orientation[1];
  msg.orientation.y = packet->orientation[2];
  msg.orientation.z = packet->orientation[3];

  // msg.data.push_back(packet->acceleration);
  // msg.data.push_back(packet->angular_velocity);
  // msg.data.push_back(packet->magnetometer);

  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_TOF(uint8_t sender_id, avionics_ToF_packet* packet, void* ros_publisher)
{
  std_msgs::Float32 msg;
  //Clear array
	msg.data = 0;

  msg.data = packet->distance;

  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_voltmeter(uint8_t sender_id, avionics_voltmeter_packet* packet, void* ros_publisher)
{
  std_msgs::Float32 msg;
  //Clear array
  // std::cout << "Voltage : " << packet->voltage << std::endl;
	msg.data = 0;

  msg.data = packet->voltage;
  // std::cout << msg.data << std::endl;
  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_massload(uint8_t sender_id, avionics_massload_packet* packet, void* ros_publisher)
{
  std_msgs::Int32 msg;
  //Clear array
	msg.data = 0;

  msg.data = packet->mass;

  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_moisture(uint8_t sender_id, avionics_moisture_packet* packet, void* ros_publisher)
{
  std_msgs::Float32 msg;
  //Clear array
	msg.data = 0;

  msg.data = packet->moisture;

  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_trap(uint8_t sender_id, avionics_trap_success_packet* packet, void* ros_publisher)
{
  std_msgs::Bool msg;
  //Clear array
	msg.data = 0;

  msg.data = packet->status;
  ROS_INFO("Trap success");
  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_caching(uint8_t sender_id, avionics_caching_success_packet* packet, void* ros_publisher)
{
  std_msgs::Bool msg;
  //Clear array
	msg.data = 0;

  msg.data = packet->status;
  ROS_INFO("caching success");
  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_system(uint8_t sender_id, Power_SystemPacket* packet, void* ros_publisher)
{
  std_msgs::Float32MultiArray msg;
  //Clear array
	msg.data.clear();

  msg.data.push_back(packet->battery_charge);
  msg.data.push_back(packet->state);

  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_voltages(uint8_t sender_id, Power_VoltagePacket* packet, void* ros_publisher)
{
  std_msgs::Float32MultiArray msg;
  //Clear array
	msg.data.clear();
  for (int i(0); i < 3; ++i) msg.data.push_back(packet->voltages[i]);

  ((ros::Publisher *)ros_publisher)->publish(msg);
}

void handle_currents(uint8_t sender_id, Power_CurrentPacket* packet, void* ros_publisher)
{
  std_msgs::Float32MultiArray msg;
  //Clear array
	msg.data.clear();
  for (int i(0); i < 3; ++i) msg.data.push_back(packet->currents[i]);

  ((ros::Publisher *)ros_publisher)->publish(msg);
}

