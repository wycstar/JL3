#ifndef _CAN_H_
#define _CAN_H_
#include "userIncludes.h"
#include "net.h"
#include "file.h"
int  CANReadTask(void);

static const int CAN_125KBPS	   = 0;
static const int CAN_250KBPS	   = 1;
static const int CAN_500KBPS	   = 2;
static const int CAN_1000KBPS	   = 3;
static const int CAN_CUSTOM_BPS	   = 4;

static const int FAULT_REG_BIT_NUM = 16;
static const int EVENT_REG_BIT_NUM = 32;
static const int TRJ_REG_BIT_NUM   = 16;
static const int DIN_REG_BIT_NUM   = 16;
static const int DOUT_REG_BIT_NUM  = 16;

static const int CAN_EVENT_REG_PARSE = 101;
static const int CAN_FAULT_REG_PARSE = 102;
static const int CAN_TRJ_REG_PARSE   = 103;
static const int CAN_DIN_REG_PARSE   = 104;
static const int CAN_DOUT_REG_PARSE  = 105;

static const int CP_SIZE = sizeof(CAN_PACKET);

typedef class can
{
private:
	int    CANHandle[CAN_CHANNEL];
	SEM_ID recvSem[CAN_CHANNEL];
	PORT_STRUCT CANPara[CAN_CHANNEL];
	int driverDINStatus[AXIS_NUM];
	int driverDOUTStatus[AXIS_NUM];
	int driverEventReg[AXIS_NUM];
	int driverPosCount[AXIS_NUM];
	int driverFaultReg[AXIS_NUM];
	BOOL driverEventRegBit[AXIS_NUM][EVENT_REG_BIT_NUM];
	BOOL driverFaultRegBit[AXIS_NUM][FAULT_REG_BIT_NUM];
	BOOL driverTrjRegBit[AXIS_NUM][TRJ_REG_BIT_NUM];
	BOOL driverDInRegBit[AXIS_NUM][DIN_REG_BIT_NUM];
	BOOL driverDOutRegBit[AXIS_NUM][DOUT_REG_BIT_NUM];
	void parseRegToBit(const UINT &reg,const int &meta,const int &op);
	
public:
	can();
	~can();	
	friend STATUS CANReadTask(void);
	friend STATUS checkInitStatus();
	STATUS writeCopleyCMD(CAN_PACKET &sendPack,const UINT &op,const float &param,const int &NodeID) const;
	STATUS readCopleyStatus(CAN_PACKET &sendPack,const UINT &op,const int &NodeID) const;
	STATUS uCMDSend(CAN_PACKET &sendPack,CAN_PACKET &recvPack,const int &channel) const;
	STATUS uCMDCheck(const CAN_PACKET &sendPack,const CAN_PACKET &recvPack) const;
	STATUS init();
	STATUS release();

}CAN;
extern CAN can;
#endif
