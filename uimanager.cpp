#include "uimanager.h"

CUimanager::CUimanager()
{
  //隐藏标题栏
  //setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowTitleHint);

  //隐藏鼠标
  //setCursor(Qt::BlankCursor);

  m_pStackedWidget = new QStackedWidget(this);

  this->addWidget(m_pStackedWidget);
  m_pPage0 = new CPage0();
  m_pPage1 = new CPage1();
  m_pPage2 = new CPage2();
  m_pPage3 = new CPage3();
  m_pPage4 = new CPage4();
  m_pPage5 = new CPage5();
  m_pPage6 = new CPage6();

  m_pStackedWidget->addWidget(m_pPage0);
  m_pStackedWidget->addWidget(m_pPage1);
  m_pStackedWidget->addWidget(m_pPage2);
  m_pStackedWidget->addWidget(m_pPage3);
  m_pStackedWidget->addWidget(m_pPage4);
  m_pStackedWidget->addWidget(m_pPage5);
  m_pStackedWidget->addWidget(m_pPage6);

   this->setGeometry(50,50,m_pPage0->width(),m_pPage0->height());
  qDebug()<< m_pPage0->width() << m_pPage0->height();
}
//void CUimanager::SwitchPage(int iPage)
//{

//}
void CUimanager::keyPressEvent(QKeyEvent *e)
{
  switch (e->key()) {
    case Qt::Key_0:
      m_pStackedWidget->setCurrentIndex(0);
      break;
    case Qt::Key_1:
      m_pStackedWidget->setCurrentIndex(1);
      break;
    case Qt::Key_2:
      m_pStackedWidget->setCurrentIndex(2);
      break;
    case Qt::Key_3:
      m_pStackedWidget->setCurrentIndex(3);
      break;
    case Qt::Key_4:
      m_pStackedWidget->setCurrentIndex(4);
      break;
    case Qt::Key_5:
      m_pStackedWidget->setCurrentIndex(5);
      break;
    case Qt::Key_6:
      m_pStackedWidget->setCurrentIndex(6);
      break;
    default:
      break;
    }
}
