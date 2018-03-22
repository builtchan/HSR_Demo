#include "GunneboGate/gunnebo.h"

Gunnebo::Gunnebo(QWidget *parent) : QWidget(parent)
{
    m_bInitFlag = false;
    m_bThreadFlag = false;
    m_bThreadExitFlag = false;
    m_bSensortThreadExitFlag = false;
    m_bSensortThreadFlag = false;
    m_InMaintainOpenDoor = false;
    m_bGetSensorStatus = false;
    m_bStop = false;
    pthread_mutex_init(&m_thread_mutex,NULL);
    pthread_mutex_init(&m_SensorMutex,NULL);

    m_StateMap.insert(EM_GAT_STACODE_FAULT_NORMAL,QString("正常状态"));
    m_StateMap.insert(EM_GAT_STACODE_AISLE_UNINIT,QString("通道未初始化"));
    m_StateMap.insert(EM_GAT_STACODE_ENTRY_INTRUSION,QString("入口闯闸"));
    m_StateMap.insert(EM_GAT_STACODE_EXIT_INTRUSION,QString("出口闯闸"));
    m_StateMap.insert(EM_GAT_STACODE_AISLE_OPEN_EXIT,QString("出站有授权信号"));
    m_StateMap.insert(EM_GAT_STACODE_AISLE_OPEN_ENTRY,QString("进站有授权信号"));
    m_StateMap.insert(EM_GAT_STACODE_MAINTAINCE_STATE,QString("维修模式"));
    m_StateMap.insert(EM_GAT_STACODE_TAIL_STATE,QString("尾随"));
    m_StateMap.insert(EM_GAT_STACODE_FAULT,QString("故障状态"));
    m_GateErrorMap.insert(EM_GATE_ERROR_CODE_PORT_ERROR,QString("门通信端口错误"));
    m_GateErrorMap.insert(EM_GATE_ERROR_CODE_COM_FAILED,QString("门通信失败"));
    m_GateErrorMap.insert(EM_GATE_ERROR_CODE_MECH_FAILED,QString("门机械故障"));
    m_GateErrorMap.insert(EM_GATE_ERROR_CODE_SENSOR_FAILED,QString("门传感器故障"));
    m_GateErrorMap.insert(EM_GATE_ERROR_CODE_CABLE_FAILED,QString("门控制线缆异常"));

    ParseRadioButtonCfg();
//    GunneboShowInit();
}
void Gunnebo::on_clicked_m_ConnectBtn()
{
    if(m_bInitFlag)
    {
        m_TipsTextEdit->append("已连接");
        return;
    }
    char szResult[100];
    GateInit(m_iCom,szResult);
}
//紧急模式
void Gunnebo::on_clicked_EmergencyBtn()
{
    if(!m_bInitFlag)
    {
        m_TipsTextEdit->append("没有初始化或连接失败");
        return;
    }
    static bool bEmergency = false;

    char szIn[10],szOut[10];
    if(bEmergency)
        szIn[0] = 0x36;
    else
        szIn[0] = 0x35;

    pthread_mutex_lock(&m_SensorMutex);
    GAT_SetMode(szIn,szOut);
    pthread_mutex_unlock(&m_SensorMutex);
    if(szOut[0])
    {
        m_EmergencyModeBtn->setText("退出紧急模式");
        bEmergency = true;
    }
    else
    {
        m_EmergencyModeBtn->setText("紧急模式");
        bEmergency = false;
    }
}

//1.维修开关门
void Gunnebo::on_clicked_MaintainModeBtn()
{
    if(!m_bInitFlag)
    {
        m_TipsTextEdit->append("没有初始化或连接失败");
        return;
    }

    char szIn[10] = {0},szOut[10];

    if(m_InMaintainOpenDoor)
        szIn[0] = 0x32;
    else
        szIn[0] = 0x31;

    pthread_mutex_lock(&m_SensorMutex);
    GAT_SetMode(szIn,szOut);
    pthread_mutex_unlock(&m_SensorMutex);
    if(szOut[0])
    {
        m_InMaintainOpenDoor = true;
        m_MaintainModeBtn->setText("维修关门");
    }
    else
    {
        m_InMaintainOpenDoor = false;
        m_MaintainModeBtn->setText("维修开门");
    }

}

