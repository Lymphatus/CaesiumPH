#include "caesiumph.h"
#include "ui_caesiumph.h"
#include "aboutdialog.h"
#include "utils.h"
#include "lossless.h"
#include "cimageinfo.h"
#include "exif.h"
#include "preferencedialog.h"
#include "usageinfo.h"
#include "networkoperations.h"
#include "qdroptreewidget.h"
#include "ctreewidgetitem.h"

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
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

#include <QDebug>

CaesiumPH::CaesiumPH(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CaesiumPH)
{
    ui->setupUi(this);
    initializeSettings();
    initializeConnections();
    initializeUI();
    readPreferences();
    checkUpdates();

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
    QSettings settings;

    //Install event filter for buttons
    ui->addFilesButton->installEventFilter(this);
    ui->addFolderButton->installEventFilter(this);
    ui->compressButton->installEventFilter(this);
    ui->removeItemButton->installEventFilter(this);
    ui->clearButton->installEventFilter(this);
    ui->showSidePanelButton->installEventFilter(this);
    ui->settingsButton->installEventFilter(this);

    //Set the headers size
    ui->listTreeWidget->header()->resizeSection(0, 180);
    ui->listTreeWidget->header()->resizeSection(1, 100);
    ui->listTreeWidget->header()->resizeSection(2, 100);
    ui->listTreeWidget->header()->resizeSection(3, 80);
    ui->listTreeWidget->header()->resizeSection(4, 100);

    //Set menu invisible for Windows/Linux
    ui->menuBar->setVisible(false);

    //Update button visibility
    ui->updateButton->setVisible(false);
    ui->updateLabel->setVisible(false);

    //Restore window state
    settings.beginGroup(KEY_PREF_GROUP_GEOMETRY);
    resize(settings.value(KEY_PREF_GEOMETRY_SIZE, QSize(880, 500)).toSize());
    move(settings.value(KEY_PREF_GEOMETRY_POS, QPoint(200, 200)).toPoint());
    ui->sidePanelDockWidget->setVisible(settings.value(KEY_PREF_GEOMETRY_PANEL_VISIBLE).value<bool>());
    on_sidePanelDockWidget_visibilityChanged(settings.value(KEY_PREF_GEOMETRY_PANEL_VISIBLE).value<bool>());
    settings.endGroup();

    //Default EXIF value
    ui->exifTextEdit->setText(tr("No EXIF info available"));

    //Sorting
    //TODO Make a preference
    ui->listTreeWidget->sortByColumn(0, Qt::AscendingOrder);
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

    //TreeWidget drop event
    connect(ui->listTreeWidget, SIGNAL(dropFinished(QStringList)), this, SLOT(showImportProgressDialog(QStringList)));
}

void CaesiumPH::initializeSettings() {
    QCoreApplication::setApplicationName("CaesiumPH");
    QCoreApplication::setOrganizationName("SaeraSoft");
    QCoreApplication::setOrganizationDomain("saerasoft.com");

    uinfo->initialize();
}

