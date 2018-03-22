#include "uimanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    CUimanager *w = CUimanager::GetInstance();
    w->show();

  return a.exec();
}
