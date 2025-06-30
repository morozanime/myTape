#include <QByteArray>
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QElapsedTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tapecatalog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTableWidget * w = ui->tableWidgetWriteFileList;
    w->clear();
    w->setRowCount(0);
    QStringList headersWrite;
    headersWrite << "File" << "Size";
    w->setColumnCount(headersWrite.count());
    w->setHorizontalHeaderLabels(headersWrite);

    w = ui->tableWidgetReadFileList;
    w->clear();
    w->setRowCount(0);
    QStringList headersRead;
    headersRead << "Offset" << "File" << "Size";
    w->setColumnCount(headersRead.count());
    w->setHorizontalHeaderLabels(headersRead);

    ioTape = new IOTape();
    ioDisk = new IODisk(ioTape);
    w_writeFileList = new Worker_WriteFileList();

    connect(ioTape, &IOTape::DriveInfoUpdateSignal, this, &MainWindow::DriveInfoUpdateSlot);
    connect(ioTape, &IOTape::MediaInfoUpdateSignal, this, &MainWindow::MediaInfoUpdateSlot);
    connect(ioTape, &IOTape::catalog_readed, this, &MainWindow::catalog_readed);
    connect(ioTape, &IOTape::change_pos, this, &MainWindow::change_pos);
    connect(ioTape, &IOTape::error_signal, this, &MainWindow::error_message);
    connect(ioTape, &IOTape::change_cache, this, &MainWindow::change_cache);
    connect(ioTape, &IOTape::log, this, &MainWindow::log);
    connect(ioDisk, &IODisk::log, this, &MainWindow::log);
    connect(ioTape, &IOTape::status, this, &MainWindow::tapeStatus);
    connect(w_writeFileList, &Worker_WriteFileList::AddFile, this, &MainWindow::worker_writefilelist_AddFile);
    connect(w_writeFileList, &Worker_WriteFileList::CatalogReady, this, &MainWindow::worker_writefilelist_CatalogReady);
    connect(w_writeFileList, &Worker_WriteFileList::Clear, this, &MainWindow::worker_writefilelist_Clear);
    connect(w_writeFileList, &Worker_WriteFileList::progress, this, &MainWindow::progress);
    connect(ioTape, &IOTape::progress1, this, &MainWindow::progress1);
    bool result = connect(ioTape, &IOTape::progress, this, &MainWindow::progress);
    if(!result) {
        error_message("Connection error");
    }
//    emit ioTape->progress(7,7,77,"77",7777);
    ioTape->start();
    ioDisk->start();

    ui_refresh();
}

MainWindow::~MainWindow()
{
    ioDisk->terminate();
    ioTape->terminate();
    w_writeFileList->terminate();
    delete ioDisk;
    delete ioTape;
    delete w_writeFileList;

    delete ui;
}

void MainWindow::on_pushButtonWriteAddFile_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select file(s)"));
    w_writeFileList->blockSize = ioTape->mediaInfo.BlockSize;
    w_writeFileList->Cmd_AddFile(fileNames);
    if(!w_writeFileList->isRunning()) {
        w_writeFileList->start();
    }
}

void MainWindow::on_pushButtonWriteAddDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select dir");
    if(dir.isEmpty())
        return;
    w_writeFileList->blockSize = ioTape->mediaInfo.BlockSize;
    w_writeFileList->Cmd_AddDir(dir);

    if(!w_writeFileList->isRunning()) {
        w_writeFileList->start();
    }
}

void MainWindow::worker_writefilelist_AddFile(QString path, qint64 size) {
    QTableWidget * w = ui->tableWidgetWriteFileList;
    int row = w->rowCount();
    w->insertRow(row);
    w->setItem(row, 0, new QTableWidgetItem(path));
    w->setItem(row, 1, new QTableWidgetItem(QString::number(size)));
}

void MainWindow::worker_writefilelist_CatalogReady(qint64 total_size){
    QTableWidget * w = ui->tableWidgetWriteFileList;
    w->resizeColumnsToContents();
    ui->labelWriteTotal->setText(psize(total_size));
    ui_refresh();
//    w_writeFileList->terminate();
}

void MainWindow::worker_writefilelist_Clear(void){
    QTableWidget * w = ui->tableWidgetWriteFileList;
    w->setRowCount(0);
    ui->labelWriteTotal->setText("0");
    ui_refresh();
}

void MainWindow::on_pushButtonWriteClear_clicked()
{
    w_writeFileList->Cmd_Clear();

    if(!w_writeFileList->isRunning()) {
        w_writeFileList->start();
    }
}

