#include <iostream>
#include <QtCore>
#include <QtNetwork>

namespace
{

QString SaveFileName(const QUrl& url) {
    QString basename = QFileInfo(url.path()).fileName();
    if (basename.isEmpty()) {
        basename = "download";
    }

    if (QFile::exists(basename)) {
        basename += ".";
        int i = 0;
        while (QFile::exists(basename + QString::number(i))) {
            ++i;
        }

        basename += QString::number(i);
    }

    return basename;
}

bool SaveToDisk(const QString& file_name, QIODevice *data) {
    QFile file(file_name);
    auto ret = file.open(QIODevice::WriteOnly);
    if (!ret) {
        std::cerr << "could not open file: " << qPrintable(file.errorString()) << std::endl;
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

bool IsHTTPRedirect(QNetworkReply *reply) {
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    switch (status_code) {
    case 301:
    case 302:
    case 303:
    case 305:
    case 308:
        return true;
    default:
        return false;
    }
}

class DownloadManager : public QObject {
    Q_OBJECT;

public:
    DownloadManager();
    void DoDownload(const QUrl &url);

public slots:
    void Execute();
    void DownloadFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager manager_;
    QVector<QNetworkReply*> downloads_;
};

DownloadManager::DownloadManager() {
    connect(&manager_, &QNetworkAccessManager::finished, this, &DownloadManager::DownloadFinished);
}

void DownloadManager::DoDownload(const QUrl& url) {
    const QNetworkRequest request(url);
    QNetworkReply* reply = manager_.get(request);
    downloads_.append(reply);
}

void DownloadManager::Execute() {
    QStringList args = QCoreApplication::arguments();
    args.takeFirst();
    if (args.isEmpty()) {
        QCoreApplication::quit();
        return;
    }

   for (const QString& arg : qAsConst(args)) {
       const QUrl url = QUrl::fromEncoded(arg.toLocal8Bit());
       DoDownload(url);
   }
}

void DownloadManager::DownloadFinished(QNetworkReply *reply) {
    QUrl url = reply->url();
    if (reply->error()) {
        std::cerr << "Download " << url.toEncoded().constData() << " Failed: %s" << qPrintable(reply->errorString()) << std::endl;
        return;
    }

    if (IsHTTPRedirect(reply)) {
        std::cout << "request was redirected" << std::endl;
        return;
    }

    QString file = SaveFileName(url);
    if (!SaveToDisk(file, reply)) {
        return;
    }

    std::cout << "Download " << url.toEncoded().constData() << " succeeded" << std::endl;

    downloads_.removeAll(reply);
    reply->deleteLater();

    if (downloads_.isEmpty()) {
        QCoreApplication::quit();
    }
}
    
} // namespace

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    DownloadManager manager;
    QTimer::singleShot(0, &manager, &DownloadManager::Execute);
    return QCoreApplication::exec();
}

#include "main.moc"