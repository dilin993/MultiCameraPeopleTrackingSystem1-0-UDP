#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>

namespace udp_client
{

class udp_client_runtime_error : public std::runtime_error
{
public:
    udp_client_runtime_error(const char *w) : std::runtime_error(w) {}
};


class udp_client
{
public:
                        udp_client(const std::string& addr, int port);
                        ~udp_client();

    int                 get_socket() const;
    int                 get_port() const;
    std::string         get_addr() const;

    int                 send(const char *msg, size_t size);

private:
    int                 f_socket;
    int                 f_port;
    std::string         f_addr;
    struct addrinfo *   f_addrinfo;
};


} // namespace udp_client
#endif
// SNAP_UDP_CLIENT_H
// vim: ts=4 sw=4 et