void MainWindow::on_pushButtonWriteWrite_clicked()
{
    if(!ioTape->isOpened() || ioTape->mediaInfo.BlockSize == 0) {
        return;
    }

    if(ioTape->GetState() != IOTape::TAPE_IDLE) {
        QMessageBox::critical(this, "Error", "Tape is busy");
        return;
    }

    if(w_writeFileList->filesToWrite.isEmpty()) {
        QMessageBox::information(this, "Info", "Nothing to write");
        return;
    }

    if(ioTape->mediaInfo.Capacity.QuadPart - ioTape->mediaPositionBytes < w_writeFileList->catalog->totalSize) {
        QMessageBox::critical(this, "Catalog too large", psize(w_writeFileList->catalog->totalSize - (ioTape->mediaInfo.Capacity.QuadPart - ioTape->mediaPositionBytes)) + " over");
        return;
    }

    QString catalogTmpFileName = "$tmp$.catalog";
    QFile fileImg(catalogTmpFileName);
    if(!fileImg.open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Truncate)) {
        QMessageBox::critical(this, "Error", "Catalog file create error");
        return;
    }

    if(fileImg.write(w_writeFileList->catalog_serialized) != w_writeFileList->catalog_serialized.length()) {
        fileImg.close();
        QMessageBox::critical(this, "Error", "Catalog file write error");
        return;
    }

    fileImg.close();

    ioDisk->Write(QFileInfo(catalogTmpFileName));
    for(int i = 0; i < w_writeFileList->filesToWrite.length(); i++) {
        ioDisk->Write(w_writeFileList->filesToWrite.value(i));
    }

    ioTape->Command(IOTape::CMD_WRITE, w_writeFileList->catalog->totalSize);
}

void MainWindow::progress1(double percent, quint64 bytes/*, QString str*/) {
    QString message = QString::number(percent) + "/" + QString::number(bytes) + "/";
    ui->statusbar->showMessage(message);
}


void MainWindow::progress(double percent, QString str, quint64 bytes, bool force) {
//    log(5, str);
    static clock_t t0 = 0;
    clock_t t1 = clock();
    if((t1 - t0) < 100 && !force) return;
    t0 = t1;

    static clock_t t00 = 0;
    static quint64 bytes00 = 0;
    static int bytes_per_second = 0;
    if(bytes == 0 || bytes < bytes00) {
        bytes00 = bytes;
        bytes_per_second = 0;
        t00 = t1;
    } else if (t1 - t00 >= 1000) {
        bytes_per_second = (int)((bytes - bytes00) * 1000 / (t1 - t00));
        bytes00 = bytes;
        t00 = t1;
    }

    static QElapsedTimer timer;
    qint64 timeRemaining = 0;
    static qint64 timeElapsed = 0;
    if(percent < 0.0000001) {
        timer.restart();
    } else if(percent < 99.999999) {
        timeRemaining = (timer.elapsed() / percent * (100.0 - percent)) / 1000;
        timeElapsed = timer.elapsed() / 1000;
    }

    QString eta_str;
    if(percent > 0.0000001) {
        eta_str += ", Time ";
        eta_str += QString("%1:").arg(timeElapsed / 3600, 2, 10, QChar('0'));
        eta_str += QString("%1:").arg((timeElapsed / 60) % 60, 2, 10, QChar('0'));
        eta_str += QString("%1").arg(timeElapsed % 60, 2, 10, QChar('0'));
    }
    if(percent < 99.999999) {
        eta_str += "/";
        eta_str += QString("%1:").arg(timeRemaining / 3600, 2, 10, QChar('0'));
        eta_str += QString("%1:").arg((timeRemaining / 60) % 60, 2, 10, QChar('0'));
        eta_str += QString("%1").arg(timeRemaining % 60, 2, 10, QChar('0'));
    }

    QString message = psize(bytes_per_second) + "/s " + QString("%1%").arg(percent, 6, 'f', 2, ' ') + eta_str + " " + str;
    ui->statusbar->showMessage(message);
}

