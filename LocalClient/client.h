#pragma once

#include <QDialog>
#include <QDataStream>
#include <QLocalSocket>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class Client : public QDialog {
    Q_OBJECT

  public:
    explicit Client(QWidget *parent = nullptr);

  private slots:
    void RequestNewFortune();
    void ReadFortune();
    void DisplayError(QLocalSocket::LocalSocketError socket_error);
    void EnableGetFortuneButton();

  private:
    QLineEdit *host_line_edit_;
    QPushButton *get_fortune_button_;
    QLabel *status_label_;
    QLocalSocket *socket_;
    QDataStream in_;
    quint32 block_size_;
    QString current_fortune_;
};
