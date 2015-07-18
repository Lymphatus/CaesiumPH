#include "caesiumph.h"
#include "ui_caesiumph.h"
#include "aboutdialog.h"
#include "global.h"
#include "lossless.h"
#include "cimageinfo.h"

#include <QProgressDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QGraphicsPixmapItem>
#include <QFileInfo>
#include <QtConcurrent>
#include <QFuture>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QDebug>

CaesiumPH::CaesiumPH(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CaesiumPH)
{
    ui->setupUi(this);
    initializeConnections();
}

CaesiumPH::~CaesiumPH()
{
    delete ui;
}

void CaesiumPH::initializeConnections() {
    //Edit menu
    //List clear
    connect(ui->actionClear_list, SIGNAL(triggered()), ui->listTreeWidget, SLOT(clear()));
    //List select all
    connect(ui->actionSelect_all, SIGNAL(triggered()), ui->listTreeWidget, SLOT(selectAll()));
    //Compress Button
    connect(ui->compressButton, SIGNAL(released()), this, SLOT(on_actionCompress_triggered()));
}

void CaesiumPH::on_actionAbout_CaesiumPH_triggered()
{
    //Start the about dialog
    AboutDialog* ad = new AboutDialog(this);
    ad->setWindowFlags(Qt::Tool | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
    ad->show();
}

void CaesiumPH::on_actionAdd_pictures_triggered()
{
    //Generate file dialog for import and slot connection when the job is done
    QFileDialog* fd = new QFileDialog(this,
                                      tr("Import files..."),
                                      QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0),
                                      inputFilter);
    fd->setFileMode(QFileDialog::ExistingFiles);
    connect(fd, SIGNAL(filesSelected(QStringList)), this, SLOT(showImportProgressDialog(QStringList)));
    //Show it
    fd->show();
}

void CaesiumPH::showImportProgressDialog(QStringList list) {

    QProgressDialog progress(tr("Importing..."), tr("Cancel"), 0, list.count(), this);
    progress.show();
    progress.setWindowModality(Qt::WindowModal);

    QString prefix = ""; //Prefix for full path folder import

    if (QDir(list[0]).exists()) {
        prefix = list[0] + QDir::separator();
        list = QDir(list[0]).entryList(inputFilterList, QDir::Files);
    }

    for (int i = 0; i < list.size(); i++) {
        //Generate new CImageInfo
        CImageInfo* currentItemInfo = new CImageInfo(prefix + list.at(i));

        //Populate list
        ui->listTreeWidget->addTopLevelItem(new QTreeWidgetItem(ui->listTreeWidget,
                                                                QStringList() << currentItemInfo->getBaseName()
                                                                << currentItemInfo->getFormattedSize()
                                                                << ""
                                                                << ""
                                                                << currentItemInfo->getFullPath()));

        progress.setValue(i);

        if (progress.wasCanceled())
            break;
    }
    progress.setValue(list.count());
}

void CaesiumPH::on_actionAdd_folder_triggered()
{
    QFileDialog* fd = new QFileDialog(this,
                                      tr("Select a folder to import..."),
                                      QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0),
                                      inputFilter);
    fd->setFileMode(QFileDialog::DirectoryOnly);
    connect(fd, SIGNAL(filesSelected(QStringList)), this, SLOT(showImportProgressDialog(QStringList)));
    //Show it
    fd->show();
}

void CaesiumPH::on_actionRemove_items_triggered()
{
    foreach (QTreeWidgetItem *i, ui->listTreeWidget->selectedItems()) {
        ui->listTreeWidget->takeTopLevelItem(ui->listTreeWidget->indexOfTopLevelItem(i));
    }
}

extern void compressRoutine(QTreeWidgetItem* item) {
    qDebug() << item->text(4);
    //BUG Sometimes files are empty. Check it out.
    cclt_optimize(item->text(4).toLocal8Bit().data(),
                  (item->text(4) + ".cmp.jpg").toLocal8Bit().data(),
                  1,
                  item->text(4).toLocal8Bit().data());
    //Gets new file info
    //TODO Change it, it must point the right output
    QFileInfo* fileInfo = new QFileInfo(item->text(4) + ".cmp.jpg");
    item->setText(2, formatSize(fileInfo->size()));
    item->setText(3, getRatio((new QFileInfo(item->text(4)))->size(), fileInfo->size()));
}

void CaesiumPH::on_actionCompress_triggered()
{
    //Register metatype for emitting changes
    qRegisterMetaType<QVector<int> >("QVector<int>");

    //Setting up a progress dialog
    QProgressDialog progressDialog;
    progressDialog.setWindowTitle(tr("CaesiumPH"));
    progressDialog.setLabelText(tr("Compressing..."));

    //Holds the list
    QList<QTreeWidgetItem*> list;

    //Setup watcher
    QFutureWatcher<void> watcher;

    //Setting up connections
    //Progress dialog
    connect(&watcher, SIGNAL(progressValueChanged(int)), &progressDialog, SLOT(setValue(int)));
    connect(&watcher, SIGNAL(progressRangeChanged(int, int)), &progressDialog, SLOT(setRange(int,int)));
    connect(&watcher, SIGNAL(finished()), &progressDialog, SLOT(reset()));
    connect(&progressDialog, SIGNAL(canceled()), &watcher, SLOT(cancel()));
    //TODO Work on that
    //connect(&watcher, SIGNAL(progressTextChanged(QString)), &progressDialog, SLOT(setLabelText(QString)));
    //Connect two slots for handling compression start/finish
    connect(&watcher, SIGNAL(started()), this, SLOT(compressionStarted()));
    connect(&watcher, SIGNAL(finished()), this, SLOT(compressionFinished()));

    //Gets the list filled
    for (int i = 0; i < ui->listTreeWidget->topLevelItemCount(); i++) {
        list.append(ui->listTreeWidget->topLevelItem(i));
    }

    //And start
    watcher.setFuture(QtConcurrent::map(list, compressRoutine));

    //Show the dialog
    progressDialog.exec();

}

void CaesiumPH::compressionStarted() {
    //Start monitoring time while compressing
    qDebug() << QTime::currentTime();
}

void CaesiumPH::compressionFinished() {
    //Get elapsed time of the compression
    qDebug() << QTime::currentTime();
}