//2.DIO控制
void Gunnebo::on_clicked_DIOContrlBtn(int iNo)
{
    for(int i=0;iNo < 3 && i < 3;i++)
    {
        if(i != iNo)
            m_DIOCtlGroup->button(i)->setChecked(false);
    }
    if(3 == iNo)
        m_AisleLedForbit->setChecked(false);
    else if(4 == iNo)
        m_AisleLedPass->setChecked(false);

    if(!m_bInitFlag)
    {
        m_TipsTextEdit->append("没有初始化或连接失败");
        return;
    }
    char szIn[10] = {0},szOut[10];
    memset(szIn,0x30,sizeof(szIn));
    if(m_Buzzer->isChecked())
        szIn[0] = 0x32;
    if(m_TopLedRed->isChecked())
        szIn[1] = 0x33;
    if(m_TopLedGreen->isChecked())
        szIn[1] = 0x31;
    if(m_TopLedYellow->isChecked())
        szIn[1] = 0x34;
    if(m_AisleLedPass->isChecked())
        szIn[2] = 0x31;
    else if(m_AisleLedForbit->isChecked())
        szIn[2] = 0x33;
    else
        szIn[2] = 0x39;
    if(0 != GAT_SetLampAndBeepStatus(szIn,szOut))
        m_TipsTextEdit->append("设置失败");
    else
        m_TipsTextEdit->append("设置成功");
}

//3.赋授权信号
void Gunnebo::on_clicked_SignalBtn()
{
    if(!m_bInitFlag)
    {
        m_TipsTextEdit->append("没有初始化或连接失败");
        return;
    }
    char szIn[10] = {0},szOut[10] = {0};
    szIn[0] = 0x33;
    GAT_Control(szIn,szOut);
    m_TipsTextEdit->append("授权成功");
}
//4.开门
void Gunnebo::on_clicked_OpenDoorBtn()
{
    if(!m_bInitFlag)
    {
        m_TipsTextEdit->append("没有初始化或连接失败");
        return;
    }
//    if(0 == m_SigalLabel->text().toInt())
//    {
//        m_TipsTextEdit->append("授权信号为零，不能发开门");
//        return;
//    }
    char szIn[10] = {0},szOut[10] = {0};
    szIn[0] = 0x32;
    GAT_Control(szIn,szOut);
    m_TipsTextEdit->append("开门成功");
}

