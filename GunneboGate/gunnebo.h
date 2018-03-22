#ifndef GUNNEBO
#define GUNNEBO
#define MYDLL_EXPORTS
#include <QWidget>
#include <QMap>
#include <pthread.h>
#include "publicdef.h"
#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QRadioButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "GAT_CssWeb.h"
#include <QPalette>

class Gunnebo : public QWidget
{
    Q_OBJECT

public:
    Gunnebo(QWidget *parent);
    ~Gunnebo();

    void GateInit(int iCom,char *pResult);
    void GateUninit();
    void GunneboShowInit(QWidget *parent);
    void GunneboShow();
    void GunneboHide();
    int OpenDoor(bool bIn);             //开门和授权
    int DIOCtrl(char *pIn);             //灯和蜂鸣器控制

    int GateGetStatus(char *pIn ,char *pOut);
    void GetSensorStatus();

    static void * GetStatusThread(void *arg);
    static void * SensorStatusThread(void *arg);

    void ParseRadioButtonCfg();
    void SetTextEdit(QTextEdit *qTextEd);
public :
    QMap<EM_GAT_STACODE ,QString> m_StateMap;
    QMap<EM_GATE_ERROR_CODE,QString> m_GateErrorMap;
    QMap<int , ST_RADIOBUTTON_CONFIG>m_RadioButtonMap;

public:

    //背景
    QLabel *m_ButtonBackground;

    QPushButton *m_EmergencyModeBtn;         //紧急模式
    QPushButton *m_MaintainModeBtn;         //维修开门
    QPushButton *m_SignalBtn;               //授权
    QPushButton *m_OpenDoorBtn;             //开门
    QLabel *m_TopLedLabel;
     //顶部警示灯
    QButtonGroup *m_DIOCtlGroup;
    QRadioButton *m_TopLedRed;
    QRadioButton *m_TopLedGreen;
    QRadioButton *m_TopLedYellow;
    QLabel *m_AisleLedLabel;
 //通道指示灯
    QRadioButton *m_AisleLedPass;
    QRadioButton *m_AisleLedForbit;
    QLabel *m_BuzzerLabel;
    QRadioButton *m_Buzzer;                 //蜂鸣器
    QPushButton *m_ConnectBtn;              //连接
//    QPushButton *m_DisconnectBtn;           //断连
    QPushButton *m_ClearBtn;                //清除提示框
    QPushButton *m_GetSwitchBtn;            //到位开关状态
    QPushButton *m_GetSenserStatusBtn;      //传感器按钮
    QPushButton *m_DeviceInfoBtn;
    QTextEdit *m_TipsTextEdit;              //提示框

    QRadioButton *m_Sensors[20];        //传感器
    QButtonGroup *m_SensorGroup;
    QLabel *m_SensorNameLb[20];
public:
    bool m_bInitFlag;
    EM_GAT_STACODE m_GateState;
    bool m_bStop;
protected:
    bool m_InMaintainOpenDoor;
    bool m_bThreadFlag;
    bool m_bThreadExitFlag;

    pthread_t m_threadId;
    pthread_mutex_t m_thread_mutex;

    bool m_bSensortThreadFlag;
    bool m_bGetSensorStatus;
    bool m_bSensortThreadExitFlag;
    pthread_t m_SensorThreadId;
    pthread_mutex_t m_SensorMutex;
    int m_iCom;

public slots:
    void on_clicked_m_ConnectBtn();
    void on_clicked_MaintainModeBtn();
    void on_clicked_SignalBtn();
    void on_clicked_OpenDoorBtn();
    void on_clicked_DIOContrlBtn(int iNo);
//    void on_clicked_DisconnectBtn();
    void on_clicked_ClearBtn();
    void on_clicked_GetSwitchBtn();
    void on_clicked_GetSenserStatus();
    void on_clicked_EmergencyBtn();
    void on_clicked_DeviceInfoBtn();

};
#endif // GUNNEBO

