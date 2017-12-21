#include "udp_server.h"
#include <iostream>

UdpServer::UdpServer() : io_service_(), socket_(io_service_, udp::endpoint(udp::v4(), kServerDefaultPort))
{
    StartReceive();
}

UdpServer::~UdpServer()
{
    Stop();
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
        const std::size_t message_length{std::min(bytes_transferred, recv_buffer_.size())};
        std::vector<std::uint8_t> new_message(recv_buffer_.begin(), recv_buffer_.begin() + message_length);

        std::lock_guard<std::mutex> buffer_lock(buffer_access_);
        last_received_message_.swap(new_message);
    }
    else
    {
        std::cout << "Error: " << error << std::endl;
    }

    StartReceive();
}

std::string UdpServer::GetLastMessageAsString() const
{
    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    return std::string(last_received_message_.begin(), last_received_message_.end());
}

std::vector<std::uint8_t> UdpServer::GetLastMessageBytes() const
{
    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    return last_received_message_;
}

void UdpServer::Start()
{
    if (IsServiceRunning())
    {
        return;
    }

    StartServiceAsynchronously();
}

bool UdpServer::IsServiceRunning() const
{
    return thread_ != nullptr;
}

void UdpServer::StartServiceAsynchronously()
{
    thread_.reset(new boost::thread([this]() { io_service_.run(); }));
}

void UdpServer::Stop()
{
    if (!IsServiceRunning())
    {
        return;
    }

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

bool UdpServer::IsRunning() const
{
    return IsServiceRunning();
}
