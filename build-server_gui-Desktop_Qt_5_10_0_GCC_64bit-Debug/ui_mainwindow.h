/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_4;
    QListWidget *lstConsole;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *txtConfigFile;
    QPushButton *btnBrowse;
    QPushButton *btnStartServer;
    QWidget *tab_2;
    QWidget *horizontalLayoutWidget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QGraphicsView *graphicsView_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QGraphicsView *graphicsView;
    QWidget *tab_3;
    QGraphicsView *glob_track_view;
    QWidget *tab_4;
    QWidget *tab_5;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setMinimumSize(QSize(800, 600));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        lstConsole = new QListWidget(tab);
        lstConsole->setObjectName(QStringLiteral("lstConsole"));

        verticalLayout_4->addWidget(lstConsole);

        widget = new QWidget(tab);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        txtConfigFile = new QLineEdit(widget);
        txtConfigFile->setObjectName(QStringLiteral("txtConfigFile"));

        horizontalLayout_3->addWidget(txtConfigFile);

        btnBrowse = new QPushButton(widget);
        btnBrowse->setObjectName(QStringLiteral("btnBrowse"));

        horizontalLayout_3->addWidget(btnBrowse);


        verticalLayout_4->addWidget(widget);


        horizontalLayout->addLayout(verticalLayout_4);

        btnStartServer = new QPushButton(tab);
        btnStartServer->setObjectName(QStringLiteral("btnStartServer"));
        QFont font;
        font.setPointSize(14);
        btnStartServer->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Tab_Icons/icons/server.jpeg"), QSize(), QIcon::Normal, QIcon::Off);
        btnStartServer->setIcon(icon);
        btnStartServer->setIconSize(QSize(32, 32));

        horizontalLayout->addWidget(btnStartServer);


        gridLayout_3->addLayout(horizontalLayout, 0, 0, 1, 1);

        QIcon icon1;
        icon1.addFile(QStringLiteral(":/Tab_Icons/icons/options-512.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab, icon1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalLayoutWidget = new QWidget(tab_2);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 652, 272));
        gridLayout_2 = new QGridLayout(horizontalLayoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_4 = new QLabel(horizontalLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_4);

        graphicsView_2 = new QGraphicsView(horizontalLayoutWidget);
        graphicsView_2->setObjectName(QStringLiteral("graphicsView_2"));
        graphicsView_2->setMinimumSize(QSize(320, 240));
        graphicsView_2->setMaximumSize(QSize(320, 240));

        verticalLayout_3->addWidget(graphicsView_2);


        gridLayout_2->addLayout(verticalLayout_3, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_3);

        graphicsView = new QGraphicsView(horizontalLayoutWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setMinimumSize(QSize(320, 240));
        graphicsView->setMaximumSize(QSize(320, 240));

        verticalLayout_2->addWidget(graphicsView);


        gridLayout_2->addLayout(verticalLayout_2, 0, 1, 1, 1);

        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Tab_Icons/icons/cctv.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_2, icon2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        glob_track_view = new QGraphicsView(tab_3);
        glob_track_view->setObjectName(QStringLiteral("glob_track_view"));
        glob_track_view->setGeometry(QRect(0, 0, 831, 611));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/Tab_Icons/icons/tracking.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_3, icon3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/Tab_Icons/icons/heatmap.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_4, icon4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/Tab_Icons/icons/analysis.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_5, icon5, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "People Tracking System", nullptr));
        label->setText(QApplication::translate("MainWindow", "Configuration file :", nullptr));
        btnBrowse->setText(QApplication::translate("MainWindow", "Browse", nullptr));
        btnStartServer->setText(QApplication::translate("MainWindow", "Start Server", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Configuration", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Camera - 1", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Camera - 2</p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Camera", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Global Tracking", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Heat Map", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("MainWindow", "Analysis", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
