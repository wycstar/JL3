#ifndef _USERTYPES_H_
#define _USERTYPES_H_

typedef unsigned int 			UINT;
typedef unsigned int* 			UINT_PTR;
typedef int* 					INT_PTR;
typedef float*					FLOAT_PTR;
typedef unsigned char 			UCHAR;
typedef unsigned char* 			UCHAR_PTR;
typedef unsigned short 			USHORT;
typedef unsigned char 			UCHAR;
typedef unsigned long long int 	ULINT;
typedef char * 					CHAR_PTR;
typedef unsigned char *			UCHAR_PTR;
typedef struct sockaddr *		SOCK_ADDR_PTR;
typedef struct sockaddr_in		SOCK_ADDR_IN;

typedef struct itimerspec ITIMERSPEC;
typedef struct timespec   TIMESPEC;
typedef struct sigevent	  SIGEVENT;
typedef union  sigval	  SIGVAL;
typedef struct stat		  STAT;

#define WORD 	unsigned short
#define DWORD 	unsigned int
#define BYTE    unsigned char

static const UINT AXIS_NUM = 5;

typedef struct /*UTC时间结构体*/
{
	WORD wYear;
	BYTE ucMonth;
	BYTE ucDay;
	BYTE ucHour;
	BYTE ucMinute;
	BYTE ucSecond;
	WORD wMilliSecond;
	WORD wMarcoSecond;
	WORD wType;
}ST_DATETIME;

//以下结构体按1个字节对齐
#pragma pack(1)
typedef struct _tSelfTestRequest
{
	UCHAR msgType;
	UCHAR msgCount;
	UCHAR command;
	UCHAR checkSum;
}STC_MSG;

typedef struct _tSelfTestResult 
{
	UCHAR msgType;
	UCHAR msgCount;
	WORD  stInfo;
	DWORD stDetail;
	DWORD remain;
	char  name[52];
	UCHAR checkSum;
}ST_MSG;

typedef struct _tFaultMsg 
{
	UCHAR msgType;
	UCHAR msgCount;
	WORD  msgClass;
	WORD  statusInfoA;
	WORD  statusInfoB;
	WORD  warnInfo;
	WORD  FaultInfoNormal;
	WORD  FaultInfoEmergency;
	WORD  remain;
	char  name[48];
	UCHAR checkSum;
}FT_MSG;

typedef struct _tAckMsg
{
	UCHAR msgType;
	UCHAR msgCount;
	UCHAR serverMsgType;
	UCHAR checkSum;
}ACK_MSG;

typedef struct _tGenMsg
{
	UCHAR msgType;
	UCHAR msgCount;
	WORD  command;
	UCHAR remain[12];
	UCHAR checkSum;
}GEN_MSG;

typedef struct _tCtlMsgNormal
{
	UCHAR msgType;
	UCHAR msgCount;
	WORD  remain;
	float tPitchPos;
	float tDriftPos;
	float tRollPos;
	float gPitchPos;
	float gDriftPos;
	float tPitchVel;
	float tDriftVel;
	float tRollVel;
	float gPitchVel;
	float gDriftVel;
	UCHAR checkSum;
}NCTL_MSG;

typedef struct _tCtlMsgWord
{
	UCHAR msgType;
	UCHAR msgCount;
	UCHAR tPitchWord;
	UCHAR tDriftWord;
	UCHAR tRollWord;
	UCHAR gPitchWord;
	UCHAR gDriftWord;
	UCHAR checkSum;
}WCTL_MSG;

typedef struct _tStaMsg
{
	UCHAR msgType;
	UCHAR msgCount;
	WORD  remain;
	float tPitch;
	float tDrift;
	float tRoll;
	float gPitch;
	float gDrift;
	UCHAR checkSum;
}STA_MSG;

#pragma pack()
#endif
