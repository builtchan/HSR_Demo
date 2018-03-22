#ifndef PLAYER
#define PLAYER
#define MYDLL_EXPORTS
#include "VOS_CSSa/VOS_CSSa.h"
#include <QWidget>
#include <pthread.h>
#include <QList>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

class CPlayer : public QWidget
{
    Q_OBJECT

public:
    CPlayer(QWidget *parent = 0);

    static CPlayer*GerInstance();
    static void* PlayThread(void*arg);

    int PlayerInit();
    void PlayerShowInit(QWidget *parent);
    void SetTextEdit(QTextEdit *qTextEd);
    int Play(char*pIn,bool bWait);
protected:

    bool m_ThreadFlag;
    bool m_ThreadExitFlag;
    pthread_t m_ThreadId;
    pthread_mutex_t m_ThreadMutex;

private:
    QLabel *m_BackGroundLb;
    QPushButton *m_PlayBtn;
    QPushButton *m_NoStopPlayBtn;

    static CPlayer*m_Player;
    QList<QString>m_SYNCPlayList;
    QTextEdit *m_TextEdit;
    bool m_bNoStopPlay;

public slots:
    void on_clicked_PlayBtn();
    void on_clicked_NoStopPlayBtn();
};




#endif // PLAYER

