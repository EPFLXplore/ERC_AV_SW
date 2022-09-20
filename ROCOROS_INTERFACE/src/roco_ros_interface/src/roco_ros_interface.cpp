#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Imu.h"

#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <iostream>
#include <unistd.h>
#include <chrono>

#include "RoCo.h"
#include "handlers_cs.h"

static sc_LED_packet packet;
static sc_trap_packet trap_packet;
static avionics_voltmeter_packet voltmeter_packet;

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

void testCallback(const std_msgs::Float32& msg);

int main(int argc, char **argv)
{
  UDevDriver* driver = new UDevDriver("/dev/ttyUSB1");


  struct termios tty;

  if(tcgetattr(driver->getFD(), &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
  }

  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;
  tty.c_cflag &= ~HUPCL;

  tty.c_lflag = 0;

  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag |= 0; 
  tty.c_iflag &= ~(IGNPAR | IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL | INPCK | IMAXBEL);
  tty.c_iflag &= ~IUTF8;

  tty.c_oflag = 0;

  tty.c_cc[VTIME] = 0;
  tty.c_cc[VMIN] = 0;

  cfsetispeed(&tty, B576000);
  cfsetospeed(&tty, B576000);

  if (tcsetattr(driver->getFD(), TCSANOW, &tty) != 0) {
      std::cout << strerror(errno) << std::endl;
  }

  NetworkBus* bus = new NetworkBus(driver);

  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  // ros::Rate loop_rate(1000);
  ros::Rate loop_rate(1000);
  ros::Rate wait(1);

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */

  // Implementation of ros publishers. Those mainly concern data from sensors. It also includes success messages
  // for science tasks.
  ros::Publisher voltage_pub = n.advertise<std_msgs::Float32>("avionics_voltage", 1000);
  ros::Publisher imu_pub = n.advertise<sensor_msgs::Imu>("avionics_imu", 1000);
  ros::Publisher tof_pub = n.advertise<std_msgs::Float32>("avionics_tof", 1000);
  ros::Publisher massload_pub = n.advertise<std_msgs::Int32>("avionics_massload", 1000);
  ros::Publisher moisture_pub = n.advertise<std_msgs::Float32>("avionics_moisture", 1000);
  ros::Publisher caching_pub = n.advertise<std_msgs::Bool>("avionics_caching_success", 1000);
  ros::Publisher trap_pub = n.advertise<std_msgs::Bool>("avionics_trap_success", 1000);

  // Implementation of ros subscribers. Whenever something is published in the following topics. A function will 
  // be called and transform the ros message into a roco packet which will be sent to its corresponding target.
  ros::Subscriber led_sub = n.subscribe<std_msgs::Bool>("sc_led", 1000, boost::bind(switch_LED_callback, _1, bus));
  ros::Subscriber trap_sub = n.subscribe<std_msgs::Bool>("sc_trap", 1000,  boost::bind(switch_trap_callback, _1, bus));
  ros::Subscriber caching_sub = n.subscribe<std_msgs::Bool>("sc_caching", 1000, boost::bind(switch_caching_callback, _1, bus));
  ros::Subscriber voltmeter_motor_sub = n.subscribe<std_msgs::Bool>("hd_voltmeter_motor", 1000, boost::bind(hd_voltmeter_motor_callback, _1, bus));
  ros::Subscriber voltmeter_tare_sub = n.subscribe<std_msgs::Bool>("hd_voltmeter_tare", 1000, boost::bind(hd_voltmeter_tare_callback, _1, bus));

  // Implementation of handlers for functions to be called whenever a specific packet arrives on the bus.
  // Those handles will unpack the incoming message and publish on their respective topics in ROS.
  bus->handle<avionics_voltmeter_packet>(&handle_voltmeter, (void*)&voltage_pub);
  bus->handle<avionics_IMU_packet>(&handle_IMU, (void*)&imu_pub);
  bus->handle<avionics_ToF_packet>(&handle_TOF, (void*)&tof_pub);
  bus->handle<avionics_massload_packet>(&handle_massload, (void*)&massload_pub);
  bus->handle<avionics_moisture_packet>(&handle_moisture, (void*)&moisture_pub);
  bus->handle<avionics_caching_success_packet>(&handle_caching, (void*)&caching_pub);
  bus->handle<avionics_trap_success_packet>(&handle_trap, (void*)&trap_pub);
  
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}

void testCallback(const std_msgs::Float32& msg){
  ROS_INFO("Voltage is : [%f]", msg.data);
}