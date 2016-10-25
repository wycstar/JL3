/* PciCanDll.h */
#ifndef __INC_PCI_CAN_DLL_H
#define __INC_PCI_CAN_DLL_H
#include "vxWorks.h"
#include "wdLib.h"
#include "ioLib.h"
#include "iosLib.h"
#include "stat.h"
#include "dirent.h"
#include "iv.h"
#include "tyLib.h"
#include "memLib.h"
#include "errno.h"
#include "errnoLib.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "intLib.h"
#include "sys/ioctl.h"
#include "semLib.h"
#include "taskLib.h"

#include	"PciCanDrv.h"
#define SELFTEST_OK        0
#define ERR_NODETECT_BORAD -1
#define ERR_NOREC_DATA     -2
#define ERR_NOMATCH_ID     -3
#define ERR_NOMATCH_DATA   -4
#define ERR_INVALID_PARA   -5
#define ERR_MISC           -6
/* Card initialization and configuration functions*/
WORD	GetDriverVersion(void);
int 	CanOpenDriver(int cardNo,int channelNo);
int 	CanCloseDriver(int handle);
int 	CanConfigPort(int handle, PORT_STRUCT *PortStruct);
int    	CanGetPortStatus(int handle, PORT_STATUS *PortStatus);
void 	CanGetReceiveEvent(int handle,SEM_ID * rcvEvent);
int 	CanInstallEvent(int handle, int index, SEM_ID * hEvent);
BYTE   	CanRead(int handle, int offset);
void   	CanWrite(int handle, int offset, BYTE data);

/*  CAN layer functions*/
void  	CanEnableReceive(int handle);
void  	CanDisableReceive(int handle);
int   	CanSendMsg(int handle, CAN_PACKET *packet);
int   	CanRcvMsg(int handle, CAN_PACKET *packet);
WORD  	CanGetRcvCnt(int handle);
/*  CAN layer status control*/
void   	CanClearOverrun(int handle);
void   	CanClearRxBuffer(int handle);
void   	CanClearTxBuffer(int handle);
BYTE   	CanGetErrorCode(int handle);
BYTE  	CanGetErrorWarningLimit(int handle);
void   	CanSetErrorWarningLimit(int handle, BYTE limit);
BYTE   	CanGetRxErrorCount(int handle);
void   	CanSetRxErrorCount(int handle, BYTE value);
BYTE   	CanGetTxErrorCount(int handle);
void   	CanSetTxErrorCount(int handle, BYTE value);
BYTE   	CanGetIntStatusReg(int handle);
STATUS  CanDevInstall(int cardNo,int channelNo);

#endif/*__INC_PCI_CAN_DLL_H*/
