#include "httpdialog.h"

#include <QtWidgets>
#include <QtNetwork>

static const char defaultUrl[] = "http://www.tapetus.pl/obrazki/n/140343_las-promienie-slonca.jpg";
static const char defaultFileName[] = "out.txt";

HttpDialog::HttpDialog(QWidget *parent)
    : QDialog(parent)
    , m_statusLabel(new QLabel(tr("Please enter the URL \n")))
    , m_urlLineEdit(new QLineEdit(defaultUrl))
    , m_downloadButton(new QPushButton(tr("Download")))
    , m_lunchCheckBox(new QCheckBox("Lunch file"))
    , m_defaultFileLineEdit(new QLineEdit(defaultFileName))
    , m_downloadDirectoryLineEdit(new QLineEdit)
    , m_reply(Q_NULLPTR)
    , m_file(Q_NULLPTR)
    , m_httpRequestAborted(false)
{
    setWindowTitle(tr("HTTP getter"));

    connect(&m_qnam, &QNetworkAccessManager::authenticationRequired,
            this, &HttpDialog::slotAuthenticationRequired);
#ifndef QT_NO_SSL
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
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    mainLayout->addWidget(buttonBox);

    m_urlLineEdit->setFocus();
}

HttpDialog::~HttpDialog()
{
}

void HttpDialog::startRequest(const QUrl &requestedUrl) {

}

void HttpDialog::downloadFile() {

}

void HttpDialog::cancelDownload() {

}

void HttpDialog::httpFinished() {

}

void HttpDialog::httpReadyRead() {

}

void HttpDialog::enableDownloadButton() {

}

void HttpDialog::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator) {

}

#ifndef QT_NO_SSL
void HttpDialog::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {

}
#endif
