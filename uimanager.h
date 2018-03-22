#ifndef UIMANAGER
#define UIMANAGER

#include "UI/page0.h"
#include "UI/page1.h"
#include "UI/page2.h"
#include "UI/page3.h"
#include "UI/page4.h"
#include "UI/page5.h"
#include "UI/page6.h"
#include "UI/page7.h"
#include "UI/page8.h"

#include <QStackedWidget>
#include <QKeyEvent>
#include "publicdef.h"
#include "GunneboGate/gunnebo.h"
#include "OmronTRE/omrontre.h"
#include "EWReader/ewreader.h"
#include "VOS_CSSa/player.h"
#include <QMediaPlayer>

class CUimanager:public QStackedWidget
{
  Q_OBJECT
public:
  CUimanager();
  virtual ~CUimanager();

  static CUimanager * GetInstance();
  static void * StateMachineThread(void *arg);
  static void * GetSystemInfoThread(void *arg);
protected:
  void keyPressEvent(QKeyEvent * e);


signals:
  void SetPage(int iPage);
public slots:
  //切换界面
  void SwitchPage(int iPage);

private:
  CPage0 * m_pPage0;
  CPage1 * m_pPage1;
  CPage2 * m_pPage2;
  CPage3 * m_pPage3;
  CPage4 * m_pPage4;
  CPage5 * m_pPage5;
  CPage6 * m_pPage6;
  CPage7 * m_pPage7;
  CPage8 * m_pPage8;

private:
  bool m_bStateMachineFlag;
  bool m_bStateMachineExit;
  pthread_t m_StateThreadId;
  pthread_mutex_t m_StateThreadMutex;
  bool m_bSystemInfo;
  bool m_bGetSysInfo;
  pthread_t m_SystemInfoThreadId;

private:
  Gunnebo* m_pGate;
  COmronTre * m_pOmronTre;
  CPlayer *m_pPlayer;
  EWReader *m_Reader;

  bool m_bMaintainMode;         //维修模式
  bool m_bCanServer;            //能不能服务

protected:
  QStackedWidget *m_pStackedWidget;
private:
  static CUimanager * m_CUIManager;

  QPushButton m_PlayBtn;
  QPushButton m_GetSysInfoBtn;
  QPushButton m_GetGateInfoBtn;
  QTextEdit m_TextEdit;
  QCheckBox m_MaintainCKBox;
  QPushButton m_ExitMaitainBtn;
  QPushButton m_ServerToMaintainBtn;
  QPushButton m_OutOfServerToMaintainBtn;

  QPushButton m_Exit;
  QPushButton m_Exit1;
  QPushButton m_Exit2;

  EM_PAGE_NAME m_CurrentStatePage;
  ST_SYSTEM_INFO m_stSystemInfo;

private:
  void StateManager();
  void GetSystemInfo();

public slots:
  void on_checked_MaintainCKBox();
  void on_clicked_ExitMaitainBtn();
  void on_clicked_IntoMaintainnBtn();
  void On_clicked_ExitBtn();
  void on_clicked_GetSysInfoBtn();
  void on_clicked_GetGateInfoBtn();
};


#endif // UIMANAGER

