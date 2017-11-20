//
// Created by dilin on 11/19/17.
//

#ifndef SERVER_SERVERUDP_H
#define SERVER_SERVERUDP_H

#include <iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "Frame.h"
#include "FrameQueue.h"

using namespace std;
using namespace cv;
using boost::asio::ip::udp;

class ServerUDP
{
public:
    ServerUDP(boost::asio::io_service& io_service,
    unsigned short port,
            FrameQueue & frames);

private:
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 1> recv_buffer_;
    void start_receive();
    void handle_receive(const boost::system::error_code& error,
                        std::size_t /*bytes_transferred*/);
    void handle_send(boost::shared_ptr<std::string> /*message*/,
                     const boost::system::error_code& /*error*/,
                     std::size_t /*bytes_transferred*/);
    Frame frame;
    FrameQueue & frames;
};


#endif //SERVER_SERVERUDP_H