//5.清除提示框
void Gunnebo::on_clicked_ClearBtn()
{
    m_TipsTextEdit->clear();
}
//6.获取到位开关值
void Gunnebo::on_clicked_GetSwitchBtn()
{
    if(!m_bInitFlag)
    {
        m_TipsTextEdit->append("没有初始化或连接失败");
        return;
    }
    char szIn[10] = {0},szOut[256] = {0};
    szIn[0] = 0x33;
    szIn[1] = 11;           //寄存器11
    pthread_mutex_lock(&m_SensorMutex);
    GAT_SetMode(szIn,szOut);
    pthread_mutex_unlock(&m_SensorMutex);
    m_TipsTextEdit->append("到位开关状态：" + QString(szOut));
}
//7.获取传感器状态
void Gunnebo::on_clicked_GetSenserStatus()
{
    if(!m_bInitFlag)
    {
        m_TipsTextEdit->append("没有初始化或连接失败");
        return;
    }
    static bool bIsStart = false;
    if(!m_bSensortThreadFlag)
    {
        int iRet = pthread_create(&m_SensorThreadId,NULL,SensorStatusThread,(void*)this);
        if(0 != iRet)
        {
            m_bSensortThreadFlag = false;
            m_TipsTextEdit->append("创建传感器线程失败");
        }
        else
        {
            m_bSensortThreadFlag = true;
            m_TipsTextEdit->append("创建传感器线程成功");
        }
    }
    if(!bIsStart)
    {
        char szIn[10] = {0},szOut[256] = {0};
        szIn[0] = 0x34;
        pthread_mutex_lock(&m_SensorMutex);
        GAT_SetMode(szIn,szOut);
        pthread_mutex_unlock(&m_SensorMutex);
        int iSensorNum = atoi(szOut);
        m_TipsTextEdit->append(QString("传感器个数:%1").arg(iSensorNum));
        m_bGetSensorStatus = true;
        bIsStart = true;
        m_GetSenserStatusBtn->setText("停止获取传感器状态");
    }
    else
    {
        bIsStart = false;
        m_bGetSensorStatus = false;
        m_GetSenserStatusBtn->setText("获取传感器状态");
    }
}
void Gunnebo::on_clicked_DeviceInfoBtn()
{
    if(!m_bInitFlag)
    {
        m_TipsTextEdit->append("没有初始化或连接失败");
        return;
    }
    char szIn[10],szOut[256] = {0};
    GAT_GetDeviceInfo(szIn,szOut);
    QString string = QString(szOut);
    m_TipsTextEdit->append("厂家全称   :"+string.section(",",0,0));
    m_TipsTextEdit->append("厂家简称   :"+string.section(",",1,1));
    m_TipsTextEdit->append("设备型号   :"+string.section(",",2,2));
    m_TipsTextEdit->append("出厂日期   :"+string.section(",",3,3));
    m_TipsTextEdit->append("序列号     :"+string.section(",",4,4));
    m_TipsTextEdit->append("闸门唯一编号:"+string.section(",",5,5));
    m_TipsTextEdit->append("闸门型号   :"+string.section(",",6,6));
    m_TipsTextEdit->append("闸门品牌   :"+string.section(",",7,7));
}

//门初始化
void Gunnebo::GateInit(int iCom,char * pResult)
{
    m_iCom = iCom;
    char szIn[20],szOut[1024] = {0};
    szIn[0] = 125;  //Demo模式
    szIn[1] = iCom;    //COM口
    GAT_Init(szIn,szOut);
    int iErrorCode = QString(szOut+4).toInt();
    if(1501 == iErrorCode || 1502 == iErrorCode)
    {
        m_bInitFlag = false;
        m_TipsTextEdit->append("门连接或初始化失败，错误码:" + QString(szOut+4));
        strcpy(pResult,szOut+4);
        return;
    }
    else
    {
        if(0x30 != szOut[4])
            m_TipsTextEdit->append("warnning" + QString(szOut+4));
        m_bInitFlag = true;
        m_TipsTextEdit->append("门初始化成功");
    }
    if(!m_bThreadFlag)
    {
        m_bThreadFlag = true;
        int iRet = pthread_create(&m_threadId,NULL,GetStatusThread,(void*)this);
        if(0 != iRet)
        {
            m_bThreadFlag = false;
            m_TipsTextEdit->append("创建门单元主线程失败");
        }
        else
        {
            m_bThreadFlag = true;
//            m_TipsTextEdit->append("创建门单元主线程成功");
        }
    }

    if(!m_bSensortThreadFlag)
    {
        int iRet = pthread_create(&m_SensorThreadId,NULL,SensorStatusThread,(void*)this);
        if(0 != iRet)
        {
            m_bSensortThreadFlag = false;
            m_TipsTextEdit->append("创建传感器线程失败");
        }
        else
        {
            m_bSensortThreadFlag = true;
//            m_TipsTextEdit->append("创建传感器线程成功");
        }
    }
}

