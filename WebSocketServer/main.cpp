#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>

#include "echo_server.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("QtWebsocket Example");
    parser.addHelpOption();

    QCommandLineOption debug_option(QStringList() << "d" << "debug", QCoreApplication::translate("main", "Debug output [default: off]"));
    parser.addOption(debug_option);

    QCommandLineOption port_option(QStringList() << "p" << "port", QCoreApplication::translate("main", "Port echo server [default: 1234]"),
                                   QCoreApplication::translate("main", "port"), QLatin1String("1234"));
    parser.addOption(port_option);
    parser.process(app);

    bool debug = parser.isSet(debug_option);
    int port = parser.value(port_option).toInt();

    auto echo_server = new EchoServer(port, debug);
    QObject::connect(echo_server, &EchoServer::closed, &app, &QCoreApplication::quit);
    return QCoreApplication::exec();
}
