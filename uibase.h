#ifndef UIBASE_H
#define UIBASE_H

#include <QWidget>

class UIBase : public QWidget
{
  Q_OBJECT

public:
  UIBase(QWidget *parent = 0);
  ~UIBase();
};

#endif // UIBASE_H
