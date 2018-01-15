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
#include "Graph.h"
#include "CameraConfig.h"
#include "serverthread.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QTime>
#include <qcustomplot.h>
#include <QFile>

QT_CHARTS_USE_NAMESPACE

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

    //heatmap and global tracking
    void update_globalTracks(vector<TrackedPoint> &trackedPoints);
    void update_heatmap(vector<Point2f> &trackedPoints);
    void analysis(vector<TrackedPoint> &trackedPoints);


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
   // void analysis( vector<Point2f> uniquePoints);

    QChartView *chartView;
    QLineSeries *series;
    QChart *chart;
    QCustomPlot * m_CustomPlot;
    // This object will hold the current value as a text
    // that will appear at the extreme right of the plot,
    QCPItemText *m_ValueIndex;

    // The time between each update, this
    // will be  used by the timer to call "updatePlot" slot
    qreal timeInterval;

    // Data buffers
    QVector<qreal> m_YData;
    QVector<qreal> m_XData;

    // Global Tracking
    Graph* graph;

    //Heatmap and Global Tracking View
    QGraphicsScene *imageScene;
    QGraphicsScene *globalScene;
    QGraphicsScene *heatmapScene;
    QGraphicsScene *overlayScene;

    cv::Mat floormap;
    cv::Mat colourMap;

    int count;

    cv::Mat map;
    cv::Mat pallete;

    cv::Mat loadFromQrc(QString qrc, int flag = IMREAD_COLOR);


};

#endif // MAINWINDOW_H
