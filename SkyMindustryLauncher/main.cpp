#include "SkyMindustryLauncher.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SkyMindustryLauncher w;
    w.show();
    return a.exec();
}