void MainWindow::ui_refresh()
{
    if(ioTape->isOpened()) {
        ui->pushButtonOpen->setText("Close");
        ui->labelBlock->setText(psize(ioTape->mediaInfo.BlockSize));
        change_pos(true);
        if(ioTape->mediaInfo.Capacity.QuadPart > 0) {
            ui->labelFree->setText(psize(ioTape->mediaInfo.Remaining.QuadPart) + " (" + QString::number(ioTape->mediaInfo.Remaining.QuadPart * 100 / ioTape->mediaInfo.Capacity.QuadPart) + "%)");
        }

        ui->pushButtonScan->setEnabled(true);
        ui->pushButtonGetPos->setEnabled(true);

        if(ioTape->mediaPositionBytes < (uint64_t) ioTape->mediaInfo.Capacity.QuadPart - (uint64_t) ioTape->mediaInfo.Remaining.QuadPart) {
            ui->pushButtonNext->setEnabled(true);
        }

        ui->pushButtonRead->setEnabled(true);
        ui->pushButtonReadAbort->setEnabled(true);
        ui->pushButtonSeek->setEnabled(true);
        ui->pushButtonSeekFirstFree->setEnabled(true);
        ui->pushButton_DriveWrite->setEnabled(true);
        ui->pushButton_MediaWrite->setEnabled(true);

//        ui->labelRawBlockSize->setText(QString::number(ioTape->mediaInfo.BlockSize));
        ui->labelRawRemaining->setText(QString::number(ioTape->mediaInfo.Remaining.QuadPart));
        ui->labelRawCapacity->setText(QString::number(ioTape->mediaInfo.Capacity.QuadPart));
        ui->labelRawPartitionCount->setText(QString::number(ioTape->mediaInfo.PartitionCount));
        ui->labelRawWriteProtected->setText(ioTape->mediaInfo.WriteProtected ? "YES" : "NO");
        ui->lineEditCache->setEnabled(false);
    } else {
        ui->pushButtonOpen->setText("Open");

        ui->pushButtonScan->setEnabled(false);
        ui->pushButtonGetPos->setEnabled(false);
        ui->pushButtonNext->setEnabled(false);
        ui->pushButtonRead->setEnabled(false);
        ui->pushButtonReadAbort->setEnabled(false);
        ui->pushButtonSeek->setEnabled(false);
        ui->pushButtonSeekFirstFree->setEnabled(false);
        ui->pushButton_DriveWrite->setEnabled(false);
        ui->pushButton_MediaWrite->setEnabled(false);

//        ui->labelRawBlockSize->setText("?");
        ui->labelRawRemaining->setText("?");
        ui->labelRawCapacity->setText("?");
        ui->labelRawPartitionCount->setText("?");
        ui->labelRawWriteProtected->setText("?");
        ui->lineEditCache->setEnabled(true);
    }

    if(w_writeFileList->filesToWrite.isEmpty() || !ioTape->isOpened() || ioTape->mediaInfo.WriteProtected) {
        ui->pushButtonWriteAbort->setEnabled(false);
        ui->pushButtonWriteWrite->setEnabled(false);
    } else {
        ui->pushButtonWriteAbort->setEnabled(true);
        ui->pushButtonWriteWrite->setEnabled(true);
    }

    if(w_writeFileList->filesToWrite.isEmpty()) {
        ui->pushButtonWriteClear->setEnabled(false);
    } else {
        ui->pushButtonWriteClear->setEnabled(true);
    }

    if(read_catalog == nullptr || read_catalog->filesOnTape.isEmpty() || !ioTape->isOpened()) {
        ui->pushButtonRestore->setEnabled(false);
        ui->pushButtonRestoreAll->setEnabled(false);
        ui->pushButtonExport->setEnabled(false);
    } else {
        ui->pushButtonRestore->setEnabled(true);
        ui->pushButtonRestoreAll->setEnabled(true);
        ui->pushButtonExport->setEnabled(true);
    }
}

void MainWindow::on_pushButtonOpen_clicked()
{
    if(ioTape->isOpened()) {
        ioTape->Close();
    } else {
        uint32_t cache = ui->lineEditCache->text().toUInt();
        if(cache == 0)
            cache = 256;
        else if(cache < 32)
            cache = 32;
        ui->lineEditCache->setText(QString::number(cache));
        ioTape->Open((QString("\\\\.\\") + ui->lineEditTapeDriveName->text()).toLatin1().data(), cache);
    }

    ui_refresh();
}

void MainWindow::on_pushButtonSeek_clicked()
{
    ioTape->Command(IOTape::CMD_SEEK, ui->lineEditSeek->text().toULongLong());
}

void MainWindow::on_pushButtonGetPos_clicked()
{
    ioTape->Command(IOTape::CMD_GETPOS);
}

