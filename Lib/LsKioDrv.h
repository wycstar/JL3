#ifndef __INC_LSKIO_DRV_H
#define __INC_LSKIO_DRV_H

#define GODSON_MIPS
#define DRV_MEM_OPT 1 /*���ʷ�ʽ io����mem*/


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
/*�ֽ���ת����:Big_endian ~ Little_endian*/
#define sw_LB_endian32(test)    \
    (UINT32)((((UINT32)(test)&(UINT32)0x000000ff)<<24)  \
    |(((UINT32)(test)&(UINT32)0x0000ff00)<<8)   \
    |(((UINT32)(test)&(UINT32)0x00ff0000)>>8)   \
    |(((UINT32)(test)&(UINT32)0xff000000)>>24))
#endif

/*************************���Ƽ����************************/
/*��8·���룬8·����*/

#define KIO_VERSION_REG      (0x10)/*�汾�Ĵ�����ֻ������λֵΪ0x100���汾��1.00*/

#define KIO_OUT_REG          (0x0) /*�����Ĵ�������8λ�洢����ֵ����д��0��ʾ�п�����1��ʾ�޿�������λֵΪ0xffff*/
#define KIO_IN_REG           (0x8) /*����Ĵ�������8λ�洢����ֵ��ֻ����1��ʾ�п��룬0��ʾ�޿��룬��λֵΪ0x0*/
#define KIO_BIND_REG         (0x14)/*��ʶ�Ĵ�����ֻ������λֵΪ0xBD*/ 
#define KIO_TEST_REG         (0x18)/*�û��Ĵ�������д����λֵΪ0xAA*/
#define KIO_SELF_SET_REG     (0x1c)/*�Ի��Ĵ�������д����λֵΪ0x0����0λΪ0��ʾ�ر��Ի����ԣ�Ϊ1��ʾ�����Ի�����*/
#define KIO_FILTER_SET       (0x20)/*�˲��Ĵ�������д����λֵΪ0x0A��0-10��ʾ0-10ms�˲�����*/
#define IER_REG              (0x24)/*�жϼĴ�������д����λֵΪ0x0��0x0��ʾ�жϹرգ�
                                     0x3��ʾ�������½����жϴ򿪣�0x1��ʾ�������жϣ�0x2��ʾ�½����ж�*/
#define IERS_R_REG           (0x28)/*�ж�״̬�Ĵ�����15-0λ��1��ʾ�����ؿ���;31-16λ��1��ʾ�½��ؿ���*/

/*************************************************************/

#define PCI9052_INCSR                       0x4c
#define KGI_O_IN_CHAN_SUM                   (8)
#define KGI_O_OUT_CHAN_SUM                  (8)
#define KGI_O_FILTER_MAX                    (0xa)
#define KGI_O_MAX_BOARD_SUM                 (0x1)
#define KGI_O_FIFO_SUM                      (1024)
#define KGI_O_BUF_SUM                       (2046)/*Ӳ��fifo��СΪ1024�����fifo��С��Ϊ4096*/

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
