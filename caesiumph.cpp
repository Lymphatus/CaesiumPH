#include "caesiumph.h"
#include "ui_caesiumph.h"
#include "aboutdialog.h"
#include "utils.h"
#include "lossless.h"
#include "cimageinfo.h"
#include "exif.h"

#include <QProgressDialog>
#include <QFileDialog>
#include <QStandardPaths>
#include <QGraphicsPixmapItem>
#include <QFileInfo>
#include <QtConcurrent>
#include <QFuture>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QImageReader>

#include <QDebug>

CaesiumPH::CaesiumPH(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CaesiumPH)
{
    ui->setupUi(this);
    initializeConnections();
    initializeUI();

#ifdef _WIN32
    QThreadPool::globalInstance()->setMaxThreadCount(1);
#endif

    //Header text to center
    //ui->listTreeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
    //ui->listTreeWidget->header()->setFont(QFont("Serif", 50));
}

CaesiumPH::~CaesiumPH() {
    delete ui;
}

void CaesiumPH::initializeUI() {
    //Set the side panel visible
    //TODO Make this a preference

    //Install event filter for buttons
    ui->addFilesButton->installEventFilter(this);
    ui->addFolderButton->installEventFilter(this);
    ui->compressButton->installEventFilter(this);
    ui->removeItemButton->installEventFilter(this);
    ui->clearButton->installEventFilter(this);
    ui->showSidePanelButton->installEventFilter(this);

    //Set the headers size
    ui->listTreeWidget->header()->resizeSection(0, 180);
    ui->listTreeWidget->header()->resizeSection(1, 100);
    ui->listTreeWidget->header()->resizeSection(2, 100);
    ui->listTreeWidget->header()->resizeSection(3, 80);
    ui->listTreeWidget->header()->resizeSection(4, 100);

    ui->menuBar->setVisible(false);
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
    } else if (obj == (QObject*) ui->showSidePanelButton) {
        if (!ui->sidePanelDockWidget->isVisible()) {
            if (event->type() == QEvent::Enter) {
                ui->showSidePanelButton->setIcon(QIcon(":/icons/ui/side_panel_active.png"));
                return true;
            } else if (event->type() == QEvent::Leave){
                ui->showSidePanelButton->setIcon(QIcon(":/icons/ui/side_panel.png"));
                return true;
            } else {
                return false;
            }
        }
        else {
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
    //Generate file dialog for import and call the progress dialog indicator
    QStringList fileList = QFileDialog::getOpenFileNames(this,
                                  tr("Import files..."),
                                  QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0),
                                  inputFilter);
    if (!fileList.isEmpty()) {
        showImportProgressDialog(fileList);
    }
}

void CaesiumPH::showImportProgressDialog(QStringList list) {

    QProgressDialog progress(tr("Importing..."), tr("Cancel"), 0, list.count(), this);
    progress.setWindowIcon(QIcon(":/icons/main/logo.png"));
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

void CaesiumPH::on_actionAdd_folder_triggered() {
    QString path = QFileDialog::getExistingDirectory(this,
                                      tr("Select a folder to import..."),
                                      QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0),
                                      QFileDialog::ShowDirsOnly);
    if (!path.isEmpty()) {
        showImportProgressDialog(QStringList() << path);
    }
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
<<<<<<< HEAD
                  QStringToChar(item->text(4) + ".cmp.jpg"),
                  0,
                  QStringToChar(item->text(4)));
    //Gets new file info
    //TODO Change it, it must point the right output
    QFileInfo* fileInfo = new QFileInfo(item->text(4) + ".cmp.jpg");
    QFileInfo* originalInfo = new QFileInfo(item->text(4));
=======
                  QStringToChar(item->text(4) + ".cmp"),
                  1,
                  QStringToChar(item->text(4)));
    //Gets new file info
    //TODO Change it, it must point the right output
    QFileInfo* fileInfo = new QFileInfo(item->text(4) + ".cmp");
