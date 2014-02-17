#include <stdio.h>
#include "ODSocket.h"

#ifdef WIN32
#pragma comment(lib, "wsock32")
#endif

ODSocket::ODSocket(SOCKET sock) {
	m_sock = sock;
}

ODSocket::~ODSocket() {
}

int ODSocket::QInit() {
#ifdef WIN32
	/*
	 http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx

	 typedef struct WSAData {
	 WORD wVersion;								//winsock version
	 WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
	 char szDescription[WSADESCRIPTION_LEN+1];
	 char szSystemStatus[WSASYSSTATUS_LEN+1];
	 unsigned short iMaxSockets;
	 unsigned short iMaxUdpDg;
	 char FAR * lpVendorInfo;
	 }WSADATA, *LPWSADATA;
	 */
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8))
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData); //win sock start up
	if (ret) {
//		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif

	return 0;
}
//this is just for windows
int ODSocket::QClean() {
#ifdef WIN32
	return (WSACleanup());
#endif
	return 0;
}

ODSocket& ODSocket::operator =(SOCKET s) {
	m_sock = s;
	return (*this);
}

ODSocket::operator SOCKET() {
	return m_sock;
}

/**
	create a socket object win/lin is the same
	domain: AF_INET / AF_INET6 / AF_UNIX 
	type: SOCK_STREAM / SOCK_DGRAM / SOCK_SEQPACKET / SOCK_RAW 
	protocol: IPPROTO_TCP / IPPROTO_SCTP / IPPROTO_UDP / IPPROTO_DCCP
*/
bool ODSocket::QCreate(int domain, int type, int protocol) {
	m_sock = socket(domain, type, protocol);
	if (m_sock == INVALID_SOCKET) {
		return false;
	}
	return true;
}

bool ODSocket::QConnect(const char* ip, unsigned short port) {
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	int ret = connect(m_sock, (struct sockaddr*) &svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool ODSocket::QBind(unsigned short port) {
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int opt = 1;
	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*) &opt, sizeof(opt))
			< 0)
		return false;

	int ret = bind(m_sock, (struct sockaddr*) &svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}
//for server
bool ODSocket::QListen(int backlog) {
	int ret = listen(m_sock, backlog);
	if (ret == SOCKET_ERROR) {
		return false;
	}
	return true;
}

bool ODSocket::QAccept(ODSocket& s, char* fromip) {
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*) &cliaddr, &addrlen);
	if (sock == SOCKET_ERROR) {
		return false;
	}

	s = sock;
	if (fromip != NULL)
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));

	return true;
}

int ODSocket::QSelect(){
	FD_ZERO(&fdR);
	FD_SET(m_sock, &fdR);

	// 第一个参数是 0 和 sockfd 中的最大值加一
	// 第二个参数是 读集, 也就是 sockset
	// 第三, 四个参数是写集和异常集, 在本程序中都为空
	// 第五个参数是超时时间, 即在指定时间内仍没有可读, 则出错
	int result= select(m_sock, &fdR, NULL, NULL, NULL);

	//case -1:                            error handled by u;
	if(-1 == result){
		return QSELECT_ERROR;
	}

	else {
		if(FD_ISSET(m_sock, &fdR)){
			return QSELECT_READ;
		}else {
			return QSELECT_NULL;
		}
	}
}



int ODSocket::QSend(const char* buf, int len, int flags) {
	int bytes;
	int count = 0;

	while (count < len) {
		const char* a= buf + count;
		bytes = send(m_sock, buf + count, len - count, flags);
		if (bytes == -1 || bytes == 0)
			return -1;
		count += bytes;
	}

	return count;
}

int ODSocket::QRecv(char* buf, int len, int flags) {
	return (recv(m_sock, buf, len, flags));
}

int ODSocket::QClose() {
#ifdef WIN32
	return (closesocket(m_sock));
#else
	return (close(m_sock));
#endif
}

int ODSocket::QGetError() {
#ifdef WIN32
	return (WSAGetLastError());
#else
	return -1;
#endif
}

bool ODSocket::QDnsParse(const char* domain, char* ip) {
	struct hostent* p;
	if ((p = gethostbyname(domain)) == NULL)
		return false;

	sprintf(ip, "%u.%u.%u.%u", (unsigned char) p->h_addr_list[0][0],
			(unsigned char) p->h_addr_list[0][1],
			(unsigned char) p->h_addr_list[0][2],
			(unsigned char) p->h_addr_list[0][3]);

	return true;
}
