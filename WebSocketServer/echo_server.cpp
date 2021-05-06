#include "echo_server.h"

#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QtCore/QDebug>

QT_USE_NAMESPACE

EchoServer::EchoServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    websocket_server_(new QWebSocketServer(QStringLiteral("Echo Server"), QWebSocketServer::NonSecureMode, this)),
    debug_(debug)
{
    if (websocket_server_->listen(QHostAddress::Any, port)) {
        if (debug_) {
            qDebug() << "Echo server listening port on port: " << port;
        }

        connect(websocket_server_, &QWebSocketServer::newConnection, this, &EchoServer::OnNewConnection);
        connect(websocket_server_, &QWebSocketServer::closed, this, &EchoServer::closed);
    }
}

EchoServer::~EchoServer()
{
    websocket_server_->close();
    qDeleteAll(clients_.begin(), clients_.end());
}

void EchoServer::OnNewConnection()
{
    QWebSocket *socket = websocket_server_->nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived, this, &EchoServer::ProcessTextMessage);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &EchoServer::ProcessBinaryMessage);
    connect(socket, &QWebSocket::disconnected, this, &EchoServer::SocketDisconnected);

    clients_ << socket;
}

void EchoServer::ProcessTextMessage(QString message) {
    auto *client = qobject_cast<QWebSocket*>(sender());
    if (debug_) {
        qDebug() << "Message Received: " << message;
    }

    if (client != nullptr) {
        client->sendTextMessage(message);
    }
}

void EchoServer::ProcessBinaryMessage(QByteArray message) {
    auto *client = qobject_cast<QWebSocket*>(sender());
    if (debug_) {
        qDebug() << "Binary Message Received: " << message;
    }

    if (client != nullptr) {
        client->sendBinaryMessage(message);
    }
}


void EchoServer::SocketDisconnected() {
    auto *client = qobject_cast<QWebSocket*>(sender());
    if (debug_) {
        qDebug() << "Socket Disconnected";
    }

    if (client != nullptr) {
        clients_.removeAll(client);
        client->deleteLater();
    }
}
