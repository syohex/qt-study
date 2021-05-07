#include "client.h"

#include <QtWidgets>
#include <QtNetwork>

#define DEFAULT_LABEL tr("This example requires that you run the Local Fortune Server example as well")

Client::Client(QWidget *parent)
    : QDialog(parent), host_line_edit_(new QLineEdit("fortune")), get_fortune_button_(new QPushButton(tr("Get Fortune"))),
      status_label_(new QLabel(DEFAULT_LABEL)), socket_(new QLocalSocket(this)) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto *host_label = new QLabel(tr("&Server name: "));
    host_label->setBuddy(host_line_edit_);
    status_label_->setWordWrap(true);

    get_fortune_button_->setDefault(true);
    auto *quit_button = new QPushButton(tr("Quit"));

    auto *button_box = new QDialogButtonBox();
    button_box->addButton(get_fortune_button_, QDialogButtonBox::ActionRole);
    button_box->addButton(quit_button, QDialogButtonBox::RejectRole);

    in_.setDevice(socket_);
    in_.setVersion(QDataStream::Qt_5_10);

    connect(host_line_edit_, &QLineEdit::textChanged, this, &Client::EnableGetFortuneButton);
    connect(get_fortune_button_, &QPushButton::clicked, this, &Client::RequestNewFortune);
    connect(quit_button, &QPushButton::clicked, this, &Client::close);
    connect(socket_, &QLocalSocket::readyRead, this, &Client::ReadFortune);
    connect(socket_, &QLocalSocket::errorOccurred, this, &Client::DisplayError);

    auto *main_layout = new QGridLayout(this);
    main_layout->addWidget(host_label, 0, 0);
    main_layout->addWidget(host_line_edit_, 0, 1);
    main_layout->addWidget(status_label_, 2, 0, 1, 2);
    main_layout->addWidget(button_box, 3, 0, 1, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    host_line_edit_->setFocus();
}

void Client::RequestNewFortune() {
    get_fortune_button_->setEnabled(false);
    block_size_ = 0;
    socket_->abort();
    socket_->connectToServer(host_line_edit_->text());
}

void Client::ReadFortune() {
    if (block_size_ == 0) {
        if (socket_->bytesAvailable() < static_cast<int>(sizeof(quint32))) {
            return;
        }

        in_ >> block_size_;
    }

    if (socket_->bytesAvailable() < block_size_ || in_.atEnd()) {
        return;
    }

    QString next_fortune;
    in_ >> next_fortune;

    if (next_fortune == current_fortune_) {
        QTimer::singleShot(0, this, &Client::RequestNewFortune);
        return;
    }

    current_fortune_ = next_fortune;
    status_label_->setText(current_fortune_);
    get_fortune_button_->setEnabled(true);
}

void Client::DisplayError(QLocalSocket::LocalSocketError socket_error) {
    switch (socket_error) {
    case QLocalSocket::ServerNotFoundError:
        QMessageBox::information(this, tr("Local Fortune Client"), tr("server not found"));
        break;
    case QLocalSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Local Fortune Client"), tr("connection failed"));
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        QMessageBox::information(this, tr("Local Fortune Client"),
                                 tr("The following error occurred: %1").arg(socket_->errorString()));
        break;
    }

    get_fortune_button_->setEnabled(true);
}

void Client::EnableGetFortuneButton() {
    get_fortune_button_->setEnabled(!host_line_edit_->text().isEmpty());
}
