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

    connect(ioTape, &IOTape::progress, this, &MainWindow::progress);
    connect(ioTape, &IOTape::catalog_readed, this, &MainWindow::catalog_readed);
    connect(ioTape, &IOTape::change_pos, this, &MainWindow::change_pos);
    connect(ioTape, &IOTape::error_signal, this, &MainWindow::error_message);
    connect(ioTape, &IOTape::change_cache, this, &MainWindow::change_cache);
    connect(ioTape, &IOTape::log, this, &MainWindow::log);
    connect(ioDisk, &IODisk::log, this, &MainWindow::log);
    ioTape->start();
    ioDisk->start();

    ui_refresh();
}

MainWindow::~MainWindow()
{
    ioDisk->terminate();
    ioTape->terminate();
    delete ioDisk;
    delete ioTape;

    delete ui;
}

void MainWindow::on_pushButtonWriteAddFile_clicked()
{
    QTableWidget * w = ui->tableWidgetWriteFileList;
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select file(s)"));
    for(int i = 0; i < fileNames.length(); i++) {
        QFileInfo f(fileNames.value(i));
        filesToWrite.append(f);
        int row = w->rowCount();
        w->insertRow(row);
        w->setItem(row, 0, new QTableWidgetItem(f.absoluteFilePath()));
        w->setItem(row, 1, new QTableWidgetItem(QString::number(f.size())));
    }
    w->resizeColumnsToContents();
    TapeCatalog catalog(ioTape->mediaInfo.BlockSize, filesToWrite);
    catalog.serialize();
    ui->labelWriteTotal->setText(psize(catalog.totalSize));
    ui_refresh();
}

static void _add0(QTableWidget * w, QString dir, QList<QFileInfo> * filesToWrite) {
    QDirIterator * itFiles = new QDirIterator(dir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    while (itFiles->hasNext()) {
        itFiles->next();
        QFileInfo f = itFiles->fileInfo();
        if(f.isDir()) {
            _add0(w, f.absoluteFilePath(), filesToWrite);
        } else {
            filesToWrite->append(f);
            int row = w->rowCount();
            w->insertRow(row);
            w->setItem(row, 0, new QTableWidgetItem(f.absoluteFilePath()));
            w->setItem(row, 1, new QTableWidgetItem(QString::number(f.size())));
        }
    }
    delete itFiles;
}

void MainWindow::on_pushButtonWriteAddDir_clicked()
{
    QTableWidget * w = ui->tableWidgetWriteFileList;
    QString dir = QFileDialog::getExistingDirectory(this, "Select dir");
    if(dir.isEmpty())
        return;
    _add0(w, dir, &filesToWrite);
    w->resizeColumnsToContents();
    TapeCatalog catalog(ioTape->mediaInfo.BlockSize, filesToWrite);
    catalog.serialize();
    ui->labelWriteTotal->setText(psize(catalog.totalSize));
    ui_refresh();
}

void MainWindow::on_pushButtonWriteClear_clicked()
{
    QTableWidget * w = ui->tableWidgetWriteFileList;
    w->setRowCount(0);
    filesToWrite.clear();
    ui->labelWriteTotal->setText("0");
    ui_refresh();
}

void MainWindow::on_pushButtonWriteWrite_clicked()
{
    if(filesToWrite.isEmpty())
        return;
    TapeCatalog catalog(ioTape->mediaInfo.BlockSize, filesToWrite);
    QByteArray catalogImg = catalog.serialize();
    if(ioTape->mediaInfo.Capacity.QuadPart - ioTape->mediaPositionBytes < catalog.totalSize) {
        QMessageBox::critical(this, "Catalog too large", psize(catalog.totalSize - (ioTape->mediaInfo.Capacity.QuadPart - ioTape->mediaPositionBytes)) + " over");
        return;
    }

    QString catalogTmpFileName = "$tmp$.catalog";
    QFile fileImg(catalogTmpFileName);
    if(fileImg.open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Truncate)) {
        fileImg.write(catalogImg);
        fileImg.close();
    }
    ioDisk->Write(QFileInfo(catalogTmpFileName));
    for(int i = 0; i < filesToWrite.length(); i++) {
        ioDisk->Write(filesToWrite.value(i));
    }

    ioTape->Command(IOTape::CMD_WRITE, catalog.totalSize);
}

