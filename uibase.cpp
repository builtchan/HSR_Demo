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
  qDebug() << "page"<<iUiPage;
  QList<ST_UI_ITEM>::iterator it = m_pData->m_stData.stDataMap[iUiPage].stItemList.begin();
  for(;it != m_pData->m_stData.stDataMap[iUiPage].stItemList.end();it++)
    {
      if(!strcmp(it->szWidgetType,"Image"))
        {
          QLabel *label = new QLabel(this);
          label->setGeometry(it->iLeft,it->iTop,it->iWidth,it->iHeight);
          if(strlen(it->szContent) > 0)
            {
              qDebug() <<"strcmp Image";
              QPixmap pix(it->szContent);
              label->setPixmap(pix);
            }
          else
            {
              label->setText(it->szText);
              label->setObjectName(it->szName);
            }
          this->setGeometry(it->iLeft,it->iTop,it->iWidth,it->iHeight);
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
