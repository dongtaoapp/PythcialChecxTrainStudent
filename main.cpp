#include "qtrainmainwindow.h"
#include <QApplication>
#include <QObject>
#include "loginbackgroundform.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTrainMainWindow w;
    w.show();
    return a.exec();
}
