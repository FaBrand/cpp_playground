#include "udp_server.h"
#include <iostream>

UdpServer::~UdpServer()
{
    Stop();
}

void UdpServer::RequestStop()
{
    io_service_.stop();
}

void UdpServer::Start()
{
    StartServiceAsynchronously();
    StartReceive();
}

void UdpServer::StartServiceAsynchronously()
{
    thread_.reset(new boost::thread([&]() { io_service_.run(); }));
}

void UdpServer::Stop()
{
    GracefullyStopService();
    PrepareServiceForNextStart();
}

void UdpServer::GracefullyStopService()
{
    io_service_.stop();
    thread_->join();
}

void UdpServer::PrepareServiceForNextStart()
{
    io_service_.reset();
    thread_.reset();
}

void UdpServer::StartReceive()
{
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        std::bind(&UdpServer::HandleReceive, this, std::placeholders::_1, std::placeholders::_2));
}

void UdpServer::HandleReceive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error || error == boost::asio::error::message_size)
    {
        UpdateMessageBuffer(bytes_transferred);
    }
    else
    {
        std::cout << "Error: " << error << std::endl;
    }

    StartReceive();
    SignalSlots();
}

void UdpServer::UpdateMessageBuffer(std::size_t bytes_transferred)
{
    // Truncate the message rather than overflowing the buffer
    const std::size_t message_length{std::min(bytes_transferred, recv_buffer_.size())};

    UdpMessage new_message(recv_buffer_.begin(), recv_buffer_.begin() + message_length);

    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    last_received_message_.swap(new_message);
}

void UdpServer::SignalSlots() const
{
    signal_.emit(GetLastMessageBytes());
}

UdpServer::UdpMessage UdpServer::GetLastMessageBytes() const
{
    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    return last_received_message_;
}

bool UdpServer::IsRunning() const
{
    return !io_service_.stopped();
}
