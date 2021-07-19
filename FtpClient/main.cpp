#include <QApplication>
#include <QFileIconProvider>
#include <QListWidget>
#include "ftpclientwindow.h"
#include "ftpmanager.h"
#include "publisherfunc.h"
using namespace publisherFunc;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FtpClientWindow w;
    w.show();

    return a.exec();
}
