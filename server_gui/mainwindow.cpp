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

    chart = new QChart();
    chart->setTitle("People Density");
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->people_view->addWidget(chartView,4,4);

    //QDateTime m_nextDataTime = QDateTime::currentDateTime();
    //LineLayer *layer = c->addLineLayer();
    QTime::currentTime().toString();

    m_CustomPlot = new QCustomPlot( this );
    // The next line is responsible for adding
    // the plot widget to our application's main window
     ui->people_view->addWidget(m_CustomPlot,4,4);
    //setCentralWidget( m_CustomPlot ); // there is no need to add and promote a widget


    // Add a graph to the plot widget ( the main graph )
    m_CustomPlot->addGraph();
    // Add a graph to the plot widget ( this will be a horizontal line )
    // that folows the current value of the main graph
    m_CustomPlot->addGraph();

    // tell the plot widget to display the x axis values as a time
    // instead of numbers
    m_CustomPlot->xAxis->setTickLabelType( QCPAxis::ltDateTime );
    // Set the x axis time string format
    m_CustomPlot->xAxis->setDateTimeFormat( "hh:mm:ss:zzz" );
    m_CustomPlot->xAxis->setAutoTickStep( true );
    m_CustomPlot->yAxis->setAutoTickStep( true );

    // Allocating memory for the text item that will
    // display the current value as a text.
    m_ValueIndex = new QCPItemText( m_CustomPlot );
    // Set the font of the text item
    m_ValueIndex->setFont(QFont(font().family(), 8)); // make font a bit larger
    // Add the text item to the plt widget
    m_CustomPlot->addItem( m_ValueIndex );

    // Change the color of the graphs
    QColor brushClr = Qt::green;
    brushClr.setAlphaF( .5 );
    m_CustomPlot->graph( 0 )->setBrush( brushClr );
    m_CustomPlot->graph( 0 )->setPen( QColor() );
    m_CustomPlot->graph( 1 )->setPen( QColor() );


    // setup a timer and start it.
    //QTimer *timer = new QTimer(this);

    //timer->start( timeInterval );
//    imageScene = new QGraphicsScene(this);
    globalScene = new QGraphicsScene(this);
    heatmapScene = new QGraphicsScene(this);

   /* ui->graphicsView->setScene(imageScene)*/
    ui->heatmap_view->setStyleSheet("background: transparent");
    ui->glob_track_view->setScene(globalScene);
    ui->heatmap_view->setScene(heatmapScene);




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

    floormap = loadFromQrc(":/images/floormap.png");
    map = Mat::zeros(266,345,CV_8UC1);
    pallete = loadFromQrc(":/images/pallete.jpg");
    colourMap = cv::Mat::zeros(266,345,CV_8UC4);
    count = 0;
    overlayScene = new QGraphicsScene(this);
    ui->overlayView->setStyleSheet("background: transparent");
    ui->overlayView->setScene(overlayScene);

    QImage qImage((const uchar *) floormap.data, floormap.cols, floormap.rows, floormap.step, QImage::Format_RGB888);
    qImage.bits();

    overlayScene->clear();

    overlayScene->addPixmap(QPixmap::fromImage(qImage));
    overlayScene->update();
    ui->overlayView->update();
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

Mat MainWindow::loadFromQrc(QString qrc, int flag)
{
    QFile file(qrc);
    cv::Mat m;
    if(file.open(QIODevice::ReadOnly))
    {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        m = imdecode(buf, flag);
    }

    return m;
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

        globalAssociation = new DataAssociation(0.8,200,320,240); // TODO: Update parameters
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
        Mat mask;
        frame.getMask(mask);
        applyColorMap(mask,imgs[n],COLORMAP_JET);

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
    globalAssociation->assignTracks(uniquePoints,graph->histograms);
    vector<ParticleFilterTracker> &globalTracks = globalAssociation->getTracks();

    vector<Point2f> trackedPoints;
    vector<Scalar> colors;
    for(auto tracker:globalTracks)
    {
        trackedPoints.push_back(tracker.getPos());
        colors.push_back(tracker.color);
    }
    updateScenes();
    analysis(trackedPoints);
    update_globalTracks(uniquePoints,colors);
    update_heatmap(uniquePoints);

}

