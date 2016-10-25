#include "control.h"
#include "net.h"
#include "file.h"
#include "can.h"

CONTROL control;

control::control()
{

}

control::~control()
{

}

STATUS control::driverEnableAct(const int node[AXIS_NUM]) const
{
	CAN_PACKET sendPack,recvPack;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	file.writeLogFile("开始功率使能操作!");
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
	{
		if(node[nodeNum])
		{
			if(ERROR == semTake(GV::isCANOccupy,SEM_TIME_OUT * SYS_FREQ))
			{
				DISPLAY_FAULT("驱动器使能：获取CAN总线资源失败!");
				file.writeLogFile("驱动器使能:CAN总线互锁信号量接收超时!");
				return ERROR;
			}
			can.writeCopleyCMD(sendPack,DRIVER_ENABLE,0,nodeNum + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
			{
				semGive(GV::isCANOccupy);
				DISPLAY_FAULT("驱动器使能：CAN数据包发送失败!");
				return ERROR;
			}
			else
				semGive(GV::isCANOccupy);
		}
	}
	return OK;
}

STATUS control::driverEnableCheck(const int node[AXIS_NUM]) const
{
	bool tIsEnable = true;
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++){
		if(node[nodeNum])
			tIsEnable &= !(GV::isSingleEnableFailed[nodeNum] = !GV::isPowerOutput[nodeNum]);}
	if(tIsEnable)
	{
		file.writeLogFile("驱动器使能成功!");
		return OK;
	}
	else
	{
		GV::isFaultOccur   = 1;
		GV::errorArray[15] = 1;
		GV::isEnableFailed = TRUE;
		stringstream ss;
		for(UINT i = 0;i < AXIS_NUM;i++){
			if(!(GV::isPowerOutput[i]) && node[i])
				ss << static_cast<char> ('A' + i) << " ";}
		ss << "轴功率使能失败";
		DISPLAY_FAULT(ss.str().c_str());
		file.writeLogFile(ss.str().c_str());
		return ERROR;
	}
}

STATUS control::driverEnable(const int node[AXIS_NUM]) const
{
	driverEnableAct(node);
	taskDelay(sysClkRateGet());
	return driverEnableCheck(node);
}

STATUS control::driverDisableAct(const int node[AXIS_NUM]) const
{
	CAN_PACKET sendPack,recvPack;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	file.writeLogFile("开始功率使能断开操作!");	
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
	{
		if(node[nodeNum])
		{
			if(ERROR == semTake(GV::isCANOccupy,SEM_TIME_OUT * SYS_FREQ))
			{
				DISPLAY_FAULT("驱动器断开使能：获取CAN总线资源失败!");
				file.writeLogFile("驱动器断开使能:CAN总线互锁信号量接收超时!");
				return ERROR;
			}
			can.writeCopleyCMD(sendPack,DRIVER_DISABLE,0,nodeNum + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
			{
				semGive(GV::isCANOccupy);
				DISPLAY_FAULT("驱动器断开使能：CAN数据包发送失败!");
				return ERROR;
			}
			else
				semGive(GV::isCANOccupy);
		}
	}
	return OK;
}

STATUS control::driverDisableCheck(const int node[AXIS_NUM]) const
{
	bool tIsDisable = true;
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
		tIsDisable &= (node[nodeNum] ? !(GV::isPowerOutput[nodeNum]) : true);
	if(tIsDisable)
	{
		file.writeLogFile("驱动器使能断开成功!");
		return OK;
	}
	else
	{
		stringstream ss;
		for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++){
			if(GV::isPowerOutput[nodeNum] && node[nodeNum])
				ss << static_cast<char> ('A' + nodeNum) << "轴功率使能断开失败!";}
		DISPLAY_FAULT(ss.str().c_str());
		file.writeLogFile(ss.str().c_str());
		return ERROR;
	}
}

STATUS control::driverDisable(const int node[AXIS_NUM]) const
{
	driverDisableAct(node);
	taskDelay(SYS_FREQ);
	return driverDisableCheck(node);
}