>>>>>>> CaesiumPH/master
    item->setText(2, formatSize(fileInfo->size()));
    item->setText(3, getRatio(originalInfo->size(), fileInfo->size()));
    originalsSize += originalInfo->size();
    compressedSize += fileInfo->size();
}

void CaesiumPH::on_actionCompress_triggered()
{
    //Reset counters
    originalsSize = compressedSize = 0;
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
    //connect(&watcher, SIGNAL(progressValueChanged(int)), &progressDialog, SLOT(setValue(int)));
    //connect(&watcher, SIGNAL(progressRangeChanged(int, int)), &progressDialog, SLOT(setRange(int,int)));
    connect(&watcher, SIGNAL(progressValueChanged(int)), ui->progressBar, SLOT(setValue(int)));
    connect(&watcher, SIGNAL(progressRangeChanged(int, int)), ui->progressBar, SLOT(setRange(int,int)));
    //connect(&watcher, SIGNAL(finished()), &progressDialog, SLOT(reset()));
    //connect(&progressDialog, SIGNAL(canceled()), &watcher, SLOT(cancel()));
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
<<<<<<< HEAD
    progressDialog.exec();
=======
    //progressDialog.exec();


>>>>>>> CaesiumPH/master
}

void CaesiumPH::compressionStarted() {
    //Start monitoring time while compressing
    qDebug() << QTime::currentTime();
}

void CaesiumPH::compressionFinished() {
    //Get elapsed time of the compression
    qDebug() << QTime::currentTime();
    qDebug() << formatSize(originalsSize) + " - " + formatSize(compressedSize) + " | " + getRatio(originalsSize, compressedSize);
}

void CaesiumPH::on_sidePanelDockWidget_topLevelChanged(bool topLevel) {
    //Check if it's floating and hide/show the line
    ui->sidePanelLine->setVisible(!topLevel);
}

void CaesiumPH::on_sidePanelDockWidget_visibilityChanged(bool visible) {
    //Handle the close event
    on_showSidePanelButton_clicked(visible);
    ui->showSidePanelButton->setChecked(visible);
}

void CaesiumPH::on_showSidePanelButton_clicked(bool checked) {
    ui->sidePanelDockWidget->setVisible(checked);
    //If it's not floating, we have a dedicated handler for that
    if (!ui->sidePanelDockWidget->isFloating()) {
        ui->sidePanelLine->setVisible(checked);
    }
    //Set icons
    if (checked) {
        ui->showSidePanelButton->setIcon(QIcon(":/icons/ui/side_panel_active.png"));
    } else {
        ui->showSidePanelButton->setIcon(QIcon(":/icons/ui/side_panel.png"));
    }
}


void CaesiumPH::on_listTreeWidget_itemSelectionChanged() {
    //Check if there's a selection
    if (ui->listTreeWidget->selectedItems().length() > 0) {
        //Get the first item selected
        QTreeWidgetItem* currentItem = ui->listTreeWidget->selectedItems().at(0);

        //Connect the global watcher to the slot
        connect(&imageWatcher, SIGNAL(finished()), this, SLOT(finishPreviewLoading()));
        //Run the image loader function
        imageWatcher.setFuture(QtConcurrent::run<QImage>(this, &CaesiumPH::loadImagePreview, currentItem->text(4)));

        //Load EXIF info
        //TODO Should run in another thread too?
        ui->exifTextEdit->setText(getExifFromPath(QStringToChar(currentItem->text(4))));
    }
}

QImage CaesiumPH::loadImagePreview(QString path) {
    //Load a scaled version of the image into memory
    QImageReader* imageReader = new QImageReader(path);
    imageReader->setScaledSize(getScaledSizeWithRatio(imageReader->size(), ui->imagePreviewLabel->size().width()));
    return imageReader->read();
}

void CaesiumPH::finishPreviewLoading() {
    //Set the image
    ui->imagePreviewLabel->setPixmap(QPixmap::fromImage(imageWatcher.result()));
}
