#include "userIncludes.h"
#include "net.h"
#include "can.h"

double GV::REMOTE_CTRL_BAUD;
ST_MSG GV::selfTestMsg;
WCTL_MSG GV::ctrlParamMsg;
UCHAR  GV::faultCode;
BOOL   GV::isStopAll;
BOOL   GV::isFaultOccur;
BOOL   GV::isPowerOn;
BOOL   GV::isEmerStop;
BOOL   GV::isFindZeroFailed;
BOOL   GV::isEnableFailed;
BOOL   GV::isShutDown;
BOOL   GV::isStHalt;
float  GV::axisActPos[AXIS_NUM];
BOOL   GV::isBoltOn[AXIS_NUM];
BOOL   GV::isPowerOutput[AXIS_NUM];
BOOL   GV::isZeroPosSet[AXIS_NUM];
BOOL   GV::errorArray[FAULT_ARRAY_SIZE];
BOOL   GV::isTargetPosSet[AXIS_NUM];
BOOL   GV::isDriverFault[AXIS_NUM];
BOOL   GV::isLimit[AXIS_NUM];
BOOL   GV::isPosLimit[AXIS_NUM];
BOOL   GV::isNegLimit[AXIS_NUM];
BOOL   GV::isSingleEnableFailed[AXIS_NUM];
char   GV::opMode[AXIS_NUM];
SEM_ID GV::isCANOccupy;
BOOL   GV::isTempStopOp;
BOOL   GV::isSelfTestRecv;
BOOL   GV::isInitTesting;
SEM_ID GV::CANAndIOSyn;
SEM_ID GV::CANReadTaskSem;
SEM_ID GV::NetSendTaskSem;
SEM_ID GV::IOReadTaskSem;
string GV::FAULT_STRING[FAULT_ARRAY_SIZE];

vector<double> GV::posCompAngle[AXIS_NUM];
vector<double> GV::posCompData[AXIS_NUM];
vector<double> GV::velCompAngle[AXIS_NUM];
vector<double> GV::velCompData[AXIS_NUM];
vector<double> GV::controlData[AXIS_NUM];
vector<double> GV::limitParam[AXIS_NUM];
vector<double> GV::lsTestVel;
vector<double> GV::lsTestAngle;
vector<double> GV::lsTestCount;

MSG_Q_ID GV::faultLogMsg;

void init(void)
{
	if(ERROR == sysClkRateSet(SYS_FREQ)){
		DISPLAY_FAULT("初始化操作：系统时钟频率设定失败!");}
	if(ERROR == sysAuxClkRateSet(AUX_FREQ)){
		DISPLAY_FAULT("初始化操作：系统辅助时钟频率设定失败!");}
	if(ERROR == sysTimestampEnable()){
		DISPLAY_FAULT("初始化操作：系统时间戳开启失败!");}
	sysClkEnable();
	sysAuxClkEnable();
	GV::isCANOccupy = semMCreate(SEM_Q_FIFO | SEM_DELETE_SAFE);
	GV::CANAndIOSyn	= semMCreate(SEM_Q_FIFO | SEM_DELETE_SAFE);
	GV::CANReadTaskSem = semBCreate(SEM_Q_FIFO ,SEM_EMPTY);
	GV::NetSendTaskSem = semBCreate(SEM_Q_FIFO ,SEM_EMPTY);
	GV::IOReadTaskSem  = semBCreate(SEM_Q_FIFO ,SEM_EMPTY);
	if(ERROR == sysAuxClkConnect((FUNCPTR)semClock,0)){
		DISPLAY_FAULT("初始化操作：辅助始终中断挂接失败!");}
	memset(GV::isPowerOutput, 0,sizeof(GV::isPowerOutput ));
	memset(GV::isZeroPosSet,  0,sizeof(GV::isZeroPosSet  ));
	memset(GV::errorArray,    0,sizeof(GV::errorArray    ));
	memset(GV::isTargetPosSet,0,sizeof(GV::isTargetPosSet));
	memset(GV::isBoltOn,      0,sizeof(GV::isBoltOn		 ));
	memset(GV::isDriverFault, 0,sizeof(GV::isDriverFault ));
	memset(GV::isLimit,       0,sizeof(GV::isLimit    	 ));
	memset(GV::isPosLimit,    0,sizeof(GV::isPosLimit  	 ));
	memset(GV::isNegLimit,    0,sizeof(GV::isNegLimit    ));
	memset(&GV::selfTestMsg,  0,STMSG_SIZE);
	memset(GV::isSingleEnableFailed,0,sizeof(GV::isSingleEnableFailed));
	string tFAULT_STRING[FAULT_ARRAY_SIZE] = {"没有上电",
											  "A轴驱动错误",
											  "B轴驱动错误",
											  "C轴驱动错误",
											  "D轴驱动错误",
											  "E轴驱动错误",
											  "A轴电气限位",
											  "B轴电气限位",
											  "C轴电气限位",
											  "A轴插销状态",
											  "B轴插销状态",
											  "C轴插销状态",
											  "D轴插销状态",
											  "E轴插销状态",
											  "急停状态",
											  "使能失败",
											  "寻零失败",
											  "关机状态"};
	for(UINT i = 0;i < FAULT_ARRAY_SIZE;i++)
		GV::FAULT_STRING[i] = tFAULT_STRING[i];
	GV::faultCode		 ^=	GV::faultCode;
	GV::isStopAll 		 ^= GV::isStopAll;
	GV::isFaultOccur	 ^= GV::isFaultOccur;
	GV::isPowerOn  		 ^= GV::isPowerOn;
	GV::isEmerStop 		 ^= GV::isEmerStop;
	GV::isShutDown 		 ^= GV::isShutDown;
	GV::isFindZeroFailed ^= GV::isFindZeroFailed;
	GV::isEnableFailed	 ^= GV::isEnableFailed;
	GV::isStHalt		 ^= GV::isStHalt;
	GV::isTempStopOp     ^= GV::isTempStopOp;
	GV::isSelfTestRecv   ^= GV::isSelfTestRecv;
	for(UINT i = 0;i < AXIS_NUM;i++){
		GV::opMode[i] = !(GV::opMode[i]^GV::opMode[i]);}
	return;
}

