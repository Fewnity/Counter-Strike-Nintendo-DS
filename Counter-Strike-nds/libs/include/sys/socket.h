// DSWifi Project - socket emulation layer defines/prototypes (sys/socket.h)
// Copyright (C) 2005-2006 Stephen Stair - sgstair@akkit.org - http://www.akkit.org
/****************************************************************************** 
DSWifi Lib and test materials are licenced under the MIT open source licence:
Copyright (c) 2005-2006 Stephen Stair

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/



#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H

#include <sys/time.h>

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define  SOL_SOCKET  0xfff    /* options for socket level */
# define SOL_TCP                6       /* TCP level */

#define PF_UNSPEC		0
#define PF_INET			2
#define PF_INET6		10

#define AF_UNSPEC		PF_UNSPEC
#define AF_INET			PF_INET
#define AF_INET6		PF_INET6

#define SOCK_STREAM		1
#define SOCK_DGRAM		2

// need to sync FIO* values with commonly accepted ones sometime
#define FIONBIO			1
#define FIONREAD		2

#define SOCKET_ERROR	-1

// send()/recv()/etc flags
// at present, only MSG_PEEK is implemented though.
#define MSG_WAITALL		0x40000000
#define MSG_TRUNC		0x20000000
#define MSG_PEEK		0x10000000
#define MSG_OOB			0x08000000
#define MSG_EOR			0x04000000
#define MSG_DONTROUTE	0x02000000
#define MSG_CTRUNC		0x01000000

// shutdown() flags:
#define SHUT_RD			1
#define SHUT_WR			2
#define SHUT_RDWR		3

/*
 * Option flags per-socket.
 */
#define  SO_DEBUG  0x0001    /* turn on debugging info recording */
#define  SO_ACCEPTCONN  0x0002    /* socket has had listen() */
#define  SO_REUSEADDR  0x0004    /* allow local address reuse */
#define  SO_KEEPALIVE  0x0008    /* keep connections alive */
#define  SO_DONTROUTE  0x0010    /* just use interface addresses */
#define  SO_BROADCAST  0x0020    /* permit sending of broadcast msgs */
#define  SO_USELOOPBACK  0x0040    /* bypass hardware when possible */
#define  SO_LINGER  0x0080    /* linger on close if data present */
#define  SO_OOBINLINE  0x0100    /* leave received OOB data in line */
#define  SO_REUSEPORT   0x0200      /* allow local address & port reuse */

#define SO_DONTLINGER   (int)(~SO_LINGER)

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF  0x1001    /* send buffer size */
#define SO_RCVBUF  0x1002    /* receive buffer size */
#define SO_SNDLOWAT  0x1003    /* send low-water mark */
#define SO_RCVLOWAT  0x1004    /* receive low-water mark */
#define SO_SNDTIMEO  0x1005    /* send timeout */
#define SO_RCVTIMEO  0x1006    /* receive timeout */
#define  SO_ERROR  0x1007    /* get error status and clear */
#define  SO_TYPE    0x1008    /* get socket type */

struct sockaddr {
	unsigned short		sa_family;
	char				sa_data[14];
};

#ifndef ntohs
#define ntohs(num) htons(num)
#define ntohl(num) htonl(num)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	extern int socket(int domain, int type, int protocol);
	extern int bind(int socket, const struct sockaddr * addr, int addr_len);
	extern int connect(int socket, const struct sockaddr * addr, int addr_len);
	extern int send(int socket, const void * data, int sendlength, int flags);
	extern int recv(int socket, void * data, int recvlength, int flags);
	extern int sendto(int socket, const void * data, int sendlength, int flags, const struct sockaddr * addr, int addr_len);
	extern int recvfrom(int socket, void * data, int recvlength, int flags, struct sockaddr * addr, int * addr_len);
	extern int listen(int socket, int max_connections);
	extern int accept(int socket, struct sockaddr * addr, int * addr_len);
	extern int shutdown(int socket, int shutdown_type);
	extern int closesocket(int socket);

	extern int ioctl(int socket, long cmd, void * arg);

	extern int setsockopt(int socket, int level, int option_name, const void * data, int data_len);
	extern int getsockopt(int socket, int level, int option_name, void * data, int * data_len);

	extern int getpeername(int socket, struct sockaddr *addr, int * addr_len);
	extern int getsockname(int socket, struct sockaddr *addr, int * addr_len);

    extern int gethostname(char *name, size_t len);
    extern int sethostname(const char *name, size_t len);

	unsigned short htons(unsigned short num);
	unsigned long htonl(unsigned long num);


	extern int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);


#ifdef __cplusplus
};
#endif


#endif