//主线程
void * Gunnebo::GetStatusThread(void *arg)
{
    Gunnebo* pGateCommon = (Gunnebo * )arg;
    char szIn[100] = {0},szOut[100] = {0};
    int iDoorOpenTime = 0;
    EM_GAT_STACODE GateStateBack = EM_GAT_STACODE_FAULT_NORMAL;
    qDebug() << "In Thread";
    while(pGateCommon->m_bThreadFlag)
    {
        while(!pGateCommon->m_bStop)
        {
            Sleep(150);
            pthread_mutex_lock(&pGateCommon->m_thread_mutex);
            memset(szOut,0,sizeof(szOut));
            pGateCommon->m_GateState = (EM_GAT_STACODE)pGateCommon->GateGetStatus(szIn,szOut);
            if(iDoorOpenTime != QString(szOut+9).toInt())
            {
                qDebug() << QString(szOut)<< szOut+9;
                iDoorOpenTime = QString(szOut+9).toInt();
                pGateCommon->m_TipsTextEdit->append(QString("扇门开关次数：%1").arg(iDoorOpenTime));
            }
            if(pGateCommon->m_GateState != GateStateBack)
            {
                if(EM_GAT_STACODE_AISLE_UNINIT == pGateCommon->m_GateState || (EM_GAT_STACODE_FAULT == pGateCommon->m_GateState && 0==strcmp(szOut,"11101502")))
                    pGateCommon->m_bInitFlag = false;
                GateStateBack = pGateCommon->m_GateState;
                pGateCommon->m_TipsTextEdit->append("状态变化，当前状态：" + pGateCommon->m_StateMap[pGateCommon->m_GateState]);
                if(EM_GAT_STACODE_FAULT_NORMAL == pGateCommon->m_GateState || EM_GAT_STACODE_MAINTAINCE_STATE == pGateCommon->m_GateState)
                    pGateCommon->m_bInitFlag = true;
            }
            //        if(pGateCommon->m_GateState != GateStateBack || (EM_GAT_STACODE_AISLE_OPEN_ENTRY == pGateCommon->m_GateState && iSignalTime != szOut[0]))
            //        {
            //            GateStateBack = pGateCommon->m_GateState;
            //            if(NULL != pGateCommon->m_TipsTextEdit)
            //                pGateCommon->m_TipsTextEdit->append("状态变化，当前状态：" + pGateCommon->m_StateMap[pGateCommon->m_GateState]);
            //            if(EM_GAT_STACODE_AISLE_OPEN_ENTRY == pGateCommon->m_GateState &&  iSignalTime != szOut[0])
            //            {
            //                iSignalTime = szOut[0];
            //            }
            //            if(EM_GAT_STACODE_AISLE_UNINIT == pGateCommon->m_GateState)
            //            {
            //                pGateCommon->m_bInitFlag = false;
            //            }
            //            if(EM_GAT_STACODE_MAINTAINCE_STATE == pGateCommon->m_GateState)
            //            {
            //                pGateCommon->m_MaintainModeBtn->setText("维修关门");
            //            }
            //            if(EM_GAT_STACODE_FAULT_NORMAL == pGateCommon->m_GateState)
            //            {
            //                pGateCommon->m_MaintainModeBtn->setText("维修开门");
            //            }
            //        }
            pthread_mutex_unlock(&pGateCommon->m_thread_mutex);
            Sleep(10);
        }
    }
    pGateCommon->m_bThreadExitFlag = true;
    return NULL;
}
//获取传感器状态线程
void* Gunnebo::SensorStatusThread(void *arg)
{
    Gunnebo*pCommonSensor = (Gunnebo*)arg;
    while(pCommonSensor->m_bSensortThreadFlag)
    {
        if(pCommonSensor->m_bGetSensorStatus)
        {
            pCommonSensor->GetSensorStatus();
            Sleep(100);
        }
    }
    pCommonSensor->m_bSensortThreadExitFlag = true;
    return NULL;
}
//开门和授权
int Gunnebo::OpenDoor(bool bIn)
{
    char szIn[10] = {0},szOut[10] = {0};
    if(bIn)
        szIn[0] = 0x32;     //开门
    else
        szIn[0] = 0x33;     //授权
    return GAT_Control(szIn,szOut);
}

