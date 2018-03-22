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
    QDomElement UIConfig = Root.firstChildElement();
    QDomElement ComConfig = UIConfig.nextSiblingElement("ComConfig");
    QDomElement UIpage = UIConfig.firstChildElement();

    while(!UIpage.isNull())
    {
<<<<<<< HEAD
        iPage = UIpage.attributeNode("id").value().toInt();
        stUIData.stItemList.clear();
        qDebug()<<"Page"<<iPage;
        QDomElement Item = UIpage.firstChildElement();
        while(!Item.isNull())
        {
            memset(&stUiItem,0,sizeof(stUiItem));
            if(Item.attributeNode("type").value().length() > 0 )
                strcpy(stUiItem.szWidgetType,Item.attributeNode("type").value().toStdString().c_str());
            if(Item.attributeNode("content").value().length() > 0 )
                strcpy(stUiItem.szContent,Item.attributeNode("content").value().toStdString().c_str());
            if(Item.attributeNode("text").value().length() > 0 )
                strcpy(stUiItem.szText,Item.attributeNode("text").value().toStdString().c_str());
            if(Item.attributeNode("name").value().length() > 0 )
                strcpy(stUiItem.szName,Item.attributeNode("name").value().toStdString().c_str());
            stUiItem.iTop = Item.attributeNode("top").value().toInt();
            stUiItem.iLeft = Item.attributeNode("left").value().toInt();
            stUiItem.iWidth = Item.attributeNode("width").value().toInt();
            stUiItem.iHeight = Item.attributeNode("height").value().toInt();

            Item = Item.nextSiblingElement();
            stUIData.stItemList.push_back(stUiItem);
=======
      iPage = UIpage.attributeNode("id").value().toInt();
      stUIData.stItemList.clear();
      qDebug()<<"Page"<<iPage;
      QDomElement Item = UIpage.firstChildElement();
      while(!Item.isNull())
        {
          memset(&stUiItem,0,sizeof(stUiItem));
          if(Item.attributeNode("type").value().length() > 0 )
            strcpy(stUiItem.szWidgetType,Item.attributeNode("type").value().toStdString().c_str());
          if(Item.attributeNode("content").value().length() > 0 )
            strcpy(stUiItem.szContent,Item.attributeNode("content").value().toStdString().c_str());
          if(Item.attributeNode("text").value().length() > 0 )
            strcpy(stUiItem.szText,Item.attributeNode("text").value().toStdString().c_str());
          if(Item.attributeNode("name").value().length() > 0 )
            strcpy(stUiItem.szName,Item.attributeNode("name").value().toStdString().c_str());
          stUiItem.iTop = Item.attributeNode("top").value().toInt();
          stUiItem.iLeft = Item.attributeNode("left").value().toInt();
          stUiItem.iWidth = Item.attributeNode("width").value().toInt();
          stUiItem.iHeight = Item.attributeNode("height").value().toInt();

          Item = Item.nextSiblingElement();
          stUIData.stItemList.push_back(stUiItem);
>>>>>>> 510673eb2a1318b344da31a5a36100bef37f4bb9
        }
        m_stData.stDataMap.insert(iPage,stUIData);
        UIpage = UIpage.nextSiblingElement();
    }
<<<<<<< HEAD

    m_GateCom = ComConfig.attributeNode("Gate").value().toInt();
    m_TreCom = ComConfig.attributeNode("Tre").value().toInt();
    m_IDCardCom = ComConfig.attributeNode("IDCard").value().toInt();

    qDebug() <<"parse "<<m_GateCom<<m_TreCom<<m_IDCardCom;

    qDebug() << "end";
    file.close();

//  for(int i = 0 ; i < 9; i++)
//    {
//      qDebug() <<"iPage" << i;
//      QList<ST_UI_ITEM>::iterator it = m_stData.stDataMap[i].stItemList.begin();
//      for(;it != m_stData.stDataMap[i].stItemList.end();it++)
//        {
//          qDebug() << it->szWidgetType;
//          qDebug() << it->szContent;
//          qDebug() << it->szName;
//        }
//    }
=======
  qDebug() << "end";
  file.close();

  for(int i = 0 ; i < 7; i++)
    {
      qDebug() <<"i" << i;
      QList<ST_UI_ITEM>::iterator it = m_stData.stDataMap[i].stItemList.begin();
      for(;it != m_stData.stDataMap[i].stItemList.end();it++)
        {
          qDebug() << it->szWidgetType;
          qDebug() << it->szContent;
          qDebug() << it->szName;
        }
    }

>>>>>>> 510673eb2a1318b344da31a5a36100bef37f4bb9

  return true;
}

//void CConfigParse::getUIData(ST_DATA *stData)
//{
//  if(NULL == stData)
//    qDebug() <<"NULL == stData";
//  memcpy(stData,&m_stData,sizeof(ST_DATA));
//}
