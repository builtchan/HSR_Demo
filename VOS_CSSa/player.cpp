#include "VOS_CSSa/player.h"
#include <QDebug>
CPlayer *CPlayer::m_Player = NULL;
CPlayer::CPlayer(QWidget *parent):QWidget(parent)
{
    m_ThreadFlag = false;
    m_ThreadExitFlag = false;
    m_bNoStopPlay = false;
    pthread_mutex_init(&m_ThreadMutex,NULL);
    m_TextEdit = NULL;
}

void CPlayer::on_clicked_PlayBtn()
{
    if(0 == VOS_DisplayVoice("100",false,0))
        qDebug() << "播放成功";
    else
        qDebug() << "播放失败";
}

void CPlayer::on_clicked_NoStopPlayBtn()
{
    static bool bFirst = true;
    if(bFirst)
    {
        bFirst = false;
        m_bNoStopPlay = true;
        m_NoStopPlayBtn->setText("停止连续音频测试");
    }
    else
    {
        bFirst = true;
        m_bNoStopPlay = false;
        m_NoStopPlayBtn->setText("连续音频测试");
    }
}
int CPlayer::Play(char *pIn, bool bWait)
{
    return VOS_DisplayVoice("100",true,0);
}

int CPlayer::PlayerInit()
{
    char szIn[10],szOut[10];
    VOS_Init(szIn,szOut);
    if(!m_ThreadFlag)
    {
        int iRet = pthread_create(&m_ThreadId,NULL,PlayThread,(void*)this);
        if(0 != iRet)
        {
            m_ThreadFlag = false;
        }
        else
        {
            m_ThreadFlag = true;
        }
    }
    return 0;
}
void CPlayer::PlayerShowInit(QWidget *parent)
{
    QString qButtonStyleSheet("QPushButton{background-color:rgb(85, 170, 255);color:white;border-radius:10px;border: 3px groove gray;\
                        border-style: outset;font-family:Arial;font-size:12px;}"
                       "QPushButton:hover{background-color:white; color: black;}"
                      "QPushButton:pressed{background-color:rgb(85, 170, 255);border-style:inset;}");
    m_BackGroundLb = new QLabel(parent);
    m_BackGroundLb->setGeometry(0,40,200,60);
    m_BackGroundLb->setText("音频");
    m_BackGroundLb->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:1 rgba(0, 0, 0, 50));\
                                  border-radius:10px;color:rgba(0, 0, 0, 95);font-size:25px;");
    m_BackGroundLb->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    m_PlayBtn = new QPushButton(parent);
    m_PlayBtn->setGeometry(0,70,100,30);
    m_PlayBtn->setStyleSheet(qButtonStyleSheet);
    m_PlayBtn->setText("音频测试");
    m_NoStopPlayBtn = new QPushButton(parent);
    m_NoStopPlayBtn->setGeometry(100,70,100,30);
    m_NoStopPlayBtn->setStyleSheet(qButtonStyleSheet);
    m_NoStopPlayBtn->setText("连续音频测试");

    connect(m_PlayBtn,SIGNAL(clicked()),this,SLOT(on_clicked_PlayBtn()));
    connect(m_NoStopPlayBtn,SIGNAL(clicked()),this,SLOT(on_clicked_NoStopPlayBtn()));
}

void CPlayer::SetTextEdit(QTextEdit *qTextEd)
{
    m_TextEdit = qTextEd;
}

void *CPlayer::PlayThread(void *arg)
{
    CPlayer *pPlayer = (CPlayer*)arg;
    pPlayer->m_SYNCPlayList.clear();
    while(pPlayer->m_ThreadFlag)
    {
        if(pPlayer->m_bNoStopPlay)
        {
            pPlayer->Play(NULL,true);
        }

    }
    pPlayer->m_ThreadExitFlag = true;
    return NULL;
}


CPlayer *CPlayer::GerInstance()
{
    if(NULL == m_Player)
    {
        m_Player = new CPlayer();
        return m_Player;
    }
    return m_Player;
}
