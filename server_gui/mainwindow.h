#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStringList>
#include <QFileInfo>
#include <QSettings>
#include <QTimer>
#include <QGraphicsScene>
#include <thread>
#include "pugixml.hpp"
#include "DataAssociation.h"
#include "graph.h"
#include "CameraConfig.h"
#include "serverthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnBrowse_clicked();

    void on_txtConfigFile_textChanged(const QString &arg1);

    void on_btnStartServer_clicked();

    // Object Tracking
    void doTracking();

private:
    Ui::MainWindow *ui;
    QString m_sSettingsFile;
    QString configFile;
    void loadSettings();
    void saveSettings();
    bool fileExists(QString path);
    QSettings *settings;

    // Server configuration
    unsigned short NUM_NODES = 0;
    unsigned short WIDTH = 0;
    unsigned short HEIGHT = 0;
    unsigned short PORT;
    vector<CameraConfig> cameraConfigs;
    double DIST_TH;
    FrameQueue frames;
    ServerThread *serverThread;
    QTimer *timer;

    // Tracking
    vector<DataAssociation*> associations;
    vector<Mat> imgs;
    QGraphicsScene* camScenes[2];
    void updateScenes();

    // Global Tracking
    Graph* graph;


};

#endif // MAINWINDOW_H