STATUS control::findZeroPos() const
{
	if(GV::isStopAll)
	{
		file.writeLogFile("全局已停止,启动失败!");
		return ERROR;
	}
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++){
		if(!GV::isPowerOutput[nodeNum] || GV::isBoltOn[nodeNum] || GV::isLimit[nodeNum]){
			DISPLAY_FAULT("寻零失败,请检查驱动器是否上电,转台是否处在插销和限位状态!");
			return ERROR;}}
	CAN_PACKET sendPack,recvPack;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	UINT checkRetryTimes = 0;
	BOOL tIsTargetPosSet = TRUE;
	BOOL tIsActPosZero	 = TRUE;
	file.writeLogFile("开始转台寻零操作!");
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
	{
		if(ERROR == semTake(GV::isCANOccupy,SEM_TIME_OUT*SYS_FREQ))
		{
			DISPLAY_FAULT("寻零操作:获取CAN总线资源失败!");
			file.writeLogFile("寻零操作:获取CAN锁信号量超时!");
			return ERROR;
		}
		can.writeCopleyCMD(sendPack,DRIVER_CMD_POS_SET,0,nodeNum + 1);
		if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		can.writeCopleyCMD(sendPack,DRIVER_CMD_VEL_SET,5,nodeNum + 1);
		if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		can.writeCopleyCMD(sendPack,DRIVER_CLEAR_MOVE,0,nodeNum + 1);
		if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		semGive(GV::isCANOccupy);
	}
	if(ERROR == semTake(GV::isCANOccupy,SEM_TIME_OUT * SYS_FREQ))
	{
		DISPLAY_FAULT("寻零操作:获取CAN总线资源失败!");
		file.writeLogFile("寻零操作:获取CAN锁信号量超时!");
		return ERROR;
	}
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
	{
		can.writeCopleyCMD(sendPack,DRIVER_ABS_START,0,nodeNum + 1);
		if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			DISPLAY_FAULT("寻零操作：CAN数据包发送失败!");
			file.writeLogFile("寻零操作:发送指令失败!");
			return ERROR;
		}
	}
	semGive(GV::isCANOccupy);
	taskDelay(sysClkRateGet());
	while(1)
	{
		tIsTargetPosSet = (tIsActPosZero = TRUE);
		for(UINT i = 0;i < AXIS_NUM;i++)
		{	
			tIsTargetPosSet &= GV::isTargetPosSet[i];
			GV::axisActPos[i] == 0 ? tIsActPosZero &= TRUE : tIsActPosZero &= FALSE;
		}
		if(tIsTargetPosSet || tIsActPosZero)
		{
			for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
				GV::isZeroPosSet[nodeNum] = TRUE;
			file.writeLogFile("寻找零位成功!");
			return OK;
		}
		else if((++checkRetryTimes) >= 600 || GV::isStopAll)
		{
			GV::isFindZeroFailed = (GV::errorArray[16] = (GV::isFaultOccur = TRUE));
			memset(&GV::isZeroPosSet,0,sizeof(GV::isZeroPosSet));
			DISPLAY_FAULT("寻零操作超时!");
			error("寻找零位不成功!",ERROR_ALL_NODE);
			return ERROR;
		}
		taskDelay(sysClkRateGet());
	}
}