int Gunnebo::DIOCtrl(char *pIn)
{
    char szOut[10];
    return GAT_SetLampAndBeepStatus(pIn,szOut);
}

//获取状态中转
int Gunnebo::GateGetStatus(char *pIn, char *pOut)
{
    return (EM_GAT_STACODE)GAT_GetStatus(pIn,pOut);
}

void Gunnebo::GetSensorStatus()
{
    char szIn[10] = {0},szOut[256] = {0};
    szIn[0] = 0x34;
    pthread_mutex_lock(&m_SensorMutex);
    GAT_SetMode(szIn,szOut);
    pthread_mutex_unlock(&m_SensorMutex);
    int iHighFaultSensor = atoi(szOut + 3);
    int iLowFaultSensor = atoi(szOut + 7);
    int iChildSensor = atoi(szOut + 11);
//    qDebug()<< iLowFaultSensor << iHighFaultSensor << iChildSensor;
    for(int i = 1;i <= 8; i++)
    {
//        qDebug() << m_RadioButtonMap[i].iBit << m_RadioButtonMap[i+8].iBit - 8;

        if((iLowFaultSensor>> (m_RadioButtonMap[i].iBit)) & 1)
        {
            m_Sensors[i-1]->setChecked(true);
        }
        else
            m_Sensors[i-1]->setChecked(false);
        if((iHighFaultSensor >> (m_RadioButtonMap[i+8].iBit - 8)) & 1 )
            m_Sensors[i+7]->setChecked(true);
        else
            m_Sensors[i+7]->setChecked(false);
        if(20 == szOut[0] && i<4)
        {
            if((iChildSensor >> (m_RadioButtonMap[i+16].iBit - 16)) & 1)
                m_Sensors[i+15]->setChecked(true);
            else
                m_Sensors[i+15]->setChecked(false);
        }
    }
}
void Gunnebo::GunneboShowInit(QWidget *parent)
{
    QString qButtonStyleSheet("QPushButton{background-color:rgb(85, 170, 255);color:white;border-radius:10px;border: 3px groove gray;\
                        border-style: outset;font-family:Arial;font-size:12px;}"
                       "QPushButton:hover{background-color:white; color: black;}"
                      "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style:inset;}");
    QString qLabelStyleSheet("background-color:rgb(205, 170, 125);color:white;border-radius:10px");

    //背景
    m_ButtonBackground = new QLabel(parent);
    m_ButtonBackground->setGeometry(0,110,210,370);
    m_ButtonBackground->setText("门单元");
    m_ButtonBackground->setAlignment(Qt::AlignTop);
    m_ButtonBackground->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:1 rgba(0, 0, 0, 50));\
                            border-radius:10px;color:rgba(0, 0, 0, 95);font-size:25px;");

    m_ClearBtn = new QPushButton(parent);
    m_ClearBtn->setGeometry(580,250,60,30);
    m_ClearBtn->setText("清除");
    m_ClearBtn->setStyleSheet(qButtonStyleSheet);

    int iTop = 140, iLeft = 0;
    m_TopLedLabel = new QLabel(parent);
    m_TopLedLabel->setGeometry(iLeft,iTop,70,35);
    m_TopLedLabel->setText(" 顶部指示灯\n 红  绿  黄");
    m_TopLedLabel->setStyleSheet(qLabelStyleSheet);
    m_AisleLedLabel = new QLabel(parent);
    m_AisleLedLabel->setGeometry(iLeft+85,iTop,70,35);
    m_AisleLedLabel->setText("通道指示灯\n 红  绿");
    m_AisleLedLabel->setAlignment(Qt::AlignCenter);
    m_AisleLedLabel->setStyleSheet(qLabelStyleSheet);

    m_BuzzerLabel = new QLabel(parent);
    m_BuzzerLabel->setGeometry(iLeft+160,iTop,40,35);
    m_BuzzerLabel->setText("蜂鸣器\n 响");
    m_BuzzerLabel->setAlignment(Qt::AlignCenter);
    m_BuzzerLabel->setStyleSheet(qLabelStyleSheet);
    iTop = 175;
    iLeft = 0;
    m_DIOCtlGroup = new QButtonGroup(parent);
    m_TopLedRed = new QRadioButton(parent);
    m_TopLedRed->setGeometry(iLeft+5,iTop,25,25);//红
    m_TopLedGreen = new QRadioButton(parent);
    m_TopLedGreen->setGeometry(iLeft+28,iTop,25,25);//绿
    m_TopLedYellow = new QRadioButton(parent);
    m_TopLedYellow->setGeometry(iLeft+54,iTop,25,25);//黄
    m_DIOCtlGroup->addButton(m_TopLedRed,0);
    m_DIOCtlGroup->addButton(m_TopLedGreen,1);
    m_DIOCtlGroup->addButton(m_TopLedYellow,2);

    m_AisleLedPass = new QRadioButton(parent);
    m_AisleLedPass->setGeometry(iLeft+102,iTop,25,25);//红
    m_AisleLedForbit = new QRadioButton(parent);
    m_AisleLedForbit->setGeometry(iLeft + 128,iTop,25,25);//绿
    m_DIOCtlGroup->addButton(m_AisleLedPass,3);
    m_DIOCtlGroup->addButton(m_AisleLedForbit,4);
    m_DIOCtlGroup->setExclusive(false);
    m_Buzzer = new QRadioButton(parent);
    m_Buzzer->setGeometry(iLeft + 178,iTop,25,25);//响
    m_DIOCtlGroup->addButton(m_Buzzer,5);

    iTop = 200;
    m_ConnectBtn = new QPushButton(parent);
    m_ConnectBtn->setGeometry(0,iTop,100,30);
    m_ConnectBtn->setText("门连接");
    m_ConnectBtn->setStyleSheet(qButtonStyleSheet);

    m_SignalBtn = new QPushButton(parent);
    m_SignalBtn->setGeometry(0,iTop + 30,100,30);
    m_SignalBtn->setText("授权");
    m_SignalBtn->setStyleSheet(qButtonStyleSheet);

    m_OpenDoorBtn = new QPushButton(parent);
    m_OpenDoorBtn->setGeometry(0,iTop + 60,100,30);
    m_OpenDoorBtn->setText("开门");
    m_OpenDoorBtn->setStyleSheet(qButtonStyleSheet);

    m_EmergencyModeBtn = new QPushButton(parent);
    m_EmergencyModeBtn->setGeometry(100,iTop,100,30);
    m_EmergencyModeBtn->setText("紧急模式");
    m_EmergencyModeBtn->setStyleSheet(qButtonStyleSheet);

    m_MaintainModeBtn = new QPushButton(parent);
    m_MaintainModeBtn->setGeometry(100,iTop + 30,100,30);
    m_MaintainModeBtn->setText("维修开门");
    m_MaintainModeBtn->setStyleSheet(qButtonStyleSheet);

    m_GetSwitchBtn = new QPushButton(parent);
    m_GetSwitchBtn->setGeometry(100,iTop + 60,100,30);
    m_GetSwitchBtn->setText("到位开关状态");
    m_GetSwitchBtn->setStyleSheet(qButtonStyleSheet);

    m_GetSenserStatusBtn = new QPushButton(parent);
    m_GetSenserStatusBtn->setGeometry(0,iTop + 90,120,30);
    m_GetSenserStatusBtn->setText("获取传感器状态");
    m_GetSenserStatusBtn->setStyleSheet(qButtonStyleSheet);

    m_DeviceInfoBtn = new QPushButton(parent);
    m_DeviceInfoBtn->setGeometry(120,iTop + 90,80,30);
    m_DeviceInfoBtn->setText("设备信息");
    m_DeviceInfoBtn->setStyleSheet(qButtonStyleSheet);

    iTop = 330;
    iLeft = 2;
    m_SensorGroup = new QButtonGroup(parent);
    m_SensorGroup->setExclusive(false);
    QMap<int, ST_RADIOBUTTON_CONFIG>::iterator it = m_RadioButtonMap.begin();
    for(;it != m_RadioButtonMap.end(); it++)
    {
        m_Sensors[it->iNum-1] = new QRadioButton(parent);
        m_SensorNameLb[it->iNum-1] = new QLabel(parent);
        m_Sensors[it->iNum-1]->setGeometry(iLeft+it->iLeft,iTop+it->iTop,40,40);
        m_SensorGroup->addButton(m_Sensors[it->iNum-1]);
        m_SensorNameLb[it->iNum-1]->setText(QString(it->szName));
        m_SensorNameLb[it->iNum-1]->setGeometry(iLeft+it->iLeft-2,iTop+it->iTop+28,18,10);
        m_SensorNameLb[it->iNum-1]->setStyleSheet("background-color:rgb(205, 170, 125);color:black;font-size:10px;font:bold");
        m_SensorNameLb[it->iNum-1]->setAlignment(Qt::AlignCenter);
    }