void MainWindow::progress(int i, int n, double percent) {
    static QElapsedTimer timer;
    uint32_t eta = 0;
    if(percent < 0.0000001) {
        timer.restart();
    } else {
        eta = (timer.elapsed() / percent * (100.0 - percent)) / 1000;
    }
    QString eta_str;
    if(percent > 0.0000001 && percent < 99.999999) {
        eta_str += ", ETA ";
        eta_str += QString("%1:").arg(eta / 3600, 2, 10, QChar('0'));
        eta_str += QString("%1:").arg((eta / 60) % 60, 2, 10, QChar('0'));
        eta_str += QString("%1").arg(eta % 60, 2, 10, QChar('0'));
    }

    ui->statusbar->showMessage(QString::number(i) + "/" + QString::number(n) + " " + QString("%1%").arg(percent, 6, 'f', 2, ' ') + eta_str);
}

void MainWindow::on_pushButtonWriteAbort_clicked()
{
    ioDisk->Flush();
    ioTape->Abort();
}

void MainWindow::ui_refresh()
{
    if(ioTape->isOpened()) {
        ui->pushButtonOpen->setText("Close");
        ui->labelBlock->setText(psize(ioTape->mediaInfo.BlockSize));
        change_pos();
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

        ui->labelRawBlockSize->setText(QString::number(ioTape->mediaInfo.BlockSize));
        ui->labelRawRemaining->setText(QString::number(ioTape->mediaInfo.Remaining.QuadPart));
        ui->labelRawCapacity->setText(QString::number(ioTape->mediaInfo.Capacity.QuadPart));
        ui->labelRawPartitionCount->setText(QString::number(ioTape->mediaInfo.PartitionCount));
        ui->labelRawWriteProtected->setText(ioTape->mediaInfo.WriteProtected ? "YES" : "NO");
    } else {
        ui->pushButtonOpen->setText("Open");

        ui->pushButtonScan->setEnabled(false);
        ui->pushButtonGetPos->setEnabled(false);
        ui->pushButtonNext->setEnabled(false);
        ui->pushButtonRead->setEnabled(false);
        ui->pushButtonReadAbort->setEnabled(false);
        ui->pushButtonSeek->setEnabled(false);
        ui->pushButtonSeekFirstFree->setEnabled(false);

        ui->labelRawBlockSize->setText("?");
        ui->labelRawRemaining->setText("?");
        ui->labelRawCapacity->setText("?");
        ui->labelRawPartitionCount->setText("?");
        ui->labelRawWriteProtected->setText("?");
    }

    if(filesToWrite.isEmpty() || !ioTape->isOpened() || ioTape->mediaInfo.WriteProtected) {
        ui->pushButtonWriteAbort->setEnabled(false);
        ui->pushButtonWriteWrite->setEnabled(false);
    } else {
        ui->pushButtonWriteAbort->setEnabled(true);
        ui->pushButtonWriteWrite->setEnabled(true);
    }

    if(filesToWrite.isEmpty()) {
        ui->pushButtonWriteClear->setEnabled(false);
    } else {
        ui->pushButtonWriteClear->setEnabled(true);
    }

    if(catalog == nullptr || catalog->filesOnTape.isEmpty() || !ioTape->isOpened()) {
        ui->pushButtonRestoreAll->setEnabled(false);
        ui->pushButtonExport->setEnabled(false);
    } else {
        ui->pushButtonRestoreAll->setEnabled(true);
        ui->pushButtonExport->setEnabled(true);
    }
}

void MainWindow::on_pushButtonOpen_clicked()
{
    if(ioTape->isOpened()) {
        ioTape->Close();
    } else {
        ioTape->Open((QString("\\\\.\\") + ui->lineEditTapeDriveName->text()).toLatin1().data(), 256);
    }

    ui_refresh();
}

void MainWindow::on_pushButtonSeek_clicked()
{
    ioTape->Command(IOTape::CMD_SEEK, ui->lineEditSeek->text().toLong());
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
    this->catalog = catalog;
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

void MainWindow::change_pos(void) {
    if(ioTape->mediaInfo.Capacity.QuadPart > 0) {
        QString a = QString::number(ioTape->mediaPositionBytes);
        a += " / ";
        a += QString::number(ioTape->mediaInfo.Capacity.QuadPart);
        a += " (" + QString::number(ioTape->mediaPositionBytes * 100 / ioTape->mediaInfo.Capacity.QuadPart) + "%)";
        ui->labelPos->setText(a);
    } else {
        ui->labelPos->setText("No tape");
    }
}

void MainWindow::error_message(QString message){
    QMessageBox::critical(this, "Error", message);
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
        f.close();
    }
}

void MainWindow::change_cache(uint64_t size) {
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
