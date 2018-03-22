#ifndef UIBASE_H
#define UIBASE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QPixmap>
#include <ConfigParse/configparse.h>

class UIBase : public QWidget
{
  Q_OBJECT

public:
  explicit UIBase(int iUiPage);
  ~UIBase();

public:
  void createUI(int iUiPage);

public:
  CConfigParse  *m_pData;

protected:
  virtual void HideSomething() = 0;
};

#endif // UIBASE_H
