#include "uimanager.h"
#include <QProcess>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QFile>
#include <QTextStream>

#include "PSEMRSIV_GAT.h"
CUimanager *CUimanager::m_CUIManager = NULL;

CUimanager::CUimanager()
{
  //隐藏标题栏
  //setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowTitleHint);

  //隐藏鼠标
  //setCursor(Qt::BlankCursor);

    setWindowFlags(Qt::FramelessWindowHint |Qt::WindowStaysOnTopHint);

    m_bMaintainMode = false;
    m_pStackedWidget = new QStackedWidget(this);
    m_bStateMachineFlag = false;
    m_bStateMachineExit = false;
    m_bSystemInfo = false;
    m_bGetSysInfo = false;
    m_bCanServer = false;
    pthread_mutex_init(&m_StateThreadMutex,NULL);

    m_CurrentStatePage = EM_PAGE_NAME_INIT;

    this->addWidget(m_pStackedWidget);
    m_pPage0 = new CPage0();
    m_pPage1 = new CPage1();
    m_pPage2 = new CPage2();
    m_pPage3 = new CPage3();
    m_pPage4 = new CPage4();
    m_pPage5 = new CPage5();
    m_pPage6 = new CPage6();
    m_pPage7 = new CPage7();
    m_pPage8 = new CPage8();

    m_pStackedWidget->addWidget(m_pPage0);
    m_pStackedWidget->addWidget(m_pPage1);
    m_pStackedWidget->addWidget(m_pPage2);
    m_pStackedWidget->addWidget(m_pPage3);
    m_pStackedWidget->addWidget(m_pPage4);
    m_pStackedWidget->addWidget(m_pPage5);
    m_pStackedWidget->addWidget(m_pPage6);
    m_pStackedWidget->addWidget(m_pPage7);
    m_pStackedWidget->addWidget(m_pPage8);

    m_pGate = new Gunnebo(m_pPage2);
    m_pOmronTre = new COmronTre(m_pPage2);
    m_pPlayer = new CPlayer(m_pPage2);
    m_Reader = new EWReader(m_pPage2);

    m_TextEdit.setParent(m_pPage2);
    m_TextEdit.setGeometry(300,280,340,200);
    m_TextEdit.setStyleSheet("background-color:rgba(85, 170, 255,50);border-radius:10px;font-size:15px;font:bold");
    m_pOmronTre->SetTextEdit(&m_TextEdit);
    m_pOmronTre->TREShowInit(m_pPage2);
    m_pPlayer->SetTextEdit(&m_TextEdit);
    m_pPlayer->PlayerShowInit(m_pPage2);
    m_pPlayer->PlayerInit();
    m_Reader->SetTextEdit(&m_TextEdit);
    m_Reader->EWReaderShowInit();
    m_pGate->SetTextEdit(&m_TextEdit);
    m_pGate->GunneboShowInit(m_pPage2);


    QString qButtonStyleSheet("QPushButton{background-color:rgb(85, 170, 255);color:white;border-radius:10px;border: 3px groove gray;\
                        border-style: outset;font-family:Arial;font-size:12px;}"
                       "QPushButton:hover{background-color:white; color: black;}"
                      "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style:inset;}");
//    QString qLabelStyleSheet("background-color:rgb(205, 170, 125);color:white;border-radius:10px");


    m_PlayBtn.setStyleSheet(qButtonStyleSheet);
    m_MaintainCKBox.setParent(m_pPage2);
    m_MaintainCKBox.setGeometry(0,0,100,30);
    m_MaintainCKBox.setText("维修模式");
    m_ExitMaitainBtn.setParent(m_pPage2);
    m_ExitMaitainBtn.setGeometry(80,0,100,30);
    m_ExitMaitainBtn.setText("退出维修");
    m_ExitMaitainBtn.setStyleSheet(qButtonStyleSheet);
    m_ServerToMaintainBtn.setParent(m_pPage1);
    m_ServerToMaintainBtn.setGeometry(540,450,100,30);
    m_ServerToMaintainBtn.setText("进入维修");
    m_ServerToMaintainBtn.setStyleSheet(qButtonStyleSheet);
    m_OutOfServerToMaintainBtn.setParent(m_pPage6);
    m_OutOfServerToMaintainBtn.setGeometry(540,450,100,30);
    m_OutOfServerToMaintainBtn.setText("进入维修");
    m_OutOfServerToMaintainBtn.setStyleSheet(qButtonStyleSheet);

    m_GetSysInfoBtn.setParent(m_pPage2);
    m_GetSysInfoBtn.setGeometry(180,0,100,30);
    m_GetSysInfoBtn.setText("获取系统信息");
    m_GetSysInfoBtn.setStyleSheet(qButtonStyleSheet);
    m_GetSysInfoBtn.hide();

    m_GetGateInfoBtn.setParent(m_pPage2);
    m_GetGateInfoBtn.setGeometry(180,0,100,30);
    m_GetGateInfoBtn.setText("获取门信息");
    m_GetGateInfoBtn.setStyleSheet(qButtonStyleSheet);

    m_Exit.setParent(m_pPage1);
    m_Exit.setGeometry(440,450,100,30);
    m_Exit.setText("退出程序");
    m_Exit.setStyleSheet(qButtonStyleSheet);
    m_Exit1.setParent(m_pPage2);
    m_Exit1.setGeometry(520,250,60,30);
    m_Exit1.setText("退出程序");
    m_Exit1.setStyleSheet(qButtonStyleSheet);
    m_Exit2.setParent(m_pPage6);
    m_Exit2.setGeometry(440,450,100,30);
    m_Exit2.setText("退出程序");
    m_Exit2.setStyleSheet(qButtonStyleSheet);

    connect(this,SIGNAL(SetPage(int)),this,SLOT(SwitchPage(int)));
    connect(&m_MaintainCKBox,SIGNAL(stateChanged(int)),this,SLOT(on_checked_MaintainCKBox()));
    connect(&m_ExitMaitainBtn,SIGNAL(clicked()),this,SLOT(on_clicked_ExitMaitainBtn()));
    connect(&m_ServerToMaintainBtn,SIGNAL(clicked()),this,SLOT(on_clicked_IntoMaintainnBtn()));
    connect(&m_OutOfServerToMaintainBtn,SIGNAL(clicked()),this,SLOT(on_clicked_IntoMaintainnBtn()));
    connect(&m_Exit,SIGNAL(clicked()),this,SLOT(On_clicked_ExitBtn()));
    connect(&m_Exit1,SIGNAL(clicked()),this,SLOT(On_clicked_ExitBtn()));
    connect(&m_Exit2,SIGNAL(clicked()),this,SLOT(On_clicked_ExitBtn()));
    connect(&m_GetSysInfoBtn,SIGNAL(clicked()),this,SLOT(on_clicked_GetSysInfoBtn()));
    connect(&m_GetGateInfoBtn,SIGNAL(clicked()),this,SLOT(on_clicked_GetGateInfoBtn()));

    this->setGeometry(0,0,m_pPage0->width()/2,m_pPage0->height());


    StateManager();

    if(!m_bStateMachineFlag)
    {
        m_bStateMachineFlag = true;
        int iRet = pthread_create(&m_StateThreadId,NULL,StateMachineThread,(void*)this);
        if(0 != iRet)
        {
            m_bStateMachineFlag = false;
            qDebug() << "UI Thread failed";
        }
        else
        {
            m_bStateMachineFlag = true;
            qDebug() << "UI Create success" << m_bStateMachineFlag;
        }
    }
    if(!m_bSystemInfo)
    {
        m_bSystemInfo = true;
        int iRet = pthread_create(&m_SystemInfoThreadId,NULL,GetSystemInfoThread,(void*)this);
        if(0 != iRet)
        {
            m_bSystemInfo = false;
        }
        else
        {
            m_bSystemInfo = true;
        }
    }
}
void CUimanager::SwitchPage(int iPage)
{
    m_pStackedWidget->setCurrentIndex((EM_PAGE_NAME)iPage);
}

