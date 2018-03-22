#include "ewreader.h"
#include <time.h>
#include <stdio.h>


char g_szName[3][10] = {{""},{"男"},{"女"}};
char g_szNation[][100] =
{ {""},{"汉"},{"蒙古"},{"回"},{"藏"},{"维吾尔"},{"苗"},{"彝"},{"壮"},{"布依"},{"朝鲜"},{"满"},{"侗"},{"瑶"},{"白"},{"土家"},{"哈尼"},
{"哈萨克"},{"傣"},{"黎"},{"傈僳"},{"佤"},{"畲"},{"高山"},{"拉祜"},{"水"},{"东乡"},{"纳西"},{"景颇"},{"柯尔克孜"},{"土"},{"达斡尔"},{"仫佬"},
{"羌"},{"布朗"},{"撒拉"},{"毛南"},{"仡佬"},{"锡伯"},{"阿昌"},{"普米"},{"塔吉克"},{"怒"},{"乌孜别克"},{"俄罗斯"},{"鄂温克"},{"德昂"},{"保安"},{"裕固"},
{"京"},{"塔塔尔"},{"独龙"},{"鄂伦春"},{"赫哲"},{"门巴"},{"珞巴"},{"基诺"}};

typedef struct _ST_ID_CARD_INFO_
{
    char szName[30];
    char szSex[2];
    char szNation[4];
    char szBirth[16];
    char szAddress[70];
    char szID[36];
    char szPliceStation[30];
    char szStartDate[16];
    char szEndDate[16];
    char szReseive[36];
    char szPictureData[1024];
}ST_ID_CARD_INFO;


EWReader::EWReader(QWidget *parent)
    : QWidget(parent)
{
    m_bInitFlag = false;
    m_bPolling = false;
    m_bEWReaderFlag = false;
    m_bEWReaderExitFlag = false;
    pthread_mutex_init(&m_EWReaderMutex,NULL);
    sem_init(&m_ReadCardSem,0,0);

}

EWReader::~EWReader()
{

}

bool EWReader::EWReaderInit(int iCom)
{
    m_iCom = iCom;
    char szIn[10],szOut[10];
    szIn[0] = iCom;
    READER_Init(szIn,szOut);

    if(!READER_SetBeep(szIn,szOut))
    {
        m_bInitFlag = false;
        m_TextEdit->append("读卡器连接失败");
        return false;
    }
    else
    {
        m_bInitFlag = true;
        m_TextEdit->append("读卡器连接成功");
    }

    if(!m_bEWReaderFlag)
    {
        m_bEWReaderFlag = true;
        int iRet = pthread_create(&m_ThreadId,NULL,EWReaderThread,(void*)this);
        if(iRet < 0)
        {
            m_bEWReaderFlag = false;
        }
        else
        {
            m_bEWReaderFlag = true;
            m_bPolling = true;
        }
    }
    return true;
}

void * EWReader::EWReaderThread(void *arg)
{
    EWReader* pEWReader = (EWReader*)arg;
    char szIn[10],szOut[2048] = {0};
    char szLastCardId[100] = {0};
    time_t tT1,tT2;
    while(pEWReader->m_bEWReaderFlag)
    {
//        usleep(1000 * 1000);
        if(pEWReader->m_bPolling)
        {
            memset(szOut,0,sizeof(szOut));
            if(pEWReader->Polling(szIn,szOut))
            {
                if(0 != strcmp(szLastCardId,szOut))
                {
                    strcpy(szLastCardId,szOut);
                    time(&tT1);
                    sem_post(&pEWReader->m_ReadCardSem);
                    qDebug() << "read card success";
                }
                else
                {
                    time(&tT2);
                    if((tT2 - tT1) > 5)
                    {
                        qDebug() << "the same can use now"<<tT2<<tT1;
                        tT1 = tT2;
                        sem_post(&pEWReader->m_ReadCardSem);
                    }
                }
            }
        }
    }
    pEWReader->m_bEWReaderExitFlag = true;
    return NULL;
}
bool EWReader::Polling(char *pIn, char *pOut)
{
    return (bool)READER_ReadId(pIn,pOut);
}

void EWReader::SetTextEdit(QTextEdit *pTextEdit)
{
    m_TextEdit = pTextEdit;
}

void EWReader::on_clicked_ConnectBtn()
{
    if(m_bInitFlag)
    {
        m_TextEdit->append("已连接");
        return;
    }
    EWReaderInit(2);
}

