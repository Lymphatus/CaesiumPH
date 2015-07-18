#ifndef CAESIUMPH_H
#define CAESIUMPH_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QTime>

namespace Ui {
class CaesiumPH;
}

class CaesiumPH : public QMainWindow
{
    Q_OBJECT

public:
    explicit CaesiumPH(QWidget *parent = 0);
    ~CaesiumPH();

private slots:
    void initializeConnections();
    void on_actionAbout_CaesiumPH_triggered();
    void showImportProgressDialog(QStringList);
    void on_actionAdd_pictures_triggered();
    void on_actionAdd_folder_triggered();
    void on_actionRemove_items_triggered();
    void on_actionCompress_triggered();
    void compressionStarted();
    void compressionFinished();

private:
    Ui::CaesiumPH *ui;
};

#endif // CAESIUMPH_H
