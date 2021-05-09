#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLocalServer;
QT_END_NAMESPACE

class Server : public QDialog {
    Q_OBJECT

  public:
    explicit Server(QWidget *parent = nullptr);

  private slots:
    void SendFortune();

  private:
    QLocalServer *server_;
    QStringList fortunes_;
};
