#ifndef EWREADER_H
#define EWREADER_H

#define MYDLL_EXPORTS
#include "EWReaderCommon.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QDebug>
#include <pthread.h>
#include <semaphore.h>
class EWReader : public QWidget
{
    Q_OBJECT

public:
    EWReader(QWidget *parent = 0);
    ~EWReader();

    static void *EWReaderThread(void*arg);


    bool m_bInitFlag;
    int m_iCom;
    bool m_bPolling;
    char m_CardId[100];

    void SetTextEdit(QTextEdit *pTextEdit);
    void EWReaderShowInit();
    bool EWReaderInit(int iCom);
    bool Polling(char*pIn,char *pOut);
private:
    QLabel m_ReaderBg;
    QPushButton m_ConnectBtn;
    QPushButton m_ResetBtn;
    QPushButton m_BeepBtn;
    QPushButton m_ReadIDBtn;
    QPushButton m_ReadAllBtn;
    QTextEdit *m_TextEdit;

public:
    bool m_bIsReadCard;
    sem_t m_ReadCardSem;

protected:
    bool m_bEWReaderFlag;
    bool m_bEWReaderExitFlag;
    pthread_t m_ThreadId;
    pthread_mutex_t m_EWReaderMutex;


public slots:
    void on_clicked_ConnectBtn();
    void on_clicked_BeepBtn();
    void on_clicked_ResetBtn();
    void on_clicked_ReadIDBtn();
    void on_clicked_ReadAllBtn();
};

#endif // EWREADER_H
