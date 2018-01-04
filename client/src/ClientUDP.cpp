//
// Created by dilin on 11/20/17.
//

#include "ClientUDP.h"

ClientUDP::ClientUDP(boost::asio::io_service & io_service,
                     string host,
                     unsigned short server_port):
        io_service(io_service),
        socket(io_service,udp::endpoint(udp::v4(), 0)),
        port(server_port),
        host(host)
{
    udp::resolver resolver(io_service);
    udp::resolver::query query(udp::v4(), host, std::to_string(server_port));
    server_endpoint = *resolver.resolve(query);
}

void ClientUDP::send(Frame frame)
{
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << frame;
    std::string outbound_data_ = archive_stream.str();

    uint32_t net_len = (uint32_t)outbound_data_.length();

    if(net_len>BUFFER_SIZE)
        throw runtime_error("Packet length exceeds buffer size! len: "
                            + to_string(net_len));

    tx_buffer[0] = (unsigned char)(net_len >> 0);
    tx_buffer[1] = (unsigned char)(net_len >> 8);
    tx_buffer[2] = (unsigned char)(net_len >> 16);
    tx_buffer[3] = (unsigned char)(net_len >> 24);
    try
    {
        copy(outbound_data_.begin(),outbound_data_.end(),tx_buffer.begin()+4);
        while(!socket.is_open())
        {
            cout << "Socket reconnecting..." << endl;
            socket.open(boost::asio::ip::udp::v4());
            udp::resolver resolver(io_service);
            udp::resolver::query query(udp::v4(), host, std::to_string(port));
            server_endpoint = *resolver.resolve(query);
        }
        socket.send_to(boost::asio::buffer(tx_buffer),
                       server_endpoint);
    }
    catch (const std::exception& e)
    {
        cerr << "Exception : " << e.what() << endl;
    }
}

ClientUDP::~ClientUDP()
{
    socket.close();
}
