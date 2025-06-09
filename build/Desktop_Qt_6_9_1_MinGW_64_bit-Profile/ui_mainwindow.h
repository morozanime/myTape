/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QLineEdit *lineEditTapeDriveName;
    QPushButton *pushButtonOpen;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_14;
    QLabel *label_state;
    QLabel *label_2;
    QLabel *labelPos;
    QLabel *label_4;
    QLabel *labelBlock;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *labelCache;
    QLineEdit *lineEditCache;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_6;
    QLabel *labelFree;
    QPushButton *pushButtonSeekFirstFree;
    QTabWidget *tabWidget;
    QWidget *tabRead;
    QVBoxLayout *verticalLayout_4;
    QTableWidget *tableWidgetReadFileList;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButtonScan;
    QPushButton *pushButtonReadAbort;
    QPushButton *pushButtonNext;
    QPushButton *pushButtonExport;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_7;
    QLabel *labelReadTotal;
    QPushButton *pushButtonRestore;
    QPushButton *pushButtonRestoreAll;
    QWidget *tabWrite;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidgetWriteFileList;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonWriteAddDir;
    QPushButton *pushButtonWriteAddFile;
    QPushButton *pushButtonWriteClear;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonWriteAbort;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_5;
    QLabel *labelWriteTotal;
    QPushButton *pushButtonWriteWrite;
    QWidget *tabRaw;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEditSeek;
    QPushButton *pushButtonSeek;
    QPushButton *pushButtonGetPos;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEditRead;
    QPushButton *pushButtonRead;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_8;
    QLabel *labelRawCapacity;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_9;
    QLabel *labelRawRemaining;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_10;
    QLabel *labelRawBlockSize;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_11;
    QLabel *labelRawPartitionCount;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_12;
    QLabel *labelRawWriteProtected;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *verticalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1152, 779);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        horizontalLayout_5->addWidget(label);

        lineEditTapeDriveName = new QLineEdit(centralwidget);
        lineEditTapeDriveName->setObjectName("lineEditTapeDriveName");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEditTapeDriveName->sizePolicy().hasHeightForWidth());
        lineEditTapeDriveName->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(lineEditTapeDriveName);

        pushButtonOpen = new QPushButton(centralwidget);
        pushButtonOpen->setObjectName("pushButtonOpen");

        horizontalLayout_5->addWidget(pushButtonOpen);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        label_14 = new QLabel(centralwidget);
        label_14->setObjectName("label_14");

        horizontalLayout_5->addWidget(label_14);

        label_state = new QLabel(centralwidget);
        label_state->setObjectName("label_state");
        label_state->setMinimumSize(QSize(100, 0));
        QFont font;
        font.setBold(true);
        label_state->setFont(font);

        horizontalLayout_5->addWidget(label_state);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        horizontalLayout_5->addWidget(label_2);

        labelPos = new QLabel(centralwidget);
        labelPos->setObjectName("labelPos");
        labelPos->setMinimumSize(QSize(200, 0));
        labelPos->setFont(font);

        horizontalLayout_5->addWidget(labelPos);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");

        horizontalLayout_5->addWidget(label_4);

        labelBlock = new QLabel(centralwidget);
        labelBlock->setObjectName("labelBlock");
        labelBlock->setMinimumSize(QSize(60, 0));
        labelBlock->setFont(font);

        horizontalLayout_5->addWidget(labelBlock);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        labelCache = new QLabel(centralwidget);
        labelCache->setObjectName("labelCache");
        labelCache->setMinimumSize(QSize(40, 0));
        labelCache->setFont(font);

        horizontalLayout_2->addWidget(labelCache);

        lineEditCache = new QLineEdit(centralwidget);
        lineEditCache->setObjectName("lineEditCache");
        lineEditCache->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(lineEditCache);

        label_13 = new QLabel(centralwidget);
        label_13->setObjectName("label_13");

        horizontalLayout_2->addWidget(label_13);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");

        horizontalLayout_2->addWidget(label_6);

        labelFree = new QLabel(centralwidget);
        labelFree->setObjectName("labelFree");
        labelFree->setMinimumSize(QSize(140, 0));
        labelFree->setFont(font);

        horizontalLayout_2->addWidget(labelFree);

        pushButtonSeekFirstFree = new QPushButton(centralwidget);
        pushButtonSeekFirstFree->setObjectName("pushButtonSeekFirstFree");

        horizontalLayout_2->addWidget(pushButtonSeekFirstFree);


        verticalLayout->addLayout(horizontalLayout_2);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabRead = new QWidget();
        tabRead->setObjectName("tabRead");
        verticalLayout_4 = new QVBoxLayout(tabRead);
        verticalLayout_4->setObjectName("verticalLayout_4");
        tableWidgetReadFileList = new QTableWidget(tabRead);
        tableWidgetReadFileList->setObjectName("tableWidgetReadFileList");
        tableWidgetReadFileList->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_4->addWidget(tableWidgetReadFileList);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        pushButtonScan = new QPushButton(tabRead);
        pushButtonScan->setObjectName("pushButtonScan");

        horizontalLayout_6->addWidget(pushButtonScan);

        pushButtonReadAbort = new QPushButton(tabRead);
        pushButtonReadAbort->setObjectName("pushButtonReadAbort");

        horizontalLayout_6->addWidget(pushButtonReadAbort);

        pushButtonNext = new QPushButton(tabRead);
        pushButtonNext->setObjectName("pushButtonNext");

        horizontalLayout_6->addWidget(pushButtonNext);

        pushButtonExport = new QPushButton(tabRead);
        pushButtonExport->setObjectName("pushButtonExport");
        pushButtonExport->setEnabled(false);

        horizontalLayout_6->addWidget(pushButtonExport);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        label_7 = new QLabel(tabRead);
        label_7->setObjectName("label_7");

        horizontalLayout_6->addWidget(label_7);

        labelReadTotal = new QLabel(tabRead);
        labelReadTotal->setObjectName("labelReadTotal");
        labelReadTotal->setMinimumSize(QSize(100, 0));
        labelReadTotal->setFont(font);

        horizontalLayout_6->addWidget(labelReadTotal);

        pushButtonRestore = new QPushButton(tabRead);
        pushButtonRestore->setObjectName("pushButtonRestore");
        pushButtonRestore->setEnabled(false);

        horizontalLayout_6->addWidget(pushButtonRestore);

        pushButtonRestoreAll = new QPushButton(tabRead);
        pushButtonRestoreAll->setObjectName("pushButtonRestoreAll");
        pushButtonRestoreAll->setEnabled(false);

        horizontalLayout_6->addWidget(pushButtonRestoreAll);


        verticalLayout_4->addLayout(horizontalLayout_6);

        tabWidget->addTab(tabRead, QString());
        tabWrite = new QWidget();
        tabWrite->setObjectName("tabWrite");
        verticalLayout_2 = new QVBoxLayout(tabWrite);
        verticalLayout_2->setObjectName("verticalLayout_2");
        tableWidgetWriteFileList = new QTableWidget(tabWrite);
        tableWidgetWriteFileList->setObjectName("tableWidgetWriteFileList");

        verticalLayout_2->addWidget(tableWidgetWriteFileList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButtonWriteAddDir = new QPushButton(tabWrite);
        pushButtonWriteAddDir->setObjectName("pushButtonWriteAddDir");

        horizontalLayout->addWidget(pushButtonWriteAddDir);

        pushButtonWriteAddFile = new QPushButton(tabWrite);
        pushButtonWriteAddFile->setObjectName("pushButtonWriteAddFile");

        horizontalLayout->addWidget(pushButtonWriteAddFile);

        pushButtonWriteClear = new QPushButton(tabWrite);
        pushButtonWriteClear->setObjectName("pushButtonWriteClear");

        horizontalLayout->addWidget(pushButtonWriteClear);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonWriteAbort = new QPushButton(tabWrite);
        pushButtonWriteAbort->setObjectName("pushButtonWriteAbort");

        horizontalLayout->addWidget(pushButtonWriteAbort);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_5 = new QLabel(tabWrite);
        label_5->setObjectName("label_5");

        horizontalLayout->addWidget(label_5);

        labelWriteTotal = new QLabel(tabWrite);
        labelWriteTotal->setObjectName("labelWriteTotal");
        labelWriteTotal->setMinimumSize(QSize(100, 0));
        labelWriteTotal->setFont(font);

        horizontalLayout->addWidget(labelWriteTotal);

        pushButtonWriteWrite = new QPushButton(tabWrite);
        pushButtonWriteWrite->setObjectName("pushButtonWriteWrite");

        horizontalLayout->addWidget(pushButtonWriteWrite);


        verticalLayout_2->addLayout(horizontalLayout);

        tabWidget->addTab(tabWrite, QString());
        tabRaw = new QWidget();
        tabRaw->setObjectName("tabRaw");
        verticalLayout_3 = new QVBoxLayout(tabRaw);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        lineEditSeek = new QLineEdit(tabRaw);
        lineEditSeek->setObjectName("lineEditSeek");
        sizePolicy.setHeightForWidth(lineEditSeek->sizePolicy().hasHeightForWidth());
        lineEditSeek->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(lineEditSeek);

        pushButtonSeek = new QPushButton(tabRaw);
        pushButtonSeek->setObjectName("pushButtonSeek");

        horizontalLayout_3->addWidget(pushButtonSeek);

        pushButtonGetPos = new QPushButton(tabRaw);
        pushButtonGetPos->setObjectName("pushButtonGetPos");

        horizontalLayout_3->addWidget(pushButtonGetPos);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        lineEditRead = new QLineEdit(tabRaw);
        lineEditRead->setObjectName("lineEditRead");
        sizePolicy.setHeightForWidth(lineEditRead->sizePolicy().hasHeightForWidth());
        lineEditRead->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(lineEditRead);

        pushButtonRead = new QPushButton(tabRaw);
        pushButtonRead->setObjectName("pushButtonRead");

        horizontalLayout_4->addWidget(pushButtonRead);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_8 = new QLabel(tabRaw);
        label_8->setObjectName("label_8");
        label_8->setMinimumSize(QSize(100, 0));

        horizontalLayout_7->addWidget(label_8);

        labelRawCapacity = new QLabel(tabRaw);
        labelRawCapacity->setObjectName("labelRawCapacity");
        labelRawCapacity->setMinimumSize(QSize(150, 0));
        labelRawCapacity->setFont(font);

        horizontalLayout_7->addWidget(labelRawCapacity);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_8);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        label_9 = new QLabel(tabRaw);
        label_9->setObjectName("label_9");
        label_9->setMinimumSize(QSize(100, 0));

        horizontalLayout_8->addWidget(label_9);

        labelRawRemaining = new QLabel(tabRaw);
        labelRawRemaining->setObjectName("labelRawRemaining");
        labelRawRemaining->setMinimumSize(QSize(150, 0));
        labelRawRemaining->setFont(font);

        horizontalLayout_8->addWidget(labelRawRemaining);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        label_10 = new QLabel(tabRaw);
        label_10->setObjectName("label_10");
        label_10->setMinimumSize(QSize(100, 0));

        horizontalLayout_9->addWidget(label_10);

        labelRawBlockSize = new QLabel(tabRaw);
        labelRawBlockSize->setObjectName("labelRawBlockSize");
        labelRawBlockSize->setMinimumSize(QSize(150, 0));
        labelRawBlockSize->setFont(font);

        horizontalLayout_9->addWidget(labelRawBlockSize);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_10);


        verticalLayout_3->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        label_11 = new QLabel(tabRaw);
        label_11->setObjectName("label_11");
        label_11->setMinimumSize(QSize(100, 0));

        horizontalLayout_10->addWidget(label_11);

        labelRawPartitionCount = new QLabel(tabRaw);
        labelRawPartitionCount->setObjectName("labelRawPartitionCount");
        labelRawPartitionCount->setMinimumSize(QSize(150, 0));
        labelRawPartitionCount->setFont(font);

        horizontalLayout_10->addWidget(labelRawPartitionCount);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_11);


        verticalLayout_3->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        label_12 = new QLabel(tabRaw);
        label_12->setObjectName("label_12");
        label_12->setMinimumSize(QSize(100, 0));

        horizontalLayout_11->addWidget(label_12);

        labelRawWriteProtected = new QLabel(tabRaw);
        labelRawWriteProtected->setObjectName("labelRawWriteProtected");
        labelRawWriteProtected->setMinimumSize(QSize(150, 0));
        labelRawWriteProtected->setFont(font);

        horizontalLayout_11->addWidget(labelRawWriteProtected);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_12);


        verticalLayout_3->addLayout(horizontalLayout_11);

        verticalSpacer = new QSpacerItem(20, 559, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        tabWidget->addTab(tabRaw, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1152, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "myTape 0.3", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Tape Drive", nullptr));
        lineEditTapeDriveName->setText(QCoreApplication::translate("MainWindow", "tape0", nullptr));
        pushButtonOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "State:", nullptr));
        label_state->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "Pos:", nullptr));
        labelPos->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Sector:", nullptr));
        labelBlock->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Cache:", nullptr));
        labelCache->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        lineEditCache->setText(QCoreApplication::translate("MainWindow", "256", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Mb", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Free:", nullptr));
        labelFree->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        pushButtonSeekFirstFree->setText(QCoreApplication::translate("MainWindow", "Seek free", nullptr));
        pushButtonScan->setText(QCoreApplication::translate("MainWindow", "Scan", nullptr));
        pushButtonReadAbort->setText(QCoreApplication::translate("MainWindow", "Abort", nullptr));
        pushButtonNext->setText(QCoreApplication::translate("MainWindow", "Next", nullptr));
        pushButtonExport->setText(QCoreApplication::translate("MainWindow", "Export CSV", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Total:", nullptr));
        labelReadTotal->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        pushButtonRestore->setText(QCoreApplication::translate("MainWindow", "Restore", nullptr));
        pushButtonRestoreAll->setText(QCoreApplication::translate("MainWindow", "Restore All", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRead), QCoreApplication::translate("MainWindow", "Read", nullptr));
        pushButtonWriteAddDir->setText(QCoreApplication::translate("MainWindow", "Add Dir", nullptr));
        pushButtonWriteAddFile->setText(QCoreApplication::translate("MainWindow", "Add File", nullptr));
        pushButtonWriteClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        pushButtonWriteAbort->setText(QCoreApplication::translate("MainWindow", "Abort", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Total:", nullptr));
        labelWriteTotal->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        pushButtonWriteWrite->setText(QCoreApplication::translate("MainWindow", "Write", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabWrite), QCoreApplication::translate("MainWindow", "Write", nullptr));
        pushButtonSeek->setText(QCoreApplication::translate("MainWindow", "Seek", nullptr));
        pushButtonGetPos->setText(QCoreApplication::translate("MainWindow", "Get pos", nullptr));
        pushButtonRead->setText(QCoreApplication::translate("MainWindow", "Read", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Capacity", nullptr));
        labelRawCapacity->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Remaining", nullptr));
        labelRawRemaining->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "BlockSize", nullptr));
        labelRawBlockSize->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "PartitionCount", nullptr));
        labelRawPartitionCount->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "WriteProtected", nullptr));
        labelRawWriteProtected->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRaw), QCoreApplication::translate("MainWindow", "Raw", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
