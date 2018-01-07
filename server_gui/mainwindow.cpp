#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    ui->btnStartServer->setEnabled(false);
    m_sSettingsFile = "./settings.ini";
    settings = new QSettings(m_sSettingsFile, QSettings::IniFormat);
    loadSettings();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(doTracking()));

    camScenes[0] = new QGraphicsScene(this);
    ui->gvCam1->setScene(camScenes[0]);
    camScenes[1] = new QGraphicsScene(this);
    ui->gvCam2->setScene(camScenes[1]);
}

MainWindow::~MainWindow()
{
    delete ui;
    settings->sync();
    serverThread->exit();
}

void MainWindow::on_btnBrowse_clicked()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                                         tr("Browse for configuration file"),
                                         tr(""),
                                         tr("xml Files (*.xml)"));
    ui->txtConfigFile->setText(fileName);
    configFile = fileName;
    saveSettings();
}

void MainWindow::loadSettings()
{
    configFile = settings->value("configuration_file", "").toString();
    if (configFile.length())
    {
        ui->txtConfigFile->setText(configFile);
    }
}

void MainWindow::saveSettings()
{
    settings->setValue("configuration_file", configFile);
    if (configFile.length())
    {
        ui->txtConfigFile->setText(configFile);
    }
    settings->sync();
}

bool MainWindow::fileExists(QString path)
{
    QFileInfo check_file(path);
       // check if file exists and if yes: Is it really a file and no directory?
       if (check_file.exists() && check_file.isFile())
       {
           return true;
       } else
       {
           return false;
       }
}

void MainWindow::updateScenes()
{
    for(int i=0;i<NUM_NODES;i++)
    {
        QImage qImage((const uchar *) imgs[i].data,
                      imgs[i].cols,
                      imgs[i].rows,
                      imgs[i].step,
                      QImage::Format_RGB888);
        qImage.bits();

        camScenes[i]->clear();

        camScenes[i]->addPixmap(QPixmap::fromImage(qImage));
        camScenes[i]->update();
    }
    ui->gvCam1->update();
    ui->gvCam2->update();
}

void MainWindow::on_txtConfigFile_textChanged(const QString &arg1)
{
    if(fileExists(arg1))
        ui->btnStartServer->setEnabled(true);
    else
        ui->btnStartServer->setEnabled(false);
}

void MainWindow::on_btnStartServer_clicked()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(configFile.toUtf8().constData());
    if(result)
    {
        pugi::xml_node config = doc.child("configuration");
        NUM_NODES = (unsigned short)config.child("main").attribute("num_nodes").as_int();
        PORT = (unsigned short)config.child("main").attribute("port").as_int();
        WIDTH = (unsigned short)config.child("main").attribute("width").as_int();
        HEIGHT = (unsigned short)config.child("main").attribute("height").as_int();
        DIST_TH = config.child("main").attribute("dist_th").as_double();

        graph = new Graph(DIST_TH);

        for(unsigned short n=0;n<NUM_NODES;n++)
        {
            string camName = "camera" + to_string(n);
            string configPath = config.child(camName.c_str()).attribute("config").as_string();

            cameraConfigs.push_back(CameraConfig());

            cameraConfigs[n].load(configPath);

            associations.push_back(
                        new DataAssociation(cameraConfigs[n].getTRACK_INIT_TH(),
                                            cameraConfigs[n].getREJ_TOL(),
                                            WIDTH,HEIGHT));
            imgs.push_back(Mat::zeros(HEIGHT,WIDTH,CV_8UC3));
        }
        ui->txtConsole->appendPlainText(tr("Configuration loaded.\n"));
        ui->btnStartServer->setEnabled(false);
        serverThread = new ServerThread(frames,PORT);
        serverThread->start();
        ui->txtConsole->appendPlainText(tr("Server started...\n"));
        timer->start(100); // Start time to handle frames
    }
    else
    {
        ui->txtConsole->appendPlainText(tr("Invalid configuration file!\n"));
    }
}

void MainWindow::doTracking()
{
    vector<Point2f> detections;
    vector<Mat> histograms;
    int k = 0;
    while(!frames.is_empty())
    {
        Frame frame = frames.dequeue();
        frame.print();
        int n = frame.cameraID;
        detections.clear();
        histograms.clear();
        k = 0;
        imgs[n] = Mat::zeros(HEIGHT,WIDTH,CV_8UC3);
        for (auto const bbox : frame.detections)
        {
            Rect detection(bbox.x, bbox.y, bbox.width, bbox.height);

            rectangle(imgs[n], detection.tl(), detection.br(), cv::Scalar(0, 255, 0), 2);

            // track bottom middle point
            detections.push_back(Point(bbox.x+bbox.width/2,bbox.y+bbox.height));

            Mat normalizedHistogram(512,1,CV_32F),histogram(512,1,CV_16U);
            for(int i=0;i<512;i++)
            {
                histogram.at<unsigned short>(i) = frame.histograms[k][i];
            }
            histogram.convertTo(histogram,CV_32F);
            normalize(histogram, normalizedHistogram, 1,0, cv::NORM_MINMAX);
            histograms.push_back(normalizedHistogram);
            k++;

        }
        associations[n]->assignTracks(detections,histograms);
        associations[n]->setTimeStamp(frame.timeStamp);
        vector<ParticleFilterTracker> &tracks = associations[n]->getTracks();
        for(unsigned int i=0;i<tracks.size();i++)
        {
            Point2f pos = tracks[i].getPos();
            drawMarker(imgs[n], pos,
                       tracks[i].color,
                       MarkerTypes::MARKER_CROSS, 20, 5);

        }
    }

    vector<TrackedPoint> groundPlanePoints;
    graph->clear();
    for(int i=0;i<NUM_NODES;i++)
    {
        vector<ParticleFilterTracker> &tracks = associations[i]->getTracks();
        for(int j=0;j<tracks.size();j++)
        {
            Point2f pos = tracks[j].getPos();
            pos = cameraConfigs[i].convertToGround(pos);
            groundPlanePoints.push_back(TrackedPoint(tracks[j].histogram,
                                                     pos,
                                                     tracks[j].color));
        }
        // do correspondence estimation
        graph->addNodes((uint8_t)i,groundPlanePoints);
    }

    vector<Point2f> uniquePoints = graph->getUniquePoints();

    updateScenes();

}
