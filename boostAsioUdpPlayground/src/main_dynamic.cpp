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
    server.Start();

    while (server.IsRunning())
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);

        const auto msg{server.GetLastMessageBytes()};

        if (msg.size() > 0)
        {
            std::cout << "Received: " << std::to_string(msg.front()) << std::endl;

            if (msg.front() > 100)
            {
                server.Stop();
            }
        }
    }

    std::cout << "Finished" << std::endl;
    return 0;
}