void CUimanager::StateManager()
{
    char szResult[256];
    m_bCanServer = false;
    if(!m_pGate->m_bInitFlag)//门 m_pPage0->m_pData->m_GateCom
    {
        m_pGate->GateInit( m_pPage0->m_pData->m_GateCom,szResult);
        if(!m_pGate->m_bInitFlag)
        {
            m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_OUTOFSERVER);
            m_pPage6->findChild<QLabel*>("LblGate")->setText(m_pGate->m_GateErrorMap[(EM_GATE_ERROR_CODE)atoi(szResult)]);
            m_CurrentStatePage = EM_PAGE_NAME_OUTOFSERVER;
        }
    }
    if(!m_pOmronTre->m_bTreInit)//磁模块 m_pPage0->m_pData->m_TreCom  m_pPage0->m_pData->m_IDCardCom
    {
        m_pOmronTre->TREInit(m_pPage0->m_pData->m_TreCom,szResult);
        if(!m_pOmronTre->m_bTreInit)
        {
            m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_OUTOFSERVER);
            m_pPage6->findChild<QLabel*>("LblTre")->setText(m_pOmronTre->m_StateToStringMap[(EM_MAG_STATECODE)atoi(szResult)]);
            m_CurrentStatePage = EM_PAGE_NAME_OUTOFSERVER;
        }
    }
    if(!m_Reader->m_bInitFlag)
    {
        if(!m_Reader->EWReaderInit(m_pPage0->m_pData->m_IDCardCom))
        {
            m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_OUTOFSERVER);
            m_pPage6->findChild<QLabel*>("LblReader")->setText("读卡器连接失败");
            m_CurrentStatePage = EM_PAGE_NAME_OUTOFSERVER;
        }
    }
    if(m_pGate->m_bInitFlag && m_pOmronTre->m_bTreInit && m_Reader->m_bInitFlag)
    {
        m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_SERVER);
        m_CurrentStatePage = EM_PAGE_NAME_SERVER;
        m_bCanServer = true;
    }
}

