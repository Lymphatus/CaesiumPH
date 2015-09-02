#ifndef CTREEWIDGETITEM_H
#define CTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <QFileInfo>

class CTreeWidgetItem : public QTreeWidgetItem
{
public:
    CTreeWidgetItem(QTreeWidget* parent) : QTreeWidgetItem(parent) {}
    CTreeWidgetItem(QTreeWidget* parent, QStringList list) : QTreeWidgetItem(parent, list) {}
private:
    bool operator< (const QTreeWidgetItem &other) const {
        int column = treeWidget()->sortColumn();
        switch (column) {
        case 1:
            return QFileInfo(text(4)).size() < QFileInfo(other.text(4)).size();
        case 2:
            //Sort by compressed size
            return QFileInfo(text(4)).size() < QFileInfo(other.text(4)).size();
        default:
            //Sort by path
            return text(column).toLower() < other.text(column).toLower();
        }
    }
};

#endif // CTREEWIDGETITEM_H
