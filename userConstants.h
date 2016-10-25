#ifndef _USERCONSTANTS_H_
#define _USERCONSTANTS_H_
#include "userTypes.h"
using namespace std;
//强迫症选项↓
static const int  FLOAT_SIZE	= sizeof(float);
static const int  INT_SIZE		= sizeof(int);
static const int  DOUBLE_SIZE	= sizeof(double);
static const int  ULINT_SIZE	= sizeof(ULINT);

//系统全局变量↓
static const int  SYS_FREQ  	   = 1000;
static const int  AUX_FREQ         = 1000;
static const int  ENC_RES 		   = 33554432;
static const int  UI_FAULT_STR_LEN = 200;

//自定义函数↓
#define MAKE_SINGLE_NODE(n) 		   \
		BOOL node[AXIS_NUM];			\
		memset(node,0,sizeof(node));	 \
		node[n] = true;
#define DISPLAY_FAULT 	ui.displayFault

//网络模块全局变量↓
#define CAN_READ_TASK_NAME  "tCANReadTask"
#define IO_READ_TASK_NAME   "tIOReadTask"
#define NET_READ_TASK_NAME  "tNetReadTask"
#define NET_SEND_TASK_NAME  "tNetSendTask"
static const int  NET_RECV_BUF_SIZE	  = 200;
static const int  NET_SEND_BUF_SIZE	  = 200;
static const int  NET_READ_FREQ = 2;
static const int  STCMSG_SIZE  	= sizeof(STC_MSG);
static const int  STMSG_SIZE  	= sizeof(ST_MSG);
static const int  FTMSG_SIZE 	= sizeof(FT_MSG);
static const int  ACKMSG_SIZE 	= sizeof(ACK_MSG);
static const int  GENMSG_SIZE 	= sizeof(GEN_MSG);
static const int  NCTLMSG_SIZE 	= sizeof(NCTL_MSG);
static const int  WCTLMSG_SIZE 	= sizeof(WCTL_MSG);
static const int  STAMSG_SIZE 	= sizeof(STA_MSG);

static const UCHAR TABLE_SELFTEST_REQ_MSG 	 = 0xE0;
static const UCHAR TABLE_SELFTEST_RESULT_MSG = 0xE1;
static const UCHAR TABLE_FAULT_INFO_MSG	  	 = 0xC4;
static const UCHAR TABLE_GEN_CTRL_MSG	  	 = 0xC5;
static const UCHAR TABLE_TARGET_NCTRL_MSG  	 = 0x81;
static const UCHAR TABLE_TARGET_WCTRL_MSG  	 = 0x82;
static const UCHAR TABLE_CURRENT_STATUS_MSG	 = 0x02;
static const UCHAR TABLE_RECV_CONFIRM_MSG 	 = 0x7F;

//CAN模块全局变量↓
static const UINT CAN_CHANNEL		= 2;
static const int  USING_CAN_CHANNEL = 0;
static const int  SEM_TIME_OUT		= 2;
static const int  CAN_READ_FREQ 	= 5;

static const int DRIVER_ABS_START 			= 101;
static const int DRIVER_REL_START			= 102;
static const int DRIVER_POS_STOP   	 		= 103;
static const int DRIVER_RESET   			= 104;
static const int DRIVER_ENABLE  			= 105;
static const int DRIVER_DISABLE 			= 106;
static const int DRIVER_POS_MODE_SET 		= 107;
static const int DRIVER_TRJ_MODE_SET   		= 108;
static const int DRIVER_CMD_POS_SET			= 109;
static const int DRIVER_CMD_VEL_SET			= 110;
static const int DRIVER_CMD_ACC_SET      	= 111;
static const int DRIVER_CMD_DEACC_SET    	= 112;
static const int DRIVER_QST_DEACC_SET    	= 113;
static const int DRIVER_POS_LIMIT_SET    	= 114;
static const int DRIVER_NEG_LIMIT_SET    	= 115;
static const int DRIVER_DIO_OUT_SET			= 116;
static const int DRIVER_CLEAR_MOVE			= 117;
static const int DRIVER_ACT_ALL_NODE		= 118;
static const int DRIVER_DISACT_ALL_NODE		= 119;
static const int DRIVER_FORBID_PDO_A		= 120;
static const int DRIVER_FORBID_PDO_B		= 121;
static const int DRIVER_FORBID_PDO_C		= 122;
static const int DRIVER_FORBID_PDO_D		= 123;
	
static const int DRIVER_DIO_IN_GET			= 201;
static const int DRIVER_DIO_OUT_GET			= 202;
static const int DRIVER_EVENT_REG_GET		= 203;
static const int DRIVER_FAULT_REG_GET		= 204;
static const int DRIVER_TRJ_REG_GET			= 205;
static const int DRIVER_ACT_POS_GET			= 206;
static const int DRIVER_ACT_VEL_GET			= 207;
static const int DRIVER_CMD_POS_GET  		= 208;
static const int DRIVER_CMD_VEL_GET   		= 209;


//转台控制模块全局变量↓
static const int  	ERROR_ALL_NODE[AXIS_NUM]   = {1,1,1,1,1};
static const float  POS_MAX_POSITION[AXIS_NUM] = {90,90,150,150,150};
static const float  NEG_MAX_POSITION[AXIS_NUM] = {-90,-90,-150,-150,-150};
static const float  MAX_ACC[AXIS_NUM] 		   = {10.0, 10.0, 10.0, 50.0, 50.0};
static const float  MAX_SPEED_POS[AXIS_NUM]	   = {20,20,20,20,20};
static const float 	MAX_SPEED_NEG[AXIS_NUM]	   = { 1, 1, 1, 3, 3};
static const UINT 	FAULT_ARRAY_SIZE 		   = 18;

//Digital IO模块全局变量↓
static const int  IO_READ_FREQ 	= 5;
#endif