void CUimanager::on_checked_MaintainCKBox()
{
    if(m_MaintainCKBox.isChecked())
    {
        m_bMaintainMode = true;
    }
    else
        m_bMaintainMode = false;
}
//退出维修
void CUimanager::on_clicked_ExitMaitainBtn()
{
    m_MaintainCKBox.setChecked(false);
    m_bMaintainMode = false;
    m_Reader->m_bPolling = true;
    m_pStackedWidget->setCurrentIndex(m_CurrentStatePage);
}
//进入维修
void CUimanager::on_clicked_IntoMaintainnBtn()
{
    m_MaintainCKBox.setChecked(true);
    m_bMaintainMode = true;
    m_Reader->m_bPolling = false;
    m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_MAINTAIN);
}

//退出程序
void CUimanager::On_clicked_ExitBtn()
{
    this->close();
}

//获取门信息
void CUimanager::on_clicked_GetGateInfoBtn()
{
    m_pGate->m_bStop = true;
    m_pGate->GateUninit();

    char szResult[256];
    ModuleInfo stModuleInfo;
    memset(stModuleInfo,0,sizeof(ModuleInfo));
    if(GAT_GetModuleInfo(&stModuleInfo) < 1)
        m_TextEdit.append("获取门模块信息失败");
    else
    {
        m_TextEdit.append("Brand       :" + QString(stModuleInfo[0].Brand));
        m_TextEdit.append("Model       :" + QString(stModuleInfo[0].Model));
        m_TextEdit.append("ID          :" + QString(stModuleInfo[0].ID));
        m_TextEdit.append("Version     :" + QString(stModuleInfo[0].Version));
        m_TextEdit.append("VerBuildTime:" + QString(stModuleInfo[0].VerBuildTime));
    }
    m_pGate->GateInit( m_pPage0->m_pData->m_GateCom,szResult);
    m_pGate->m_bStop = false;
}
//获取系统信息
void CUimanager::on_clicked_GetSysInfoBtn()
{
    if(!m_bGetSysInfo)
        m_TextEdit.append("正在读取系统信息");
    else
    {
        m_TextEdit.append("IP:" + m_stSystemInfo.IP);
        m_TextEdit.append("MAC:" + m_stSystemInfo.MAC);
        m_TextEdit.append("网关:" + m_stSystemInfo.Gateway);
        m_TextEdit.append("系统名称:" + m_stSystemInfo.HostName);
        m_TextEdit.append("OS 名称:" + m_stSystemInfo.OSName);
        m_TextEdit.append("OS 版本:" + m_stSystemInfo.OSVersion);
        m_TextEdit.append("系统制造商:" + m_stSystemInfo.OSMaker);
        m_TextEdit.append("系统型号:" + m_stSystemInfo.OSModel);
        m_TextEdit.append("系统类型:" + m_stSystemInfo.OSType);
        m_TextEdit.append("物理内存总量:" + m_stSystemInfo.TotalMemory);
        m_TextEdit.append("可用的物理内存:" + m_stSystemInfo.AvaluableMemory);
        m_TextEdit.append("主板序列号:" + m_stSystemInfo.BaseBoardSerialNum);
        m_TextEdit.append("bios序列号:" + m_stSystemInfo.BIOSSerialNum);
        m_TextEdit.append("CPU序列号:" + m_stSystemInfo.CPUProcessorId);
        m_TextEdit.append("硬盘序列号:" + m_stSystemInfo.DiskDriveModel);
    }
}

