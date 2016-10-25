#ifndef _USERINCLUDES_H_
#define _USERINCLUDES_H_
#include "userConstants.h"
#include <errnoLib.h>
#include <fioLib.h>
#include <hostLib.h>
#include <inetLib.h>
#include <ioLib.h>
#include <logLib.h>
#include <msgQLib.h>
#include <math.h>
#include <rebootLib.h>
#include <string.h>
#include <stdlib.h>
#include <sysLib.h>
#include <sockLib.h>
#include <inetLib.h>
#include <stdioLib.h>
#include <strLib.h>
#include <taskLib.h>
#include <tickLib.h>
#include <time.h>
#include <vxWorks.h> 
#include <fstream>
#include <clocale>
#include <iomanip>
#include <iostream>
#include <map>
#include <sioLib.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sigeventCommon.h>
#include <ugl/ugl.h>
#include <ugl/uglos.h>
#include <ugl/uglMsg.h>
#include <ugl/uglfont.h>
#include <ugl/uglinput.h>
#include <ugl/ugldib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stat.h>
#include "ui.h"
#include "Lib/rapidjson/document.h"
#include "Lib/rapidjson/prettywriter.h"
extern "C"
{
#include "Lib/LsKioDll.h"
#include "Lib/LsKioDrv.h"
#include "Lib/PciCanDrv.h"
#include "Lib/PciCanDll.h"
#include "Lib/PciCanDrvRegs.h"
extern void GetSysDateTime(ST_DATETIME *psDateTime);
extern void SetSysDateTime(ST_DATETIME *psDateTime);
}
using std::vector;
using std::string;
using std::stringstream;
using std::map;
using std::pair;
using std::make_pair;

void   init();
STATUS semClock(void);
STATUS checkInitStatus();
template <typename T>
inline UCHAR checkSum(T &a)
{
	UCHAR sum = 0;
	for(UINT i = 0;i < sizeof(a) - 1;i++)
		sum += *((UCHAR_PTR)&a + i);
	return sum;
}

namespace GV
{
	extern double REMOTE_CTRL_BAUD;
	extern ST_MSG selfTestMsg;
	extern WCTL_MSG ctrlParamMsg;
	extern UCHAR  faultCode;
	extern BOOL   isStopAll;
	extern BOOL   isFaultOccur;
	extern BOOL   isPowerOn;
	extern BOOL   isEmerStop;
	extern BOOL   isFindZeroFailed;
	extern BOOL   isEnableFailed;
	extern BOOL   isShutDown;
	extern BOOL   isStHalt;
	extern float  axisActPos[AXIS_NUM];
	extern BOOL   isBoltOn[AXIS_NUM];
	extern BOOL   isPowerOutput[AXIS_NUM];
	extern BOOL   isZeroPosSet[AXIS_NUM];
	extern BOOL   errorArray[FAULT_ARRAY_SIZE];
	extern BOOL   isTargetPosSet[AXIS_NUM];
	extern BOOL   isDriverFault[AXIS_NUM];
	extern BOOL   isLimit[AXIS_NUM];
	extern BOOL   isPosLimit[AXIS_NUM];
	extern BOOL   isNegLimit[AXIS_NUM];
	extern BOOL   isSingleEnableFailed[AXIS_NUM];
	extern BOOL   isTempStopOp;
	extern BOOL   isSelfTestRecv;
	extern char   opMode[AXIS_NUM];
	extern SEM_ID CANReadTaskSem;
	extern SEM_ID NetSendTaskSem;
	extern SEM_ID IOReadTaskSem;
	extern SEM_ID isCANOccupy;
	extern SEM_ID CANAndIOSyn;
	extern MSG_Q_ID faultLogMsg;
	extern BOOL   isInitTesting;
	extern string FAULT_STRING[FAULT_ARRAY_SIZE];

	extern vector<double> posCompAngle[AXIS_NUM];
	extern vector<double> posCompData[AXIS_NUM];
	extern vector<double> velCompAngle[AXIS_NUM];
	extern vector<double> velCompData[AXIS_NUM];
	extern vector<double> controlData[AXIS_NUM];
	extern vector<double> limitParam[AXIS_NUM];
	extern vector<double> lsTestVel;
	extern vector<double> lsTestAngle;
	extern vector<double> lsTestCount;
};
#endif
