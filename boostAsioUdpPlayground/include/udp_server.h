#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <array>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "signals/signal.h"

using boost::asio::ip::udp;

class UdpServer
{
  public:
    ~UdpServer();
    void Start();
    void Stop();
    void RequestStop();
    bool IsRunning() const;

    template <typename... Arg>
    void register_receiver(Arg&&... arg)
    {
        signal_.connect(std::forward<Arg>(arg)...);
    }

    using UdpMessage = std::vector<std::uint8_t>;

  private:
    Signal<const UdpMessage&> signal_;

    mutable std::mutex buffer_access_;
    std::unique_ptr<boost::thread> thread_;
    boost::asio::io_service io_service_{};

    static const std::uint32_t kServerDefaultPort{4567};
    udp::socket socket_{io_service_, udp::endpoint(udp::v4(), kServerDefaultPort)};
    udp::endpoint remote_endpoint_;

    static const std::size_t kServerBufferSize{1500};
    std::array<std::uint8_t, kServerBufferSize> recv_buffer_;
    UdpMessage last_received_message_;

    void UpdateMessageBuffer(std::size_t bytes_transferred);
    UdpMessage GetLastMessageBytes() const;

    void StartReceive();
    void HandleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);
    void SignalSlots() const;

    void StartServiceAsynchronously();
    void PrepareServiceForNextStart();
    void GracefullyStopService();
};

#endif  // UDP_SERVER_H
