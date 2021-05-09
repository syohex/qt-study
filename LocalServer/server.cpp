#include "server.h"

#include <QtWidgets>
#include <QtNetwork>

Server::Server(QWidget *parent) : QDialog(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    server_ = new QLocalServer(this);
    if (!server_->listen("fortune")) {
        QMessageBox::critical(this, tr("Local Fortune Server"), tr("Unable to start server"));
        close();
        return;
    }

    QLabel *status_label = new QLabel();
    status_label->setWordWrap(true);
    status_label->setText(tr("The server is running"));

    fortunes_ << tr("You've been leading a dog's life. Stay off the furniture.");
    fortunes_ << tr("You've got to think about tomorrow");
    fortunes_ << tr("You will be suprised by a loud noise");
    fortunes_ << tr("You will feel hungry again in another hour");
    fortunes_ << tr("You might have mail");
    fortunes_ << tr("You cannot kill time without injuring eternity");
    fortunes_ << tr("Compures are not intelligent. They only think they are");

    QPushButton *quit_button = new QPushButton(tr("Quit"));
    quit_button->setAutoDefault(false);
    connect(quit_button, &QPushButton::clicked, this, &Server::close);
    connect(server_, &QLocalServer::newConnection, this, &Server::SendFortune);

    QHBoxLayout *button_layout = new QHBoxLayout(this);
    button_layout->addStretch(1);
    button_layout->addWidget(quit_button);
    button_layout->addStretch(1);

    QHBoxLayout *main_layout = new QHBoxLayout(this);
    main_layout->addWidget(status_label);
    main_layout->addLayout(button_layout);

    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Server::SendFortune() {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    const int fortune_index = QRandomGenerator::global()->bounded(0, fortunes_.size());
    const QString &message = fortunes_.at(fortune_index);
    out << quint32(message.size());
    out << message;

    QLocalSocket *client_connection = server_->nextPendingConnection();
    connect(client_connection, &QLocalSocket::disconnected, client_connection, &QLocalSocket::deleteLater);

    client_connection->write(block);
    client_connection->flush();
    client_connection->disconnectFromServer();
}
