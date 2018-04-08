#include "udp_server_dynamic_buffer.h"
#include <iostream>

UdpServerDynamicBuffer::~UdpServerDynamicBuffer()
{
    Stop();
}

void UdpServerDynamicBuffer::Start()
{
    StartReceive();
    StartServiceAsynchronously();
}

void UdpServerDynamicBuffer::Stop()
{
    GracefullyStopService();
    PrepareServiceForNextStart();
}

bool UdpServerDynamicBuffer::IsRunning() const
{
    return !io_service_.stopped();
}

void UdpServerDynamicBuffer::RequestStop()
{
    io_service_.stop();
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
    SignalSlots();
    StartReceive();
}

void UdpServerDynamicBuffer::ProcessNewMessage()
{
    const std::size_t available_bytes{socket_.available()};
    UdpMessage recv_buffer(available_bytes);

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

void UdpServerDynamicBuffer::SignalSlots() const
{
    signal_.emit(GetLastMessageBytes());
}

void UdpServerDynamicBuffer::UpdateMessageBufferWith(UdpMessage& new_message)
{
    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    last_received_message_.swap(new_message);
}

UdpServerDynamicBuffer::UdpMessage UdpServerDynamicBuffer::GetLastMessageBytes() const
{
    std::lock_guard<std::mutex> buffer_lock(buffer_access_);
    return last_received_message_;
}

void UdpServerDynamicBuffer::StartServiceAsynchronously()
{
    thread_.reset(new boost::thread([this]() { io_service_.run(); }));
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