STATUS control::runCustom(const float pos, const float vel, const int channel) const
{
	if(GV::isStopAll)
	{
		DISPLAY_FAULT("系统已清除状态,停止接收启动指令,启动失败!");
		file.writeLogFile("系统已清除状态,停止接收启动指令!");
		return ERROR;
	}
	for(UINT nodeNum = 0; nodeNum < AXIS_NUM; nodeNum++){
		if(!GV::isPowerOutput[nodeNum] || !GV::isZeroPosSet[nodeNum] || (GV::opMode[nodeNum] != 0x1) || GV::isBoltOn[nodeNum] || GV::isLimit[nodeNum]){
			printf("%d %d %d %x\n",GV::isPowerOutput[nodeNum],GV::isZeroPosSet[nodeNum],GV::isBoltOn[nodeNum],GV::opMode[nodeNum]);
			DISPLAY_FAULT("启动失败,请检查驱动器是否上电,转台是否处在插销和限位状态!");
			return ERROR;}}
	CAN_PACKET sendPack,recvPack;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	stringstream logStr;
	if(OK != semTake(GV::isCANOccupy,SEM_TIME_OUT*SYS_FREQ)){
		DISPLAY_FAULT("启动操作:获取CAN总线资源失败!");
		file.writeLogFile("启动操作:获取CAN锁信号量超时!");
		return ERROR;}
	can.writeCopleyCMD(sendPack,DRIVER_CMD_POS_SET, pos, channel + 1);
	if(ERROR == can.uCMDSend(sendPack, recvPack, USING_CAN_CHANNEL))
	{
		semGive(GV::isCANOccupy);
		return ERROR;
	}
	can.writeCopleyCMD(sendPack,DRIVER_CMD_VEL_SET, vel, channel + 1);
	if(ERROR == can.uCMDSend(sendPack, recvPack, USING_CAN_CHANNEL))
	{
		semGive(GV::isCANOccupy);
		return ERROR;
	}
	can.writeCopleyCMD(sendPack, DRIVER_CMD_ACC_SET, MAX_ACC[channel], channel + 1);
	if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
	{
		semGive(GV::isCANOccupy);
		return ERROR;
	}
	can.writeCopleyCMD(sendPack, DRIVER_CMD_DEACC_SET, MAX_ACC[channel], channel + 1);
	if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
	{
		semGive(GV::isCANOccupy);
		return ERROR;
	}
	can.writeCopleyCMD(sendPack, DRIVER_CLEAR_MOVE, 0, channel + 1);
	if(ERROR == can.uCMDSend(sendPack, recvPack, USING_CAN_CHANNEL))
	{
		semGive(GV::isCANOccupy);
		return ERROR;
	}
	can.writeCopleyCMD(sendPack, DRIVER_ABS_START, 0, channel + 1);
	if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
	{
		semGive(GV::isCANOccupy);
		DISPLAY_FAULT("启动操作：CAN数据包发送失败!");
		file.writeLogFile("启动操作:指令发送失败!");
		return ERROR;
	}
	semGive(GV::isCANOccupy);
	logStr << static_cast<char>('A' + channel) << "轴定位模式运行  " 
		   << "绝对位置：" << pos << "	"
		   << "运行速率：" << vel << "	";
	file.writeLogFile(logStr.str().c_str());
	return OK;
}

STATUS runPreset()
{
	UINT64 endTick = 0;
	UINT64 eStartTick = 0;
	if(GV::isStopAll)
	{
		DISPLAY_FAULT("系统已清除状态,停止接收启动指令,启动失败!");
		file.writeLogFile("系统已清除状态,停止接收启动指令!");
		return ERROR;
	}
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++){
		if(!GV::isPowerOutput[nodeNum] || !GV::isZeroPosSet[nodeNum] || (GV::opMode[nodeNum] != 0x1) || GV::isBoltOn[nodeNum] || GV::isLimit[nodeNum]){
			printf("%d %d %d %x\n",GV::isPowerOutput[nodeNum],GV::isZeroPosSet[nodeNum],GV::isBoltOn[nodeNum],GV::opMode[nodeNum]);
			DISPLAY_FAULT("启动失败,请检查驱动器是否上电,转台是否处在插销和限位状态!");
			return ERROR;}}
	CAN_PACKET sendPack,recvPack;
	stringstream ss;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	UCHAR buf[WCTLMSG_SIZE];
	memset(buf, 0, WCTLMSG_SIZE);
	memcpy(buf, &GV::ctrlParamMsg, WCTLMSG_SIZE);
	int validBit = 0;
	for(validBit = 0;validBit < 5;validBit++){
		if(buf[2 + validBit] ^ 0xff){
			break;}}
	if(buf[2 + validBit] == 0xa0){
		control.findZeroPos();
		return OK;}
	else{
		ss << buf[2 + validBit] - 0xa0;
		for(int seg = 0; seg < file.jsonDoc[ss.str().c_str()]["seg"].GetInt(); seg++)
		{
			if(OK != semTake(GV::isCANOccupy,SEM_TIME_OUT*SYS_FREQ))
			{
				DISPLAY_FAULT("启动操作:获取CAN总线资源失败!");
				file.writeLogFile("启动操作:获取CAN锁信号量超时!");
				return ERROR;
			}
			can.writeCopleyCMD(sendPack,
							   DRIVER_CMD_POS_SET,
							   file.jsonDoc[ss.str().c_str()]["angle"][seg].GetDouble(),
							   validBit + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
				semGive(GV::isCANOccupy);
				return ERROR;}
			can.writeCopleyCMD(sendPack,
							   DRIVER_CMD_VEL_SET,
							   fabs(file.jsonDoc[ss.str().c_str()]["angle"][seg].GetDouble() / file.jsonDoc[ss.str().c_str()]["time"][seg].GetDouble()) * 1.2,
							   validBit + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
				semGive(GV::isCANOccupy);
				return ERROR;}
			can.writeCopleyCMD(sendPack,
							   DRIVER_CMD_ACC_SET,
							   MAX_ACC[validBit],
							   validBit + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
				semGive(GV::isCANOccupy);
				return ERROR;}
			can.writeCopleyCMD(sendPack,
							   DRIVER_CMD_DEACC_SET,
							   MAX_ACC[validBit],
							   validBit + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
				semGive(GV::isCANOccupy);
				return ERROR;}
			can.writeCopleyCMD(sendPack,
							   DRIVER_CLEAR_MOVE,
							   0,
							   validBit + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
				semGive(GV::isCANOccupy);
				return ERROR;}
			can.writeCopleyCMD(sendPack,
							   DRIVER_REL_START,
							   0,
							   validBit + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
				semGive(GV::isCANOccupy);
				return ERROR;}
			semGive(GV::isCANOccupy);
			endTick = tick64Get();
			taskDelay(static_cast<int>((file.jsonDoc[ss.str().c_str()]["time"][seg].GetDouble()
									  * SYS_FREQ)) - (seg == 0 ? 0 : (endTick - eStartTick)));
			eStartTick = tick64Get();
		}}
	return OK;
}

