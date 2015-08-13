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

    //Header text to center
    //ui->listTreeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
    //ui->listTreeWidget->header()->setFont(QFont("Serif", 50));
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
    //UI buttons
    connect(ui->compressButton, SIGNAL(released()), this, SLOT(on_actionCompress_triggered()));
    connect(ui->addFilesButton, SIGNAL(released()), this, SLOT(on_actionAdd_pictures_triggered()));
    connect(ui->addFolderButton, SIGNAL(released()), this, SLOT(on_actionAdd_folder_triggered()));
    connect(ui->removeItemButton, SIGNAL(released()), this, SLOT(on_actionRemove_items_triggered()));
    connect(ui->clearButton, SIGNAL(released()), ui->listTreeWidget, SLOT(clear()));

    //Install event filter for buttons
    ui->addFilesButton->installEventFilter(this);
    ui->addFolderButton->installEventFilter(this);
    ui->compressButton->installEventFilter(this);
    ui->removeItemButton->installEventFilter(this);
    ui->clearButton->installEventFilter(this);
}

//Button hover functions
bool CaesiumPH::eventFilter(QObject *obj, QEvent *event) {
    if (obj == (QObject*) ui->addFilesButton) {
        if (event->type() == QEvent::Enter) {
            ui->addFilesButton->setIcon(QIcon(":/icons/ui/add_hover.png"));
            return true;
        } else if (event->type() == QEvent::Leave){
            ui->addFilesButton->setIcon(QIcon(":/icons/ui/add.png"));
            return true;
        } else {
            return false;
        }
    } else if (obj == (QObject*) ui->addFolderButton) {
        if (event->type() == QEvent::Enter) {
            ui->addFolderButton->setIcon(QIcon(":/icons/ui/folder_hover.png"));
            return true;
        } else if (event->type() == QEvent::Leave){
            ui->addFolderButton->setIcon(QIcon(":/icons/ui/folder.png"));
            return true;
        } else {
            return false;
        }
    } else if (obj == (QObject*) ui->compressButton) {
        if (event->type() == QEvent::Enter) {
            ui->compressButton->setIcon(QIcon(":/icons/ui/compress_hover.png"));
            return true;
        } else if (event->type() == QEvent::Leave){
            ui->compressButton->setIcon(QIcon(":/icons/ui/compress.png"));
            return true;
        } else {
            return false;
        }
    } else if (obj == (QObject*) ui->removeItemButton) {
        if (event->type() == QEvent::Enter) {
            ui->removeItemButton->setIcon(QIcon(":/icons/ui/remove_hover.png"));
            return true;
        } else if (event->type() == QEvent::Leave){
            ui->removeItemButton->setIcon(QIcon(":/icons/ui/remove.png"));
            return true;
        } else {
            return false;
        }
    } else if (obj == (QObject*) ui->clearButton) {
        if (event->type() == QEvent::Enter) {
            ui->clearButton->setIcon(QIcon(":/icons/ui/clear_hover.png"));
            return true;
        } else if (event->type() == QEvent::Leave){
            ui->clearButton->setIcon(QIcon(":/icons/ui/clear.png"));
            return true;
        } else {
            return false;
        }
    } else {
        //Pass the event on to the parent class
        return QWidget::eventFilter(obj, event);
    }
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
    //BUG Sometimes files are empty. Check it out.
    cclt_optimize(QStringToChar(item->text(4)),
                  QStringToChar(item->text(4) + ".cmp.jpg"),
                  0,
                  QStringToChar(item->text(4)));
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

#ifdef _WIN32
    QThreadPool::globalInstance()->setMaxThreadCount(1);
#endif
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




