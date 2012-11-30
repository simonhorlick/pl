#include "multicast_source.h"
#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

bool MulticastSource::Initialise() {
    int fd;

    // Create a standard UDP socket
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return false;
    }

    int yes = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        perror("setsockopt");
        return false;
    }

    // Bind to a local port so we can listen to packets from the remote address
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return false;
    }

    // Request to join the multicast group
    ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(ip.c_str());
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if(setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt");
        return false;
    }

    long tbytes = 0;
    socklen_t addrlen = sizeof(addr);
    const int bufsize = 1024; // TODO: What should this be?
    char buf[bufsize];
    while(1) {
        int bytes;
        if((bytes = recvfrom(fd, buf, bufsize, 0, (sockaddr*)&addr, &addrlen)) < 0) {
            perror("recvfrom");
            return true;
        }
        tbytes += bytes;
        std::cout << "Read " << tbytes << " bytes\n";
    }
    return true;
}

int MulticastSource::Read(uint8_t* buffer, int size) {
    return -1;
}

