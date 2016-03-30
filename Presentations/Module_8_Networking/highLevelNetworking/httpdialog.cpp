#include "httpdialog.h"
#include "ui_authenticationdialog.h"

#include <QtWidgets>
#include <QtNetwork>

static const char defaultUrl[] = "http://www.tapetus.pl/obrazki/n/140343_las-promienie-slonca.jpg";
static const char defaultFileName[] = "out.txt";

HttpDialog::HttpDialog(QWidget *parent)
    : QDialog(parent)
    , m_statusLabel(new QLabel(tr("Please enter the URL \n")))
    , m_urlLineEdit(new QLineEdit(defaultUrl))
    , m_downloadButton(new QPushButton(tr("Download")))
    , m_cancelButton(new QPushButton(tr("Cancel")))
    , m_lunchCheckBox(new QCheckBox("Lunch file"))
    , m_defaultFileLineEdit(new QLineEdit(defaultFileName))
    , m_downloadDirectoryLineEdit(new QLineEdit)
    , m_progressBar(new QProgressBar)
    , m_reply(Q_NULLPTR)
    , m_file(Q_NULLPTR)
    , m_httpRequestAborted(false)
{
    setWindowTitle(tr("HTTP getter"));

    connect(&m_qnam, &QNetworkAccessManager::authenticationRequired,
            this, &HttpDialog::slotAuthenticationRequired);
#ifndef QT_NO_SSL
    qDebug() << "SSL is defined";
    connect(&m_qnam, &QNetworkAccessManager::sslErrors,
            this, &HttpDialog::sslErrors);
#endif


    QFormLayout *formLayout = new QFormLayout;
    m_urlLineEdit->setClearButtonEnabled(true);
    connect(m_urlLineEdit, &QLineEdit::textChanged, this, &HttpDialog::enableDownloadButton);
    formLayout->addRow(tr("URL"), m_urlLineEdit);
    QString downloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (downloadDirectory.isEmpty() || !QFileInfo(downloadDirectory).isDir()) {
        downloadDirectory = QDir::currentPath();
    }

    m_downloadDirectoryLineEdit->setText(QDir::toNativeSeparators(downloadDirectory));
    formLayout->addRow(tr("Download directory"), m_downloadDirectoryLineEdit);
    formLayout->addRow(tr("Default file"), m_defaultFileLineEdit);

    m_lunchCheckBox->setChecked(true);
    formLayout->addRow(m_lunchCheckBox);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);

    mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    m_statusLabel->setWordWrap(true);
    mainLayout->addWidget(m_statusLabel);
    m_downloadButton->setDefault(true);
    connect(m_downloadButton, &QAbstractButton::clicked, this, &HttpDialog::downloadFile);
    QPushButton *quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(m_downloadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(m_cancelButton, QDialogButtonBox::ActionRole);
    m_cancelButton->setEnabled(false);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(m_progressBar);
    m_progressBar->setMinimum(0);
    m_progressBar->setValue(0);

    connect(m_cancelButton, &QAbstractButton::clicked, this, &HttpDialog::cancelDownload);
    m_urlLineEdit->setFocus();
}

HttpDialog::~HttpDialog() {
}

void HttpDialog::startRequest(const QUrl &requestedUrl) {
    m_progressBar->setMaximum(0);
    m_progressBar->setValue(0);

    m_url = requestedUrl;
    m_httpRequestAborted = false;

    m_reply = m_qnam.get(QNetworkRequest(m_url));
    connect(m_reply, &QNetworkReply::finished, this, &HttpDialog::httpFinished);
    connect(m_reply, &QIODevice::readyRead, this, &HttpDialog::httpReadyRead);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &HttpDialog::networkReplyProgress);

    m_statusLabel->setText(tr("Downloading %1...").arg(m_url.toString()));
}

void HttpDialog::downloadFile() {
    const QString urlSpec = m_urlLineEdit->text().trimmed();
    if (urlSpec.isEmpty()) {
        return;
    }

    const QUrl newUrl = QUrl::fromUserInput(urlSpec);
    if (!newUrl.isValid()) {
        QMessageBox::information(this, tr("Error"),
                                 tr("Invalid URL %1: %2").arg(urlSpec, newUrl.errorString()));
        return;
    }

    QString fileName = newUrl.fileName();
    if (fileName.isEmpty()) {
        fileName = m_defaultFileLineEdit->text().trimmed();
    }

    if (fileName.isEmpty()) {
        fileName = defaultFileName;
    }

    QString downloadDirectory = QDir::cleanPath(m_downloadDirectoryLineEdit->text().trimmed());
    if (!downloadDirectory.isEmpty() && QFileInfo(downloadDirectory).isDir()) {
        fileName.prepend(downloadDirectory + '/');
    }

    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("Overwrite existing file"),
                                  tr("There already exists a file called %1 in the current directory. Overwrite?")
                                  .arg(fileName),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
            return;
        }

        QFile::remove(fileName);
    }

    m_file = openFileForWrite(fileName);

    if (!m_file) {
        return;
    }

    m_downloadButton->setEnabled(false);
    m_cancelButton->setEnabled(true);

    //schedule request
    startRequest(newUrl);
}