void MainWindow::on_pushButtonRead_clicked()
{
    uint64_t toRead = (uint64_t) ui->lineEditRead->text().toLong() * ioTape->mediaInfo.BlockSize;
    if(toRead == 0)
        return;
    QString fn = QFileDialog::getSaveFileName(this, "Save " + QString::number(toRead) + " bytes to file");
    if(fn.isEmpty())
        return;
    ioDisk->Read(fn, toRead);
}

void MainWindow::on_pushButtonScan_clicked()
{
    ioTape->Command(IOTape::CMD_SCAN, 0);
}

void MainWindow::catalog_readed(TapeCatalog * catalog) {
    this->read_catalog = catalog;
    QTableWidget * w = ui->tableWidgetReadFileList;
    uint64_t total = 0;
    w->setRowCount(0);
    for(int i = 0; i < catalog->filesOnTape.length(); i++) {
        int row = w->rowCount();
        w->insertRow(row);
        w->setItem(row, 0, new QTableWidgetItem(QString::number(catalog->filesOnTape.value(i).offset)));
        w->setItem(row, 1, new QTableWidgetItem(catalog->filesOnTape.value(i).fileNamePath));
        w->setItem(row, 2, new QTableWidgetItem(QString::number(catalog->filesOnTape.value(i).fileSize)));
        total += catalog->filesOnTape.value(i).fileSize;
    }
    w->resizeColumnsToContents();
    ui->labelReadTotal->setText(psize(total));
    ui_refresh();
}

void MainWindow::change_pos(bool force) {
    if(ioTape->mediaInfo.Capacity.QuadPart > 0) {
        static clock_t t0 = 0;
        clock_t t1 = clock();
        if((t1 - t0) < 100 && !force) return;
        t0 = t1;
        QString a = QString::number(ioTape->mediaPositionBytes);
        a += " / ";
        a += QString::number(ioTape->mediaInfo.Capacity.QuadPart);
        a += " (" + QString::number(ioTape->mediaPositionBytes * 100 / ioTape->mediaInfo.Capacity.QuadPart) + "%)";
        ui->labelPos->setText(a);
    } else {
        ui->labelPos->setText("No tape");
    }
}

void MainWindow::tapeStatus(quint32 st) {
    if(ioTape == nullptr) {
        ui->label_state->setText(QString::number((uint32_t) st));
    } else {
        const char * s = ioTape->StatusString((int) st);
        ui->label_state->setText(QString(s) + "(" + QString::number((uint32_t) st) + ")");
    }
}

void MainWindow::error_message(QString message) {
    QMessageBox::critical(this, "Error", message);
}

void MainWindow::on_pushButtonRestore_clicked()
{
    QTableWidget * w = ui->tableWidgetReadFileList;
    if(ioTape->tapeCatalog == nullptr || ioTape->tapeCatalog->filesOnTape.isEmpty() || w->selectedItems().length() == 0)
        return;
    QString dir = QFileDialog::getExistingDirectory(this, "Select root dir");
    if(dir.isEmpty())
        return;
    QList<int> selected;
    for(int i = 0; i < w->rowCount(); i++) {
        if(w->item(i, 0)->isSelected()) {
            selected.append(i);
        }
    }
    if(selected.isEmpty())
        return;
    ioDisk->Restore(dir, ioTape->tapeCatalog, selected);
}

void MainWindow::on_pushButtonRestoreAll_clicked()
{
    if(ioTape->tapeCatalog == nullptr || ioTape->tapeCatalog->filesOnTape.isEmpty())
        return;
    QString dir = QFileDialog::getExistingDirectory(this, "Select root dir");
    if(dir.isEmpty())
        return;
    ioDisk->Restore(dir, ioTape->tapeCatalog);
}

void MainWindow::log(int level, QString message) {
    QFile f = QFile("__log__");
    if(f.open(QFile::Append)) {
        f.write((QString::number(level) + message).toLatin1().append('\n'));
        f.flush();
        f.close();
    }
}

void MainWindow::change_cache(quint64 size) {
    static clock_t t0 = 0;
    clock_t t1 = clock();
    if((t1 - t0) < 100) return;
    t0 = t1;
    ui->labelCache->setText(psize(size));
}

void MainWindow::on_pushButtonSeekFirstFree_clicked()
{
    if(ioTape->mediaInfo.BlockSize > 0) {
        ioTape->Command(IOTape::CMD_SEEK, (ioTape->mediaInfo.Capacity.QuadPart - ioTape->mediaInfo.Remaining.QuadPart) / ioTape->mediaInfo.BlockSize);
    }
}

