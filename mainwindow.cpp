#include <QByteArray>
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>
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
    ui->labelWriteTotal->setText(QString::number(catalog.totalSize / 1024) + " kb");
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
    ui->labelWriteTotal->setText(QString::number(catalog.totalSize / 1024) + " kb");
}

void MainWindow::on_pushButtonWriteClear_clicked()
{
    QTableWidget * w = ui->tableWidgetWriteFileList;
    w->setRowCount(0);
    filesToWrite.clear();
    ui->labelWriteTotal->setText("0");
}

void MainWindow::on_pushButtonWriteWrite_clicked()
{
    if(filesToWrite.isEmpty())
        return;
    TapeCatalog catalog(ioTape->mediaInfo.BlockSize, filesToWrite);
    QByteArray catalogImg = catalog.serialize();
    if(ioTape->mediaInfo.Capacity.QuadPart - ioTape->mediaPositionBytes < catalog.totalSize) {
        QMessageBox::critical(this, "Catalog too large", QString::number((catalog.totalSize - (ioTape->mediaInfo.Capacity.QuadPart - ioTape->mediaPositionBytes)) / 1024) + "kb over");
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
    ui->statusbar->showMessage(QString::number(i) + "/" + QString::number(n) + " " + QString::number(percent) + "%");
}

void MainWindow::on_pushButtonWriteAbort_clicked()
{
    ioDisk->Flush();
    ioTape->Abort();
}

void MainWindow::on_pushButtonOpen_clicked()
{
    if(ioTape->isOpened()) {
        ioTape->Close();
    } else {
        ioTape->Open((QString("\\\\.\\") + ui->lineEditTapeDriveName->text()).toLatin1().data(), 256);
    }

    if(ioTape->isOpened()) {
        ui->pushButtonOpen->setText("Close");
        ui->labelBlock->setText(QString::number(ioTape->mediaInfo.BlockSize));
        change_pos();
    } else {
        ui->pushButtonOpen->setText("Open");
    }
}

void MainWindow::on_pushButtonSeek_clicked()
{
    ioTape->Command(IOTape::CMD_SEEK, ui->lineEditSeek->text().toLong());
}

void MainWindow::on_pushButtonGetPos_clicked()
{
    ioTape->GetPosition();
    change_pos();
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
    ui->labelReadTotal->setText(QString::number(total / 1024) + " kb");
}

void MainWindow::change_pos(void) {
    QString a = QString::number(ioTape->mediaPositionBytes);
    a += " / ";
    a += QString::number(ioTape->mediaInfo.Capacity.QuadPart);
    a += " (" + QString::number(ioTape->mediaPositionBytes * 100 / ioTape->mediaInfo.Capacity.QuadPart) + "%)";
    ui->labelPos->setText(a);
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
    ui->labelCache->setText(QString::number(size / 1024 / 1024) + " Mb");
}
