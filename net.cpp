#include "net.h"

NET net;
net::net()
{

}

NET::~net()
{
	
}

STATUS NET::init()
{
	int isReuse = 1;
	memset(&sendToAddr,   0, SOCK_ADDR_IN_SIZE);
	memset(&recvFromAddr, 0, SOCK_ADDR_IN_SIZE);
	memset(&sourceAddr,   0, SOCK_ADDR_IN_SIZE);
	sendToAddr.sin_family = recvFromAddr.sin_family = AF_INET;
	sendToAddr.sin_port   = recvFromAddr.sin_port   = htons(BIND_PORT);
	sendToAddr.sin_addr.s_addr   = htonl(INADDR_BROADCAST);
	recvFromAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(ERROR == (sFD = socket(AF_INET, SOCK_DGRAM, 0)) ||
	   ERROR == (rFD = socket(AF_INET, SOCK_DGRAM, 0))){
		DISPLAY_FAULT("网络初始化:创建套接字失败,请重启计算机!");
		printf("Socket Est ERROR!\n");
		return ERROR;}
	setsockopt(sFD, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<CHAR_PTR>(&isReuse), INT_SIZE);
	setsockopt(rFD, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<CHAR_PTR>(&isReuse), INT_SIZE);
	if (ERROR == bind(rFD, reinterpret_cast<SOCK_ADDR_PTR>(&recvFromAddr), SOCK_ADDR_IN_SIZE) ||
		ERROR == bind(sFD, reinterpret_cast<SOCK_ADDR_PTR>(&sendToAddr),   SOCK_ADDR_IN_SIZE)){
		close(rFD);
		close(sFD);
		DISPLAY_FAULT("网络初始化:套接字地址绑定失败,请重启计算机!");
		printf("Socket Bind ERROR!\n");
		return ERROR;}
	sentMsgQueue	 = msgQCreate(10, NET_SEND_BUF_SIZE, MSG_Q_FIFO);
	sendingMsgQueue	 = msgQCreate(10, NET_SEND_BUF_SIZE, MSG_Q_FIFO);
	currentSendCount = 0;
	return OK;
}

STATUS NET::release()
{
	semDelete(GV::NetSendTaskSem);
	close(sFD);
	close(rFD);
	sFD = rFD = 0;
	msgQDelete(sendingMsgQueue);
	msgQDelete(sentMsgQueue);
	return OK;
};

