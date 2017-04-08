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
  ST_DATA stData;
  m_pData->getUIData(&stData);

  if(stData.stDataMap.find(iUiPage) == stData.stDataMap.end())
    {
      qDebug() << "Didn’t find this page"<<iUiPage;
    }

  QList<ST_UI_ITEM>::iterator it = stData.stDataMap[iUiPage].stItemList.begin();
  for(;it != stData.stDataMap[iUiPage].stItemList.end();it++)
    {
      if(it->qWidgetType == "Image")
        {
          QLabel *label = new QLabel(this);
          label->setGeometry(it->iLeft,it->iTop,it->iWidth,it->iHeight);
          if(!it->qContent.isEmpty())
            {
              QPixmap pix(it->qContent);
              label->setPixmap(pix);
            }
          else
            {
              label->setText(it->qText);
            }
          label->setObjectName(it->qName);
        }
      if(it->qWidgetType == "Label")
        {
          QLabel *label = new QLabel(this);
          label->setGeometry(it->iLeft,it->iTop,it->iWidth,it->iHeight);
          label->setText(it->qText);
          label->setObjectName(it->qName);
        }
    }

}
