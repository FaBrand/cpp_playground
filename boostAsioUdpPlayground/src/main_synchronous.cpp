#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <boost/asio.hpp>
#include "udp_server_synchronous.h"

int main()
{
    std::cout << "Listening" << std::endl;
    UdpServer server;

    server.register_receiver([](const auto& msg) {
        if (msg.size() > 0)
        {
            std::cout << "Received: " << std::to_string(msg.front()) << std::endl;
        }
    });

    server.Start();

    std::cout << "Finished" << std::endl;
    return 0;
}