void CUimanager::GetSystemInfo()
{
    m_bGetSysInfo = false;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();//IP
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
       {
           if (address.toString().contains("127.0."))
           {
               continue;
           }
            qDebug() <<address.toString();

            m_stSystemInfo.IP = address.toString();
//            break;
       }
    }
    //获取所有网络接口的列表
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {

        //MAC地址
        qDebug() << "HardwareAddress:" << netInterface.hardwareAddress();
        m_stSystemInfo.MAC = netInterface.hardwareAddress();


        break;
//        QList<QNetworkAddressEntry> entryList = netInterface.addressEntries();

//        //遍历每一个IP地址(每个包含一个IP地址，一个子网掩码和一个广播地址)
//        foreach(QNetworkAddressEntry entry, entryList)
//        {
//            //IP地址
//            qDebug() << "IP Address:" << entry.ip().toString();
//            m_TextEdit.append("IP Address:" + entry.ip().toString());

//            //子网掩码
//            qDebug() << "Netmask:" << entry.netmask().toString();
//            m_TextEdit.append("Netmask:" + entry.netmask().toString());

//            //广播地址
//            qDebug() << "Broadcast:" << entry.broadcast().toString();
//            m_TextEdit.append("Broadcast:" + entry.broadcast().toString());
//        }
    }
    QProcess cmd_pro ;
    QString cmd_str;
    QString result;
    cmd_str = QString("route print");
    cmd_pro.start("cmd.exe", QStringList() << "/c" << cmd_str);
    cmd_pro.waitForStarted();
    cmd_pro.waitForFinished();

    result = cmd_pro.readAll();
    QString pattern("0\.0\.0\.0 *(0|128)\.0\.0\.0 *([0-9\.]*)");
    QRegExp rx(pattern);
    int pos = result.indexOf(rx);
    if (pos >= 0)
    {
        qDebug()<<"reg::"<<rx.cap(0)<<rx.cap(1)<<rx.cap(2);           //网关

        m_stSystemInfo.Gateway = rx.cap(2);
    }

    QFile file("systeminformation.txt");
    if(!file.open(QFile::ReadWrite | QFile::Text))
    {
        qDebug() << "Open systeminformation.txt failed";
        return;
    }

    result.clear();
    cmd_str.clear();
    cmd_str = QString("systeminfo");
    cmd_pro.start("cmd.exe", QStringList() << "/c" << cmd_str);
    cmd_pro.waitForStarted();
    cmd_pro.waitForFinished();
    result = result.fromLocal8Bit(cmd_pro.readAll());
