#ifndef __INC_LSKIO_DRV_H
#define __INC_LSKIO_DRV_H

#define GODSON_MIPS
#define DRV_MEM_OPT 1 /*访问方式 io或者mem*/


#define DEBUG_KGI_O_DRV
#ifdef DEBUG_KGI_O_DRV

#define DRV_DEBUG_OFF			   0x0000
#define DRV_DEBUG_READ			 0x0001
#define	DRV_DEBUG_WRITE			 0x0002
#define DRV_DEBUG_INT			   0x0004
#define DRV_DEBUG_MUX_INT		 0x0008
#define	DRV_DEBUG_POLL			 (DRV_DEBUG_POLL_RX | DRV_DEBUG_POLL_TX)
#define	DRV_DEBUG_POLL_RX		 0x0010
#define	DRV_DEBUG_POLL_TX		 0x0020
#define	DRV_DEBUG_INIT			 0x0040
#define	DRV_DEBUG_IOCTL			 0x0080
#define	DRV_DEBUG_SELF		   0x1000
#define DRV_DEBUG_ALL			   0xffff

#define KGIO_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)				\
	if (d_iKgi_oDrvDbg & FLG)										\
            logMsg(X0, X1, X2, X3, X4, X5, X6);

#else /*DEBUG_CAN_DRV*/

#define DRV_LOG(DBG_SW, X0, X1, X2, X3, X4, X5, X6)

#endif

#define LS_KGI_O_DRV_VER          	 (0x100)
#define LS_KGI_O_VEN_ID           	 (0X0771)
#define LS_KGI_O_DEV_ID           	 (0Xfd10)
#define	PCI_KGI_O_GET_VER			        0x200
#define	PCI_KGI_O_READ_UINT8		      0x201
#define	PCI_KGI_O_READ_UINT16		      0x202
#define	PCI_KGI_O_READ_UINT32		      0x203
#define	PCI_KGI_O_WRITE_UINT8		      0x204
#define	PCI_KGI_O_WRITE_UINT16	 	    0x205
#define	PCI_KGI_O_WRITE_UINT32	      0x206
#define PCI_KGI_O_GET_SEM             0x207
#define PCI_KGI_O_REC_DATA            0x208
#define PCI_KGI_INT_IN_DATA           0x209

#define  PCI_BASE_ADDRESS_MEM_MASK	(~0x07)
#define  PCI_COMMAND_IO			0x1	/* Enable response in I/O space */
#define  PCI_COMMAND_MEM		0x2	/* Enable response in I/O space */

#if DRV_MEM_OPT
#define KGI_O_READ_MEM_UINT8(addr)          (*(UINT8*)(addr))
#define KGI_O_READ_MEM_UINT16(addr)         (*(UINT16*)(addr))
#define KGI_O_READ_MEM_UINT32(addr)         (*(UINT32*)(addr))
#define KGI_O_WRITE_MEM_UINT8(addr,data)    ((*(UINT8*)(addr))=(UINT8)(data))
#define KGI_O_WRITE_MEM_UINT16(addr,data)   ((*(UINT16*)(addr))=(UINT16)(data))
#define KGI_O_WRITE_MEM_UINT32(addr,data)   ((*(UINT32*)(addr))=(UINT32)(data))
#else
#define KGI_O_READ_MEM_UINT8(addr)          (sysInByte(addr))
#define KGI_O_READ_MEM_UINT16(addr)         (sysInWord(addr))
#define KGI_O_READ_MEM_UINT32(addr)         (sysInLong(addr))
#define KGI_O_WRITE_MEM_UINT8(addr,data)    sysOutByte((addr),(data))
#define KGI_O_WRITE_MEM_UINT16(addr,data)   sysOutWord((addr),(data))
#define KGI_O_WRITE_MEM_UINT32(addr,data)   sysOutLong((addr),(data))
#endif

#if 0 
/*字节序转换宏:Big_endian ~ Little_endian*/
#define sw_LB_endian32(test)    \
    (UINT32)((((UINT32)(test)&(UINT32)0x000000ff)<<24)  \
    |(((UINT32)(test)&(UINT32)0x0000ff00)<<8)   \
    |(((UINT32)(test)&(UINT32)0x00ff0000)>>8)   \
    |(((UINT32)(test)&(UINT32)0xff000000)>>24))
#endif

/*************************控制计算机************************/
/*共8路开入，8路开出*/

#define KIO_VERSION_REG      (0x10)/*版本寄存器，只读，复位值为0x100，版本号1.00*/

#define KIO_OUT_REG          (0x0) /*开出寄存器，低8位存储开出值，读写，0表示有开出，1表示无开出，复位值为0xffff*/
#define KIO_IN_REG           (0x8) /*开入寄存器，低8位存储开入值，只读，1表示有开入，0表示无开入，复位值为0x0*/
#define KIO_BIND_REG         (0x14)/*标识寄存器，只读，复位值为0xBD*/ 
#define KIO_TEST_REG         (0x18)/*用户寄存器，读写，复位值为0xAA*/
#define KIO_SELF_SET_REG     (0x1c)/*自环寄存器，读写，复位值为0x0，第0位为0表示关闭自环测试，为1表示开启自环测试*/
#define KIO_FILTER_SET       (0x20)/*滤波寄存器，读写，复位值为0x0A，0-10表示0-10ms滤波可设*/
#define IER_REG              (0x24)/*中断寄存器，读写，复位值为0x0，0x0表示中断关闭，
                                     0x3表示上升沿下降沿中断打开，0x1表示上升沿中断，0x2表示下降沿中断*/
#define IERS_R_REG           (0x28)/*中断状态寄存器，15-0位：1表示上升沿开入;31-16位：1表示下降沿开入*/

/*************************************************************/

#define PCI9052_INCSR                       0x4c
#define KGI_O_IN_CHAN_SUM                   (8)
#define KGI_O_OUT_CHAN_SUM                  (8)
#define KGI_O_FILTER_MAX                    (0xa)
#define KGI_O_MAX_BOARD_SUM                 (0x1)
#define KGI_O_FIFO_SUM                      (1024)
#define KGI_O_BUF_SUM                       (2046)/*硬件fifo大小为1024，软件fifo大小设为4096*/

typedef struct
{
	DEV_HDR 	DevHdr;
	UINT32		dwDevAdrs;
	UINT32		dwDevCfgAdrs;
	INT32     iIntLevel;
	UINT8			cIsOpen;
	UINT32    dwKgi_oBufHead;
	UINT32    dwKgi_oBufTail;
	UINT32    dwDevUnitNum;
	SEM_ID    iRecSemId;
	UINT32    dwKgiInbuf[KGI_O_BUF_SUM];
}PCI_KGI_O_DEV;
typedef struct 
{
	PCI_KGI_O_DEV	*pDevice;
	DEV_HDR   DevHdr;
	INT32     iIntLevel;
  INT32   	iIntVect;
  UINT32		dwDevBaseAddr;	
  UINT32		dwDevCfgAddr;
  UINT32    dwDrvVer;
  UINT32    dwDevUnitNum;
}PCI_KGI_O_DRV_CTRL;
typedef struct
{
	UINT32 dwOffset;	
	UINT32 dwData;	
}KGI_O_DATA_STRUCT;

extern STATUS D_Kgi_o_InstallDrv(UINT32 dwKgi_oNum);
extern UINT8  GodsonGetBaseIrq();
extern STATUS GodsonPicIntDisable(int iIntLevel);
extern STATUS GodsonPicIntEnable(int iIntLevel);


#endif  /* __INC_LSKIO_DRV_H */