STATUS NET::CMDExplain(UCHAR recvBuf[]) const
{
	if((recvBuf[0] >> 7) & 0x1)
	{
		char sendBuf[NET_SEND_BUF_SIZE];
		memset(sendBuf, 0, NET_SEND_BUF_SIZE);
		ACK_MSG ack;
		ack.msgCount 	  = recvBuf[1];
		ack.msgType  	  = 0x7f;
		ack.serverMsgType = recvBuf[0];
		ack.checkSum 	  = checkSum(ack);
		memcpy(sendBuf, &ack, ACKMSG_SIZE);
		sendBuf[NET_SEND_BUF_SIZE - 1] = ACKMSG_SIZE;
		msgQSend(sendingMsgQueue, sendBuf, NET_SEND_BUF_SIZE, WAIT_FOREVER, MSG_PRI_URGENT);
	}
	switch(recvBuf[0])
	{
		case TABLE_SELFTEST_REQ_MSG:{
			STC_MSG stcMsg;
			memset(&stcMsg, 0, STCMSG_SIZE);
			memcpy(&stcMsg, recvBuf, STCMSG_SIZE);
			if(stcMsg.checkSum == checkSum(stcMsg) &&
			   stcMsg.command == 0xAA){
				GV::isSelfTestRecv = true;
				char sendBuf[NET_SEND_BUF_SIZE];
				memset(sendBuf, 0, NET_SEND_BUF_SIZE);
				GV::selfTestMsg.msgCount = currentSendCount;
				GV::selfTestMsg.checkSum = checkSum(GV::selfTestMsg);
				memcpy(sendBuf, &GV::selfTestMsg, STMSG_SIZE);
				sendBuf[NET_SEND_BUF_SIZE - 1] = STMSG_SIZE;
				msgQSend(sendingMsgQueue, sendBuf, NET_SEND_BUF_SIZE, WAIT_FOREVER, MSG_PRI_NORMAL);}
			else
				DISPLAY_FAULT("网络指令解析:收到自检请求,校验不正确!");}
			break;
		case TABLE_GEN_CTRL_MSG:{
			GEN_MSG gm;
			memset(&gm, 0, GENMSG_SIZE);
			memcpy(&gm,recvBuf,GENMSG_SIZE);
			if(gm.checkSum == checkSum(gm)){
				switch(gm.command){
					case 0x0001:
						control.findZeroPos();
						break;
					case 0x0002:
						control.driverDisable(ERROR_ALL_NODE);
						break;
					case 0x0003:
						printf("KaiShiGongZuo!\n");
						break;
					case 0x0004:
						control.stop();
						break;
					case 0x0005:
						control.exit();
						taskDelay(SYS_FREQ);
						reboot(0);
						break;
					default:
						DISPLAY_FAULT("网络指令解析:未知的通用控制命令!");}}
			else{
				DISPLAY_FAULT("网络指令解析:收到通用控制命令,校验不正确!");}}
			break;
		case TABLE_TARGET_NCTRL_MSG:{
			NCTL_MSG ctrlParamMsg;
			memset(&ctrlParamMsg, 0, NCTLMSG_SIZE);
			memcpy(&ctrlParamMsg, recvBuf, NCTLMSG_SIZE);
			if(ctrlParamMsg.checkSum == checkSum(ctrlParamMsg)){
				float pos = 0.0;
				float vel = 0.0;
				UINT nodeNum = 0;
				for(nodeNum = 0; nodeNum < AXIS_NUM; nodeNum++){
					int t = 0;
					memcpy(&t, ((CHAR_PTR)&(ctrlParamMsg.tPitchPos) + FLOAT_SIZE * nodeNum), FLOAT_SIZE);
					if(t != -1){
						memcpy(&pos, &t, FLOAT_SIZE);
						memcpy(&vel, ((CHAR_PTR)&(ctrlParamMsg.tPitchPos) + FLOAT_SIZE * (nodeNum + AXIS_NUM)), FLOAT_SIZE);
						break;}}
				vel = fabs(vel);
				if(pos > POS_MAX_POSITION[nodeNum]) 	 pos = POS_MAX_POSITION[nodeNum];
				else if(pos < NEG_MAX_POSITION[nodeNum]) pos = NEG_MAX_POSITION[nodeNum];
				if(vel > MAX_SPEED_POS[nodeNum]) 		 vel = MAX_SPEED_POS[nodeNum];
				else if(vel < MAX_SPEED_NEG[nodeNum]) 	 vel = MAX_SPEED_NEG[nodeNum];
				control.runCustom(pos, vel, nodeNum);}
			else{
				DISPLAY_FAULT("网络指令解析:收到转台轴角控制命令,校验不正确!");}}
			break;
		case TABLE_TARGET_WCTRL_MSG:{
			memset(&GV::ctrlParamMsg, 0, WCTLMSG_SIZE);
			memcpy(&GV::ctrlParamMsg, recvBuf, WCTLMSG_SIZE);
			if(GV::ctrlParamMsg.checkSum == checkSum(GV::ctrlParamMsg)){
				taskSpawn("tRunPreset", 110, VX_FP_TASK, 20000, (FUNCPTR)runPreset,0,0,0,0,0,0,0,0,0,0);}
			else
				DISPLAY_FAULT("网络指令解析:收到转台轨迹控制命令,校验不正确!");}
			break;
		case TABLE_RECV_CONFIRM_MSG:
			break;
		default:
			DISPLAY_FAULT("网络指令解析:收到未知网络数据!");
			break;
	}
	return OK;
}

