//
// Created by dilin on 11/10/17.
//
#include <iostream>
#include "pugixml.hpp"
#include <opencv2/opencv.hpp>
#include <csignal>
#include "BGSDetector.h"
#include "ClientUDP.h"

#define DISPLAY_MAIN "Display Window"


using namespace std;
using namespace cv;


BGSDetector *detector;
ClientUDP *client;
bool trainingMode = false;

void signalHandler(int signum)
{
    cout << "Interrupt signal (" << signum << ") received.\n";

    // cleanup and close up stuff here

    if (trainingMode)
        detector->trainDetector();

    delete detector;

    delete client;

    exit(signum);
}

int main(int argc, const char *argv[])
{

#ifndef DISPLAY_MAIN
    signal(SIGINT, signalHandler);
#endif

    string serverIP = "localhost";
    unsigned short serverPort = 8080;
    string videoSource = "/dev/video0";
    string detectorSource;
    uint8_t cameraID = 0;
    int WIDTH, HEIGHT, FPS, BGS_TH;

    try
    {
        if (argc > 1)
        {
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(argv[1]);
            if (result)
            {
                pugi::xml_node config = doc.child("configuration");
                serverIP = config.child("server").attribute("ip").as_string();
                videoSource = config.child("video").attribute("source").as_string();
                WIDTH = config.child("video").attribute("width").as_int();
                HEIGHT = config.child("video").attribute("height").as_int();
                FPS = config.child("video").attribute("fps").as_int();
                cameraID = (uint8_t) config.child("camera").attribute("id").as_int();
                serverPort = (unsigned short) config.child("server").attribute("port").as_int();
                trainingMode = config.child("detector").attribute("training").as_bool();
                detectorSource = config.child("detector").attribute("source").as_string();
                BGS_TH = config.child("detector").attribute("bgs_th").as_int();
                detector = new BGSDetector(BGS_TH,
                                           BGS_MOVING_AVERAGE,
                                           false,
                                           detectorSource,
                                           trainingMode);
            }
        }

        boost::asio::io_service io_service;
        if (!trainingMode)
        {
            client = new ClientUDP(io_service, serverIP, serverPort);
        }


        Mat img;

        VideoCapture vcap(videoSource);
        if (!vcap.isOpened())
        {
            cout << "Error opening video stream or file" << endl;
            return -1;
        }

        vcap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
        vcap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
        vcap.set(CV_CAP_PROP_FPS, FPS);
        vcap.set(CV_CAP_PROP_AUTOFOCUS, 0);

        if (trainingMode)
            FPS = 1000;


#ifdef DISPLAY_MAIN
        namedWindow(DISPLAY_MAIN);
#endif
        int p, k;
        uint16_t frameNo = 0;

        for (;;)
        {

            vcap >> img;

            if (!img.data)
            {
                break;
            }


            vector<Rect> detections = detector->detect(img);

            if (!trainingMode)
            {
                Frame frame;
                frame.frameNo = frameNo;
                frame.cameraID = cameraID;
                for (int q = 0; q < detections.size(); q++)
                {
                    BoundingBox bbox;
                    bbox.x = detections[q].x;
                    bbox.y = detections[q].y;
                    bbox.width = detections[q].width;
                    bbox.height = detections[q].height;
                    frame.detections.push_back(bbox);

                    vector<uint16_t> histogram(512);
                    for (int r = 0; r < 512; r++)
                    {
                        histogram[r] = detector->histograms[q].at<unsigned short>(r);
                    }

                    frame.histograms.push_back(histogram);
                }
                frameNo++;
                frame.set_now();
                client->send(frame);
            }


#ifdef DISPLAY_MAIN
            for (int i = 0; i < detections.size(); i++)
            {
                rectangle(img, detections[i].tl(), detections[i].br(), cv::Scalar(0, 255, 0), 2);
            }
            imshow(DISPLAY_MAIN, img);
            if (waitKey(1000 / FPS) > 0)
            {
                break;
            }

#endif

        }
    }
    catch (exception e)
    {
        cerr << "Error: " << e.what() << endl;
    }


    signalHandler(0);

    return -1;

}

