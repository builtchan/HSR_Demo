#include "ConfigParse/configparse.h"

CConfigParse *CConfigParse::m_pInstance = NULL;

CConfigParse::CConfigParse()
{
  parseXMl();
}

CConfigParse::~CConfigParse()
{
  if(NULL != m_pInstance)
    delete m_pInstance;
}

CConfigParse * CConfigParse::getInstance()
{
  if(NULL == m_pInstance)
    {
      m_pInstance = new CConfigParse();
    }
  return m_pInstance;
}

bool CConfigParse::parseXMl()
{
  int iPage = 0;
  ST_UI_DATA stUIData;
  ST_UI_ITEM stUiItem;
  memset(&stUiItem,0,sizeof(stUiItem));

  //1 open
  QFile file(UI_CONFIG_PATH);
  if(!file.open(QFile::ReadOnly | QFile::Text))
    {
      qDebug() << "Open " << UI_CONFIG_PATH << "failed";
      return false;
    }

  //2、将text文件以utf-8转成流数据
  QTextStream stream(&file);
  stream.setCodec(QTextCodec::codecForName("utf-8"));

  //3、转成String后再转换成文档
  QString string = stream.readAll();
  QDomDocument Domc;
  Domc.setContent(string);
  if(Domc.isNull() || file.size() <= 0)
    {
      qDebug() << "The xml file is Empty";
      return false;
    }

  //根节点LineMap
  QDomElement Root = Domc.documentElement();

  QDomElement UIpage = Root.firstChildElement();

  while(!UIpage.isNull())
    {
      iPage = UIpage.attributeNode("id").value().toInt();
      qDebug()<<"Page"<<iPage;
      QDomElement Item = UIpage.firstChildElement();
      while(!Item.isNull())
        {
          memset(&stUiItem,0,sizeof(stUiItem));
          if(Item.attributeNode("type").value().length() > 0 )
            stUiItem.qWidgetType = Item.attributeNode("type").value();
          if(Item.attributeNode("content").value().length() > 0 )
            stUiItem.qContent = Item.attributeNode("content").value();
          if(Item.attributeNode("text").value().length() > 0 )
            stUiItem.qText = Item.attributeNode("text").value();
          if(Item.attributeNode("name").value().length() > 0 )
            stUiItem.qName = Item.attributeNode("name").value();
          stUiItem.iTop = Item.attributeNode("top").value().toInt();
          stUiItem.iLeft = Item.attributeNode("left").value().toInt();
          stUiItem.iWidth = Item.attributeNode("width").value().toInt();
          stUiItem.iHeight = Item.attributeNode("height").value().toInt();

          Item = Item.nextSiblingElement();
          stUIData.stItemList.push_back(stUiItem);
        }
      m_stData.stDataMap.insert(iPage,stUIData);
      UIpage = UIpage.nextSiblingElement();
    }
  file.close();
  return true;
}

void CConfigParse::getUIData(ST_DATA *stData)
{
  if(NULL == stData)
    qDebug() <<"NULL == stData";
  memcpy(stData,&m_stData,sizeof(ST_DATA));
}
