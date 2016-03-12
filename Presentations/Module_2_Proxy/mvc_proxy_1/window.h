#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "mysortfilterproxymodel.h"

class QAbstractItemModel;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSortFilterProxyModel;
class QAbstractProxyModel;
class QTreeView;
class QRadioButton;
class QDateEdit;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

    void setSourceModel(QAbstractItemModel *model);

private slots:
    void filterRegExpChanged();
    void filterColumnChanged(int column);
    void sortChanged();
    void radioToggled();
    void manualFilterToggled();
    void dateFilterChanged();

private:
    void enableAdvancedMode();
    void disableAdvanceMode();

    QGroupBox *sourceGroupBox;
    QGroupBox *proxyGroupBox;
    QTreeView *sourceView;
    QTreeView *proxyView;
    QCheckBox *filterCaseSensitivityCheckBox;
    QCheckBox *sortCaseSessitivityCheckBox;
    QCheckBox *customModelCheckBox;
    QLabel *filterPatternLabel;
    QLabel *filterSyntaxLabel;
    QLabel *filterColumnLabel;
    QLineEdit *filterPatternLineEdit;
    QComboBox *filterSyntaxComboBox;
    QComboBox *filterColumnComboBox;
    QRadioButton *simpleRadioBtn;
    QRadioButton *advancedRadioBtn;
    QAbstractItemModel *sourceModel;
    QSortFilterProxyModel *sortFilterModel;
    MySortFilterProxyModel *customSortFilterModel;

    QLabel *fromDateLabel;
    QLabel *toDateLabel;
    QDateEdit *fromDateEdit;
    QDateEdit *toDateEdit;
};

#endif // WINDOW_H
