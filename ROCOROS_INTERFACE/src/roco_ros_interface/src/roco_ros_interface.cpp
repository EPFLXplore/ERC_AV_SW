#include "ros/ros.h"
#include "std_msgs/String.h"

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
  UDevDriver* driver = new UDevDriver("/dev/ttyUSB0");


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

  ros::Rate loop_rate(1);
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
   * is the number+ of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  // ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);



  // bus->handle<avionics_voltmeter_packet>(&handle_voltmeter);
  ros::Publisher led_pub = n.advertise<std_msgs::Bool>("avionics_led", 1000);
  ros::Publisher trap_pub = n.advertise<std_msgs::Bool>("avionics_trap", 1000);
  ros::Publisher voltage_pub = n.advertise<std_msgs::Float32>("avionics_voltage", 1000);
  // ros::Publisher voltage_pub = n.advertise<std_msgs::Float32>("avionics_voltage", 1000);
  // ros::Publisher voltage_pub = n.advertise<std_msgs::Float32>("avionics_voltage", 1000);
  // ros::Publisher voltage_pub = n.advertise<std_msgs::Float32>("avionics_voltage", 1000);
  // ros::Publisher voltage_pub = n.advertise<std_msgs::Float32>("avionics_voltage", 1000);

  ros::Subscriber led_sub = n.subscribe<std_msgs::Bool>("avionics_led", 1000, boost::bind(switch_LED_callback, _1, bus));
  ros::Subscriber trap_sub = n.subscribe<std_msgs::Bool>("avionics_trap", 1000,  boost::bind(switch_trap_callback, _1, bus));
  ros::Subscriber caching_sub = n.subscribe<std_msgs::Bool>("avionics_caching", 1000, boost::bind(switch_caching_callback, _1, bus));

  // ros::Subscriber voltage_sub = n.subscribe("avionics_voltage", 1000, testCallback);
  // bus->handle<avionics_voltmeter_packet>(&handle_voltmeter, (void*)&voltage_pub);
  bus->handle<avionics_voltmeter_packet>(&handle_voltmeter, (void*)&voltage_pub);
  std_msgs::Bool msg;
  msg.data = false;
  
  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  while (ros::ok())
  {

     msg.data = !msg.data;
     ROS_INFO("LED state : [%d]", msg.data);
     led_pub.publish(msg);
    //  std::cout << "MAIN LOOP LED SENT" << std::endl;
     trap_pub.publish(msg);
    //  std::cout << "MAIN LOOP TRAP SENT" << std::endl;
    // loop to try to (re)connect to server if disconnected
    // if(!(client->is_connected()))
    // {
    //   int result = client->connectClient();
    //   if(result < 0) {
  	//     std::cout << "Power Supply connection failed with error code " << result << std::endl;
  	//     std::cout << std::strerror(errno) << std::endl;
    //   }
    // }

    // used to handle ros communication events, i.e. callback to function
    // ros::spinOnce();

    // enforces the frequency at which this while loop runs (here at 1H z, see above)
    // loop_rate.sleep();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}

void testCallback(const std_msgs::Float32& msg){
  ROS_INFO("Voltage is : [%f]", msg.data);
}