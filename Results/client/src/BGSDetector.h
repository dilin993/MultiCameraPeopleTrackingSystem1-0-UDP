//
// Created by dilin on 10/23/17.
//

#ifndef TRACK_BGSDETECTOR_H
#define TRACK_BGSDETECTOR_H

#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/bgsegm.hpp>
#include "Detector.h"
#include "Histogram.h"

//#define BGS_DEBUG_MODE

#define FRAME_WAIT 300

class Blob
{
public:
    Blob(std::vector<cv::Point> _contour);
    std::vector<cv::Point> currentContour;

    cv::Rect currentBoundingRect;

    std::vector<cv::Point> centerPositions;

    double dblCurrentDiagonalSize;
    double dblCurrentAspectRatio;

};

enum
{
    BGS_MOVING_AVERAGE=1,
    BGS_GMM=2,
    BGS_HW=3
};

struct DetectionRecord
{
    float data[8];
};

class BGSDetector : public Detector
{
public:
    explicit BGSDetector(double TH=30,
                         int method=BGS_MOVING_AVERAGE,
                         bool doGammaCorrection=false,
                         string coeffFilePath="",
                         bool trainingMode=false);
    std::vector<cv::Rect> detect(cv::Mat &img);
    vector<DetectionRecord> data;
    void trainDetector();
    int method;
    cv::Mat mask;

private:
    void backgroundSubstraction(cv::Mat &frame0, cv::Mat &frame1, cv::Mat &frame2
            , cv::Mat &bgModel, cv::Mat &mask, double TH=15);
    cv::Mat frames[3];
    cv::Mat bgModel;
    uint8_t frameCount;
    double TH;
    cv::Ptr<cv::BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
    void GammaCorrection(cv::Mat& src, cv::Mat& dst, float fGamma);
    bool doGamaCorrection;
    string coeffFilePath;
    FileStorage coeffFile;
    bool trainingMode;
    double coeff[64];
    PCA pca;
    float detectorTH=0;


};


#endif //TRACK_BGSDETECTOR_H
