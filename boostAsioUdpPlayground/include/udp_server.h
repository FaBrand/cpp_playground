#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <array>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::udp;

class UdpServer
{
  public:
    UdpServer();
    ~UdpServer();
    void Start();
    void Stop();
    bool IsRunning() const;
    std::string GetLastMessageAsString() const;
    std::vector<std::uint8_t> GetLastMessageBytes() const;

  private:
    static const std::uint32_t kServerDefaultPort{4567};
    static const std::size_t kServerBufferSize{1500};

    mutable std::mutex buffer_access_;
    std::unique_ptr<boost::thread> thread_;
    boost::asio::io_service io_service_;

    udp::socket socket_;
    udp::endpoint remote_endpoint_;

    std::array<std::uint8_t, kServerBufferSize> recv_buffer_;
    std::vector<std::uint8_t> last_received_message_;

    void StartReceive();
    void HandleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

    bool IsServiceRunning() const;
    void StartServiceAsynchronously();
    void PrepareServiceForNextStart();
    void GracefullyStopService();
};

#endif  // UDP_SERVER_H
