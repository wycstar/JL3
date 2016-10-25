
#ifndef __INC_LSKIO_DLL_H
#define __INC_LSKIO_DLL_H
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
#include "sysLib.h"
#include "LsKioDrv.h"
#define KGI_OERR_OK                              (0)
#define KGI_OERR_DRV_INSTALLFAIL                 (-1)
#define KGI_OERR_DEVICE_OPENFAIL                 (-2)
#define KGI_OERR_INVALID_PARA                    (-3)
#define KGI_OERR_DEVICE_NOINIT                   (-4)
#define KGI_OERR_DEVICE_READ                     (-5)
#define KGI_OERR_DEVICE_WRITE                    (-6)
#define KGI_OERR_GETSEM_FAIL					           (-7)
#define KGI_OERR_CREATEMUX_FAIL                  (-8)
#define KGI_OERR_REC_TIMEOUT                     (-9)
#define KGI_OERR_REC_ERR                         (-10)

/*下面是提供给用户的外部接口*/
/************************************************************************
原型：   STATUS Drv_KGIO_Init(BYTE CardId)
功能：   初始化KGIO设备.
参数：   CardId,Kgio的板卡号,只有一块Kgio板时,该参数为0。
返回值： 成功返回OK,失败返回ERROR.
*************************************************************************/
extern STATUS Drv_KGIO_Init(UINT8 CardId);

/************************************************************************
原型：STATUS Drv_KGO_Write(UINT8 CardId,UINT32 dwOutData)
功能：开关量输出8路控制.
参数：CardId,Kgio的板卡号,该参数为0;
	    dwOutData：需要输出的8路IO状态值.
返回值：成功返回OK,失败返回ERROR.
*************************************************************************/
extern STATUS Drv_KGO_Write(UINT8 CardId,UINT8 dwOutData);

/***************************************************************************
原型：STATUS Drv_KGO_Get(UINT8 CardId,UINT32 *pKGOStat)
功能：回读当前的开出状态。
参数：CardId,Kgio的板卡号，非负整数，该参数为0。
		  pKGOStat,回读8路开出状态的缓冲区，非空。
返回值：OK表示回读输出成功，ERROR表示回读失败。
注意事项：无 。
*****************************************************************************/
extern STATUS Drv_KGO_Get(UINT8 CardId,UINT8 *pKGOStat);

/***************************************************************************
原型：Drv_KGI_Filter_Para_Write(UINT8 CardId,UINT32 dwFilterNum)
功能：设置8路IO开入的滤波参数。
参数：CardId,Kgio的板卡号，非负整数，该参数为0。
		  dwFilterNum,滤波参数，取值范围为1到10。
返回值：OK表示成功，ERROR表示失败。

*****************************************************************************/
extern STATUS Drv_KGI_Filter_Para_Write(UINT8 CardId,UINT8 dwFilterNum);

/***************************************************************************
原型：Drv_KGI_Filter_Para_Read(UINT8 CardId)
功能：回读滤波参数，该接口实际就是读出Drv_KGI_Filter_Para_Write写入的滤波参数。
参数：CardId,Kgio的板卡号，非负整数，该参数为0。
返回值：0到10表示设置的滤波值，0xffffffff表示KGI设备未初始化，0xffff0000表示输入参数不合法。
注意事项： 无 。
*****************************************************************************/
extern UINT32 Drv_KGI_Filter_Para_Read(UINT8 CardId);

/***************************************************************************
原型：STATUS Drv_KGI_Get(UINT8 CardId,UINT32 *pdwSignal)
功能：读取8路开入状态的当前状态值。
参数：CardId,Kgio的板卡号，非负整数，该参数为0。
	  pdwSignal：存储读取到的IO状态值的缓冲区。每一路1表示有开入，0表示无开入。
返回值：OK表示读取当前输入状态值成功，ERROR表示读取当前输入状态值失败。
注意事项：无。
*****************************************************************************/
extern STATUS Drv_KGI_Get(UINT8 CardId,UINT8 *pdwSignal);

/***************************************************************************
原型：UINT32 Drv_Get_Version(UINT8 CardId)
功能：获取驱动版本号
参数：CardId,Kgio的板卡号，非负整数，该参数为0。
返回值：版本号,如0x100表示1.00版本。
*****************************************************************************/
extern UINT32 Drv_Get_Version(UINT8 CardId);

#endif/*__INC_LSKIO_DLL_H*/





