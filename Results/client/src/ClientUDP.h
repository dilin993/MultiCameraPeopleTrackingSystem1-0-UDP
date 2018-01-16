//
// Created by dilin on 11/20/17.
//

#ifndef CLIENT_CLIENTUDP_H
#define CLIENT_CLIENTUDP_H

#include <iostream>
#include <string>
#include<opencv2/opencv.hpp>
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include <boost/serialization/vector.hpp>
#include "Frame.h"

#define BUFFER_SIZE 49152

using namespace std;
using namespace cv;
using boost::asio::ip::udp;

class ClientUDP
{
public:
    ClientUDP(boost::asio::io_service & io_service,
              string host,
              unsigned short server_port);
    void send(Frame frame);
    ~ClientUDP();

private:
    boost::array<unsigned char,BUFFER_SIZE> tx_buffer;
    boost::asio::io_service& io_service;
    udp::socket socket;
    udp::endpoint server_endpoint;
    unsigned short port;
    string host;
};


#endif //CLIENT_CLIENTUDP_H