STATUS netRecvTask()
{
	UCHAR buf[NET_RECV_BUF_SIZE];
	UCHAR waitBuf[NET_RECV_BUF_SIZE];
	fd_set 	recvFD;
	timeval timeOut;
	timeOut.tv_sec  = 0;
	timeOut.tv_usec = 100000;
	ULINT startTick = 0;
	int   waitTimes = 0;
	BOOL  isWait 	= false;
	UCHAR awaitMsgCount = 0x0;
	memset(waitBuf, 0, NET_RECV_BUF_SIZE);
	int nSize = SOCK_ADDR_IN_SIZE;
	while(1)
	{
		if(GV::isStopAll)
			return ERROR;
		if(GV::isTempStopOp)
			taskDelay(SYS_FREQ / 5);
		else{
		FD_ZERO(&recvFD);
		FD_SET(net.rFD, &recvFD);
		int ret = select(net.rFD + 1, &recvFD, NULL, NULL, &timeOut);
		if(ERROR != msgQReceive(net.sentMsgQueue, (CHAR_PTR)waitBuf, NET_RECV_BUF_SIZE, NO_WAIT))
		{
			memcpy(&startTick, (waitBuf + NET_SEND_BUF_SIZE - 1 - ULINT_SIZE), ULINT_SIZE);
			awaitMsgCount = waitBuf[1];
			isWait = true;
		}
		if(ret == ERROR)
		{
			control.error("网络接收任务:获取数据包错误!", ERROR_ALL_NODE);
			return ERROR;
		}
		else if(!ret && isWait && ((++waitTimes) >= 10))
		{
			DISPLAY_FAULT("发送网络数据包超时,与主机的通信可能已断开!");
			waitTimes = 0;
			isWait    = false;
			if((++(waitBuf[NET_SEND_BUF_SIZE - 2 - ULINT_SIZE])) <= NET::MAX_RESEND_COUNT)
				msgQSend(net.sendingMsgQueue, (CHAR_PTR)waitBuf, NET_SEND_BUF_SIZE, WAIT_FOREVER,MSG_PRI_URGENT);
		}
		else if(FD_ISSET(net.rFD, &recvFD))
		{
			memset(buf, 0, NET_RECV_BUF_SIZE);
			recvfrom(net.rFD, (CHAR_PTR)buf, NET_RECV_BUF_SIZE, 0, reinterpret_cast<SOCK_ADDR_PTR>(&net.sourceAddr),&nSize);
			if(isWait && buf[1] == awaitMsgCount){
				isWait = (waitTimes = 0);
				memset(waitBuf, 0, NET_RECV_BUF_SIZE);}
			net.CMDExplain(buf);
		}}
	}
	return ERROR;
}

STATUS netSendTask()
{
	char    tempSendBuf[NET_SEND_BUF_SIZE];
	STA_MSG tableStatusMsg;
	while(1)
	{
		semTake(GV::NetSendTaskSem, WAIT_FOREVER);
		if(!GV::isSelfTestRecv)
			continue;
		memset(tempSendBuf, 0, NET_SEND_BUF_SIZE);
		memset(&tableStatusMsg, 0, STAMSG_SIZE);
		tableStatusMsg.msgType 	= TABLE_CURRENT_STATUS_MSG;
		tableStatusMsg.msgCount = net.currentSendCount;	
		memcpy(&(tableStatusMsg.tPitch), &(GV::axisActPos[0]), AXIS_NUM * FLOAT_SIZE);
		tableStatusMsg.checkSum = checkSum(tableStatusMsg);
		memcpy(tempSendBuf, &tableStatusMsg, STAMSG_SIZE);
		tempSendBuf[NET_SEND_BUF_SIZE - 1] = STAMSG_SIZE;
		if(msgQNumMsgs(net.sendingMsgQueue) == 0)
			msgQSend(net.sendingMsgQueue, tempSendBuf, NET_SEND_BUF_SIZE,WAIT_FOREVER, MSG_PRI_NORMAL);
		memset(tempSendBuf,0,NET_SEND_BUF_SIZE);
		msgQReceive(net.sendingMsgQueue, tempSendBuf, NET_RECV_BUF_SIZE,WAIT_FOREVER);
		if(0 != net.sourceAddr.sin_addr.s_addr){
			net.sendToAddr.sin_addr = net.sourceAddr.sin_addr;
			net.sendToAddr.sin_port = htons(10000 + (tempSendBuf[0] == TABLE_CURRENT_STATUS_MSG) * 2000);}
		if(ERROR == sendto(net.sFD,
						   tempSendBuf,
						   tempSendBuf[NET_SEND_BUF_SIZE - 1],
						   0,
						   reinterpret_cast<SOCK_ADDR_PTR>(&net.sendToAddr),
						   SOCK_ADDR_IN_SIZE)){
			DISPLAY_FAULT("网络发送任务:发送内容失败!");}
		if((tempSendBuf[0] >> 7) & 0x1){
			ULINT tick = tick64Get();
			memcpy(tempSendBuf + NET_SEND_BUF_SIZE - 1 - ULINT_SIZE, &tick,ULINT_SIZE);
			msgQSend(net.sentMsgQueue, tempSendBuf, NET_SEND_BUF_SIZE,WAIT_FOREVER, MSG_PRI_NORMAL);}
		net.currentSendCount++;
	}
	return OK;
}
