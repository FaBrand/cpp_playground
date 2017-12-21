#ifndef UDP_SERVER_DYNAMIC_BUFFER_H
#define UDP_SERVER_DYNAMIC_BUFFER_H

#include <array>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::udp;

class UdpServerDynamicBuffer
{
    typedef std::vector<std::uint8_t> Message;

  public:
    UdpServerDynamicBuffer();
    ~UdpServerDynamicBuffer();
    UdpServerDynamicBuffer& operator=(const UdpServerDynamicBuffer&) = delete;

    void Start();
    void Stop();
    bool IsRunning() const;
    std::string GetLastMessageAsString() const;
    UdpServerDynamicBuffer::Message GetLastMessageBytes() const;

  private:
    static const std::uint32_t kServerDefaultPort{4567};

    mutable std::mutex buffer_access_;
    std::unique_ptr<boost::thread> thread_;
    boost::asio::io_service io_service_;

    udp::socket socket_;
    udp::endpoint remote_endpoint_;

    Message last_received_message_;

    void StartReceive();
    void HandleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

    void UpdateMessageBufferWith(Message& new_message);
    void ProcessNewMessage();
    bool IsServiceRunning() const;
    void StartServiceAsynchronously();
    void PrepareServiceForNextStart();
    void GracefullyStopService();
};

#endif  // UDP_SERVER_DYNAMIC_BUFFER_H
