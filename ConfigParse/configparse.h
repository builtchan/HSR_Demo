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
  QString qWidgetType;        //需要生成的控件类型
  int iTop;                   //上偏移
  int iLeft;                  //左偏移
  int iWidth;                 //控件宽
  int iHeight;                //控件高
  QString qContent;           //附带内容
  QString qText;              //Text
  QString qName;
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
  void getUIData(ST_DATA * stData);


private:

  static CConfigParse * m_pInstance;

  ST_DATA m_stData;


};

#endif // CCONFIGPARSE

