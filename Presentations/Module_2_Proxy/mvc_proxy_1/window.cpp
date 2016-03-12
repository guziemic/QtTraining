#include <QtWidgets>

#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    //create sort filter models
    sortFilterModel = new QSortFilterProxyModel(this);
    customSortFilterModel = new MySortFilterProxyModel;

    // create views
    sourceView = new QTreeView;
    sourceView->setRootIsDecorated(false);
    sourceView->setAlternatingRowColors(true);

    proxyView = new QTreeView;
    proxyView->setRootIsDecorated(false);
    proxyView->setAlternatingRowColors(true);
    proxyView->setModel(sortFilterModel);
    proxyView->setSortingEnabled(true);

    //layouting
    sourceGroupBox = new QGroupBox(tr("Original Model"));
    proxyGroupBox = new QGroupBox(tr("Sorted/Filtered model"));

    QHBoxLayout *sourceLayout = new QHBoxLayout;
    sourceLayout->addWidget(sourceView);
    sourceGroupBox->setLayout(sourceLayout);

    QGridLayout *proxyLayout = new QGridLayout;
    proxyLayout->addWidget(proxyView, 0, 0, 1, 3);

    proxyGroupBox->setLayout(proxyLayout);

    simpleRadioBtn = new QRadioButton(tr("Si&mple"));
    advancedRadioBtn = new QRadioButton(tr("&Advanced"));
    customModelCheckBox = new QCheckBox(tr("&Custom model"));
    QHBoxLayout *radioButtonsLayout = new QHBoxLayout;
    radioButtonsLayout->addWidget(simpleRadioBtn);
    radioButtonsLayout->addWidget(advancedRadioBtn);
    radioButtonsLayout->addWidget(customModelCheckBox);
    QWidget *radioWidget = new QWidget;
    radioWidget->setLayout(radioButtonsLayout);

    simpleRadioBtn->setChecked(true);
    customModelCheckBox->setChecked(false);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(radioWidget);
    mainLayout->addWidget(sourceGroupBox);
    mainLayout->addWidget(proxyGroupBox);

    setLayout(mainLayout);
    setWindowTitle(tr("Basic sort / filter model"));
    resize(600, 450);


    //adding more advance controls
    sortCaseSessitivityCheckBox = new QCheckBox(tr("Case sensitive sorting"));
    filterCaseSensitivityCheckBox = new QCheckBox(tr("Case sensitive filter"));

    filterPatternLineEdit = new QLineEdit;
    filterPatternLabel = new QLabel(tr("&Filter pattern:"));
    filterPatternLabel->setBuddy(filterPatternLineEdit);

    filterSyntaxComboBox = new QComboBox;
    filterSyntaxComboBox->addItem(tr("Regular expression"), QRegExp::RegExp);
    filterSyntaxComboBox->addItem(tr("Wildcard"), QRegExp::Wildcard);
    filterSyntaxComboBox->addItem(tr("Fixed string"), QRegExp::FixedString);
    filterSyntaxLabel = new QLabel(tr("Filter &syntax:"));
    filterSyntaxLabel->setBuddy(filterSyntaxComboBox);

    filterColumnComboBox = new QComboBox;
    filterColumnComboBox->addItem(tr("Hotel"));
    filterColumnComboBox->addItem(tr("Price"));
    filterColumnComboBox->addItem(tr("Sender"));
    filterColumnComboBox->addItem(tr("Date"));
    filterColumnLabel = new QLabel(tr("Filter &column"));
    filterColumnLabel->setBuddy(filterColumnComboBox);

    connect(filterPatternLineEdit, &QLineEdit::textChanged, this, &Window::filterRegExpChanged);
    connect(filterSyntaxComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &Window::filterRegExpChanged);
    connect(filterColumnComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &Window::filterColumnChanged);
    connect(filterCaseSensitivityCheckBox, &QCheckBox::toggled, this, &Window::filterRegExpChanged);
    connect(sortCaseSessitivityCheckBox, &QCheckBox::toggled, this, &Window::sortChanged);

    //date filtering widgets
    fromDateEdit = new QDateEdit;
    fromDateEdit->setDate(QDate(1970, 01, 01));
    fromDateLabel = new QLabel(tr("&From:"));
    fromDateLabel->setBuddy(fromDateEdit);

    toDateEdit = new QDateEdit;
    toDateEdit->setDate(QDate(2099, 12, 31));
    toDateLabel = new QLabel(tr("&To"));
    toDateLabel->setBuddy(toDateEdit);
    //and connect it to handling slots
    connect(fromDateEdit, &QDateEdit::dateChanged, this, &Window::dateFilterChanged);
    connect(toDateEdit, &QDateEdit::dateChanged, this, &Window::dateFilterChanged);

    fromDateEdit->hide();
    fromDateLabel->hide();
    toDateEdit->hide();
    toDateLabel->hide();

    proxyLayout->addWidget(filterPatternLabel, 1, 0);
    proxyLayout->addWidget(filterPatternLineEdit, 1, 1, 1, 2);
    proxyLayout->addWidget(filterSyntaxLabel, 2, 0);
    proxyLayout->addWidget(filterSyntaxComboBox, 2, 1, 1, 2);
    proxyLayout->addWidget(filterColumnLabel, 3, 0);
    proxyLayout->addWidget(filterColumnComboBox, 3, 1, 1, 2);
    proxyLayout->addWidget(filterCaseSensitivityCheckBox, 4, 0 , 1, 2);
    proxyLayout->addWidget(sortCaseSessitivityCheckBox, 4, 2);
    proxyLayout->addWidget(fromDateLabel, 5, 0);
    proxyLayout->addWidget(fromDateEdit, 5, 1, 1, 2);
    proxyLayout->addWidget(toDateLabel, 6, 0);
    proxyLayout->addWidget(toDateEdit, 6, 1, 1, 2);

    disableAdvanceMode();

    connect(simpleRadioBtn, &QRadioButton::clicked, this, &Window::radioToggled);
    connect(advancedRadioBtn, &QRadioButton::clicked, this, &Window::radioToggled);
    connect(customModelCheckBox, &QCheckBox::clicked, this, &Window::manualFilterToggled);
}

