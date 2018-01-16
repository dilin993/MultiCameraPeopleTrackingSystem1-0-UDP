//
// Created by dilin on 10/10/17.
//

#ifndef TRACK_GRAPH_H
#define TRACK_GRAPH_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;

struct TrackedPoint
{
public:
    TrackedPoint(uint8_t cameraID,
                 cv::MatND histogram,
                 cv::Point2f location,
                 cv::Scalar color);
    uint8_t cameraID;
    cv::Mat histogram;
    cv::Point2f location;
    cv::Scalar color;
    void setTimeStamp(Time timeStamp);
    const Time getTimeStamp();
 private:
    Time timeStamp;
};

class Graph
{
public:
    Graph(double DIST_TH);
    ~Graph();
    void addNode(TrackedPoint trackedPoint);
    void addNodes(std::vector<TrackedPoint> trackedPoints);
    std::vector<TrackedPoint> getUniquePoints();
    void clear();

private:
    std::vector<TrackedPoint> nodes;
    std::vector<std::vector<double>> cost;
    void calculateCosts();
    double DIST_TH;
    double var_m=0.5;
    double alpha = 0.5;
    cv::Mat combineHistograms(cv::Mat &hist1 ,cv::Mat &hist2);
    cv::Scalar combineColor(cv::Scalar color1,cv::Scalar color2);
    double averageError(cv::Point2f a, cv::Point2f b, cv::MatND histA, cv::MatND histB);

};


#endif //TRACK_GRAPH_H
