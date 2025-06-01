#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include "iotape.h"
#include "iodisk.h"
#include "worker_writefilelist.h"

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
    void progress(int i, int n, double percent, QString str);
    void catalog_readed(TapeCatalog * catalog);
    void change_pos(void);
    void error_message(QString message);
    void change_cache(uint64_t size);
    void log(int level, QString message);
    void worker_writefilelist_AddFile(QString path, qint64 size);
    void worker_writefilelist_CatalogReady(qint64 total_size);
    void worker_writefilelist_Clear(void);
    void tapeStatus(DWORD st);

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

    void on_pushButtonSeekFirstFree_clicked();

    void on_pushButtonNext_clicked();

    void on_pushButtonExport_clicked();

    void on_pushButtonReadAbort_clicked();

    void on_pushButtonRestore_clicked();

private:
    Ui::MainWindow *ui;

    IOTape * ioTape = nullptr;
    IODisk * ioDisk = nullptr;
    Worker_WriteFileList * w_writeFileList = nullptr;
//    QList<QFileInfo> filesToWrite;
    TapeCatalog * read_catalog = nullptr;

    QString psize(uint64_t size) {
        if(size > 3ULL * 1024 * 1024 * 1024)
            return QString::number(size / (1024 * 1024 * 1024)) + " Gb";
        else if(size > 3 * 1024 * 1024)
            return QString::number(size / (1024 * 1024)) + " Mb";
        else if(size > 3 * 1024)
            return QString::number(size / 1024) + " kb";
        else
            return QString::number(size);
    }
    void ui_refresh();
};
#endif // MAINWINDOW_H
