#include "udp_server_dynamic_buffer.h"
#include <iostream>

UdpServerDynamicBuffer::UdpServerDynamicBuffer()
    : io_service_(), socket_(io_service_, udp::endpoint(udp::v4(), kServerDefaultPort))
{
    StartReceive();
}

UdpServerDynamicBuffer::~UdpServerDynamicBuffer()
{
    Stop();
}

void UdpServerDynamicBuffer::StartReceive()
{
    socket_.async_receive(boost::asio::null_buffers(), [this](const auto& error, const auto& bytes_transferred) {
        this->HandleReceive(error, bytes_transferred);
    });
}

void UdpServerDynamicBuffer::HandleReceive(const boost::system::error_code& error, std::size_t)
{
    if (error == boost::asio::error::operation_aborted)
    {
        return;
    }

    ProcessNewMessage();

    StartReceive();
}

void UdpServerDynamicBuffer::ProcessNewMessage()
{
    const std::size_t available_bytes{socket_.available()};
    Message recv_buffer(available_bytes);

    boost::system::error_code receive_error;
    const boost::asio::socket_base::message_flags receive_flags{0};
    socket_.receive_from(
        boost::asio::buffer(recv_buffer, available_bytes), remote_endpoint_, receive_flags, receive_error);

    if (!receive_error)
    {
        UpdateMessageBufferWith(recv_buffer);
    }
    else
    {
        std::cerr << "Error: " << receive_error << std::endl;
    }
}

void UdpServerDynamicBuffer::UpdateMessageBufferWith(Message& new_message)
{
    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    last_received_message_.swap(new_message);
}

std::string UdpServerDynamicBuffer::GetLastMessageAsString() const
{
    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    return std::string(last_received_message_.begin(), last_received_message_.end());
}

UdpServerDynamicBuffer::Message UdpServerDynamicBuffer::GetLastMessageBytes() const
{
    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    return last_received_message_;
}

void UdpServerDynamicBuffer::Start()
{
    if (IsServiceRunning())
    {
        return;
    }

    StartServiceAsynchronously();
}

bool UdpServerDynamicBuffer::IsServiceRunning() const
{
    return thread_ != nullptr;
}

void UdpServerDynamicBuffer::StartServiceAsynchronously()
{
    thread_.reset(new boost::thread([this]() { io_service_.run(); }));
}

void UdpServerDynamicBuffer::Stop()
{
    if (!IsServiceRunning())
    {
        return;
    }

    GracefullyStopService();
    PrepareServiceForNextStart();
}

void UdpServerDynamicBuffer::GracefullyStopService()
{
    io_service_.stop();
    thread_->join();
}

void UdpServerDynamicBuffer::PrepareServiceForNextStart()
{
    io_service_.reset();
    thread_.reset();
}

bool UdpServerDynamicBuffer::IsRunning() const
{
    return IsServiceRunning();
}