STATUS control::stop() const
{
	CAN_PACKET sendPack,recvPack;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
	{
		if(OK != semTake(GV::isCANOccupy,SEM_TIME_OUT * SYS_FREQ)){
			DISPLAY_FAULT("停止操作:获取CAN总线资源失败!");
			file.writeLogFile("停止操作:获取CAN锁信号量超时!");
			return ERROR;}
		can.writeCopleyCMD(sendPack,DRIVER_POS_STOP,0,nodeNum + 1);
		if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
			semGive(GV::isCANOccupy);
			DISPLAY_FAULT("停止操作：CAN数据包发送失败!");
			return ERROR;}
		semGive(GV::isCANOccupy);
	}
	file.writeLogFile("转台停止转动!");
	return OK;
}

STATUS control::error(const char *content,const int node[AXIS_NUM]) const
{
	CAN_PACKET sendPack,recvPack;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	driverDisable(node);
	faultLEDControl(true,node);
	return OK;
}

STATUS control::exit() const
{
	CAN_PACKET sendPack,recvPack;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	driverDisable(ERROR_ALL_NODE);
	faultLEDControl(false,ERROR_ALL_NODE);
	taskDelay(sysClkRateGet() / 2);
	GV::isStopAll = TRUE;
	taskDelay(sysClkRateGet() / 2);
	can.release();
	net.release();
	digitalIO.release();
	DISPLAY_FAULT("关机退出!");
	file.writeLogFile("关机退出!");
	return OK;
}

STATUS control::faultLEDControl(const BOOL &isLightOn,const int node[AXIS_NUM]) const
{
	CAN_PACKET sendPack,recvPack;
	memset(&sendPack,0,CP_SIZE);
	memset(&recvPack,0,CP_SIZE);
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
	{
		if(node[nodeNum] & 0x1)
		{
			if(ERROR == semTake(GV::isCANOccupy,SEM_TIME_OUT * SYS_FREQ))
			{
				file.writeLogFile("亮灯操作:获取CAN锁信号量超时!");
				return ERROR;
			}
			can.writeCopleyCMD(sendPack,DRIVER_DIO_OUT_SET,(isLightOn ? 2:0),nodeNum + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
				semGive(GV::isCANOccupy);
				return ERROR;}
			semGive(GV::isCANOccupy);
		}
	}
	return OK;
}
