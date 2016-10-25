#ifndef __INC_PCI9052_CAN_REGS_H
#define __INC_PCI9052_CAN_REGS_H

#include	"PciCanDrv.h"

#define PCI_CAN_VERSION  201 /*modify by ghy 20121219*/ 
 
#define MODE 	0x00 	/*方式寄存器*/
#define CDR 	0x1F	/*时钟分频寄存器*/
#define IER 	0x04	/*中断允许寄存器*/
#define	AMR 	0x14	/*接收屏蔽寄存器*/
#define ACR 	0x10	/*接收代码寄存器*/
#define BTR0 	0x06	/*总线定时寄存器0*/
#define BTR1 	0x07	/*总线定时寄存器1*/
#define OCR 	0x08	/*输出控制寄存器*/
#define RBSA 	0x1E 	/*接收缓存器起始地址寄存器*/

#define RXERR 	0x0E 	/*接收错误计数寄存器*/
#define TXERR 	0x0F 	/*发送错误计数寄存器*/

#define EWLR 	0x0D 	/*错误报警限制寄存器*/
#define ECC 	0x0C 	/*错误代码捕捉寄存器*/

#define CANTXB 	0x10 	/*SJA1000 发送缓存区首址*/
#define CANRXB 	0x10	    /*接收缓冲区首地址*/


#define CMR 	0x01 	/*命令寄存器地址*/

#define RMC 	0x1D 	/*RX信息计数器*/

#define SR  	0x02	/*状态寄存器*/
#define DAMR 	0x15 	/*接收屏蔽寄存器内容在片内RAM 中的首址*/
#define	DACR 	0x2A 	/*接收代码寄存器内容在片内RAM 中底址*/
#define IR 	0x03		/*中断寄存器*/

#define	RI	0x01
#define	TI	0x02
#define	EI	0x04
#define	DOI	0x08
#define	WUI	0x10
#define	EPI	0x20
#define	ALI	0x40
#define	BEI	0x80

#define ALC 	11		/*仲裁丢失捕捉寄存器*/

#define	CMR0	0x01
#define	CMR1	0x02
#define	CMR2	0x04
#define	CMR3	0x08
#define	CMR4	0x10
#define	CMR5	0x20
#define	CMR6	0x40
#define	CMR7	0x80

#define	DISABLE_ISR	0x00
#define	ENABLE_ISR	0xff

#define  PCI_BASE_ADDRESS_IO_MASK	(~0x03)
#define  PCI_COMMAND_IO			0x1	/* Enable response in I/O space */
#define  PATTERNS	512 /*old = 512*/
#define  SELFTESTMODE	0
#define  WORKMODE	1
#define  MODE11		0
#define  MODE29		1
#define  MODE11DATAOFFSET 3
#define  MODE29DATAOFFSET 5
#define  SFF		0x00
#define  EFF		0x80

#define  USETIMEOUT		1
#define  SENDTIMEOUT	1000


typedef struct
{
	/*DEV_HDR 	*pDevHdr;*/
	DEV_HDR 	DevHdr;
	ULONG		  dwDevAdrs;
	char 		  cCanMode;
	CAN_PACKET 		          stRcvMsg[PATTERNS];
	CAN_PACKET_COUNT_POINT	stRcvPacketCP;
	PORT_STRUCT		          stPortInfo;
	int		    iChannelNo;
	SEM_ID    channelSemId[8];	/* chnnel sigal */
	SEM_ID    mSndMuxSemId;
	SEM_ID    mRcvMuxSemId;
	int 		  iAllowOffset;
	char		  cIsOpen;
	char      cAllowRecSema; /*add by ghy 20090221*/
	
}PCI9052_CAN_DEV;
typedef struct 
{
	PCI9052_CAN_DEV	    	*pstDevice;
	int        		iIntLevel;
  int   			  iIntVect;
  ULONG			    dwCardBaseAddr[CHANNELNUN];   			       
  ULONG			    dwIoCtrlCode;		
}PCI9052_CAN_DRV_CTRL;

#endif  /* __INC_PCI9052_CAN_REGS_H */