//    qDebug() <<result;
    file.write(result.toStdString().c_str(),strlen(result.toStdString().c_str()));
    QString qHostName,qOSName,qOSVersion,qSystemFacture,qOSModel,qOSType,qTotalMemory,qAvaluableMemory;
    int iIndex = 1;
    QString string;
    do
    {
        string = result.section("\n",iIndex,iIndex);
        if(string.contains("主机名"))
        {
            qHostName = string.remove("主机名:").trimmed();
            qDebug() <<"系统名称:"<<qHostName;

            m_stSystemInfo.HostName = qHostName;
        }
        else if(string.contains("BIOS 版本"))
        {
            iIndex++;
            continue;
        }
        else if(string.contains("OS 名称"))
        {
            qOSName = string.remove("OS 名称:").trimmed();
            qDebug() <<qOSName;

            m_stSystemInfo.OSName = qOSName;
        }
        else if(string.contains("OS 版本"))
        {
            qOSVersion = string.remove("OS 版本:").trimmed();
            qDebug() <<qOSVersion;

            m_stSystemInfo.OSVersion = qOSVersion;
        }
        else if(string.contains("系统制造商"))
        {
            qSystemFacture = string.remove("系统制造商:").trimmed();
            qDebug() <<qSystemFacture;

            m_stSystemInfo.OSMaker = qSystemFacture;
        }
        else if(string.contains("系统型号"))
        {
            qOSModel = string.remove("系统型号:").trimmed();
            qDebug() <<qOSModel;

            m_stSystemInfo.OSModel = qOSModel;
        }
        else if(string.contains("系统类型"))
        {
            qOSType = string.remove("系统类型:").trimmed();
            qDebug() <<qOSType;

            m_stSystemInfo.OSType = qOSType;
        }
        else if(string.contains("物理内存总量"))
        {
            qTotalMemory = string.remove("物理内存总量:").trimmed();
            qDebug() <<qTotalMemory;

            m_stSystemInfo.TotalMemory = qTotalMemory;
        }
        else if(string.contains("可用的物理内存"))
        {
            qAvaluableMemory = string.remove("可用的物理内存:").trimmed();
            qDebug() <<qAvaluableMemory;

            m_stSystemInfo.AvaluableMemory = qAvaluableMemory;
        }


        iIndex++;
    }while(string.length() != 0);

    cmd_str.clear();
    result.clear();
    cmd_str = QString("wmic baseboard get serialnumber");
    cmd_pro.start("cmd.exe", QStringList() << "/c" << cmd_str);
    cmd_pro.waitForStarted(5000);
    cmd_pro.waitForFinished(5000);
    result = QString(cmd_pro.readAll()).remove("SerialNumber").trimmed();//主板序列号
    qDebug() <<result;
    m_stSystemInfo.BaseBoardSerialNum = result;
    file.write(result.toStdString().c_str(),result.length());
    file.write("\n",1);


    cmd_str.clear();
    result.clear();
    cmd_str = QString("wmic bios get serialnumber");
    cmd_pro.start("cmd.exe", QStringList() << "/c" << cmd_str);
    cmd_pro.waitForStarted(5000);
    cmd_pro.waitForFinished(5000);
    result = QString(cmd_pro.readAll()).remove("SerialNumber").trimmed();//bios序列号
    qDebug() <<result;
    m_stSystemInfo.BIOSSerialNum = result;
    file.write(result.toStdString().c_str(),result.length());
    file.write("\n",1);


    cmd_str.clear();
    result.clear();
    cmd_str = QString("wmic CPU get ProcessorID");
    cmd_pro.start("cmd.exe", QStringList() << "/c" << cmd_str);
    cmd_pro.waitForStarted(5000);
    cmd_pro.waitForFinished(5000);
    result = QString(cmd_pro.readAll()).remove("ProcessorId").trimmed();//CPU序列号
    qDebug() <<result;
    m_stSystemInfo.CPUProcessorId = result;
    file.write(result.toStdString().c_str(),result.length());
    file.write("\n",1);


    cmd_str.clear();
    result.clear();
    cmd_str = QString("wmic diskdrive get Model");
    cmd_pro.start("cmd.exe", QStringList() << "/c" << cmd_str);
    cmd_pro.waitForStarted(5000);
    cmd_pro.waitForFinished(5000);
    result = QString(cmd_pro.readAll()).remove("Model").trimmed();//硬盘序列号（硬盘名称）
    qDebug() <<result;
    m_stSystemInfo.DiskDriveModel = result;
    file.write(result.toStdString().c_str(),result.length());


    file.close();
    m_bGetSysInfo = true;
}

void *CUimanager::GetSystemInfoThread(void *arg)
{
    CUimanager *pUIManager = (CUimanager*) arg;
    pUIManager->GetSystemInfo();
    while(pUIManager->m_bSystemInfo)
    {
        Sleep(100);
    }
    return NULL;
}

