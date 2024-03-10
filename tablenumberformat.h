#ifndef TABLENUMBERFORMAT_H
#define TABLENUMBERFORMAT_H

#include <QStyledItemDelegate>
#include <QObject>

class TableNumberFormat : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TableNumberFormat(QObject *parent = nullptr);
    virtual QString displayText(const QVariant &value, const QLocale &locale) const;
};

#endif // TABLENUMBERFORMAT_H