//    GunneboHide();
    connect(m_ConnectBtn,SIGNAL(clicked()),this,SLOT(on_clicked_m_ConnectBtn()));
    connect(m_ClearBtn,SIGNAL(clicked()),this,SLOT(on_clicked_ClearBtn()));
    connect(m_MaintainModeBtn,SIGNAL(clicked()),this,SLOT(on_clicked_MaintainModeBtn()));
    connect(m_SignalBtn,SIGNAL(clicked()),this,SLOT(on_clicked_SignalBtn()));
    connect(m_OpenDoorBtn,SIGNAL(clicked()),this,SLOT(on_clicked_OpenDoorBtn()));
    connect(m_GetSwitchBtn,SIGNAL(clicked()),this,SLOT(on_clicked_GetSwitchBtn()));
    connect(m_GetSenserStatusBtn,SIGNAL(clicked()),this,SLOT(on_clicked_GetSenserStatus()));
    connect(m_DIOCtlGroup,SIGNAL(buttonClicked(int)),this,SLOT(on_clicked_DIOContrlBtn(int)));
//    connect(m_AisleLedGroup,SIGNAL(buttonClicked(int)),this,SLOT(on_clicked_DIOContrlBtn(int)));
    connect(m_EmergencyModeBtn,SIGNAL(clicked()),this,SLOT(on_clicked_EmergencyBtn()));
    connect(m_DeviceInfoBtn,SIGNAL(clicked()),this,SLOT(on_clicked_DeviceInfoBtn()));
}
//展现控件
void Gunnebo::GunneboShow()
{
    qDebug() <<"GunneboShow";
    m_EmergencyModeBtn->show();
    m_MaintainModeBtn->show();         //维修开门
    m_SignalBtn->show();               //授权
//    m_SigalLabel->show();
//    m_SigalLabelTip->show();
    m_OpenDoorBtn->show();             //开门
    m_TopLedLabel->show();
    //顶部警示灯
    m_TopLedRed->show();
    m_TopLedGreen->show();
    m_TopLedYellow->show();
    m_AisleLedLabel->show();
    //通道指示灯
    m_AisleLedPass->show();
    m_AisleLedForbit->show();
    m_Buzzer->show();                 //蜂鸣器
//    m_DIOContrlBtn->show();
//    m_ConnectBtn->show();              //连接
//    m_DisconnectBtn->show();           //断连
    m_ClearBtn->show();                //清除提示框
    m_GetSwitchBtn->show();            //到位开关状态
    m_GetSenserStatusBtn->show();      //传感器按钮

//    m_ComLabel->show();
//    m_Com->show();
    m_TipsTextEdit->show();              //提示框

    for(int i=0;i < 20; i++)
        m_Sensors[i]->show();        //传感器
}
//隐藏控件
void Gunnebo::GunneboHide()
{
    qDebug() <<"GunneboHide";
    m_EmergencyModeBtn->hide();
    m_MaintainModeBtn->hide();         //维修开门
    m_SignalBtn->hide();               //授权
//    m_SigalLabel->hide();
//    m_SigalLabelTip->hide();
    m_OpenDoorBtn->hide();             //开门
    m_TopLedLabel->hide();
    //顶部警示灯
    m_TopLedRed->hide();
    m_TopLedGreen->hide();
    m_TopLedYellow->hide();
    m_AisleLedLabel->hide();
    //通道指示灯
    m_AisleLedPass->hide();
    m_AisleLedForbit->hide();
    m_Buzzer->hide();                 //蜂鸣器
//    m_DIOContrlBtn->hide();
//    m_ConnectBtn->hide();              //连接
//    m_DisconnectBtn->hide();           //断连
    m_ClearBtn->hide();                //清除提示框
    m_GetSwitchBtn->hide();            //到位开关状态
    m_GetSenserStatusBtn->hide();      //传感器按钮

//    m_ComLabel->hide();
//    m_Com->hide();
    m_TipsTextEdit->hide();              //提示框

    for(int i=0;i < 20; i++)
        m_Sensors[i]->hide();        //传感器
}