void EWReader::on_clicked_ResetBtn()
{
    if(!m_bInitFlag)
    {
        m_TextEdit->append("读卡器未连接/未初始化");
        return;
    }
    char szIn[10],szOut[10];
    if(!READER_Reset(szIn,szOut))
    {
        m_TextEdit->append("Reset failed");
        return;
    }
    else
    {
        m_TextEdit->append("Reset success");
        m_bInitFlag = true;
    }
}
void EWReader::on_clicked_BeepBtn()
{
    if(!m_bInitFlag)
    {
        m_TextEdit->append("读卡器未连接/未初始化");
        return;
    }
    char szIn[10],szOut[10];
    if(!READER_SetBeep(szIn,szOut))
    {
        m_TextEdit->append("SetBeep failed");
        return;
    }
    else
    {
        m_TextEdit->append("SetBeep success");
    }
}

void EWReader::on_clicked_ReadIDBtn()
{
    if(!m_bInitFlag)
    {
        m_TextEdit->append("读卡器未连接/未初始化");
        return;
    }
    char szIn[10],szOut[1024];
    if(!READER_ReadId(szIn,szOut))
    {
        m_TextEdit->append("ReadID failed");
        return;
    }
    else
    {
        m_TextEdit->append("ReadID success");
        m_TextEdit->append("ID:" + QString(szOut));
        sem_post(&m_ReadCardSem);
    }
}
void EWReader::on_clicked_ReadAllBtn()
{
    if(!m_bInitFlag)
    {
        m_TextEdit->append("未连接/未初始化");
        return;
    }
    char szIn[10],szOut[2048];
    if(!READER_ReadAllInfo(szIn,szOut))
    {
        m_TextEdit->append("ReadAll failed");
        return;
    }
    else
    {
        m_TextEdit->append("ReadAll success");
    }
    QString info = info.fromLocal8Bit(szOut);
    m_TextEdit->append("名字：" + info.section(",",0,0));
    m_TextEdit->append("性别：" + info.section(",",1,1));
    m_TextEdit->append("民族：" + info.section(",",2,2));
    m_TextEdit->append("生日：" + info.section(",",3,3));
    m_TextEdit->append("地址：" + info.section(",",4,4));
    m_TextEdit->append("身份证号：" + info.section(",",5,5));
    m_TextEdit->append("办理部门：" + info.section(",",6,6));
    m_TextEdit->append("办理日期：" + info.section(",",7,7));
    m_TextEdit->append("有效期：" + info.section(",",8,8));
}
void EWReader::EWReaderShowInit()
{
    QString qButtonStyleSheet("QPushButton{background-color:rgb(85, 170, 255);color:white;border-radius:10px;border: 3px groove gray;\
                        border-style: outset;font-family:Arial;font-size:12px;}"
                       "QPushButton:hover{background-color:white; color: black;}"
                      "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style:inset;}");

    int iLeft = 300, iTop = 150;


    m_ReaderBg.setParent(this);
    m_ReaderBg.setText("读卡器");
    m_ReaderBg.setGeometry(iLeft,iTop,340,60);
    m_ReaderBg.setAlignment(Qt::AlignTop|Qt::AlignLeft);
    m_ReaderBg.setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:1 rgba(0, 0, 0, 50));\
                             border-radius:10px;color:rgba(0, 0, 0, 95);font-size:25px;");
    iLeft+=140;
    m_ConnectBtn.setParent(this);
    m_ConnectBtn.setText("连接");
    m_ConnectBtn.setGeometry(iLeft,iTop,100,30);
    m_ConnectBtn.setStyleSheet(qButtonStyleSheet);
    m_ResetBtn.setParent(this);
    m_ResetBtn.setText("重启");
    m_ResetBtn.setGeometry(iLeft+100,iTop,100,30);
    m_ResetBtn.setStyleSheet(qButtonStyleSheet);
    m_BeepBtn.setParent(this);
    m_BeepBtn.setText("蜂鸣器");
    m_BeepBtn.setGeometry(iLeft -100,30+iTop,100,30);
    m_BeepBtn.setStyleSheet(qButtonStyleSheet);
    m_ReadIDBtn.setParent(this);
    m_ReadIDBtn.setText("读身份证");
    m_ReadIDBtn.setGeometry(iLeft,30+iTop,100,30);
    m_ReadIDBtn.setStyleSheet(qButtonStyleSheet);
    m_ReadAllBtn.setParent(this);
    m_ReadAllBtn.setText("读详细信息");
    m_ReadAllBtn.setGeometry(iLeft+100,30+iTop,100,30);
    m_ReadAllBtn.setStyleSheet(qButtonStyleSheet);


    connect(&m_ConnectBtn,SIGNAL(clicked()),this,SLOT(on_clicked_ConnectBtn()));
    connect(&m_ResetBtn,SIGNAL(clicked()),this,SLOT(on_clicked_ResetBtn()));
    connect(&m_BeepBtn,SIGNAL(clicked()),this,SLOT(on_clicked_BeepBtn()));
    connect(&m_ReadIDBtn,SIGNAL(clicked()),this,SLOT(on_clicked_ReadIDBtn()));
    connect(&m_ReadAllBtn,SIGNAL(clicked()),this,SLOT(on_clicked_ReadAllBtn()));
}