void CaesiumPH::readPreferences() {
    //Read important parameters from settings
    QSettings settings;

    settings.beginGroup(KEY_PREF_GROUP_COMPRESSION);
    params.exif = settings.value(KEY_PREF_COMPRESSION_EXIF).value<bool>();
    params.progressive = settings.value(KEY_PREF_COMPRESSION_PROGRESSIVE).value<bool>();
    settings.endGroup();

    params.overwrite = settings.value(KEY_PREF_GROUP_GENERAL + KEY_PREF_GENERAL_OVERWRITE).value<bool>();
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
    } else if (obj == (QObject*) ui->settingsButton) {
        if (event->type() == QEvent::Enter) {
            ui->settingsButton->setIcon(QIcon(":/icons/ui/settings_hover.png"));
            return true;
        } else if (event->type() == QEvent::Leave){
            ui->settingsButton->setIcon(QIcon(":/icons/ui/settings.png"));
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

        //Validate extension
        if (!isJPEG(QStringToChar(prefix + list.at(i)))) {
            qDebug() << "NOT JPEG";
            continue;
        }

        //Generate new CImageInfo
        CImageInfo* currentItemInfo = new CImageInfo(prefix + list.at(i));

        //Populate list
        QStringList itemContent = QStringList() << currentItemInfo->getBaseName()
                                                << currentItemInfo->getFormattedSize()
                                                << ""
                                                << ""
                                                << currentItemInfo->getFullPath();
        qDebug() << itemContent;

        ui->listTreeWidget->addTopLevelItem(new CTreeWidgetItem(ui->listTreeWidget,
                                                                itemContent));

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

void CaesiumPH::on_actionRemove_items_triggered() {
    for (int i = 0; i < ui->listTreeWidget->selectedItems().count(); i++) {
        ui->listTreeWidget->takeTopLevelItem(ui->listTreeWidget->indexOfTopLevelItem(ui->listTreeWidget->selectedItems().at(i)));
    }
}

extern void compressRoutine(CTreeWidgetItem* item) {
    QString inputPath = item->text(4);
    QFileInfo* originalInfo = new QFileInfo(item->text(4));
    QString outputPath;
    if (params.overwrite) {
        outputPath = inputPath;
    } else {
        outputPath = originalInfo->filePath().replace(originalInfo->completeBaseName(),
                                                      originalInfo->baseName() + "_compressed");
    }
    //BUG Sometimes files are empty. Check it out.
    cclt_optimize(QStringToChar(inputPath),
                  QStringToChar(outputPath),
                  params.exif,
                  params.progressive,
                  QStringToChar(inputPath));
    //Gets new file info
    //TODO Ratio is wrong if overwrites
    QFileInfo* fileInfo = new QFileInfo(outputPath);
    item->setText(2, formatSize(fileInfo->size()));
    item->setText(3, getRatio(originalInfo->size(), fileInfo->size()));
    originalsSize += originalInfo->size();
    compressedSize += fileInfo->size();

    //Usage reports
    if (originalInfo->size() > uinfo->max_bytes) {
        uinfo->setMax_bytes(originalInfo->size());
    }

    if ((originalInfo->size() - fileInfo->size()) * 100 / (double) originalInfo->size() > uinfo->best_ratio) {
        uinfo->setBest_ratio((originalInfo->size() - fileInfo->size()) * 100 / (double) originalInfo->size());
    }
}

void CaesiumPH::on_actionCompress_triggered() {
    //Read preferences again
    readPreferences();
    //Reset counters
    originalsSize = compressedSize = 0;
    //Register metatype for emitting changes
    qRegisterMetaType<QVector<int> >("QVector<int>");

    //Setting up a progress dialog
    QProgressDialog progressDialog;
    progressDialog.setWindowTitle(tr("CaesiumPH"));
    progressDialog.setLabelText(tr("Compressing..."));

    //Holds the list
    QList<CTreeWidgetItem*> list;

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
        list.append((CTreeWidgetItem*) ui->listTreeWidget->topLevelItem(i));
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
    qDebug() << formatSize(originalsSize) + " - " + formatSize(compressedSize) + " | " + getRatio(originalsSize, compressedSize);
    //Set parameters for usage info
    uinfo->setCompressed_bytes(uinfo->compressed_bytes + originalsSize);
    uinfo->setCompressed_pictures(uinfo->compressed_pictures + ui->listTreeWidget->topLevelItemCount());

    uinfo->writeJSON();
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
        CTreeWidgetItem* currentItem = (CTreeWidgetItem*) ui->listTreeWidget->selectedItems().at(0);

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

void CaesiumPH::on_settingsButton_clicked() {
    NetworkOperations* no = new NetworkOperations(this);
    no->uploadUsageStatistics();
    PreferenceDialog* pd = new PreferenceDialog(this);
    pd->show();
}

void CaesiumPH::closeEvent(QCloseEvent *event) {
    QSettings settings;

    //Save window geometry
    settings.beginGroup(KEY_PREF_GROUP_GEOMETRY);
    settings.setValue(KEY_PREF_GEOMETRY_SIZE, size());
    settings.setValue(KEY_PREF_GEOMETRY_POS, pos());
    settings.setValue(KEY_PREF_GEOMETRY_PANEL_VISIBLE, ui->sidePanelDockWidget->isVisible());
    settings.endGroup();

    if (settings.value(KEY_PREF_GROUP_GENERAL + KEY_PREF_GENERAL_PROMPT).value<bool>()) {
        //Display a prompt
        int res = QMessageBox::warning(this, tr("CaesiumPH"),
                                       tr("Do you really want to exit?"),
                                       QMessageBox::Ok | QMessageBox::Cancel);
        //Exit if OK, go back if Cancel
        //TODO Translate?
        switch (res) {
            case QMessageBox::Ok:
                event->accept();
                break;
            case QMessageBox::Cancel:
                event->ignore();
            default:
                break;
        }
    } else {
        event->accept();
    }
}

void CaesiumPH::checkUpdates() {
    qDebug() << "Check updates called";
    NetworkOperations* op = new NetworkOperations();
    op->checkForUpdates();
    connect(op, SIGNAL(checkForUpdatesFinished(int)), this, SLOT(updateAvailable(int)));
}

void CaesiumPH::updateAvailable(int version) {
    qDebug() << version;
    ui->updateButton->setVisible(version > versionNumber);
    ui->updateLabel->setVisible(ui->updateButton->isVisible());
}
