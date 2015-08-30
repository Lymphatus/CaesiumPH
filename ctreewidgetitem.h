#ifndef CTREEWIDGETITEM_H
#define CTREEWIDGETITEM_H

#include <QTreeWidgetItem>

class CTreeWidgetItem : public QTreeWidgetItem
{
public:
    CTreeWidgetItem(QTreeWidget* parent) : QTreeWidgetItem(parent) {}
    CTreeWidgetItem(QTreeWidget* parent, QStringList list) : QTreeWidgetItem(parent, list) {}
private:
    bool operator< (const QTreeWidgetItem &other) const {
        int column = treeWidget()->sortColumn();
        return text(column).toLower() < other.text(column).toLower();
    }
};

#endif // CTREEWIDGETITEM_H
