/*
 * define file about portable socket class.
 * description:this sock is suit both windows and linux
 * design:odison
 * e-mail:odison@126.com>
 *
 */

#ifndef _ODSOCKET_H_
#define _ODSOCKET_H_

#ifdef WIN32
	#include <winsock2.h>
	typedef int				socklen_t;
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <arpa/inet.h>
	typedef int				SOCKET;

	//#pragma region define win32 const variable in linux
	#define INVALID_SOCKET	-1
	#define SOCKET_ERROR	-1
	//#pragma endregion
#endif
	
#define QSELECT_ERROR	-1
#define QSELECT_READ	1
#define QSELECT_NULL	0

class ODSocket {

public:
	ODSocket(SOCKET sock = INVALID_SOCKET);
	~ODSocket();

	// Create socket object for snd/recv data
	bool QCreate(int af, int type, int protocol = 0);

	// Connect socket
	bool QConnect(const char* ip, unsigned short port);
	//#region server
	// Bind socket
	bool QBind(unsigned short port);

	// Listen socket
	bool QListen(int backlog = 5);

	// Accept socket
	bool QAccept(ODSocket& s, char* fromip = NULL);
	//#endregion
	int QSelect();
	// Send socket
	int QSend(const char* buf, int len, int flags = 0);

	// Recv socket
	int QRecv(char* buf, int len, int flags = 0);

	// Close socket
	int QClose();

	// Get errno
	int QGetError();

	//#pragma region just for win32
	// Init winsock DLL
	static int QInit();
	// Clean winsock DLL
	static int QClean();
	//#pragma endregion

	// Domain parse
	static bool QDnsParse(const char* domain, char* ip);

	ODSocket& operator = (SOCKET s);

	operator SOCKET ();

protected:
	SOCKET m_sock;
	fd_set  fdR;
};

#endif
