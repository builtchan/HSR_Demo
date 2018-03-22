#ifndef PUBLICDEF_H
#define PUBLICDEF_H
#include <QString>
typedef enum
{
    EM_PAGE_NAME_INIT = 0,
    EM_PAGE_NAME_SERVER = 1,
    EM_PAGE_NAME_MAINTAIN = 2,
    EM_PAGE_NAME_CHECKOK = 3,
    EM_PAGE_NAME_TAKETICKET = 4,
    EM_PAGE_NAME_CHECKFAIL = 5,
    EM_PAGE_NAME_OUTOFSERVER = 6,
    EM_PAGE_NAME_ENTRYINTRUDED = 7,
    EM_PAGE_NAME_EXITINTRUDED = 8,
}EM_PAGE_NAME;

#define PUBLIC_CONFIG_PATH "C:/HSR/SensorCfg.xml"

typedef enum
{
    EM_GAT_STACODE_FAULT_NORMAL = 00,		//正常状态
    EM_GAT_STACODE_AISLE_UNINIT = 101,	//通道未初始化
    EM_GAT_STACODE_ENTRY_INTRUSION = 103,	//入口闯闸
    EM_GAT_STACODE_EXIT_INTRUSION = 104,	//出口闯闸
    EM_GAT_STACODE_AISLE_OPEN_EXIT = 105,	//通道打开状态出口
    EM_GAT_STACODE_AISLE_OPEN_ENTRY = 106,//通道打开状态入口
    EM_GAT_STACODE_MAINTAINCE_STATE = 107,//维护状态
    EM_GAT_STACODE_TAIL_STATE = 108,		//尾随状态
    EM_GAT_STACODE_FAULT = 109,			//故障状态
}EM_GAT_STACODE;

typedef enum
{
    EM_GATE_ERROR_CODE_PORT_ERROR = 1501,   //通信端口错误
    EM_GATE_ERROR_CODE_COM_FAILED = 1502,   //通信失败
    EM_GATE_ERROR_CODE_MECH_FAILED = 1503,  //机械故障
    EM_GATE_ERROR_CODE_SENSOR_FAILED = 1504,//传感器故障
    EM_GATE_ERROR_CODE_CABLE_FAILED = 1505, //控制线缆异常
}EM_GATE_ERROR_CODE;


typedef struct _ST_RADIOBUTTON_CONFIG_
{
    int iNum;
    int iBit;
    int iTop;
    int iLeft;
    int iWidth;
    int iHeight;
    char szName[10];
}ST_RADIOBUTTON_CONFIG;


typedef enum
{
    EM_MAG_STATECODE_PARAM_IN_ERROR = 201,//输入参数错误
    EM_MAG_STATECODE_OPEN_COM_FAIL = 202,//打开串口错误
    EM_MAG_STATECODE_APPLY_RAM_FAIL = 203,//申请内存失败
    EM_MAG_STATECODE_FUNC_RELY_ERROR = 204,//函数依赖条件错误
    EM_MAG_STATECODE_HARDWRAE_ERROR = 205,//硬件错误
    EM_MAG_STATECODE_RCV_DATA_FAIL = 206,//接受数据失败
    EM_MAG_STATECODE_MSG_CHECK_FAIL = 207,//消息校验失败
    EM_MAG_STATECODE_IN_FUNC_ERROR = 208,//内部函数错误
    EM_MAG_STATECODE_SEND_MSG_FAIL = 209,//发送消息失败
    EM_MAG_STATECODE_READ_MAG_FAIL = 220,//读磁失败
    EM_MAG_STATECODE_WRITE_MAG_FAIL = 222,//写磁失败
    EM_MAG_STATECODE_W_R_MAG_CHECK_FAIL = 223,//写磁读磁校验错
    EM_MAG_STATECODE_PRE_READ_MAG_FAIL = 224,//预读磁失败
    EM_MAG_STATECODE_BAD_TICKET = 225,//破票
    EM_MAG_STATECODE_RE_TICKET = 226,//重张
    EM_MAG_STATECODE_NO_TICKET = 227,//非磁票
    EM_MAG_STATECODE_OPER_TIMEOUT = 240,//操作超时
    EM_MAG_STATECODE_JAM_TICKET = 241,//卡票
    EM_MAG_STATECODE_TPU_NO_CONNECT = 242,//读卡器失去连接
    EM_MAG_STATECODE_READ_TICKET_NO_TICKET = 260,//读票时无票操作
}EM_MAG_STATECODE;


typedef enum
{
    EM_TICKET_POSITION_NO = 0,
    EM_TICKET_POSITION_INLET = 1,
    EM_TICKET_POSITION_OUTLET = 2,
}EM_TICKET_POSITION;

typedef struct _ST_SYSTEM_INFO_
{
   QString HostName;
   QString IP;
   QString MAC;
   QString Gateway;
   QString BaseBoardSerialNum;
   QString BIOSSerialNum;
   QString CPUProcessorId;
   QString DiskDriveModel;
   QString OSName;
   QString OSVersion;
   QString OSMaker;
   QString OSModel;
   QString OSType;
   QString TotalMemory;
   QString AvaluableMemory;
}ST_SYSTEM_INFO;


//typedef struct _ST_MODULEINFO_
//{
//    char ID[32];
//    char Model[32];
//    char Brand[32];
//    char Version[32];
//    char VerBuildTime[80];
//}ModuleInfo;


#endif // PUBLICDEF_H

