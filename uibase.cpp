#include "uibase.h"

UIBase::UIBase(int iPage)
{
  m_pData = CConfigParse::getInstance();
  createUI(iPage);
}

UIBase::~UIBase()
{

}

void UIBase::createUI(int iUiPage)
{
  if(m_pData->m_stData.stDataMap.find(iUiPage) == m_pData->m_stData.stDataMap.end())
    {
      qDebug() << "Didn’t find this page"<<iUiPage;
    }
<<<<<<< HEAD
  qDebug() << "page"<<iUiPage;
=======

>>>>>>> 510673eb2a1318b344da31a5a36100bef37f4bb9
  QList<ST_UI_ITEM>::iterator it = m_pData->m_stData.stDataMap[iUiPage].stItemList.begin();
  for(;it != m_pData->m_stData.stDataMap[iUiPage].stItemList.end();it++)
    {
      if(!strcmp(it->szWidgetType,"Image"))
        {
          QLabel *label = new QLabel(this);
          label->setGeometry(it->iLeft,it->iTop,it->iWidth,it->iHeight);
          if(strlen(it->szContent) > 0)
            {
<<<<<<< HEAD
              qDebug() <<"strcmp Image";
=======
>>>>>>> 510673eb2a1318b344da31a5a36100bef37f4bb9
              QPixmap pix(it->szContent);
              label->setPixmap(pix);
            }
          else
            {
              label->setText(it->szText);
              label->setObjectName(it->szName);
            }
<<<<<<< HEAD
          this->setGeometry(it->iLeft,it->iTop,it->iWidth,it->iHeight);
=======
//          this->setGeometry(it->iLeft,it->iTop,it->iWidth,it->iHeight);
>>>>>>> 510673eb2a1318b344da31a5a36100bef37f4bb9
        }
      if(!strcmp(it->szWidgetType,"Label"))
        {
          qDebug() <<"strcmp Label";
          QLabel *label = new QLabel(this);
          label->setGeometry(it->iLeft,it->iTop,it->iWidth,it->iHeight);
          label->setText(it->szText);
          label->setObjectName(it->szName);
        }
    }

}