STATUS semClock(void)
{
	static ULINT clockCount;
	++clockCount;
	if((clockCount % (AUX_FREQ / CAN_READ_FREQ)) == 0)
		semGive(GV::CANReadTaskSem);
	if((clockCount % (AUX_FREQ / NET_READ_FREQ)) == 0)
		semGive(GV::NetSendTaskSem);
	if((clockCount % (AUX_FREQ / IO_READ_FREQ))  == 0)
		semGive(GV::IOReadTaskSem);
	if((clockCount % (AUX_FREQ * AUX_FREQ/(NET_READ_FREQ * CAN_READ_FREQ))) == 0)
		clockCount = 0;
	return OK;
}

STATUS checkInitStatus()
{
	GV::isInitTesting = true;
	BOOL tIsLimit 		  = FALSE;
	BOOL tIsBoltOn  	  = FALSE;
	BOOL tIsDriverFault   = FALSE;
	BOOL isDelayAfterBolt = FALSE;
	GV::isShutDown 		  = FALSE;
	BOOL errorArray[FAULT_ARRAY_SIZE];
	BOOL faultLED[AXIS_NUM];
	UCHAR buf			  = 0;
	CAN_PACKET sendPack;
	CAN_PACKET recvPack;
	memset(&sendPack,  0, sizeof(CAN_PACKET));
	memset(&recvPack,  0, sizeof(CAN_PACKET));
	memset(errorArray, 0, sizeof(errorArray));
	memset(faultLED,   0, sizeof(faultLED  ));
	taskSpawn(NET_READ_TASK_NAME,101,VX_FP_TASK,20000,(FUNCPTR)netRecvTask,0,0,0,0,0,0,0,0,0,0);
	taskSpawn(NET_SEND_TASK_NAME,103,VX_FP_TASK,20000,(FUNCPTR)netSendTask,0,0,0,0,0,0,0,0,0,0);
	for(UINT nodeNum = 0;nodeNum < AXIS_NUM;nodeNum++)
	{
		can.readCopleyStatus(sendPack,DRIVER_DIO_IN_GET,nodeNum + 1);
		if(ERROR == can.uCMDSend(sendPack,recvPack,USING_CAN_CHANNEL)){
			DISPLAY_FAULT("自检操作:获取驱动器初始DIO输入失败!");}
		memcpy(&can.driverDINStatus[nodeNum],&(recvPack.cData[4]),4);
		can.parseRegToBit(can.driverDINStatus[nodeNum],nodeNum,CAN_DIN_REG_PARSE);
		GV::isLimit[nodeNum] = !(can.driverDInRegBit[nodeNum][2]) | !(can.driverDInRegBit[nodeNum][3]);
		tIsLimit 			|= GV::isLimit[nodeNum];
		GV::isBoltOn[nodeNum]  		= can.driverDInRegBit[nodeNum][4];
		tIsBoltOn 		  	  	   |= GV::isBoltOn[nodeNum];
		GV::isPowerOutput[nodeNum]	= !(can.driverEventRegBit[nodeNum][12]);
		GV::isDriverFault[nodeNum]  = can.driverEventRegBit[nodeNum][22];
		tIsDriverFault 		       |= GV::isDriverFault[nodeNum];
	}
	digitalIO.read(&buf);
	GV::isEmerStop  = (buf >> 1) & 0x1;
	
	string tFaultString;
	for(UINT i = 0;i < FAULT_ARRAY_SIZE;i++)
	{
		GV::selfTestMsg.stDetail |= ((GV::errorArray[i] & 0x1) << i);
		tFaultString.append(errorArray[i] & 0x1 ? GV::FAULT_STRING[i] + "\n":"");
	}
	strcpy(GV::selfTestMsg.name,tFaultString.c_str());
	GV::selfTestMsg.stDetail == 0x0? GV::selfTestMsg.stInfo = (0xC0 << 8) | 0xAA:
									 GV::selfTestMsg.stInfo = (0xC0 << 8) | 0xFF;
	strcpy(GV::selfTestMsg.name, GV::selfTestMsg.stDetail == 0x0 ? "自检结果正常" : 
															       tFaultString.c_str());
	GV::selfTestMsg.msgType = TABLE_SELFTEST_RESULT_MSG;
	
	if(GV::isShutDown || GV::isEmerStop || tIsLimit || tIsDriverFault || tIsBoltOn)
	{
		string str(tIsLimit 	  ? "自检错误:限位开关触发!" 	:
				  (tIsDriverFault ? "自检错误:驱动器错误!" 	:
				  (tIsBoltOn	  ? "自检错误:插销插上!" 		:
				  (GV::isShutDown ? "自检错误:关机按钮按下!" 	:
								    "自检错误:急停按钮按下!"))));
		DISPLAY_FAULT(str.c_str());
		file.writeLogFile(str.c_str());
		for(UINT i = 0;i < AXIS_NUM;i++){
			GV::errorArray[1 + i] = GV::isDriverFault[i];
			GV::errorArray[9 + i] = GV::isBoltOn[i];
			faultLED[i] = GV::isLimit[i] | GV::isBoltOn[i];}
		for(UINT i = 0;i < 3;i++){
			GV::errorArray[6 + i] = GV::isLimit[i];}
		GV::errorArray[14] = GV::isEmerStop;
		GV::errorArray[17] = GV::isShutDown;
		char sendBuf[NET_SEND_BUF_SIZE];
		memset(sendBuf,0,NET_SEND_BUF_SIZE);
		FT_MSG faultMsg;
		faultMsg.msgType  = TABLE_FAULT_INFO_MSG;
		faultMsg.msgCount = net.currentSendCount;
		faultMsg.msgClass = 0xC0 | (0x2 << 14);
		faultMsg.FaultInfoNormal = GV::isEmerStop << 4
								 | tIsBoltOn << 2
							     | tIsLimit << 1
							     | tIsDriverFault;
		faultMsg.checkSum = checkSum(faultMsg);
		strcpy(faultMsg.name,str.c_str());
		memcpy(sendBuf,&faultMsg,FTMSG_SIZE);
		sendBuf[NET_SEND_BUF_SIZE - 1] = FTMSG_SIZE;
		msgQSend(net.sendingMsgQueue,sendBuf,NET_SEND_BUF_SIZE,WAIT_FOREVER,MSG_PRI_URGENT);
		taskDelay(SYS_FREQ / 2);
		control.faultLEDControl(true, faultLED);
		if(!tIsBoltOn && (tIsLimit || tIsDriverFault || GV::isShutDown || GV::isEmerStop))
			return ERROR;
	}
	taskSpawn(IO_READ_TASK_NAME ,103,VX_FP_TASK,20000,(FUNCPTR)IOReadTask ,0,0,0,0,0,0,0,0,0,0);
	taskSpawn(CAN_READ_TASK_NAME,100,VX_FP_TASK,20000,(FUNCPTR)CANReadTask,0,0,0,0,0,0,0,0,0,0);
	taskDelay(SYS_FREQ);
	while(GV::isStHalt)
	{
		if(!isDelayAfterBolt)
			DISPLAY_FAULT("自检操作:检测到插销,请拔出插销后继续!");
		taskDelay(SYS_FREQ / 2);
		isDelayAfterBolt = TRUE;
	}
	taskDelay(isDelayAfterBolt ? 3 * SYS_FREQ : 0);
	control.driverEnable(ERROR_ALL_NODE);
	control.findZeroPos();
	DISPLAY_FAULT("转台初始化完成!");
	GV::isInitTesting = false;
	return OK;
}
