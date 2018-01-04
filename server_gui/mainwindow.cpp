#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    ui->lstConsole->addItem(tr("Configuration file not loaded!"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                                         tr("Browse for configuration file"),
                                         tr(""),
                                         tr("xml Files (*.xml)"));
    ui->txtConfigFile->setText(fileName);
}
