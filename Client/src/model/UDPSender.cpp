#include <iostream>

#include "UDPSender.h"
#ifdef __linux
#include <arpa/inet.h>
#include <unistd.h>
#endif

UDPSender::UDPSender(const char* dest_ip, int dest_port)
{
    this->ip = std::string(dest_ip);
    this->port = dest_port;
    

    //std::cout << "ip: " << this->ip << "  port: " <<  this->port << std::endl;

#ifdef _WIN32
    WSAStartup(MAKEWORD(2, 2), &data);

    memset(&dest_IPv6, 0, sizeof(dest_IPv6));
    memset(&local_IPv6, 0, sizeof(local_IPv6));
#endif

    if (inet_pton(AF_INET6, dest_ip, &dest_IPv6.sin6_addr) == 1)
    {
        // valid IPv6 address
        this->s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

        dest_IPv6.sin6_family = AF_INET6;
        dest_IPv6.sin6_port = htons(dest_port);
        local_IPv6.sin6_addr = IN6ADDR_ANY_INIT;
        local_IPv6.sin6_family = AF_INET6;
        local_IPv6.sin6_port = htons(0);

        //local_IPv6.sin6_addr = IN6ADDR_ANY_INIT;
        this->valid = bind(s, (sockaddr*)&local_IPv6, sizeof(local_IPv6)) == 0;
        if (!this->valid) {
            std::cout << "Couldn't bind to IPv6 address." << std::endl;
        }
    }
    else if (inet_pton(AF_INET, dest_ip, &dest.sin_addr) == 1)
    {
        // valid IPv4 address
        this->s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        
        dest.sin_family = AF_INET;
        dest.sin_port = htons(dest_port);

        local.sin_family = AF_INET;
        local.sin_port = htons(0);
        //local.sin_addr.S_un.S_addr = INADDR_ANY;
        this->valid = bind(s, (sockaddr*)&local, sizeof(local)) == 0;
        if (!this->valid) {
            std::cout << "Couldn't bind to IPv4 address." << std::endl;
        }
    }
    else
    {
        // In case of invalid address better mark this sender as invalid
        // and not use loopback. User of this class hould know whether he/she 
        // has made a mistake.
        this->valid = false;
        std::cout << "Invalid IP Address" << std::endl;
    }

}

UDPSender::~UDPSender()
{
    std::cout << "Closing connection" << std::endl;
#ifdef _WIN32
    closesocket(s);
    WSACleanup();
#elif __linux
    if (s) close(s);
#endif
}


void UDPSender::send_data(double* d)
{
    // Make packet
    const char* pkt = (char*)d;
    sendto(s, pkt, BUFFER_SIZE, 0, (sockaddr*)&dest, (dest.sin_family == AF_INET6 ? sizeof(dest_IPv6) : sizeof(dest)));
};


