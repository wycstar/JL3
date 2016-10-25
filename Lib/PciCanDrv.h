#ifndef __INC_PCI9052_CAN_DRV_H
#define __INC_PCI9052_CAN_DRV_H

#define DEBUG_CAN_DRV
#ifdef DEBUG_CAN_DRV

#define DRV_DEBUG_OFF			  0x0000
#define DRV_DEBUG_READ			0x0001
#define	DRV_DEBUG_WRITE			0x0002
#define DRV_DEBUG_INT			  0x0004
#define DRV_DEBUG_RX		    0x0008
#define	DRV_DEBUG_START		  0x0010
#define	DRV_DEBUG_TX		    0x0020
#define	DRV_DEBUG_INIT			0x0040
#define	DRV_DEBUG_IOCTL			0x0080
#define	DRV_DEBUG_STOP			0x0100
#define DRV_DEBUG_PROBE     0x0200
#define	DRV_DEBUG_SELF		  0x1000
#define DRV_DEBUG_ALL			  0xffff

#define DRV_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)				\
	if (d_iCanDrvDbg & FLG)										\
            logMsg(X0, X1, X2, X3, X4, X5, X6);
#else /*DEBUG_CAN_DRV*/
#define DRV_LOG(DBG_SW, X0, X1, X2, X3, X4, X5, X6)
#endif/*DEBUG_CAN_DRV*/

#define PCI_CAN_VERTOR_ID   0x0771
#define PCI_CAN_DEV_ID      0x1801
#define	PCI9052_CAN0	0x00
#define	PCI9052_CAN1	0x01

#ifndef BYTE
typedef unsigned char BYTE;
#endif  /*  BYTE*/

#ifndef WORD
typedef unsigned short WORD;
#endif  /*  WORD*/

#ifndef DWORD
typedef unsigned long DWORD;
#endif  /*  DWORD*/

#define	PCI_CAN_GET_VER			       0x200
#define	PCI_CAN_SET_CONFIG		     0x201
#define	PCI_CAN_GET_CONFIG		     0x202
#define	PCI_CAN_ENABLE_RCV		     0x203
#define	PCI_CAN_DISABLE_RCV		     0x204
#define PCI_CAN_SET_ALLOW_RCV_SEMA 0x205/*add by ghy 20090221*/	
#define	PCI_CAN_CLEAR_OVERRUN		   0x207
#define	PCI_CAN_CLEAR_RXBUFFER		 0x208
#define	PCI_CAN_CLEAR_TXBUFFER		 0x209
#define	PCI_CAN_GET_ERROR_CODE		 0x20A
#define	PCI_CAN_GET_EWARNING_LMT	 0x20B
#define	PCI_CAN_SET_EWARNING_LMT	 0x20C
#define	PCI_CAN_GET_RXERROR_COUNT	 0x20D
#define	PCI_CAN_SET_RXERROR_COUNT	 0x20E
#define	PCI_CAN_GET_TXERROR_COUNT	 0x20F
#define	PCI_CAN_SET_TXERROR_COUNT	 0x210
#define	PCI_CAN_GET_INT_STATUS_REG 0x211
#define	PCI_CAN_GET_RCV_MSG_COUNT	 0x212
#define	PCI_CAN_INSATLL_EVENT		   0x213
#define	PCI_CAN_GET_SEM_ID		     0x214
#define	PCI_CAN_READ_R			       0x215
#define	PCI_CAN_WRITE_R			       0x216
#define PCI_CAN_IOCTL_READ         0x217
#define PCI_CAN_IOCTL_WRITE        0x218

#define CAN_BPS_125000	0
#define CAN_BPS_250000	1
#define CAN_BPS_500000	2
#define CAN_BPS_1000000	3
#define CAN_BPS_CUSTOM	4

#define CAN_BPS_5000	  5
#define CAN_BPS_10000	  6
#define CAN_BPS_20000	  7
#define CAN_BPS_40000	  8
#define CAN_BPS_50000	  9
#define CAN_BPS_80000	  10
#define CAN_BPS_100000	11
#define CAN_BPS_200000	12
#define CAN_BPS_400000	13
#define CAN_BPS_666000	14
#define CAN_BPS_800000	15

#define  CHANNELNUN	2
#define  CARDNUM	1
#define  MODE11WORK		0
#define  MODE29WORK		1
#define  MODE11TEST		2
#define  MODE29TES		3

/*  Define for can port struct*/
typedef struct _tagPORT_STRUCT
{
    int iMode;       		/*  0   for 11-bit work iMode;     1 for 29-bit work mode*/
    				            /*  2   for 11-bit self test mode;3 for 29-bit self test mode*/
    DWORD dwAccCode;
    DWORD dwAccMask;
    int iBaudrate;		/*	0 : 125KBps, 1 : 250KBps, 2 : 500KBps, 3 : 1MBps,*/
				               /*	4 : Self-Defined */
		/*	Used only if iBaudrate = 4 */		               
	  BYTE cBrp;
	  BYTE cTseg1;
	  BYTE cTseg2;	
	  BYTE cSjw;
	  BYTE cSam; 
	  /* Used only if iBaudrate = 4 */         
}PORT_STRUCT;

/*  Define can packet struct*/
typedef struct _tagCAN_PACKET
{
    DWORD dwCanID;       /*  CAN id		*/
    BYTE cRtr;           /*  RTR bit		*/
    BYTE cLen;           /*  Data length		*/
    BYTE cData[8];       /*  Data		*/
    DWORD dwTime;         /*  Occur time (non use)*/
    BYTE reserved;      /*  future use		*/
}CAN_PACKET;

typedef struct _tagCAN_PACKET_COUNT_POINT
{
    WORD wHead;       /*  CAN_PACKET	head	*/
    WORD wTail;       /*  CAN_PACKET	tail	*/ 
}CAN_PACKET_COUNT_POINT;
/*  Define DeviceNet packet struct*/
typedef struct _tagDEVICENET_PACKET
{
    BYTE cGroup;         /*  DeviceNet Group	*/
    BYTE cMAC_ID;        /*  MAC_ID		*/
    BYTE cHostMAC_ID;    /*  HostMAC_ID		*/
    BYTE cMESSAGE_ID;    /*  Message ID		*/
    BYTE cLen;           /*  Data Length		*/
    BYTE cData[8];       /*  Data		*/
    DWORD cTime;         /*  Occur time(for future use)*/
    BYTE reserved;      /*  future use		*/
}DEVICENET_PACKET;

/*  Define CAN status register*/

struct PORTREG_BIT
{
    unsigned char cRxBuffer      : 1;
    unsigned char cDataOverrun   : 1;
    unsigned char cTxBuffer      : 1;
    unsigned char cTxEnd         : 1;
    unsigned char cRxStatus      : 1;
    unsigned char cTxStatus      : 1;
    unsigned char cErrorStatus   : 1;
    unsigned char cBusStatus     : 1;   
};

typedef union _tagPORT_REG
{
	union
	{
		struct PORTREG_BIT bit;
	  unsigned char cReg;
	}portReg;
}PORT_REG;

typedef struct _tagPORT_STATUS
{
    PORT_STRUCT stPort;
    PORT_REG stStatus;
}PORT_STATUS;

struct InstallEvent
{
	int iIndex;
	SEM_ID * phEvent;
};

extern STATUS          install_pci_can_drv(int iCardNo,int iChannelNo);
extern STATUS          Pci9052_can_detect();
extern WORD             PeliCANGetDriverVersion();
#endif  /* __INC_PCI9052_CAN_DRV_H */