Window::~Window()
{
}

void Window::setSourceModel(QAbstractItemModel *model) {
    sortFilterModel->setSourceModel(model);
    customSortFilterModel->setSourceModel(model);
    sourceView->setModel(model);
    sourceModel = model;
}

void Window::filterRegExpChanged() {
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(filterSyntaxComboBox->itemData(filterSyntaxComboBox->currentIndex()).toInt());
    Qt::CaseSensitivity caseSensitivity = filterCaseSensitivityCheckBox->isChecked() ? Qt::CaseSensitive
                                                                                     : Qt::CaseInsensitive;

    QRegExp regExp(filterPatternLineEdit->text(), caseSensitivity, syntax);
    if (customModelCheckBox->checkState() == Qt::Checked) {
        customSortFilterModel->setFilterRegExp(regExp);
    } else {
        sortFilterModel->setFilterRegExp(regExp);
    }
}

void Window::filterColumnChanged(int column) {
    Q_UNUSED(column);
    if (customModelCheckBox->checkState() == Qt::Checked) {
        customSortFilterModel->setFilterKeyColumn(filterColumnComboBox->currentIndex());
    } else {
        sortFilterModel->setFilterKeyColumn(filterColumnComboBox->currentIndex());
    }
}

void Window::sortChanged() {
    if (customModelCheckBox->checkState() == Qt::Checked) {
        customSortFilterModel->setSortCaseSensitivity(sortCaseSessitivityCheckBox->isChecked() ? Qt::CaseSensitive
                                                                                               : Qt::CaseInsensitive);
    } else {
        sortFilterModel->setSortCaseSensitivity(sortCaseSessitivityCheckBox->isChecked() ? Qt::CaseSensitive
                                                                                         : Qt::CaseInsensitive);
    }
}

void Window::radioToggled() {
    if (simpleRadioBtn->isChecked()) {
        qDebug() << "disableAdvanceMode";
        disableAdvanceMode();
    } else if (advancedRadioBtn->isChecked()) {
        qDebug() << "enableAdvancedMode";
        enableAdvancedMode();
    } else {
        qDebug() << "No button to handle";
    }
}

void Window::manualFilterToggled() {
    // load custom proxy model object
    if (customModelCheckBox->checkState() == Qt::Checked) {
        proxyView->setModel(customSortFilterModel);
        sortChanged();
        filterRegExpChanged();
        //disable filter column set
        customSortFilterModel->setFilterKeyColumn(-1);
        filterColumnLabel->hide();
        filterColumnComboBox->hide();
        //show rows for date set
        fromDateEdit->show();
        fromDateLabel->show();
        toDateEdit->show();
        toDateLabel->show();
    } else {
        proxyView->setModel(sortFilterModel);
        sortChanged();
        filterRegExpChanged();
        //enable filter column
        sortFilterModel->setFilterKeyColumn(filterColumnComboBox->currentIndex());
        filterColumnLabel->show();
        filterColumnComboBox->show();
        //hide date set
        fromDateEdit->hide();
        fromDateLabel->hide();
        toDateEdit->hide();
        toDateLabel->hide();
    }
}

void Window::dateFilterChanged() {
    customSortFilterModel->setFilterMinimumDate(fromDateEdit->date());
    customSortFilterModel->setFilterMaximumDate(toDateEdit->date());
}

void Window::enableAdvancedMode() {
    proxyView->sortByColumn(2, Qt::AscendingOrder);
    filterColumnComboBox->setCurrentIndex(2);

    filterPatternLineEdit->setText("Andy|Ann");
    filterCaseSensitivityCheckBox->setChecked(true);
    sortCaseSessitivityCheckBox->setChecked(true);

    filterPatternLabel->show();
    filterPatternLineEdit->show();
    filterSyntaxLabel->show();
    filterSyntaxComboBox->show();
    filterColumnLabel->show();
    filterColumnComboBox->show();
    filterCaseSensitivityCheckBox->show();
    sortCaseSessitivityCheckBox->show();
    customModelCheckBox->setEnabled(true);
}

void Window::disableAdvanceMode() {
    filterPatternLabel->hide();
    filterPatternLineEdit->hide();
    filterSyntaxLabel->hide();
    filterSyntaxComboBox->hide();
    filterColumnLabel->hide();
    filterColumnComboBox->hide();
    filterCaseSensitivityCheckBox->hide();
    sortCaseSessitivityCheckBox->hide();

    filterPatternLineEdit->setText("");
    filterCaseSensitivityCheckBox->setChecked(false);
    sortCaseSessitivityCheckBox->setChecked(false);
    customModelCheckBox->setEnabled(false);
}