void MainWindow::on_pushButtonNext_clicked()
{
    if(ioTape->tapeCatalog == nullptr || ioTape->tapeCatalog->filesOnTape.isEmpty() || ioTape->mediaInfo.BlockSize == 0)
        return;
    ioTape->Command(IOTape::CMD_SEEK, (ioTape->tapeCatalog->offsetOnTape + ioTape->tapeCatalog->totalSize) / ioTape->mediaInfo.BlockSize);
}

void MainWindow::on_pushButtonExport_clicked()
{
    if(ioTape->tapeCatalog == nullptr || ioTape->tapeCatalog->filesOnTape.isEmpty())
        return;
    QString csv = QFileDialog::getSaveFileName(this, "Save catalog");
    if(csv.isEmpty())
        return;
    if(ioTape->tapeCatalog->export_to_file(QFile(csv))) {
        QMessageBox::critical(this, "Open file error", "Error open file for writing");
        return;
    }
}

void MainWindow::on_pushButtonReadAbort_clicked()
{
    ioDisk->Flush();
    ioTape->Abort();
}

void MainWindow::on_pushButtonWriteAbort_clicked()
{
    ioDisk->Flush();
    ioTape->Abort();
}

void MainWindow::DriveInfoUpdateSlot(void) {
    if(ioTape == nullptr)
        return;
    ui->lineEdit_DriveInfoECC->setText(QString::number(ioTape->driveInfo.ECC ? 1 : 0));
    ui->lineEdit_DriveInfoCompression->setText(QString::number(ioTape->driveInfo.Compression ? 1 : 0));
    ui->lineEdit_DriveInfoDataPadding->setText(QString::number(ioTape->driveInfo.DataPadding ? 1 : 0));
    ui->lineEdit_DriveInfoReportSetmarks->setText(QString::number(ioTape->driveInfo.ReportSetmarks ? 1 : 0));
    ui->label_DriveInfoDefaultBlockSize->setText(QString::number(ioTape->driveInfo.DefaultBlockSize));
    ui->label_DriveInfoMaximumBlockSize->setText(QString::number(ioTape->driveInfo.MaximumBlockSize));
    ui->label_DriveInfoMinimumBlockSize->setText(QString::number(ioTape->driveInfo.MinimumBlockSize));
    ui->label_DriveInfoMaximumPartitionCount->setText(QString::number(ioTape->driveInfo.MaximumPartitionCount));
    ui->label_DriveInfoFeaturesLow->setText(QString::number(ioTape->driveInfo.FeaturesLow, 16));
    ui->label_DriveInfoFeaturesHigh->setText(QString::number(ioTape->driveInfo.FeaturesHigh, 16));
    ui->lineEdit_DriveInfoEOTWarningZoneSize->setText(QString::number(ioTape->driveInfo.EOTWarningZoneSize));
}

void MainWindow::on_pushButton_DriveWrite_clicked()
{
    if(ioTape == nullptr || ioTape->nullTape)
        return;
    TAPE_SET_DRIVE_PARAMETERS   driveParams;
    driveParams.ECC = ui->lineEdit_DriveInfoECC->text().toInt();
    driveParams.Compression = ui->lineEdit_DriveInfoCompression->text().toInt();
    driveParams.DataPadding = ui->lineEdit_DriveInfoDataPadding->text().toInt();
    driveParams.ReportSetmarks = ui->lineEdit_DriveInfoReportSetmarks->text().toInt();
    driveParams.EOTWarningZoneSize = ui->lineEdit_DriveInfoEOTWarningZoneSize->text().toULong();
    ioTape->setTapeDriveParameters(&driveParams);
}

void MainWindow::MediaInfoUpdateSlot(void) {
    if(ioTape == nullptr)
        return;
    ui->labelRawCapacity->setText(QString::number(ioTape->mediaInfo.Capacity.QuadPart));
    ui->labelRawRemaining->setText(QString::number(ioTape->mediaInfo.Remaining.QuadPart));
    ui->lineEdit_MediaBlockSize->setText(QString::number(ioTape->mediaInfo.BlockSize));
    ui->labelRawPartitionCount->setText(QString::number(ioTape->mediaInfo.PartitionCount));
    ui->labelRawWriteProtected->setText(QString::number(ioTape->mediaInfo.WriteProtected));
}

void MainWindow::on_pushButton_MediaWrite_clicked()
{
    if(ioTape == nullptr || ioTape->nullTape)
        return;
    TAPE_SET_MEDIA_PARAMETERS   mediaParams;
    mediaParams.BlockSize = ui->lineEdit_MediaBlockSize->text().toInt();
    ioTape->setTapeMediaParameters(&mediaParams);
}
