//
// Created by dilin on 11/19/17.
//

#include "ServerUDP.h"

ServerUDP::ServerUDP(boost::asio::io_service &io_service,
                     unsigned short port,
                     FrameQueue &frames):
socket_(io_service, udp::endpoint(udp::v4(), port)),
frames(frames)
{
    start_receive();
}

void ServerUDP::start_receive()
{
    socket_.async_receive(
            boost::asio::buffer(rx_buffer),
            boost::bind(&ServerUDP::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

void ServerUDP::handle_receive(const boost::system::error_code &error,
                               std::size_t bytes_transferred)
{
    if (!error || error == boost::asio::error::message_size)
    {
        uint32_t net_len = 0;
        net_len = net_len | rx_buffer[0];
        net_len = net_len | (rx_buffer[1] << 8);
        net_len = net_len | (rx_buffer[2] << 16);
        net_len = net_len | (rx_buffer[3] << 24);


        std::string archive_data;
        std::copy(rx_buffer.begin()+4,
                  rx_buffer.begin()+net_len+4,
                  std::back_inserter(archive_data));

        std::istringstream archive_stream(archive_data);
        boost::archive::text_iarchive archive(archive_stream);
        Frame t;
        archive >> t;

        boost::chrono::high_resolution_clock::time_point stop =
                boost::chrono::high_resolution_clock::now();
        boost::chrono::high_resolution_clock::time_point start = t.timeStamp;
        std::cout
                << "packet delay = "
                << boost::chrono::duration_cast<boost::chrono::milliseconds>(stop - start).count()
                << " ms"
                << std::endl;

        frames.enqueue(t);

        start_receive();
    }
}
