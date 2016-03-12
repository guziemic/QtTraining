#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>


/**
 * @brief The MySortFilterProxyModel class
 *
 * Filters our data by specifying a given period of time and
 * sort the senders by their email addresses.
 */
class MySortFilterProxyModel : public QSortFilterProxyModel
{
public:
    MySortFilterProxyModel(QObject *parent = 0);

    void setFilterMinimumDate(const QDate &date);
    QDate filterMinimumDate() const {
        return m_minDate;
    }

    void setFilterMaximumDate(const QDate &date);
    QDate filterMaximumDate() const {
        return m_maxDate;
    }

protected:
    //function to only accept rows with valid dates
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;
    // sort the senders by their email addresses
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE;

private:
    // determine if a date is valid
    bool dateInRange(const QDate &date) const;

    QDate m_minDate;
    QDate m_maxDate;
};

#endif // MYSORTFILTERPROXYMODEL_H