void HttpDialog::cancelDownload() {
    m_statusLabel->setText(tr("Download canceled"));
    m_httpRequestAborted = true;
    m_reply->abort();
    m_downloadButton->setEnabled(true);
    m_cancelButton->setEnabled(false);
}

void HttpDialog::httpFinished() {
    QFileInfo fi;
    if (m_file) {
        fi.setFile(m_file->fileName());
        m_file->close();
        delete m_file;
        m_file = Q_NULLPTR;
    }

    if (m_httpRequestAborted) {
        m_reply->deleteLater();
        m_reply = Q_NULLPTR;
        return;
    }

    if (m_reply->error()) {
        QFile::remove(fi.absoluteFilePath());
        m_statusLabel->setText(tr("Download failed:\n%1").arg(m_reply->errorString()));
        m_downloadButton->setEnabled(true);
        m_cancelButton->setEnabled(false);
        m_reply->deleteLater();
        m_reply = Q_NULLPTR;
        return;
    }

    const QVariant rediractionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    m_reply->deleteLater();
    m_reply = Q_NULLPTR;

    if (!rediractionTarget.isNull()) {
        const QUrl redirectedUrl = m_url.resolved(rediractionTarget.toUrl());
        if (QMessageBox::question(this, tr("Redirect")
                , tr("Redirect to %1 ?").arg(redirectedUrl.toString())
                , QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ) {
            m_downloadButton->setEnabled(true);
            m_cancelButton->setEnabled(false);
            return;
        }

        m_file = openFileForWrite(fi.absolutePath());
        if (!m_file) {
            m_downloadButton->setEnabled(true);
            m_cancelButton->setEnabled(false);
            return;
        }

        startRequest(redirectedUrl);
        return;
    }

    m_statusLabel->setText(tr("Downloaded %1 bytes to %2\nin\n%3")
                           .arg(fi.size()).arg(fi.fileName()).arg(QDir::toNativeSeparators(fi.absolutePath())));
    if (m_lunchCheckBox->isChecked()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absoluteFilePath()));
    }

    m_downloadButton->setEnabled(true);
    m_cancelButton->setEnabled(false);
}

void HttpDialog::httpReadyRead() {
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (m_file) {
        m_file->write(m_reply->readAll());
    }
}

void HttpDialog::enableDownloadButton() {
    m_downloadButton->setEnabled(!m_urlLineEdit->text().isEmpty());
}

void HttpDialog::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator) {
    QDialog authenticationDialog;
    Ui::Dialog ui;
    ui.setupUi(&authenticationDialog);
    authenticationDialog.adjustSize();
    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm(), m_url.host()));

    // Did the URL have information? Fill the UI
    // This is only relevant if the URL-supplied credentials were wrong
    ui.userEdit->setText(m_url.userName());
    ui.passwordEdit->setText(m_url.password());

    if (authenticationDialog.exec() == QDialog::Accepted) {
        authenticator->setUser(ui.userEdit->text());
        authenticator->setPassword(ui.passwordEdit->text());
    }
}

#ifndef QT_NO_SSL
void HttpDialog::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    Q_UNUSED(reply);
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }

    if (QMessageBox::warning(this, tr("SSL Errors"),
                             tr("One or more SSL errors has occurred:\n%1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        m_reply->ignoreSslErrors();
    }
}
#endif

void HttpDialog::networkReplyProgress(qint64 bytesRead, qint64 bytesTotal) {
    m_progressBar->setMaximum(bytesTotal);
    m_progressBar->setValue(bytesRead);
}

QFile *HttpDialog::openFileForWrite(const QString &fileName) {
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Error"),
                                 tr("Unable to save file %1: %2")
                                    .arg(QDir::toNativeSeparators(fileName), file->errorString()));
        return Q_NULLPTR;
    }

    return file.take();
}
