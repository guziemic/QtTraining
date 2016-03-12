#include "mysortfilterproxymodel.h"

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void MySortFilterProxyModel::setFilterMinimumDate(const QDate &date) {
    m_minDate = date;
    invalidateFilter();
}

void MySortFilterProxyModel::setFilterMaximumDate(const QDate &date) {
    m_maxDate = date;
    invalidateFilter();
}

bool MySortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
//    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex index1 = sourceModel()->index(source_row, 2, source_parent);
    QModelIndex index2 = sourceModel()->index(source_row, 3, source_parent);

    return (//sourceModel()->data(index0).toString().contains(filterRegExp()) ||
            sourceModel()->data(index1).toString().contains(filterRegExp()))
            && dateInRange(sourceModel()->data(index2).toDate());
}

// aim is to sort senders by their email adresses (field in database contains Name <address@.com> as string
bool MySortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const {
    QVariant leftData = sourceModel()->data(source_left);
    QVariant rightData = sourceModel()->data(source_right);

    if (leftData.type() == QVariant::String) {
        static QRegExp emailPattern("[\\w\\.]*@[\\w\\.]*)");

        QString leftString = leftData.toString();
        if (source_left.column() == 2 && emailPattern.indexIn(leftString) != -1) {
            leftString = emailPattern.cap(1);
        }

        QString rightString = rightData.toString();
        if (source_right.column() == 2 && emailPattern.indexIn(rightString) != -1) {
            rightString = emailPattern.cap(1);
        }

        return QString::localeAwareCompare(leftString, rightString) < 0;
    }

    //forward other types to parent class
    return QSortFilterProxyModel::lessThan(source_left, source_right);
}

bool MySortFilterProxyModel::dateInRange(const QDate &date) const {
    return (!m_minDate.isValid() || date > m_minDate)
            && (!m_maxDate.isValid() || date < m_maxDate);
}
