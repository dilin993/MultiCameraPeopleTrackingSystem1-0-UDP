//
// Created by dilin on 11/10/17.
//
#include <iostream>
#include "pugixml.hpp"
#include <opencv2/opencv.hpp>
#include <csignal>
#include "BGSDetector.h"
#include "core.h"

#define DISPLAY_MAIN "Display Window"


using namespace std;
using namespace cv;


uint8_t data_array[IMG_SIZE];
bool init;
float bgmodel[4*BGM_SIZE];


BGSDetector *detector;
bool trainingMode = false;

void signalHandler(int signum) {
    cout << "Interrupt signal (" << signum << ") received.\n";

    // cleanup and close up stuff here

    if (trainingMode)
        detector->trainDetector();

    delete detector;

    exit(signum);
}

int main(int argc, const char *argv[]) {

    string serverIP = "localhost";
    unsigned short serverPort = 8080;
    string videoSource = "/dev/video0";
    string detectorSource;
    uint8_t cameraID = 0;
    int width, height, FPS, BGS_TH;


    if (argc > 1) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(argv[1]);
        if (result) {
            pugi::xml_node config = doc.child("configuration");
            serverIP = config.child("server").attribute("ip").as_string();
            videoSource = config.child("video").attribute("source").as_string();
            width = config.child("video").attribute("width").as_int();
            height = config.child("video").attribute("height").as_int();
            FPS = config.child("video").attribute("fps").as_int();
            cameraID = (uint8_t) config.child("camera").attribute("id").as_int();
            serverPort = (unsigned short) config.child("server").attribute("port").as_int();
            trainingMode = config.child("detector").attribute("training").as_bool();
            detectorSource = config.child("detector").attribute("source").as_string();
            BGS_TH = config.child("detector").attribute("bgs_th").as_int();
            detector = new BGSDetector(BGS_TH,
                                       BGS_HW,
                                       false,
                                       detectorSource,
                                       trainingMode);
        }
    }


    Mat img, gray;
    Mat mask(HEIGHT, WIDTH, CV_8UC1);

    VideoCapture vcap(videoSource);
    if (!vcap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    vcap.set(CV_CAP_PROP_FRAME_WIDTH, width);
    vcap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    vcap.set(CV_CAP_PROP_FPS, FPS);
    vcap.set(CV_CAP_PROP_AUTOFOCUS, 0);

    if (trainingMode)
        FPS = 1000;


#ifdef DISPLAY_MAIN
    namedWindow(DISPLAY_MAIN);
#endif
    int p, k;
    uint16_t frameNo = 0;

    init = true;

    for (;;) {

        vcap >> img;

        if (!img.data) {
            break;
        }


        vector<Rect> detections;

        if (detector->method == BGS_HW) {
            cvtColor(img, gray, CV_BGR2GRAY);

            memcpy(data_array, gray.data, IMG_SIZE);

            bgsub(data_array, mask.data, init, bgmodel);

            if (init) init = false;
            detections = detector->detect(mask);
        } else
            detections = detector->detect(img);


        for (int i = 0; i < detections.size(); i++) {
            rectangle(img, detections[i].tl(), detections[i].br(), cv::Scalar(0, 255, 0), 2);
        }
        imshow(DISPLAY_MAIN, img);
        if(!trainingMode)
        {
            waitKey(0);

            for(int i=0;i<9;i++) vcap.grab();
        }
        else
        {
            if(waitKey(1)>0) break;
        }

    }

    signalHandler(0);

    return -1;

}

