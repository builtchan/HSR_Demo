#ifndef UIMANAGER
#define UIMANAGER

#include "UI/page0.h"
#include "UI/page1.h"
#include "UI/page2.h"
#include "UI/page3.h"
#include "UI/page4.h"
#include "UI/page5.h"
#include "UI/page6.h"

#include <QStackedWidget>
#include <QKeyEvent>

class CUimanager:public QStackedWidget
{
  Q_OBJECT
public:
  CUimanager();

protected:
  void keyPressEvent(QKeyEvent * e);

public slots:
  //切换界面
//  void SwitchPage(int iPage);

private:
  CPage0 * m_pPage0;
  CPage1 * m_pPage1;
  CPage2 * m_pPage2;
  CPage3 * m_pPage3;
  CPage4 * m_pPage4;
  CPage5 * m_pPage5;
  CPage6 * m_pPage6;

  QStackedWidget *m_pStackedWidget;

};


#endif // UIMANAGER

