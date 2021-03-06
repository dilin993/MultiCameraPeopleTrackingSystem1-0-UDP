//
// Created by dilin on 11/11/17.
//

#include <iostream>
#include <thread>
#include "pugixml.hpp"
#include "DataAssociation.h"
#include "graph.h"
#include "CameraConfig.h"
#include "ServerUDP.h"



#define DISPLAY_FLAG 1

using namespace std;

unsigned short port = 8080;

void server_thread_func(FrameQueue & frames)
{
    boost::asio::io_service io_service;
    ServerUDP server(io_service,port,frames);
    io_service.run();
}

int main(int argc, const char * argv[])
{
    try
    {
        unsigned short num_nodes = 0;
        unsigned short WIDTH = 0;
        unsigned short HEIGHT = 0;
        vector<CameraConfig> cameraConfigs;
        double DIST_TH;

        if(argc>1)
        {
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(argv[1]);
            if(result)
            {
                pugi::xml_node config = doc.child("configuration");
                num_nodes = (unsigned short)config.child("main").attribute("num_nodes").as_int();
                port = (unsigned short)config.child("main").attribute("port").as_int();
                WIDTH = (unsigned short)config.child("main").attribute("width").as_int();
                HEIGHT = (unsigned short)config.child("main").attribute("height").as_int();
                DIST_TH = config.child("main").attribute("dist_th").as_double();

                for(unsigned short n=0;n<num_nodes;n++)
                {
                    string camName = "camera" + to_string(n);
                    string configPath = config.child(camName.c_str()).attribute("config").as_string();

                    cameraConfigs.push_back(CameraConfig());

                    cameraConfigs[n].load(configPath);
                }
            }
            else
            {
                cout << "Invalid configuration file." << endl;
                return -1;
            }
        }
        else
        {
            cout << "usage: ./client <configuration-file>" << endl;
            return -1;
        }

        Graph graph(DIST_TH);
        vector<DataAssociation*> associations(num_nodes);

#ifdef DISPLAY_FLAG
        vector<Mat> imgs(num_nodes);
        vector<string> windowNames(num_nodes);
#endif

        for(unsigned short n=0;n<num_nodes;n++)
        {
            windowNames[n] = "camera" + to_string(cameraConfigs[n].getCameraID());
            associations[n] = new DataAssociation(cameraConfigs[n].getTRACK_INIT_TH(),
                                              cameraConfigs[n].getREJ_TOL(),
                                              WIDTH,HEIGHT);
#ifdef DISPLAY_FLAG
            namedWindow(windowNames[n]);
#endif
        }

        char chCheckForEscKey = 0;
        int sizes[3] = {8,8,8};
        vector<Point2f> detections;
        vector<Mat> histograms;
        uint8_t n;
        int k=0;
        vector<TrackedPoint> groundPlanePoints;

        FrameQueue frames;
        thread server_thread(server_thread_func,ref(frames)); // start server thread
        while(chCheckForEscKey!=27)
        {
            if(!frames.is_empty())
            {
                Frame frame = frames.dequeue();
                frame.print();

                n = frame.cameraID;

                detections.clear();
                histograms.clear();

                k=0;

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
                vector<ParticleFilterTracker> &tracks = associations[n]->getTracks();

#ifdef DISPLAY_FLAG
                // draw detections
//                imgs[n] = Mat::zeros(HEIGHT,WIDTH,CV_8UC3);

                groundPlanePoints.clear();

                for(int i=0;i<tracks.size();i++)
                {
                    Point2f pos = tracks[i].getPos();
//                    if(tracks[i].totalVisibleCount>30)
                        drawMarker(imgs[n], pos,
                                   tracks[i].color,
                                   MarkerTypes::MARKER_CROSS, 20, 5);
                    pos = cameraConfigs[n].convertToGround(pos);
                    groundPlanePoints.push_back(TrackedPoint(tracks[i].histogram,
                                                             pos));

                }



                imshow(windowNames[n],imgs[n]);
#endif

                // do correspondence estimation
                graph.addNodes((uint8_t)n,groundPlanePoints);

                vector<Point2f> uniquePoints = graph.getUniquePoints();

#ifdef DISPLAY_FLAG
                chCheckForEscKey = waitKey(1);
#endif

            }
        }

        server_thread.join();
    }
    catch (std::exception& e)
    {
        cerr << e.what() << endl;
    }

    return(0);
}