#ifndef UDP_SERVER_SYNCHRONOUS_H
#define UDP_SERVER_SYNCHRONOUS_H

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include "signals/signal.h"

using boost::asio::ip::udp;

class UdpServer
{
  public:
    using UdpMessage = std::vector<std::uint8_t>;

    ~UdpServer();
    void Start();
    void Stop();
    bool IsRunning() const;

    template <typename... Arg>
    void register_receiver(Arg&&... arg)
    {
        signal_.connect(std::forward<Arg>(arg)...);
    }

    UdpMessage GetLastMessageBytes() const;

  private:
    Signal<const UdpMessage&> signal_;
    static const std::size_t kServerBufferSize{1500};
    boost::asio::io_service io_service_{};

    static const std::uint32_t kServerDefaultPort{4567};
    udp::socket socket_{io_service_, udp::endpoint(udp::v4(), kServerDefaultPort)};
    udp::endpoint remote_endpoint_;

    std::array<std::uint8_t, kServerBufferSize> recv_buffer_;
    UdpMessage last_received_message_;

    void UpdateMessageBuffer(std::size_t bytes_transferred);

    void StartReceive();
    void HandleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);
    void SignalSlots() const;

    bool IsServiceRunning() const;
    void StartService();
    void PrepareServiceForNextStart();
    void GracefullyStopService();
};

#endif  // UDP_SERVER_SYNCHRONOUS_H
