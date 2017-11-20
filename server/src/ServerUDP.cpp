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

}

void ServerUDP::start_receive()
{
    socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&ServerUDP::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

void ServerUDP::handle_receive(const boost::system::error_code &error,
                               std::size_t bytes_transferred)
{
    if (!error || error == boost::asio::error::message_size)
    {


        start_receive();
    }
}

void ServerUDP::handle_send(boost::shared_ptr<std::string>,
                            const boost::system::error_code &,
                            std::size_t bytes_transferred)
{

}
