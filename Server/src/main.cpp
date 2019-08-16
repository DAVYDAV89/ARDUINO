#include <QCoreApplication>
#include "qserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QServer server;
    
    return a.exec();
}
