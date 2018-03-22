#ifndef OMRONTRE_H
#define OMRONTRE_H

#define MYDLL_EXPORTS
#include <QWidget>
#pragma pack(1)
#include "MAG_CssWeb.h"
#pragma pack()
#include <pthread.h>
#include <QMap>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include "publicdef.h"
#include <semaphore.h>
#include <QDebug>
#include <QButtonGroup>

class COmronTre : public QWidget
{
    Q_OBJECT

public:
    COmronTre(QWidget *parent = 0);
    ~COmronTre();

    static void * TicketPOSThread(void * arg);

    int TREInit(int iCom,char *pResult);
    int CheckStart(char *pIn,char *pOut);
    int CheckPos(char *pIn,char *pOut);
    int ReadData(char *pIn,char *pOut);
    int Eject(char *pIn,char *pOut);
    int WritePrintEject(char *pIn,char *pOut);
    int LightControl(char *pIn,char *pOut);

    void SetTextEdit(QTextEdit *qTextEd);
    void TREShowInit(QWidget *parent);
public:
    bool m_bTreInit;
    bool m_CanRead;
    bool m_bFatalError;
    bool m_bCheckSuccess;
    T_EW_WRITE_PRINT_MAGCARD_INFO m_stWriteInfo;
    tMagCardTrackInfo m_ReadTicketInfo;
    tMagCardTrackInfo m_WriteData;
    sem_t m_Check_Sem;
    sem_t m_Ticket_Take_Sem;
    EM_TICKET_POSITION m_emTicketPos;
    EM_TICKET_POSITION m_emCheckSuccessPos;
    EM_MAG_STATECODE m_emTreState;
    QMap<EM_MAG_STATECODE,QString> m_StateToStringMap;

private:
    bool m_bThreadFlag;
    bool m_bThreadExitFlag;
    pthread_t m_ThreadId;
    pthread_mutex_t m_ThreadMutex;
    int m_iCom;
private:

    QLabel *m_ButtonBackground;
    QTextEdit *m_TextEdit;
    QPushButton m_Connect;
//    QLabel m_ComLabel;
//    QLineEdit m_Com;
    QLabel m_PrintTip;
    QButtonGroup m_PrintGroup;
    QRadioButton m_PrintSquare;
    QRadioButton m_PrintTriangle;
    QRadioButton m_PrintNothing;

    QButtonGroup m_InletLedGroup;
    QRadioButton m_RedLed;
    QRadioButton m_GreenLed;
    QRadioButton m_Twinkle;

    QButtonGroup m_EjectPosGroup;
    QRadioButton m_EjectInlet;
    QRadioButton m_EjectOutlet;

//    QPushButton m_ReadTicketBtn;
    QPushButton m_WriteTicketBtn;
    QPushButton m_EjectBtn;
    QPushButton m_LedControlBtn;

public slots:
    void on_clicked_ConnectBtn();
    void on_clicked_ReadTicketBtn();
    void on_clicked_WriteTicketBtn();
    void on_clicked_EjectBtn();
    void on_clicked_LedControlBtn();

};

#endif // OMRONTRE_H
