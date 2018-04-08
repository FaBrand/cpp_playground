#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "udp_server_dynamic_buffer.h"

int main()
{
    std::cout << "Listening" << std::endl;
    UdpServerDynamicBuffer server;

    server.register_receiver([&server](const auto& msg) {
        if (msg.size() > 0)
        {
            std::cout << "Received: " << std::to_string(msg.front()) << std::endl;
            if (msg.front() >= 100)
            {
                server.RequestStop();
            }
        }
    });

    server.Start();
    while (server.IsRunning())
    {
    }

    std::cout << "Finished" << std::endl;
    return 0;
}
