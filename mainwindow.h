#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include "iotape.h"
#include "iodisk.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void progress(int i, int n, double percent);
    void catalog_readed(TapeCatalog * catalog);
    void change_pos(void);
    void error_message(QString message);
    void change_cache(uint64_t size);
    void log(int level, QString message);

private slots:
    void on_pushButtonWriteAddFile_clicked();

    void on_pushButtonWriteClear_clicked();

    void on_pushButtonWriteWrite_clicked();

    void on_pushButtonWriteAddDir_clicked();

    void on_pushButtonWriteAbort_clicked();


    void on_pushButtonOpen_clicked();

    void on_pushButtonSeek_clicked();

    void on_pushButtonGetPos_clicked();

    void on_pushButtonRead_clicked();

    void on_pushButtonScan_clicked();

    void on_pushButtonRestoreAll_clicked();

private:
    Ui::MainWindow *ui;

    IOTape * ioTape = nullptr;
    IODisk * ioDisk = nullptr;
    QList<QFileInfo> filesToWrite;
};
#endif // MAINWINDOW_H
