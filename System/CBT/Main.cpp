#include "RoCo.h"

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <iostream>

#include "matplotlibcpp.h"


namespace plt = matplotlibcpp;


void onConnectPacket(uint8_t sender, ConnectPacket* packet);
void onDopplerPacket(uint8_t sender, DopplerPacket* packet);


std::vector<float> x;
std::vector<float> y;

int main() {
    UDevDriver* driver = new UDevDriver("/dev/StaticProbe");

    struct termios tty;

    if(tcgetattr(driver->getFD(), &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    if (tcsetattr(driver->getFD(), TCSANOW, &tty) != 0) {
        std::cout << strerror(errno) << std::endl;
    }

    RadioBus* bus = new RadioBus(driver);

    bus->handle(&onConnectPacket);
    bus->handle(&onDopplerPacket);

    while(driver->isConnected()) {
        plt::clf();

		plt::plot(x, y);
		plt::title("Carrier frequency offset");
		plt::pause(0.02);

        std::this_thread::yield();
    }

    return 0;
}


void onConnectPacket(uint8_t sender, ConnectPacket* packet) {
    if(IS_RELIABLE(packet, ConnectPacket)) {
        std::cout << "Device '" << packet->name << "' connected" << std::endl;
    }
}

void onDopplerPacket(uint8_t sender, DopplerPacket* packet) {
    if(IS_RELIABLE(packet, DopplerPacket)) {
        x.push_back(packet->timestamp * 1e-9);
        y.push_back(packet->cfo * 1.0f);
        std::cout << packet->timestamp << "," << packet->cfo << std::endl;
    }

    if(x.size() > 1024) {
        x.erase(x.begin());
        y.erase(y.begin());
    }
}
