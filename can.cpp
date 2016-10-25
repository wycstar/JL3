#include "can.h"
#include "control.h"

CAN can;

can::can()
{

}

can::~can()
{

}

STATUS can::init()
{
	CAN_PACKET tSendPack, tRecvPack;
	for(UINT i = 0;i < CAN_CHANNEL;i++)
	{
		if(ERROR == CanDevInstall(0,i))
		{
			DISPLAY_FAULT("CAN初始化:驱动加载失败，请重启计算机!");
			return ERROR;
		}
		
		if(ERROR == (CANHandle[i] = CanOpenDriver(0,i)))
		{
			DISPLAY_FAULT("CAN初始化:驱动注册失败，请重启计算机!");
			return ERROR;
		}
	
		CANPara[i].iBaudrate = CAN_500KBPS;
		CANPara[i].iMode 	 = 0;
		CANPara[i].dwAccCode = 0xffffffff;
		CANPara[i].dwAccMask = 0xffffffff;
	
		if(ERROR == CanConfigPort(CANHandle[i],&CANPara[i]))
		{
			DISPLAY_FAULT("CAN初始化:参数设置失败，请重启计算机!");
			return ERROR;
		}
		recvSem[i] = semCCreate(SEM_Q_FIFO ,0);
		CanEnableReceive(CANHandle[i]);
		CanInstallEvent(CANHandle[i], 0, &recvSem[i]);
	}
	taskDelay(SYS_FREQ / 2);
	if(OK != semTake(GV::isCANOccupy,SEM_TIME_OUT * SYS_FREQ)){
		DISPLAY_FAULT("CAN初始化:获取CAN资源失败，请重启计算机!");
		return ERROR;}
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
	{
		for(UINT i = 0;i < 4;i++)
		{
			writeCopleyCMD(tSendPack,DRIVER_FORBID_PDO_A + i,0,nodeNum + 1);
			if(ERROR == uCMDSend(tSendPack,tRecvPack,USING_CAN_CHANNEL)){
			    DISPLAY_FAULT("CAN初始化:发送CAN数据包失败，请重启计算机!");
			    return ERROR;}
		}
		writeCopleyCMD(tSendPack,DRIVER_DIO_OUT_SET,0,nodeNum + 1);
		if(ERROR == uCMDSend(tSendPack,tRecvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		writeCopleyCMD(tSendPack,DRIVER_ACT_ALL_NODE,0,nodeNum + 1);
		if(ERROR == uCMDSend(tSendPack,tRecvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		writeCopleyCMD(tSendPack,DRIVER_POS_MODE_SET,0,nodeNum + 1);
		if(ERROR == uCMDSend(tSendPack,tRecvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		writeCopleyCMD(tSendPack,DRIVER_TRJ_MODE_SET,0,nodeNum + 1);
		if(ERROR == uCMDSend(tSendPack,tRecvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		writeCopleyCMD(tSendPack,DRIVER_CMD_ACC_SET,10,nodeNum + 1);
		if(ERROR == uCMDSend(tSendPack,tRecvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		writeCopleyCMD(tSendPack,DRIVER_CMD_DEACC_SET,10,nodeNum + 1);
		if(ERROR == uCMDSend(tSendPack,tRecvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}
		writeCopleyCMD(tSendPack,DRIVER_QST_DEACC_SET, nodeNum < 3 ? 10 : 50,nodeNum + 1);
		if(ERROR == uCMDSend(tSendPack,tRecvPack,USING_CAN_CHANNEL))
		{
			semGive(GV::isCANOccupy);
			return ERROR;
		}		
	}
	semGive(GV::isCANOccupy);
	return OK;
}

STATUS CAN::release()
{
	if(ERROR == semDelete(GV::isCANOccupy))
		printf("release:Delete Sem isCANOccupy ERROR!\n");
	if(ERROR == semDelete(GV::CANReadTaskSem))
		printf("release:Delete Sem CANReadTaskSem ERROR!\n");
	for(UINT i = 1;i < CAN_CHANNEL;i++)
	{
		if(ERROR == semDelete(recvSem[i]))
			printf("release:Delete Sem recvSem[%d] ERROR!\n",i);
	}
	for(UINT i = 1;i < CAN_CHANNEL;i++)
	{
		if(ERROR == CanCloseDriver(CANHandle[i]))
			printf("release:Close CAN Channel %d ERROR!\n",i);
	}
	return OK;
}

STATUS can::uCMDSend(CAN_PACKET &sendPack,CAN_PACKET &recvPack,const int &channel) const
{
	ULINT startTime = 0;
	ULINT endTime   = 0;
	string faultPreWriteString;
	string procName("uCMDSend:");
	if(channel < 0 || channel > static_cast<int>(CAN_CHANNEL))
	{
		DISPLAY_FAULT("CAN发送任务:错误的CAN口号,请重启计算机!");
		faultPreWriteString.append("CAN通信错误:错误的通道号!");
		goto ERROR_PROC;
	}
	if((CanSendMsg(CANHandle[channel],&sendPack)) == ERROR)
	{
		DISPLAY_FAULT("CAN发送任务:发送数据包错误,请重启计算机!");
		faultPreWriteString.append("CAN发送任务:发送数据包错误!");
		goto ERROR_PROC;
	}
	if(sendPack.dwCanID == 0 && sendPack.cData[0] == 0x01)
		return OK;
	startTime = tick64Get();
	while(1)
	{
		if(ERROR == semTake(recvSem[channel],SEM_TIME_OUT * SYS_FREQ))
		{
			DISPLAY_FAULT("CAN发送任务:驱动器响应超时,请重启计算机!");
			faultPreWriteString.append("CAN发送任务:总线缓冲区中断信号量接收超时!");
			goto ERROR_PROC;
		}
		endTime = tick64Get();
		if((endTime - startTime) > SEM_TIME_OUT* SYS_FREQ )
		{
			DISPLAY_FAULT("CAN发送任务:从驱动器收到错误回应,请重启计算机!");
			faultPreWriteString.append("CAN发送任务:总线数据接收超时!");
			goto ERROR_PROC;
		}
		if(CanRcvMsg(CANHandle[channel],&recvPack) == ERROR)
		{
			DISPLAY_FAULT("CAN发送任务:驱动器响应错误,请重启计算机!");
			faultPreWriteString.append("CAN发送任务:接收错误!");
			goto ERROR_PROC;
		}
		if(ERROR == uCMDCheck(sendPack,recvPack))
			continue;
		else
			return OK;
	}
ERROR_PROC:
	char sendBuf[NET_SEND_BUF_SIZE];
	memset(sendBuf,0,NET_SEND_BUF_SIZE);
	FT_MSG faultMsg;
	memset(&faultMsg,0,FTMSG_SIZE);
	faultMsg.msgType  = TABLE_FAULT_INFO_MSG;
	faultMsg.msgCount = net.currentSendCount;
	faultMsg.msgClass = 0xC0 | (0x2 << 14);
	faultMsg.statusInfoA = 0x1;
	strcpy(faultMsg.name,"CAN通讯错误,驱动器或已断电,请重启计算机!");
	memcpy(sendBuf, &faultMsg, FTMSG_SIZE);
	sendBuf[NET_SEND_BUF_SIZE - 1] = FTMSG_SIZE;
	msgQSend(net.sendingMsgQueue,sendBuf,NET_SEND_BUF_SIZE,WAIT_FOREVER,MSG_PRI_URGENT);
	DISPLAY_FAULT("CAN发送任务:CAN通讯错误,驱动器或已断电,请重启计算机!");
	stringstream ss;
	ss << faultPreWriteString
	   << "Send Index:" << std::hex << static_cast<unsigned>(sendPack.cData[2]) << std::hex << static_cast<unsigned>(sendPack.cData[1]) << "  "
	   << "Recv Index:" << std::hex << static_cast<unsigned>(recvPack.cData[2]) << std::hex << static_cast<unsigned>(recvPack.cData[1]);
	file.writeLogFile(ss.str().c_str());
	return ERROR;
}

STATUS can::uCMDCheck(const CAN_PACKET &sendPack,const CAN_PACKET &recvPack) const
{
	if((recvPack.cData[0]) == 0x60
			||(recvPack.cData[0]) == 0x43
			||(recvPack.cData[0]) == 0x4b
			||(recvPack.cData[0]) == 0x4f){
		return ((sendPack.cData[1] == recvPack.cData[1]) && (sendPack.cData[2] == recvPack.cData[2])) - 1;}
	else if((recvPack.cData[0]) == 0x0)
		return (sendPack.cData[0] == 0x01 && sendPack.cData[1] == 0x00) - 1;
	else
		return ERROR;
}

STATUS can::writeCopleyCMD(CAN_PACKET &sendPack,const UINT &op,const float &param,const int &NodeID) const
{
	memset(&sendPack,0,CP_SIZE);
	sendPack.dwCanID = 0x600+NodeID;
	switch(op)
	{
		case DRIVER_ABS_START:		//绝对启动
			sendPack.cLen 	  = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x40;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0x3f;
			break;
		case DRIVER_REL_START:		//相对启动
			sendPack.cLen     = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x40;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0x5f;
			break;
		case DRIVER_POS_STOP:		//停止
			sendPack.cLen 	  = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x40;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0x1f;
			sendPack.cData[5] = 0x1;
			break;
		case DRIVER_RESET:			//复位清错
			sendPack.cLen 	  = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x40;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0xab;
			break;
		case DRIVER_ENABLE:			//使能
			sendPack.cLen     = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x40;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0xf;
			break;
		case DRIVER_DISABLE:		//断开使能
			sendPack.cLen     = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x40;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0xe;
			break;
		case DRIVER_POS_MODE_SET:	//位置模式设置
			sendPack.cLen 	  = 5;
			sendPack.cData[0] = 0x2F;
			sendPack.cData[1] = 0x60;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0x01;
			break;
		case DRIVER_TRJ_MODE_SET:	//轨迹模式设置
			sendPack.cLen     = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x86;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0x0;
			break;
		case DRIVER_CMD_POS_SET:{	//目标位置设置
			double roll = static_cast<double>(param) / 360.0;
			int posLineCount = static_cast<int>(roll * ENC_RES);
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x7a;
			sendPack.cData[2] = 0x60;
			memcpy(&(sendPack.cData[4]),&posLineCount,INT_SIZE);}
			break;
		case DRIVER_CMD_VEL_SET:{		//目标速度设置
			double roll = static_cast<double>(param) / 36.0;
			int velLineCount = static_cast<int>(roll * ENC_RES);
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x81;
			sendPack.cData[2] = 0x60;
			memcpy(&(sendPack.cData[4]),&velLineCount,INT_SIZE);}
			break;
		case DRIVER_CMD_ACC_SET:{		//目标加速度设置
			double roll = static_cast<double>(param) / 3600.0;
			int accLineCount = static_cast<int>(roll * ENC_RES);
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x83;
			sendPack.cData[2] = 0x60;
			memcpy(&(sendPack.cData[4]),&accLineCount,INT_SIZE);}
			break;
		case DRIVER_CMD_DEACC_SET:{		//目标减速度设置
			double roll = static_cast<double>(param) / 3600.0;
			int deAccLineCount = static_cast<int>(roll * ENC_RES);
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x84;
			sendPack.cData[2] = 0x60;
			memcpy(&(sendPack.cData[4]),&deAccLineCount,INT_SIZE);}
			break;
		case DRIVER_QST_DEACC_SET:{		//目标减速度设置
			double roll = static_cast<double>(param) / 3600.0;
			int deAccLineCount = static_cast<int>(roll * ENC_RES);
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x85;
			sendPack.cData[2] = 0x60;
			memcpy(&(sendPack.cData[4]),&deAccLineCount,INT_SIZE);}
			break;
		case DRIVER_POS_LIMIT_SET:{		//正向软限位设置
			double pPosRoll = static_cast<double>(param) / 360.0;
			int pPosLimit = static_cast<int>(pPosRoll * ENC_RES);
			sendPack.cLen     = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x7d;
			sendPack.cData[2] = 0x60;
			sendPack.cData[3] = 0x02;
			memcpy(&(sendPack.cData[4]),&pPosLimit,INT_SIZE);}
			break;
		case DRIVER_NEG_LIMIT_SET:{		//负向软限位设置	
			double nPosRoll = static_cast<double>(param) / 360.0;
			int nPosLimit = static_cast<int>(nPosRoll * ENC_RES);
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x7d;
			sendPack.cData[2] = 0x60;
			sendPack.cData[3] = 0x01;
			memcpy(&(sendPack.cData[4]),&nPosLimit,INT_SIZE);}
			break;
		case DRIVER_DIO_OUT_SET:{		//输出设置
			sendPack.cLen     = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x94;
			sendPack.cData[2] = 0x21;
			int tempInt = static_cast<int>(param);
			memcpy(&(sendPack.cData[4]),&tempInt,INT_SIZE);}
			break;
		case DRIVER_CLEAR_MOVE:		//清空第四位和第五位
			sendPack.cLen     = 6;
			sendPack.cData[0] = 0x2b;
			sendPack.cData[1] = 0x40;
			sendPack.cData[2] = 0x60;
			sendPack.cData[4] = 0x0f;
			break;
		case DRIVER_ACT_ALL_NODE:	//启动所有节点
			sendPack.dwCanID  = 0x0;
			sendPack.cLen     = 2;
			sendPack.cData[0] = 0x01;
			break;
		case DRIVER_DISACT_ALL_NODE:	//关闭所有节点
			sendPack.dwCanID  = 0x0;
			sendPack.cLen     = 2;
			sendPack.cData[0] = 0x02;
			break;
		case DRIVER_FORBID_PDO_A:    //禁止PDO映射↓
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x00;
			sendPack.cData[2] = 0x18;
			sendPack.cData[3] = 0x01;
			sendPack.cData[4] = 0x01;
			sendPack.cData[5] = 0x03;
			sendPack.cData[6] = 0x00;
			sendPack.cData[7] = 0x80;
			break;
		case DRIVER_FORBID_PDO_B:
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x01;
			sendPack.cData[2] = 0x18;
			sendPack.cData[3] = 0x01;
			sendPack.cData[4] = 0x01;
			sendPack.cData[5] = 0x03;
			sendPack.cData[6] = 0x00;
			sendPack.cData[7] = 0x80;
			break;
		case DRIVER_FORBID_PDO_C:
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x02;
			sendPack.cData[2] = 0x18;
			sendPack.cData[3] = 0x01;
			sendPack.cData[4] = 0x01;
			sendPack.cData[5] = 0x03;
			sendPack.cData[6] = 0x00;
			sendPack.cData[7] = 0x80;
			break;
		case DRIVER_FORBID_PDO_D:
			sendPack.cLen = 8;
			sendPack.cData[0] = 0x23;
			sendPack.cData[1] = 0x03;
			sendPack.cData[2] = 0x18;
			sendPack.cData[3] = 0x01;
			sendPack.cData[4] = 0x01;
			sendPack.cData[5] = 0x03;
			sendPack.cData[6] = 0x00;
			sendPack.cData[7] = 0x80;
			break;
		default:
			DISPLAY_FAULT("writeCopleyCMD:Unknown Status!\n");
			return ERROR;
	}
	return OK;
}

STATUS can::readCopleyStatus(CAN_PACKET &sendPack,const UINT &op,const int &NodeID) const
{
	memset(&sendPack,0,CP_SIZE);
	sendPack.dwCanID = 0x600+NodeID;
	switch(op)
	{
		case DRIVER_DIO_IN_GET:		//输入状态
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x90;
			sendPack.cData[2] = 0x21;
			break;
		case DRIVER_DIO_OUT_GET:		//输出状态
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x94;
			sendPack.cData[2] = 0x21;
			break;
		case DRIVER_EVENT_REG_GET:	//事件状态寄存器
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x02;
			sendPack.cData[2] = 0x10;
			break;
		case DRIVER_FAULT_REG_GET:	//故障状态寄存器
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x83;
			sendPack.cData[2] = 0x21;
			break;
		case DRIVER_TRJ_REG_GET:	//轨迹状态寄存器
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x52;
			sendPack.cData[2] = 0x22;
			break;
		case DRIVER_ACT_POS_GET:		//实际位置
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x40;
			sendPack.cData[2] = 0x22;
			break;
		case DRIVER_ACT_VEL_GET:		//实际速度
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x69;
			sendPack.cData[2] = 0x60;
			break;
		case DRIVER_CMD_POS_GET:		//指令位置
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x22;
			sendPack.cData[2] = 0x21;
			break;
		case DRIVER_CMD_VEL_GET:		//指令速度
			sendPack.cLen = 4;
			sendPack.cData[0] = 0x40;
			sendPack.cData[1] = 0x6B;
			sendPack.cData[2] = 0x60;
			break;
		default:
			DISPLAY_FAULT("readCopleyStatus:Unknown Status!\n");
			return ERROR;
	}
	return OK;
}

void can::parseRegToBit(const UINT &reg,const int &meta,const int &op)
{
	switch(op)
	{
		case 101:
			for(int i = 0;i < EVENT_REG_BIT_NUM;i++)
				driverEventRegBit[meta][i] = (reg>>i)&0x01;
			break;
		case 102:
			for(int i = 0;i < FAULT_REG_BIT_NUM;i++)
				driverFaultRegBit[meta][i] = (reg>>i)&0x01;
			break;
		case 103:
			for(int i = 0;i < TRJ_REG_BIT_NUM;i++)
				driverTrjRegBit[meta][i] = (reg>>i)&0x01;
			break;
		case 104:
			for(int i = 0;i < DIN_REG_BIT_NUM;i++)
				driverDInRegBit[meta][i + 1] = (reg>>i)&0x01;
			break;
		case 105:
			for(int i = 0;i < DOUT_REG_BIT_NUM;i++)
				driverDOutRegBit[meta][i + 1] = (reg>>i)&0x01;
			break;
		default:
			DISPLAY_FAULT("parseRegToBit:OP Not Correct!\n");
	}
}

STATUS CANReadTask(void)
{
	CAN_PACKET sendPack,recvPack;
	BOOL lastIsPosLimit[AXIS_NUM];
	BOOL lastIsNegLimit[AXIS_NUM];
	BOOL lastIsLimit[AXIS_NUM];
	BOOL lastIsBolt[AXIS_NUM];
	BOOL lastIsDriverFault[AXIS_NUM];
	BOOL tErrorNode[AXIS_NUM];
	BOOL lastIsBoltOn  = false;
	ULINT disableStartTime[AXIS_NUM];
	ULINT boltEndTime = 0;
	memset(&sendPack,		 0,CP_SIZE);
	memset(&recvPack,		 0,CP_SIZE);
	memset(tErrorNode,		 0,sizeof(tErrorNode));
	memset(lastIsPosLimit,	 0,sizeof(lastIsPosLimit));
	memset(lastIsNegLimit,	 0,sizeof(lastIsNegLimit));
	memset(lastIsLimit,	  	 0,sizeof(lastIsLimit));
	memset(lastIsBolt,	  	 0,sizeof(lastIsBolt));
	memset(lastIsDriverFault,0,sizeof(lastIsDriverFault));
	memset(disableStartTime ,0,sizeof(disableStartTime));
	while(1)
	{
		bool tIsBoltOn 	    = false;
		bool tIsDriverFault = false;
		bool tIsLimit 	    = false;
		if(!(GV::isStopAll)){
		if(ERROR == semTake(GV::CANReadTaskSem,SEM_TIME_OUT * SYS_FREQ))
		{
			DISPLAY_FAULT("CAN监测任务:定时信号量接收超时,请重启计算机!");
			file.writeLogFile("CAN监测任务:CAN总线定时信号量接收超时!");
			return ERROR;
		}
		for(UINT nodeNum = 0; nodeNum < AXIS_NUM; nodeNum++)
		{
			if(ERROR == semTake(GV::isCANOccupy, SEM_TIME_OUT * SYS_FREQ))
			{
				DISPLAY_FAULT("CAN监测任务:获取CAN总线资源失败,请重启计算机!");
				file.writeLogFile("CAN监测任务:CAN总线互锁信号量接收超时!");
				return ERROR;
			}
			can.readCopleyStatus(sendPack,DRIVER_DIO_IN_GET,nodeNum + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
			{
				DISPLAY_FAULT("CAN监测任务:获取驱动器IO输入失败,请重启计算机!");
				return ERROR;
			}
			memcpy(&can.driverDINStatus[nodeNum],&(recvPack.cData[4]),4);
			can.parseRegToBit(can.driverDINStatus[nodeNum],nodeNum,CAN_DIN_REG_PARSE);

			can.readCopleyStatus(sendPack,DRIVER_DIO_OUT_GET,nodeNum	+ 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
			{
				DISPLAY_FAULT("CAN监测任务:获取驱动器IO输出失败,请重启计算机!");
				return ERROR;
			}
			memcpy(&can.driverDOUTStatus[nodeNum],&(recvPack.cData[4]),4);
			can.parseRegToBit(can.driverDOUTStatus[nodeNum],nodeNum,CAN_DOUT_REG_PARSE);

			can.readCopleyStatus(sendPack,DRIVER_EVENT_REG_GET,nodeNum + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
			{
				DISPLAY_FAULT("CAN监测任务:获取驱动器事件状态寄存器失败,请重启计算机!");
				return ERROR;
			}
			memcpy(&can.driverEventReg[nodeNum],&(recvPack.cData[4]),4);
			can.parseRegToBit(can.driverEventReg[nodeNum],nodeNum,CAN_EVENT_REG_PARSE);
			GV::isTargetPosSet[nodeNum] = !(can.driverEventRegBit[nodeNum][27]);
			GV::isPowerOutput[nodeNum]  = !(can.driverEventRegBit[nodeNum][12]);

			can.readCopleyStatus(sendPack,DRIVER_ACT_POS_GET,nodeNum + 1);
			if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL))
			{
				DISPLAY_FAULT("CAN监测任务:获取转台当前位置失败,请重启计算机!");
				return ERROR;
			}
			memcpy(&can.driverPosCount[nodeNum],&(recvPack.cData[4]),4);
			float tAxisActPos = (can.driverPosCount[nodeNum] * 360.0) / ENC_RES;
			if(tAxisActPos > 180.0)
				tAxisActPos -= static_cast<int>(tAxisActPos / 180.0) * 180;
			else if(tAxisActPos < -180.0)
				tAxisActPos += static_cast<int>((-tAxisActPos) / 180.0) * 180;
			GV::axisActPos[nodeNum] = tAxisActPos;
			semGive(GV::isCANOccupy);
			//判断驱动器状态
			if((lastIsBolt[nodeNum] == false) && (can.driverDInRegBit[nodeNum][4] == true) && !GV::isInitTesting){
				GV::isBoltOn[nodeNum] = true;
				GV::isTempStopOp = true;
				disableStartTime[nodeNum] = (disableStartTime[nodeNum] == 0 ? tick64Get() : disableStartTime[nodeNum]);
				MAKE_SINGLE_NODE(nodeNum)
				control.driverDisableAct(node);
				control.faultLEDControl(true,node);
				GV::isFaultOccur = true;
				DISPLAY_FAULT("插销插上,相应轴使能已断开");}
			else if((lastIsBolt[nodeNum] == true) && (can.driverDInRegBit[nodeNum][4] == false) &&!GV::isInitTesting){
				GV::isBoltOn[nodeNum] = false;
				GV::isTempStopOp = false;
				MAKE_SINGLE_NODE(nodeNum)
				control.faultLEDControl(false,node);
				DISPLAY_FAULT("插销拔出,相应轴3秒后重新使能");}
			if((disableStartTime[nodeNum] != 0)  && ((tick64Get() - disableStartTime[nodeNum]) >= static_cast<ULINT>(SYS_FREQ))){
				MAKE_SINGLE_NODE(nodeNum)
				disableStartTime[nodeNum] = 0;}

			if((lastIsDriverFault[nodeNum] == false) && (can.driverEventRegBit[nodeNum][22] == true)){
				GV::isDriverFault[nodeNum] = true;
				GV::isFaultOccur 		   = true;
				DISPLAY_FAULT("驱动器发生错误，请重启计算机!");}
			else if((lastIsDriverFault[nodeNum] == true) && (can.driverEventRegBit[nodeNum][22] == false))
				GV::isDriverFault[nodeNum] = false;

			if((lastIsPosLimit[nodeNum] == false) && ((!(can.driverDInRegBit[nodeNum][2])) == true)){
				GV::isPosLimit[nodeNum] = true;
				DISPLAY_FAULT("正限位触发，请重启计算机!");}
			else if((lastIsPosLimit[nodeNum] == true) && ((!(can.driverDInRegBit[nodeNum][2])) == false)){
				GV::isPosLimit[nodeNum] = false;}

			if((lastIsNegLimit[nodeNum] == false) && ((!(can.driverDInRegBit[nodeNum][3])) == true)){
				GV::isNegLimit[nodeNum] = true;
				DISPLAY_FAULT("负限位触发，请重启计算机!");}
			else if((lastIsNegLimit[nodeNum] == true) && ((!(can.driverDInRegBit[nodeNum][3])) == false))
				GV::isNegLimit[nodeNum] = false;
				
			GV::isLimit[nodeNum] = GV::isPosLimit[nodeNum] | GV::isNegLimit[nodeNum];
				if((lastIsLimit[nodeNum] == false) && (GV::isLimit[nodeNum] == true)){
				GV::isLimit[nodeNum] = true;
				MAKE_SINGLE_NODE(nodeNum)
				control.faultLEDControl(true,node);
				GV::isFaultOccur     = true;}
			else if((lastIsLimit[nodeNum] == true) && (GV::isLimit[nodeNum] == false)){
				MAKE_SINGLE_NODE(nodeNum)
				control.faultLEDControl(false,node);
				GV::isLimit[nodeNum] = false;}
			tIsLimit |= GV::isLimit[nodeNum];
			
			tIsBoltOn |= GV::isBoltOn[nodeNum];
			tIsDriverFault |= GV::isDriverFault[nodeNum];
			tErrorNode[nodeNum] = ((GV::isBoltOn[nodeNum]
								  | GV::isDriverFault[nodeNum]
								  | (nodeNum < 3? GV::isLimit[nodeNum]:0)) & 0x1);
			//保存上一次的值
			lastIsBolt[nodeNum] = can.driverDInRegBit[nodeNum][4];
			lastIsDriverFault[nodeNum] = can.driverDInRegBit[nodeNum][22];
			if(nodeNum < 3)
			{
				lastIsPosLimit[nodeNum] = !can.driverDInRegBit[nodeNum][2];
				lastIsNegLimit[nodeNum] = !can.driverDInRegBit[nodeNum][3];
				lastIsLimit[nodeNum] = lastIsPosLimit[nodeNum] | lastIsNegLimit[nodeNum];
			}
		}
		if(ERROR == semTake(GV::CANAndIOSyn,SYS_FREQ / 10))
		{
			DISPLAY_FAULT("CAN监测任务:CAN和IO同步信号量接收超时,请重新启动计算机!");
			file.writeLogFile("CAN和IO同步信号量接收超时!");
			return ERROR;
		}
		if((0 != (boltEndTime = (((lastIsBoltOn == true) && (tIsBoltOn == false)) ? tick64Get() : boltEndTime)))
				&& ((tick64Get() - boltEndTime) >= 3 * SYS_FREQ)
				&& !tIsBoltOn){
			boltEndTime	= 0;
			control.driverEnableAct(ERROR_ALL_NODE);}
		lastIsBoltOn = tIsBoltOn;
		GV::isStHalt = tIsBoltOn;}
		if(GV::isFaultOccur)
		{
			GV::faultCode = tIsDriverFault
							 | tIsLimit 			<< 1
							 | tIsBoltOn 			<< 2
							 | (!GV::isPowerOn) 	<< 3
							 | GV::isEmerStop 		<< 4
							 | GV::isEnableFailed 	<< 5
							 | GV::isFindZeroFailed << 6;
			for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
			{
				GV::errorArray[1 + nodeNum] = GV::isDriverFault[nodeNum];
				GV::errorArray[9 + nodeNum] = GV::isBoltOn[nodeNum];
			}
			for(UINT nodeNum = 0;nodeNum < 3;nodeNum++)
				GV::errorArray[6 + nodeNum] = GV::isLimit[nodeNum];
			char sendBuf[NET_SEND_BUF_SIZE];
			memset(sendBuf,0,NET_SEND_BUF_SIZE);
			FT_MSG faultMsg;
			memset(&faultMsg, 0, FTMSG_SIZE);
			faultMsg.msgType = TABLE_FAULT_INFO_MSG;
			faultMsg.msgCount = net.currentSendCount;
			faultMsg.msgClass = 0xC0 | (0x1 << 14);
			faultMsg.statusInfoA = 0x0;
			faultMsg.FaultInfoNormal = GV::isEmerStop << 4
									 | tIsBoltOn << 2
								     | tIsLimit << 1
								     | tIsDriverFault;
			faultMsg.checkSum = checkSum(faultMsg);
			stringstream str;
			for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++){
				str << "轴" << nodeNum + 1 << ":" << (GV::isDriverFault[nodeNum] ? "F1":"")
												  << (nodeNum < 3? (GV::isLimit[nodeNum] ? "F2":""):"")
												  << (GV::isBoltOn[nodeNum] ? "F3":"")
												  << (GV::isSingleEnableFailed[nodeNum] ? "F6":"") << "\n";}
			str << "其他错误:" << (!(GV::isPowerOn)	   ? "F4":"")
							  << (GV::isEmerStop 	   ? "F5":"")
							  << (GV::isFindZeroFailed ? "F7":"");
			strcpy(faultMsg.name,str.str().c_str());
			memcpy(sendBuf,&faultMsg,FTMSG_SIZE);
			sendBuf[NET_SEND_BUF_SIZE - 1] = FTMSG_SIZE;
			msgQSend(net.sendingMsgQueue,sendBuf,NET_SEND_BUF_SIZE,WAIT_FOREVER,MSG_PRI_URGENT);
			if(GV::isStopAll)
				return ERROR;
			GV::faultCode = 0x0;
			memset(GV::errorArray,0,sizeof(GV::errorArray));
			GV::isFaultOccur = false;
			//其他错误情况处理
		}
	}
	return ERROR;
}
