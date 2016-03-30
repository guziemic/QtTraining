#ifndef HTTPDIALOG_H
#define HTTPDIALOG_H

#include <QDialog>
#include <QUrl>
#include <QNetworkAccessManager>

class QFile;
class QLabel;
class QLineEdit;
class QCheckBox;

class HttpDialog : public QDialog
{
    Q_OBJECT

public:
    HttpDialog(QWidget *parent = 0);
    ~HttpDialog();

    void startRequest(const QUrl &requestedUrl);

private slots:
    void downloadFile();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void enableDownloadButton();
    void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
#ifndef QT_NO_SSL
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif

private:
    QFile *openFileForWrite(const QString &fileName);

    QLabel *m_statusLabel;
    QLineEdit *m_urlLineEdit;
    QPushButton *m_downloadButton;
    QCheckBox *m_lunchCheckBox;
    QLineEdit *m_defaultFileLineEdit;
    QLineEdit *m_downloadDirectoryLineEdit;

    QUrl m_url;
    QNetworkAccessManager m_qnam;
    QNetworkReply *m_reply;
    QFile *m_file;
    bool m_httpRequestAborted;


};

#endif // HTTPDIALOG_H