void Gunnebo::SetTextEdit(QTextEdit *qTextEd)
{
    m_TipsTextEdit = qTextEd;
}

void Gunnebo::GateUninit()
{
    char szIn[10],szOut[10];
    szIn[0] = 88;
    GAT_Init(szIn,szOut);
}

Gunnebo::~Gunnebo()
{

}

void Gunnebo::ParseRadioButtonCfg()
{
    //1 open
     QFile file(PUBLIC_CONFIG_PATH);
     if(!file.open(QFile::ReadOnly | QFile::Text))
       {
         qDebug() << "Open " << PUBLIC_CONFIG_PATH << "failed";
         return;
       }

     //2、将text文件以utf-8转成流数据
     QTextStream stream(&file);
     stream.setCodec(QTextCodec::codecForName("utf-8"));

     //3、转成String后再转换成文档
     QString string = stream.readAll();
     QDomDocument Domc;
     Domc.setContent(string);
     if(Domc.isNull() || file.size() <= 0)
       {
         qDebug() << "The xml file is Empty";
         return;
       }
     QDomElement Config = Domc.documentElement();
     QDomElement SernorCfg = Config.firstChildElement();
     QDomElement Sernsor = SernorCfg.firstChildElement();
     m_RadioButtonMap.clear();
     ST_RADIOBUTTON_CONFIG stRadioButtonCfg;
     while(!Sernsor.isNull())
     {
        memset(&stRadioButtonCfg,0,sizeof(stRadioButtonCfg));
        stRadioButtonCfg.iNum = Sernsor.attributeNode("num").value().toInt();
        stRadioButtonCfg.iBit = Sernsor.attributeNode("bit").value().toInt();
        stRadioButtonCfg.iTop = Sernsor.attributeNode("top").value().toInt();
        stRadioButtonCfg.iLeft = Sernsor.attributeNode("left").value().toInt();
        stRadioButtonCfg.iWidth = Sernsor.attributeNode("width").value().toInt();
        stRadioButtonCfg.iHeight = Sernsor.attributeNode("height").value().toInt();
        strcpy(stRadioButtonCfg.szName,Sernsor.attributeNode("name").value().toStdString().c_str());
        m_RadioButtonMap.insert(stRadioButtonCfg.iNum,stRadioButtonCfg);
        Sernsor = Sernsor.nextSiblingElement();
     }
//     QList<int ,ST_RADIOBUTTON_CONFIG>::iterator it = m_RadioButtonMap.begin();
//     for(;it != m_RadioButtonMap.end(); it++)
//         qDebug() << it->iNum << it->iBit << it->iTop << it->iLeft;
}
