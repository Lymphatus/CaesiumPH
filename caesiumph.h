#ifndef CAESIUMPH_H
#define CAESIUMPH_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFutureWatcher>
#include <QTime>

namespace Ui {
class CaesiumPH;
}

class CaesiumPH : public QMainWindow
{
    Q_OBJECT

public:
    explicit CaesiumPH(QWidget *parent = 0);
    bool eventFilter(QObject *obj, QEvent *event);
    ~CaesiumPH();

private slots:
    void initializeConnections();
    void initializeUI();
    void on_actionAbout_CaesiumPH_triggered();
    void showImportProgressDialog(QStringList);
    void on_actionAdd_pictures_triggered();
    void on_actionAdd_folder_triggered();
    void on_actionRemove_items_triggered();
    void on_actionCompress_triggered();
    void compressionStarted();
    void compressionFinished();
    void on_sidePanelDockWidget_topLevelChanged(bool topLevel);
    void on_sidePanelDockWidget_visibilityChanged(bool visible);
    void on_showSidePanelButton_clicked(bool checked);
    void on_listTreeWidget_itemSelectionChanged();
    QImage loadImagePreview(QString path);
    void finishPreviewLoading();


private:
    Ui::CaesiumPH *ui;
    QFutureWatcher<QImage> imageWatcher; //Image preview loader

};

#endif // CAESIUMPH_H
