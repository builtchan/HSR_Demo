
#include "OmronTRE/omrontre.h"

//有效磁数据
char Stack[3][54] ={
    {0xf7,0x04,0x54,0x03,0xf9,0x56,0xc4,0xe1,0x28,0xad,0x6f,0x20,0x1e,0x01,0x90,0x9c,0x00,0x01,0x5c,0x80,0xec,0xa4,0x1f,0x90,0xb0,0x00,0x00,0x07,0x70,0x45,0x40,0x3f,0x95,0x6c,0x4e,0x12,0x8a,0xd6,0xf2,0x01,0xe0,0x19,0x09,0xc0,0x00,0x15,0xc8,0x0e,0xca,0x41,0xf9,0x0b,0x40,0x00},
    {0xf6,0xb0,0x94,0x46,0x85,0x20,0xb2,0x0a,0xd0,0x10,0x82,0x05,0xc4,0x03,0xb4,0x0c,0xa0,0xc0,0x00,0x00,0xd3,0x6f,0xff,0x94,0x7e,0x80,0x00,0x07,0x6b,0x09,0x44,0x68,0x52,0x0b,0x20,0xad,0x01,0x08,0x20,0x5c,0x40,0x3b,0x40,0xca,0x0c,0x00,0x00,0x0d,0x36,0xff,0xf9,0x47,0xc0,0x00},
    {0xf7,0x8e,0x1a,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1f,0x8c,0x94,0x80,0x00,0x07,0x78,0xe1,0xa4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xf8,0xc9,0x40,0x00}
};
COmronTre::COmronTre(QWidget *parent)
    : QWidget(parent)
{
    m_bTreInit = false;
    m_bThreadExitFlag = false;
    m_bThreadFlag = false;
    m_CanRead = false;
    m_bCheckSuccess = false;
    m_bFatalError = false;
    pthread_mutex_init(&m_ThreadMutex,NULL);
    memset(&m_ReadTicketInfo,0,sizeof(m_ReadTicketInfo));
    memset(&m_WriteData,0,sizeof(m_WriteData));

    for(int i = 0; i < 3 ;i++)
    {
        m_WriteData[i].bFlag = true;
        m_WriteData[i].iDataLen = 432;
        memcpy(m_WriteData[i].ucData,Stack[i],54);
    }

    sem_init(&m_Check_Sem,0,0);
    sem_init(&m_Ticket_Take_Sem,0,0);
    m_emTicketPos = EM_TICKET_POSITION_NO;
    {
    m_StateToStringMap.insert(EM_MAG_STATECODE_PARAM_IN_ERROR,QString("磁模块输入参数错误"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_OPEN_COM_FAIL,QString("磁模块打开串口错误"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_APPLY_RAM_FAIL,QString("磁模块申请内存失败"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_FUNC_RELY_ERROR,QString("磁模块函数依赖条件错误"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_HARDWRAE_ERROR,QString("磁模块硬件错误"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_RCV_DATA_FAIL,QString("磁模块接受数据失败"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_MSG_CHECK_FAIL,QString("磁模块消息校验失败"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_IN_FUNC_ERROR,QString("磁模块内部函数错误"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_SEND_MSG_FAIL,QString("磁模块发送消息失败"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_READ_MAG_FAIL,QString("磁模块读磁失败"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_WRITE_MAG_FAIL,QString("磁模块写磁失败"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_W_R_MAG_CHECK_FAIL,QString("磁模块写磁读磁校验错"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_PRE_READ_MAG_FAIL,QString("磁模块预读磁失败"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_BAD_TICKET,QString("磁模块破票"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_RE_TICKET,QString("磁模块重张"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_NO_TICKET,QString("磁模块非磁票"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_OPER_TIMEOUT,QString("磁模块操作超时"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_JAM_TICKET,QString("磁模块卡票"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_TPU_NO_CONNECT,QString("磁模块读卡器失去连接"));
    m_StateToStringMap.insert(EM_MAG_STATECODE_READ_TICKET_NO_TICKET,QString("磁模块读票时无票操作"));
    }

//    T_EW_WRITE_PRINT_MAGCARD_INFO stTest;

//    qDebug() <<"T_EW_IPC_MAGCARD_TRACK_INFO :" << sizeof(T_EW_IPC_MAGCARD_TRACK_INFO);
//    qDebug() <<"T_EW_WRITE_PRINT_MAGCARD_INFO :" << sizeof(stTest.tWriteMagInfo);
    connect(&m_Connect,SIGNAL(clicked()),this,SLOT(on_clicked_ConnectBtn()));
//    connect(&m_ReadTicketBtn,SIGNAL(clicked()),this,SLOT(on_clicked_ReadTicketBtn()));
    connect(&m_WriteTicketBtn,SIGNAL(clicked()),this,SLOT(on_clicked_WriteTicketBtn()));
    connect(&m_EjectBtn,SIGNAL(clicked()),this,SLOT(on_clicked_EjectBtn()));
    connect(&m_LedControlBtn,SIGNAL(clicked()),this,SLOT(on_clicked_LedControlBtn()));
}

COmronTre::~COmronTre()
{
    char szIn[10],szOut[10];
    if(m_bTreInit)
        MAG_Uninit(szIn,szOut);
    if(m_bThreadFlag)
    {
        m_bThreadFlag = false;
        while(!m_bThreadExitFlag)
            Sleep(50);
        void *Thread_exit;
        pthread_join(m_ThreadId,&Thread_exit);
        pthread_mutex_destroy(&m_ThreadMutex);
    }
}

int COmronTre::TREInit(int iCom,char *pResult)
{
    m_iCom = iCom;
    char szIn[10],szOut[100];
    szIn[0] = 125;
    szIn[1] = iCom;        //com
    int iRet = MAG_Init(szIn,szOut);
    m_bTreInit = false;
    if(1 == iRet)
    {
        m_TextEdit->append("磁模块连接或初始化失败，错误码:" + QString(szOut+4));
        strcpy(pResult,szOut+4);
        return -1;
    }
    else if(EM_MAG_STATECODE_JAM_TICKET == iRet)
    {
        m_TextEdit->append("卡票");
        return -1;
    }
    else if(EM_MAG_STATECODE_HARDWRAE_ERROR == iRet)
    {
        m_TextEdit->append("模块故障");
        return -1;
    }
    Sleep(7000);
    iRet = MAG_CheckStart(szIn,szOut);
    if(0 == iRet)
    {
        m_bTreInit = true;
    }
    else if(1 == iRet)
    {
        m_TextEdit->append("MAG_CheckStart Failed");
        return -1;
    }
    else
    {
        m_TextEdit->append(m_StateToStringMap[(EM_MAG_STATECODE)iRet]);
        return -1;
    }

    if(!m_bThreadFlag)
    {
        m_bThreadFlag = true;
        int iRet = pthread_create(&m_ThreadId,NULL,TicketPOSThread,(void*)this);
        if(0 != iRet)
        {
            m_bThreadFlag = false;
            m_TextEdit->append("创建磁票检测线程失败");
        }
        else
        {
            m_bThreadFlag = true;
            m_TextEdit->append("创建磁票检测线程成功");
        }
    }
    return 0;
}

void COmronTre::on_clicked_ConnectBtn()
{
    if(m_bTreInit)
    {
        m_TextEdit->append("已连接");
        return;
    }
    char szResult[100];
    TREInit(m_iCom,szResult);
}
//读操作
void COmronTre::on_clicked_ReadTicketBtn()
{
    if(!m_bTreInit)
    {
        m_TextEdit->append("连接失败或者初始化失败");
        return;
    }
//    if(!m_CanRead)
//    {
//        m_TextEdit->append("当前状态不能进行读操作");
//        return;
//    }
    if(m_bFatalError)
    {
        m_TextEdit->append("有故障");
        return;
    }
    m_bCheckSuccess = false;
    char szIn[10],szOut[3072];
    int iRet = MAG_Read(szIn,szOut);
    if(0 == iRet)
    {
        m_bCheckSuccess = true;
        memcpy(&m_ReadTicketInfo,szOut,(sizeof(T_EW_IPC_MAGCARD_TRACK_INFO)*3));
        m_TextEdit->append("读成功");
    }
    else if(1 == iRet)
        m_TextEdit->append("读失败");
    else
        m_TextEdit->append(m_StateToStringMap[(EM_MAG_STATECODE)iRet]);
}
//写操作
void COmronTre::on_clicked_WriteTicketBtn()
{
    if(!m_bTreInit)
    {
        m_TextEdit->append("连接失败或者初始化失败");
        return;
    }
    if(m_bFatalError)
    {
        m_TextEdit->append("有故障");
        return;
    }
    if(m_bCheckSuccess)
    {
        memset(&m_stWriteInfo,0,sizeof(m_stWriteInfo));
//        memcpy(&stWriteInfo.tWriteMagInfo,m_ReadTicketInfo,sizeof(m_ReadTicketInfo));
        memcpy(&m_stWriteInfo.tWriteMagInfo,&m_WriteData,sizeof(m_WriteData));
        if(m_PrintNothing.isChecked())
            m_stWriteInfo.iPrintFlagInfo = 0;
        else if(m_PrintTriangle.isChecked())
            m_stWriteInfo.iPrintFlagInfo = 1;
        else
            m_stWriteInfo.iPrintFlagInfo = 2;
        char szOut[10];
        szOut[0] = 125;
//        memcpy(szIn,&m_stWriteInfo,sizeof(T_EW_WRITE_PRINT_MAGCARD_INFO));
        int iRet = MAG_Eject((char *)&m_stWriteInfo,szOut);
        if(0 == iRet)
        {
            m_TextEdit->append("写成功");
        }
        else
            m_TextEdit->append(m_StateToStringMap[(EM_MAG_STATECODE)iRet]);
    }
    m_bCheckSuccess = false;
    m_emCheckSuccessPos = EM_TICKET_POSITION_OUTLET;
}
//退票操作
void COmronTre::on_clicked_EjectBtn()
{
    if(!m_bTreInit)
    {
        m_TextEdit->append("连接失败或者初始化失败");
        return;
    }
    if(m_bFatalError)
    {
        m_TextEdit->append("有故障");
        return;
    }

//    if(EM_TICKET_POSITION_INLET == pTreCommon->m_emTicketPos)
//    {
        char szIn[10],szOut[10];
        if(m_EjectInlet.isChecked())
        {
            szIn[0] = 0x31;
            m_emCheckSuccessPos = EM_TICKET_POSITION_INLET;
        }
        else
        {
            szIn[0] = 0x32;
            m_emCheckSuccessPos = EM_TICKET_POSITION_OUTLET;
        }

        int iRet = MAG_Eject(szIn,szOut);
        if(0 == iRet)
            m_TextEdit->append("操作成功");
        else if(1 == iRet)
        {
            m_TextEdit->append("操作失败");
        }
        else if(0 != iRet)
        {
            m_TextEdit->append("操作失败");
            m_TextEdit->append(m_StateToStringMap[(EM_MAG_STATECODE)iRet]);
        }
    m_bCheckSuccess = false;
//    }
//    else
//        m_TextEdit->append("没有磁票");

}
//进票口灯操作
void COmronTre::on_clicked_LedControlBtn()
{
    if(!m_bTreInit)
    {
        m_TextEdit->append("连接失败或者初始化失败");
        return;
    }
    char szIn[10],szOut[10];
    if(m_GreenLed.isChecked())
    {
        szIn[0] = 0x31;
        if(m_Twinkle.isChecked())
            szIn[0] = 0x35;
    }
    else if(m_RedLed.isChecked())
    {
        szIn[0] = 0x33;
        if(m_Twinkle.isChecked())
            szIn[0] = 0x36;
    }

    MAG_LightControl(szIn,szOut);
}

void * COmronTre::TicketPOSThread(void *arg)
{
    COmronTre * pTreCommon = (COmronTre * )arg;
    char szIn[10] = {0};
    char szOut[3072] = {0};
    int iRet = -1,iRetLast = -1;
    EM_TICKET_POSITION emLastTicketPos = EM_TICKET_POSITION_NO;
    pTreCommon->m_emCheckSuccessPos = EM_TICKET_POSITION_INLET;
    while(pTreCommon->m_bThreadFlag)
    {
        if(pTreCommon->m_bTreInit)
        {
            Sleep(10);
            if(EM_TICKET_POSITION_INLET == pTreCommon->m_emCheckSuccessPos)        //检测入口
                szIn[0] = 0x31;
            else
                szIn[0] = 0x32;

            memset(szOut,0,sizeof(szOut));
            iRet = pTreCommon->CheckPos(szIn,szOut);

            if(0 == iRet)
            {
                if(0x30 == szOut[0] && EM_TICKET_POSITION_NO == pTreCommon->m_emTicketPos)//无票
                    continue;
                if(szIn[0] == 0x31 && 0x31 == szOut[0] && EM_TICKET_POSITION_NO == pTreCommon->m_emTicketPos)//有票插入
                {
                    emLastTicketPos = pTreCommon->m_emTicketPos;
                    pTreCommon->m_emTicketPos = EM_TICKET_POSITION_INLET;
                    pTreCommon->m_TextEdit->append("有票插入,读票");
                    pTreCommon->ReadData(szIn,szOut);
                }
                else if(szIn[0] == 0x32 && 0x31 == szOut[0] && EM_TICKET_POSITION_INLET == pTreCommon->m_emTicketPos)//出票口有票
                {
                    emLastTicketPos = pTreCommon->m_emTicketPos;
                    pTreCommon->m_emTicketPos = EM_TICKET_POSITION_OUTLET;
                    pTreCommon->m_TextEdit->append("出票口有票");
                    pTreCommon->m_emCheckSuccessPos = EM_TICKET_POSITION_OUTLET;
                    sem_post(&pTreCommon->m_Ticket_Take_Sem);
                    pTreCommon->m_CanRead = false;
                }
                else if(szIn[0] == 0x32 && 0x30 == szOut[0] && EM_TICKET_POSITION_OUTLET == pTreCommon->m_emTicketPos)//出票口票被取走
                {
                    emLastTicketPos = pTreCommon->m_emTicketPos;
                    pTreCommon->m_emTicketPos = EM_TICKET_POSITION_NO;
                    pTreCommon->m_TextEdit->append("出票口票被取走");
                    pTreCommon->m_emCheckSuccessPos = EM_TICKET_POSITION_INLET;
                    pTreCommon->m_CanRead = false;
                    szIn[0] = 0x31;
                    pTreCommon->LightControl(szIn,szOut);
                    szIn[0] = 0x35;
                    pTreCommon->LightControl(szIn,szOut);
                    qDebug() <<"出票口票被取走";
                    sem_post(&pTreCommon->m_Ticket_Take_Sem);
                }
                else if(szIn[0] == 0x31 && 0x31 == szOut[0] && EM_TICKET_POSITION_INLET == pTreCommon->m_emTicketPos)//入票口有票
                {
                    emLastTicketPos = pTreCommon->m_emTicketPos;
                    pTreCommon->m_emTicketPos = EM_TICKET_POSITION_INLET;
                    pTreCommon->m_emCheckSuccessPos = EM_TICKET_POSITION_INLET;
                    //pTreCommon->m_TextEdit->append("入票口有票");
                }
                else if(szIn[0] == 0x31 && 0x30 == szOut[0] && EM_TICKET_POSITION_INLET == emLastTicketPos)//入票口票被取走
                {
                    emLastTicketPos = pTreCommon->m_emTicketPos;
                    pTreCommon->m_emTicketPos = EM_TICKET_POSITION_NO;
                    pTreCommon->m_emCheckSuccessPos = EM_TICKET_POSITION_INLET;
                    pTreCommon->m_TextEdit->append("入票口票被取走");
                    qDebug() <<"入票口票被取走";
                    sem_post(&pTreCommon->m_Ticket_Take_Sem);
                    pTreCommon->m_CanRead = false;
                    szIn[0] = 0x31;
                    pTreCommon->LightControl(szIn,szOut);
                    szIn[0] = 0x35;
                    pTreCommon->LightControl(szIn,szOut);
                }
            }
            else
            {
                if(iRet != iRetLast)
                {
                    iRetLast = iRet;
                    pTreCommon->m_TextEdit->append(pTreCommon->m_StateToStringMap[(EM_MAG_STATECODE)iRet]);
                }
                if(EM_MAG_STATECODE_WRITE_MAG_FAIL == iRet)
                    pTreCommon->m_emCheckSuccessPos = EM_TICKET_POSITION_INLET;
                pTreCommon->m_CanRead = false;
                continue;
            }
        }
        Sleep(10);
    }
    pTreCommon->m_bThreadExitFlag = true;
    return NULL;
}
//Pos中转
int COmronTre::CheckPos(char *pIn, char *pOut)
{
    return MAG_CheckTicketPos(pIn,pOut);
}
int COmronTre::ReadData(char *pIn, char *pOut)
{
    int iRet = MAG_Read(pIn,pOut);
    if(0 == iRet)
    {
        m_bCheckSuccess = true;
        m_TextEdit->append("读票成功");
        qDebug() << "读票成功";
        memcpy(&m_ReadTicketInfo,pOut,(sizeof(T_EW_IPC_MAGCARD_TRACK_INFO)*3));
    }
    else
    {
        m_TextEdit->append("读票失败");
        qDebug() << "读票失败";
        m_bCheckSuccess = false;
    }

    sem_post(&m_Check_Sem);
    return iRet;
}
int COmronTre::Eject(char *pIn, char *pOut)
{
    if(0x31 == pIn[0])
        m_emCheckSuccessPos = EM_TICKET_POSITION_INLET;
    else
        m_emCheckSuccessPos = EM_TICKET_POSITION_OUTLET;
    return MAG_Eject(pIn,pOut);
}
int COmronTre::WritePrintEject(char *pIn, char *pOut)
{
    qDebug() <<"WritePrintEject";
    memset(&m_stWriteInfo,0,sizeof(m_stWriteInfo));
    memcpy(&m_stWriteInfo.tWriteMagInfo,&m_WriteData,sizeof(m_WriteData));

    if(pIn[0] == 0x30)
        m_stWriteInfo.iPrintFlagInfo = 0;
    else if(pIn[0] == 0x31)
        m_stWriteInfo.iPrintFlagInfo = 1;
    else
        m_stWriteInfo.iPrintFlagInfo = 2;
    char szIn[3072],szOut[10];
    szOut[0] = 125;
    memcpy(szIn,&m_stWriteInfo,sizeof(T_EW_WRITE_PRINT_MAGCARD_INFO));

    m_emCheckSuccessPos = EM_TICKET_POSITION_OUTLET;
    int iRet = MAG_Eject((char*)&m_stWriteInfo,szOut);
    if(iRet != 0)
        m_emCheckSuccessPos = EM_TICKET_POSITION_INLET;
    return iRet;
}
int COmronTre::LightControl(char *pIn, char *pOut)
{
    return MAG_LightControl(pIn,pOut);
}

void COmronTre::SetTextEdit(QTextEdit *qTextEd)
{
    m_TextEdit = qTextEd;
}

void COmronTre::TREShowInit(QWidget *parent)
{

    QString qButtonStyleSheet("QPushButton{background-color:rgb(85, 170, 255);color:white;border-radius:10px;border: 3px groove gray;\
                        border-style: outset;font-family:Arial;font-size:12px;}"
                       "QPushButton:hover{background-color:white; color: black;}"
                      "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style:inset;}");
    QString qLabelStyleSheet("background-color:rgb(205, 170, 125);color:white;border-radius:10px");

    //背景
    m_ButtonBackground = new QLabel(this);
    m_ButtonBackground->setGeometry(300,0,340,120);
    m_ButtonBackground->setText("磁模块");
    m_ButtonBackground->setAlignment(Qt::AlignBottom | Qt::AlignCenter);
    m_ButtonBackground->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:1 rgba(0, 0, 0, 50));\
                            border-radius:10px;color:rgba(0, 0, 0, 95);font-size:25px;");

    int iLeft = 300;
//    m_ComLabel.setParent(parent);
//    m_ComLabel.setGeometry(0,0,30,30);
//    m_ComLabel.setText("COM");
//    m_Com.setParent(parent);
//    m_Com.setGeometry(30,0,30,30);
//    m_Com.setText("1");
    m_Connect.setParent(parent);
    m_Connect.setGeometry(0+iLeft,0,80,30);
    m_Connect.setText("磁模块连接");
    m_Connect.setStyleSheet(qButtonStyleSheet);
//    m_TextEdit->setParent(parent);
//    m_TextEdit->setGeometry(240,0,400,480);

    m_PrintTip.setParent(parent);
    m_PrintTip.setGeometry(0+iLeft,30,100,30);
    m_PrintTip.setText("写票打印样式选择:");
    m_PrintTip.setAlignment(Qt::AlignCenter);
    m_PrintTip.setStyleSheet(qLabelStyleSheet);
    m_PrintGroup.setParent(parent);
    m_PrintGroup.addButton(&m_PrintSquare);
    m_PrintGroup.addButton(&m_PrintTriangle);
    m_PrintGroup.addButton(&m_PrintNothing);
    m_PrintSquare.setParent(parent);
    m_PrintSquare.setGeometry(120+iLeft,30,50,30);
    m_PrintSquare.setText("□");
    m_PrintSquare.setChecked(true);
    m_PrintTriangle.setParent(parent);
    m_PrintTriangle.setGeometry(160+iLeft,30,50,30);
    m_PrintTriangle.setText("△");
    m_PrintNothing.setParent(parent);
    m_PrintNothing.setGeometry(200+iLeft,30,80,30);
    m_PrintNothing.setText("不打印");


    m_LedControlBtn.setParent(parent);
    m_LedControlBtn.setGeometry(240+iLeft,0,100,30);
    m_LedControlBtn.setText("进票口灯设置");
    m_LedControlBtn.setStyleSheet(qButtonStyleSheet);
    m_InletLedGroup.setParent(parent);
    m_InletLedGroup.addButton(&m_RedLed);
    m_InletLedGroup.addButton(&m_GreenLed);
    m_RedLed.setParent(parent);
    m_RedLed.setGeometry(120+iLeft,0,40,30);
    m_RedLed.setText("红");
    m_GreenLed.setParent(parent);
    m_GreenLed.setGeometry(160+iLeft,0,40,30);
    m_GreenLed.setText("绿");
    m_Twinkle.setParent(parent);
    m_Twinkle.setGeometry(200+iLeft,0,40,30);
    m_Twinkle.setText("闪");

//    m_ReadTicketBtn.setParent(parent);
//    m_ReadTicketBtn.setGeometry(0+iLeft,60,60,30);
//    m_ReadTicketBtn.setText("读票");
    m_WriteTicketBtn.setParent(parent);
    m_WriteTicketBtn.setGeometry(280+iLeft,30,60,30);
    m_WriteTicketBtn.setText("写票");
    m_WriteTicketBtn.setStyleSheet(qButtonStyleSheet);

    m_EjectPosGroup.setParent(parent);
    m_EjectPosGroup.addButton(&m_EjectInlet);
    m_EjectPosGroup.addButton(&m_EjectOutlet);
    m_EjectInlet.setParent(parent);
    m_EjectInlet.setGeometry(120+iLeft,60,80,30);
    m_EjectInlet.setText("进票口");
    m_EjectInlet.setChecked(true);
    m_EjectOutlet.setParent(parent);
    m_EjectOutlet.setGeometry(200+iLeft,60,80,30);
    m_EjectOutlet.setText("出票口");
    m_EjectBtn.setParent(parent);
    m_EjectBtn.setGeometry(280+iLeft,60,60,30);
    m_EjectBtn.setText("退票");
    m_EjectBtn.setStyleSheet(qButtonStyleSheet);
}
