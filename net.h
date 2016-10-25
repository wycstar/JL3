#ifndef _NET_H_
#define _NET_H_
#include "userIncludes.h"
#include "control.h"

int netRecvTask();
int netSendTask();

static const int  SOCK_ADDR_SIZE    = sizeof(struct sockaddr);
static const int  SOCK_ADDR_IN_SIZE = sizeof(struct sockaddr_in);

typedef class net
{
private:
	int sFD;
	int rFD;
	MSG_Q_ID sentMsgQueue;
	SOCK_ADDR_IN sendToAddr;
	SOCK_ADDR_IN recvFromAddr;
	SOCK_ADDR_IN sourceAddr;
	static const int BIND_PORT 			= 10000;
	static const int MAX_RESEND_COUNT   = 2;
	static const int RESEND_TIME_VAL   	= 1000;
	STATUS CMDExplain(UCHAR recvBuf[]) const;

public:
	UCHAR 	 currentSendCount;
	MSG_Q_ID sendingMsgQueue;
	
public:
	net();
	~net();
	STATUS init();
	friend STATUS netRecvTask();
	friend STATUS netSendTask();
	STATUS release();
}NET;
extern NET net;
#endif
