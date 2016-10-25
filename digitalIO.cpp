#include "digitalIO.h"
#include "net.h"
#include "file.h"
#include "can.h"
#include "control.h"

DIO digitalIO;

STATUS DIO::init()
{
	BOOL isPowerOnPrinted = false;
	memset(IORegBit,0,sizeof(IORegBit));
	memset(lastIORegBit,0,sizeof(lastIORegBit));
	if(ERROR == Drv_KGIO_Init(0)){
		DISPLAY_FAULT("IO初始化:IO口初始化错误!");
		return ERROR;}
	else{
		while(1){
			UCHAR ioBuf = 0xff;
			Drv_KGI_Get(0, &ioBuf);
			if(ioBuf & 0x1)
				break;
			if(!isPowerOnPrinted){
				DISPLAY_FAULT("转台未上电,请上电!");
				isPowerOnPrinted = true;}
			taskDelay(SYS_FREQ / 2);}
		taskDelay(5 * sysClkRateGet());
		return OK;}
}

STATUS DIO::release()
{
	if(ERROR == semDelete(GV::IOReadTaskSem)){
		printf("release:Delete Sem IOReadTaskSem ERROR!\n");
		return ERROR;}
	else
		return OK;
}

STATUS DIO::read(UCHAR *buff)
{
	if(OK == Drv_KGI_Get(0,buff))
		return OK;
	else
		return ERROR;
}

STATUS DIO::write(UCHAR buff)
{
	Drv_KGO_Write(0,buff);
	return 0;
}

STATUS IOReadTask()
{
#define DEBUG
	UCHAR buff = 0;
	while(!(GV::isStopAll))
	{
		if(ERROR == semTake(GV::IOReadTaskSem,WAIT_FOREVER))
		{
			DISPLAY_FAULT("IO监测任务定时信号量获取超时,任务已停止!");
			return ERROR;
		}
		if(OK == (Drv_KGI_Get(0,&buff)))
		{
			for(int i = 0;i < DIO::IO_REG_BIT_NUM;i++){
				digitalIO.IORegBit[i] = (buff>>i)&0x01;}
			if((digitalIO.IORegBit[0] == true) && (digitalIO.lastIORegBit[0] == false))
			{
				file.writeLogFile("驱动器上电");
				GV::errorArray[0] = 0;
				GV::isPowerOn     = 1;
			}
			else if((digitalIO.IORegBit[0] == false) && (digitalIO.lastIORegBit[0] == true))
			{
				DISPLAY_FAULT("驱动器已断电");
				file.writeLogFile("驱动器已断电");
				GV::errorArray[0] = 1;
				GV::isPowerOn     = 0;
				GV::isFaultOccur  = true;
			}
			if((digitalIO.IORegBit[1] == true) && (digitalIO.lastIORegBit[1] == false))
			{
				file.writeLogFile("急停触发,驱动器断电,关机退出!");
				DISPLAY_FAULT("急停触发,请重启计算机!");
				GV::errorArray[14] = 1;
				GV::isEmerStop	   = true;
				GV::isFaultOccur   = true;
				GV::isStopAll	   = true;
			}
		}
		else{
			DISPLAY_FAULT("读取数字IO状态失败!");}
		memcpy(&(digitalIO.lastIORegBit),&(digitalIO.IORegBit),sizeof(digitalIO.IORegBit));
		semGive(GV::CANAndIOSyn);
	}
	return ERROR;
}
