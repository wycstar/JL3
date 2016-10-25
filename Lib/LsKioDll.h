
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

/*�������ṩ���û����ⲿ�ӿ�*/
/************************************************************************
ԭ�ͣ�   STATUS Drv_KGIO_Init(BYTE CardId)
���ܣ�   ��ʼ��KGIO�豸.
������   CardId,Kgio�İ忨��,ֻ��һ��Kgio��ʱ,�ò���Ϊ0��
����ֵ�� �ɹ�����OK,ʧ�ܷ���ERROR.
*************************************************************************/
extern STATUS Drv_KGIO_Init(UINT8 CardId);

/************************************************************************
ԭ�ͣ�STATUS Drv_KGO_Write(UINT8 CardId,UINT32 dwOutData)
���ܣ����������8·����.
������CardId,Kgio�İ忨��,�ò���Ϊ0;
	    dwOutData����Ҫ�����8·IO״ֵ̬.
����ֵ���ɹ�����OK,ʧ�ܷ���ERROR.
*************************************************************************/
extern STATUS Drv_KGO_Write(UINT8 CardId,UINT8 dwOutData);

/***************************************************************************
ԭ�ͣ�STATUS Drv_KGO_Get(UINT8 CardId,UINT32 *pKGOStat)
���ܣ��ض���ǰ�Ŀ���״̬��
������CardId,Kgio�İ忨�ţ��Ǹ��������ò���Ϊ0��
		  pKGOStat,�ض�8·����״̬�Ļ��������ǿա�
����ֵ��OK��ʾ�ض�����ɹ���ERROR��ʾ�ض�ʧ�ܡ�
ע������� ��
*****************************************************************************/
extern STATUS Drv_KGO_Get(UINT8 CardId,UINT8 *pKGOStat);

/***************************************************************************
ԭ�ͣ�Drv_KGI_Filter_Para_Write(UINT8 CardId,UINT32 dwFilterNum)
���ܣ�����8·IO������˲�������
������CardId,Kgio�İ忨�ţ��Ǹ��������ò���Ϊ0��
		  dwFilterNum,�˲�������ȡֵ��ΧΪ1��10��
����ֵ��OK��ʾ�ɹ���ERROR��ʾʧ�ܡ�

*****************************************************************************/
extern STATUS Drv_KGI_Filter_Para_Write(UINT8 CardId,UINT8 dwFilterNum);

/***************************************************************************
ԭ�ͣ�Drv_KGI_Filter_Para_Read(UINT8 CardId)
���ܣ��ض��˲��������ýӿ�ʵ�ʾ��Ƕ���Drv_KGI_Filter_Para_Writeд����˲�������
������CardId,Kgio�İ忨�ţ��Ǹ��������ò���Ϊ0��
����ֵ��0��10��ʾ���õ��˲�ֵ��0xffffffff��ʾKGI�豸δ��ʼ����0xffff0000��ʾ����������Ϸ���
ע����� �� ��
*****************************************************************************/
extern UINT32 Drv_KGI_Filter_Para_Read(UINT8 CardId);

/***************************************************************************
ԭ�ͣ�STATUS Drv_KGI_Get(UINT8 CardId,UINT32 *pdwSignal)
���ܣ���ȡ8·����״̬�ĵ�ǰ״ֵ̬��
������CardId,Kgio�İ忨�ţ��Ǹ��������ò���Ϊ0��
	  pdwSignal���洢��ȡ����IO״ֵ̬�Ļ�������ÿһ·1��ʾ�п��룬0��ʾ�޿��롣
����ֵ��OK��ʾ��ȡ��ǰ����״ֵ̬�ɹ���ERROR��ʾ��ȡ��ǰ����״ֵ̬ʧ�ܡ�
ע������ޡ�
*****************************************************************************/
extern STATUS Drv_KGI_Get(UINT8 CardId,UINT8 *pdwSignal);

/***************************************************************************
ԭ�ͣ�UINT32 Drv_Get_Version(UINT8 CardId)
���ܣ���ȡ�����汾��
������CardId,Kgio�İ忨�ţ��Ǹ��������ò���Ϊ0��
����ֵ���汾��,��0x100��ʾ1.00�汾��
*****************************************************************************/
extern UINT32 Drv_Get_Version(UINT8 CardId);

#endif/*__INC_LSKIO_DLL_H*/





