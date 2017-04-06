#include "uibase.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  UIBase w;
  w.show();

  return a.exec();
}
