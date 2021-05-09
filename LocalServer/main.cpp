#include <QApplication>

#include "server.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(Server::tr("Local Fortune Server"));
    Server server;
    server.show();
    return QApplication::exec();
}