void MainWindow::analysis( vector<Point2f> uniquePoints)
{
    // Get the current time
    QTime tm = QTime::currentTime();
    qreal seconds = 60 * 60 * tm.hour() + 60 * tm.minute() + tm.second();
    qreal timeValue = 1000 * seconds + tm.msec();

    // Add the time the x data buffer
    m_XData.append( timeValue );

    // Generate random data with small variations
    // This will generate a random intiger between [ 0 , 1 ]
    qreal r = static_cast<qreal>( rand() ) / RAND_MAX  ;
    // the next value will be 80 plus or minus 5
    qreal value = 80 + 5 * r;
    m_YData.append( uniquePoints.size());

    // Keep the data buffers size under 100 value each,
    // so our moemoty won't explode with random numbers
    if( m_XData.size() > 100 ){
        m_XData.remove( 0 );
        m_YData.remove( 0 );
    }

    // Add the data to the graph
    m_CustomPlot->graph( 0 )->setData( m_XData , m_YData );
    // Now this is the tricky part, the previous part
    // was easy and nothing new in it.

    // Set the range of the vertical and horizontal axis of the plot ( not the graph )
    // so all the data will be centered. first we get the min and max of the x and y data
    QVector<double>::iterator xMaxIt = std::max_element( m_XData.begin() , m_XData.end() );
    QVector<double>::iterator xMinIt = std::min_element( m_XData.begin() , m_XData.end() );
    QVector<double>::iterator yMaxIt = std::max_element( m_YData.begin() , m_YData.end() );


    qreal yPlotMin = 0;
    qreal yPlotMax = *yMaxIt;

    qreal xPlotMin = *xMinIt;
    qreal xPlotMax = *xMaxIt;

    // The yOffset just to make sure that the graph won't take the whole
    // space in the plot widget, and to keep a margin at the top, the same goes for xOffset
    qreal yOffset = 0.3 * ( yPlotMax - yPlotMin ) ;
    qreal xOffset = 0.5 *( xPlotMax - xPlotMin );
    m_CustomPlot->xAxis->setRange( xPlotMin , xPlotMax + xOffset );
    m_CustomPlot->yAxis->setRange(yPlotMin , yPlotMax + yOffset);
    //************************************************************//
    // Generate the data for the horizontal line, that changes with
    // the last value of the main graph
    QVector<double> tmpYYData;
    QVector<double> tmpXXData;
    // Since it's a horizontal line, we only need to feed it two points
    // and both points have the y value
    tmpYYData.append( m_YData.last() );
    tmpYYData.append( m_YData.last() );

    // To make the line cross the plot widget horizontally,
    // from extreme left to extreme right
    tmpXXData.append( m_XData.first() );
    tmpXXData.append( m_XData.last() + xOffset );
    // Finaly set the horizental line data
    m_CustomPlot->graph( 1 )->setData( tmpXXData , tmpYYData );
//************************************************************//
    // Now to the text item that displays the current value
    // as a string.
    // These are the coordinates of the text item,
    // the offsets here are just to make the text appear
    // next and above a the horizontal line.
    qreal indexX = m_XData.last() + 0.5 * xOffset;
    qreal indexY = m_YData.last() + 0.2 * yOffset;

    // Set the coordinate that we calculated
    m_ValueIndex->position->setCoords( indexX , indexY );
    // Set the text that we want to display
    m_ValueIndex->setText(  QString::number( tmpYYData.last() ) + " " );


    // Update the plot widget
    m_CustomPlot->replot();

//    series = new QLineSeries();
//    int people_count = uniquePoints.size();



//   // series->append(QTime::currentTime().toString(), people_count);
//    series->append(2, 4);
//    series->append(3, 8);
//    series->append(7, 4);
//    series->append(10, 5);
//    //*series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

//    //chart->add
//    chart->legend()->hide();
//    chart->addSeries(series);
//    chart->createDefaultAxes();

//    ui->textEdit->append(QTime::currentTime().toString());
}

void MainWindow::update_globalTracks(vector<Point2f> &trackedPoints,vector<Scalar> &colors)
{
//    cv::Mat image;
    cv::Mat temp;
    floormap.copyTo(temp);

    for(int j=0;j<trackedPoints.size();j++)
    {
        Point2f pos(trackedPoints[j].x,
                    trackedPoints[j].y);
        drawMarker(temp, pos,
                   Scalar(0,0,0),//colors[j],
                   MarkerTypes::MARKER_CROSS, 5, 5);

    }

    QImage qImage((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    qImage.bits();

    globalScene->clear();

    globalScene->addPixmap(QPixmap::fromImage(qImage));
    globalScene->update();
    ui->glob_track_view->update();
}

void MainWindow::update_heatmap(vector<Point2f> &trackedPoints){

    //cv::Mat colourMap = cv::Mat::zeros(266,345,CV_8UC3);

    for(int k=0;k<trackedPoints.size();k++){
        int x = trackedPoints[k].x;
        int y = trackedPoints[k].y;
        //int val = pallete.at<cv::Vec3b>(0,0)[1];

        for (int i=0;i<266;i++){
            for (int j=0;j<345;j++){
                uint8_t temp = (map.at<uint8_t>(i,j)*count)/255;
                if (i>=y-5 && i<=y+5 && j>=x-5 &&j<=x+5){
                    map.at<uint8_t>(i,j) = (temp+25)*255/(count+1);
                    if (map.at<uint8_t>(i,j)==0){
                        colourMap.at<cv::Vec4b>(i,j)={0,0,0,0};
                    }
                    else{
                        cv::Vec3b val = pallete.at<cv::Vec3b>(map.at<uint8_t>(i,j),1);
                        colourMap.at<cv::Vec4b>(i,j) = {val[0],val[1],val[2],255};
                    }
//                map.at<uint8_t>(i,j)+=255;
                }
                else{
                    map.at<uint8_t>(i,j) = (temp)*255/(count+1);
                    if (map.at<uint8_t>(i,j)==0){
                        colourMap.at<cv::Vec4b>(i,j)={0,0,0,0};
                    }
                    else{
                        cv::Vec3b val = pallete.at<cv::Vec3b>(map.at<uint8_t>(i,j),1);
                        colourMap.at<cv::Vec4b>(i,j) = {val[0],val[1],val[2],255};
                    }
                }
            }
        }


        count+=1;
    }

//    cv::Mat colourMap;
//    cv::applyColorMap(map,colourMap,cv::COLORMAP_JET);
    cv::cvtColor(colourMap,colourMap,CV_BGRA2RGBA);
    //addWeighted(colourMap,0.9,floormap,0.1,0.0,colourMap);
    //colourMap = floormap + colourMap;
    //colourMap = cv::Mat::ones(266,345,CV_8UC3);

    QImage qImage((const uchar *) colourMap.data, colourMap.cols, colourMap.rows, colourMap.step, QImage::Format_RGBA8888);
    qImage.bits();

    heatmapScene->clear();

    heatmapScene->addPixmap(QPixmap::fromImage(qImage));
    heatmapScene->update();
    ui->heatmap_view->update();

}

