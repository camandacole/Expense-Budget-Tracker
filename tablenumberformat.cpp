#include "tablenumberformat.h"

TableNumberFormat::TableNumberFormat(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QString TableNumberFormat::displayText(const QVariant &value, const QLocale &locale) const{
    QString formattedNum = locale.toString(value.toDouble(), 'f', 2);
    return formattedNum;
}
