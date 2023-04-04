/*
 * @Author: HH
 * @Date: 2023-04-02 00:18:22
 * @LastEditTime: 2023-04-03 23:37:09
 * @LastEditors: HH
 * @Description: 
 * @FilePath: /WebREST/WebREST/core/socket.cc
 */

#ifndef WebREST_SOCKET_CC_
#define WebREST_SOCKET_CC_

#include "socket.h"
#include "inet_address.h"

using namespace WebREST;

int Socket::CreateNonBlockingFD()
{
    int sockfd = ::socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    assert( sockfd > 0 );
    return sockfd;
}

InetAddress Socket::get_local_addr(int sockfd)
{
    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof local_addr);
    socklen_t addr_len = static_cast<socklen_t>(sizeof local_addr);
    int ret = ::getsockname(sockfd, (struct sockaddr*)&local_addr, &addr_len);
    // assert(ret >= 0);

    return InetAddress(local_addr);    
}

void Socket::bind(const InetAddress& addr)
{
    int ret = ::bind(sockfd_, const_cast<const sockaddr*>(addr.sock_addr()), addr.len());
    assert( ret != -1 );
}

void Socket::listen()
{
    int ret = ::listen(sockfd_, SOMAXCONN);
    assert( ret != -1 );
}

int Socket::accpet(InetAddress& client_addr)
{
    socklen_t len = client_addr.len();
    //TODO: blocking IO, need to modify non-blocking
    int connfd = accept(sockfd_, client_addr.sock_addr(), &len);
    if ( connfd < 0 )
    {
        printf("Socket:: errno is: %d\n", errno);
        ::exit(0);
    }
    // assert( connfd >= 0 );
    return connfd;
}

int Socket::close(int sockfd)
{
    int ret = ::close(sockfd);
    assert( ret != -1 );
    return ret;
}

#endif