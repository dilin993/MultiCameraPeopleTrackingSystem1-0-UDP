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
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <iostream>
#include <vector>
#include <boost/serialization/vector.hpp>
#include "Frame.h"
#include "FrameQueue.h"
#include<fstream>

#define BUFFER_SIZE 49152

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
    boost::array<unsigned char, BUFFER_SIZE> rx_buffer;
    void start_receive();
    void handle_receive(const boost::system::error_code& error,
                        std::size_t bytes_transferred);
    Frame frame;
    FrameQueue & frames;
};


#endif //SERVER_SERVERUDP_H