void * CUimanager::StateMachineThread(void *arg)
{
    CUimanager *pUIManager = (CUimanager*) arg;
    EM_GAT_STACODE emLastState = EM_GAT_STACODE_FAULT;
    char szIn[256] = {0},szOut[3072] = {0};
    qDebug() << pUIManager->m_bStateMachineFlag;
    while(pUIManager->m_bStateMachineFlag)
    {
        if(!pUIManager->m_bMaintainMode && pUIManager->m_bCanServer)
        {
            //正常服务
            if(EM_GAT_STACODE_FAULT_NORMAL == pUIManager->m_pGate->m_GateState && emLastState != pUIManager->m_pGate->m_GateState)
            {
                emLastState = pUIManager->m_pGate->m_GateState;
                emit pUIManager->SetPage(EM_PAGE_NAME_SERVER);
            }
            //维修
            if(EM_GAT_STACODE_MAINTAINCE_STATE == pUIManager->m_pGate->m_GateState && emLastState != pUIManager->m_pGate->m_GateState)
            {
                emLastState = pUIManager->m_pGate->m_GateState;
                emit pUIManager->SetPage(EM_PAGE_NAME_MAINTAIN);
            }
            //进站闯闸
            if(EM_GAT_STACODE_ENTRY_INTRUSION == pUIManager->m_pGate->m_GateState && emLastState != pUIManager->m_pGate->m_GateState)
            {
                emLastState = pUIManager->m_pGate->m_GateState;
                emit pUIManager->SetPage(EM_PAGE_NAME_ENTRYINTRUDED);
            }
            //出站闯闸
            if(EM_GAT_STACODE_EXIT_INTRUSION == pUIManager->m_pGate->m_GateState && emLastState != pUIManager->m_pGate->m_GateState)
            {
                emLastState = pUIManager->m_pGate->m_GateState;
                emit pUIManager->SetPage(EM_PAGE_NAME_EXITINTRUDED);
            }
            //暂停服务
            if((EM_GAT_STACODE_AISLE_UNINIT == pUIManager->m_pGate->m_GateState ||
                EM_GAT_STACODE_FAULT == pUIManager->m_pGate->m_GateState )&& emLastState != pUIManager->m_pGate->m_GateState)
            {
                emLastState = pUIManager->m_pGate->m_GateState;
                emit pUIManager->SetPage(EM_PAGE_NAME_OUTOFSERVER);
            }
            if(!sem_trywait(&pUIManager->m_pOmronTre->m_Check_Sem))
            {
                qDebug() << "读票了";
                pUIManager->m_TextEdit.append("读票了");
                emLastState = pUIManager->m_pGate->m_GateState;
                if(pUIManager->m_pOmronTre->m_bCheckSuccess)
                {
                    emit pUIManager->SetPage(EM_PAGE_NAME_CHECKOK);
                    pUIManager->m_pGate->m_GateState = EM_GAT_STACODE_AISLE_OPEN_ENTRY;
                    szIn[0] = 0x32;
                    if(0 == pUIManager->m_pOmronTre->WritePrintEject(szIn,szOut))//写票
                    {
                        pUIManager->m_pGate->OpenDoor(false);       //授权
                        szIn[0] = 0x30;
                        szIn[1] = 0x31;
                        szIn[2] = 0x32;
                        pUIManager->m_pGate->DIOCtrl(szIn) ;            //亮灯
                    }
                    else
                    {
                        emit pUIManager->SetPage(EM_PAGE_NAME_SERVER);
                        pUIManager->m_pGate->m_GateState = EM_GAT_STACODE_FAULT_NORMAL;
                    }

                }
                else
                {
                    emit pUIManager->SetPage(EM_PAGE_NAME_CHECKFAIL);
                    pUIManager->m_pGate->m_GateState = EM_GAT_STACODE_AISLE_OPEN_ENTRY;
                    szIn[0] = 0x31;
                    pUIManager->m_pOmronTre->Eject(szIn,szOut);//退票
                    szIn[0] = 0x30;
                    szIn[1] = 0x33;
                    szIn[2] = 0x32;
                    pUIManager->m_pGate->DIOCtrl(szIn) ;            //亮灯
                }
            }
            //请通过
            if(!sem_trywait(&pUIManager->m_pOmronTre->m_Ticket_Take_Sem))
            {
                emLastState = pUIManager->m_pGate->m_GateState;
                if(EM_TICKET_POSITION_OUTLET == pUIManager->m_pOmronTre->m_emTicketPos)
                {
                    emit pUIManager->SetPage(EM_PAGE_NAME_TAKETICKET);
                }
                else if(EM_TICKET_POSITION_NO == pUIManager->m_pOmronTre->m_emTicketPos)
                {
//                    szIn[0] = 0x35;
//                    pUIManager->m_pOmronTre->LightControl(szIn,szOut);
                    pUIManager->m_pGate->OpenDoor(true);            //开门
                    qDebug() << "开门";
                    szIn[0] = 0x30;
                    szIn[1] = 0x30;
                    szIn[2] = 0x32;
                    pUIManager->m_pGate->DIOCtrl(szIn) ;            //消警告
                    emit pUIManager->SetPage(EM_PAGE_NAME_SERVER);
                }
            }
            if(!sem_trywait(&pUIManager->m_Reader->m_ReadCardSem))
            {
                qDebug() << "读到卡";
                emit pUIManager->SetPage(EM_PAGE_NAME_CHECKOK);
                pUIManager->m_pGate->OpenDoor(false);
                pUIManager->m_Reader->m_bPolling = false;
                Sleep(50);
                pUIManager->m_pGate->OpenDoor(true);
                szIn[0] = 0x30;
                szIn[1] = 0x34;
                szIn[2] = 0x32;
                pUIManager->m_pGate->DIOCtrl(szIn) ;            //亮灯
                Sleep(1000);
                pUIManager->m_Reader->m_bPolling = true;
                szIn[0] = 0x30;
                szIn[1] = 0x30;
                szIn[2] = 0x32;
                pUIManager->m_pGate->DIOCtrl(szIn) ;            //消警告
                emit pUIManager->SetPage(EM_PAGE_NAME_SERVER);
            }

        }
        if(pUIManager->m_pGate->m_bInitFlag && pUIManager->m_pOmronTre->m_bTreInit && pUIManager->m_Reader->m_bInitFlag)
        {
            pUIManager->m_CurrentStatePage = EM_PAGE_NAME_SERVER;
            pUIManager->m_bCanServer = true;
        }
            else
        {
            pUIManager->m_CurrentStatePage = EM_PAGE_NAME_OUTOFSERVER;
            pUIManager->m_bCanServer = false;
        }



        Sleep(10);
    }
    pUIManager->m_bStateMachineExit = true;
    return NULL;
}

