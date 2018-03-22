#ifndef CCONFIGPARSE
#define CCONFIGPARSE

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <QDomAttr>
#include <QString>
#include <QList>
#include <QMap>
#include <QDebug>

#define UI_CONFIG_PATH "C:/HSR/UIConfig.xml"
typedef struct _ST_UI_ITEM_
{
  char szWidgetType[20];        //需要生成的控件类型
  int iTop;                   //上偏移
  int iLeft;                  //左偏移
  int iWidth;                 //控件宽
  int iHeight;                //控件高
  char szContent[256];           //附带内容
  char szText[100];              //Text
  char szName[20];
}ST_UI_ITEM;

typedef struct _ST_UI_DATA_
{
  QList<ST_UI_ITEM> stItemList;
}ST_UI_DATA;

typedef struct _ST_DATA_
{
  QMap<int,ST_UI_DATA> stDataMap;
}ST_DATA;


class CConfigParse:QObject
{
  Q_OBJECT

public:
  CConfigParse();
  ~CConfigParse();
  static CConfigParse * getInstance();

  bool parseXMl();

public:
  ST_DATA m_stData;

  int m_GateCom;
  int m_TreCom;
  int m_IDCardCom;

private:

  static CConfigParse * m_pInstance;

};

#endif // CCONFIGPARSE

