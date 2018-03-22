#include "uimanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
<<<<<<< HEAD

    QApplication a(argc, argv);
    CUimanager *w = CUimanager::GetInstance();
    w->show();
=======
  QApplication a(argc, argv);
  CUimanager w;
  w.show();
>>>>>>> 510673eb2a1318b344da31a5a36100bef37f4bb9

  return a.exec();
}
