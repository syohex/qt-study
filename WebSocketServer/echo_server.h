#pragma once

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class EchoServer : public QObject {
    Q_OBJECT

public:
    explicit EchoServer(quint16 port, bool debug, QObject *parent = nullptr);
    ~EchoServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void OnNewConnection();
    void ProcessTextMessage(QString message);
    void ProcessBinaryMessage(QByteArray message);
    void SocketDisconnected();

private:
    QWebSocketServer *websocket_server_;
    QList<QWebSocket*> clients_;
    bool debug_;
};