CUimanager::~CUimanager()
{
    delete m_pPage0;
    delete m_pPage1;
    delete m_pPage2;
    delete m_pPage3;
    delete m_pPage4;
    delete m_pPage5;
    delete m_pPage6;
    delete m_pPage7;
    delete m_pPage8;
    delete m_pStackedWidget;
    delete m_pGate;

    if(m_bSystemInfo)
    {
        m_bSystemInfo = false;
        void * Thread_exit;
        pthread_join(m_SystemInfoThreadId,&Thread_exit);
    }

    if(m_bStateMachineFlag)
    {
        m_bStateMachineFlag = false;
        while(!m_bStateMachineExit)
            Sleep(50);
        void * Thread_exit;
        pthread_join(m_StateThreadId,&Thread_exit);
        pthread_mutex_destroy(&m_StateThreadMutex);
    }
    qDebug() << "exit";
}

CUimanager * CUimanager::GetInstance()
{
    if(NULL == m_CUIManager)
    {
        m_CUIManager = new CUimanager();
    }
    return m_CUIManager;
}
void CUimanager::keyPressEvent(QKeyEvent *e)
{
  switch (e->key()) {
    case Qt::Key_0:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_INIT);
      break;
    case Qt::Key_1:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_SERVER);
      break;
    case Qt::Key_2:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_MAINTAIN);
      break;
    case Qt::Key_3:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_CHECKOK);
      break;
    case Qt::Key_4:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_TAKETICKET);
      break;
    case Qt::Key_5:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_CHECKFAIL);
      break;
    case Qt::Key_6:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_OUTOFSERVER);
      break;
    case Qt::Key_7:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_ENTRYINTRUDED);
      break;
    case Qt::Key_8:
      m_pStackedWidget->setCurrentIndex(EM_PAGE_NAME_EXITINTRUDED);
      break;
    default:
      break;
    }
}
