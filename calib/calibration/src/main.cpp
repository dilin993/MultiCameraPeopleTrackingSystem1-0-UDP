//
// Created by dilin on 1/3/18.
//
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define W_TITLE "Calibration"
#define W_MAP "FloorMap"

Mat img;
Mat mapImg;
vector<Point2f> points;
vector<Point2f> dstPoints;





void mouse_callback(int  event, int  x, int  y, int  flag, void *param)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        // Store point coordinates
        points.push_back(Point2f(x,y));
        circle(img, points.back(), 3, Scalar(0, 0, 255));
        string text = to_string(points.size());
        putText(img,text.c_str(), cvPoint(x+10,y+10),
                FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,0), 1, CV_AA);
        imshow(W_TITLE,img);
    }
}

void mouse_callback2(int  event, int  x, int  y, int  flag, void *param){
    if (event == EVENT_LBUTTONDOWN)
    {
        // Store point coordinates
        dstPoints.push_back(Point2f(x,y));
        circle(mapImg, dstPoints.back(), 3, Scalar(0, 0, 255));
        string text = to_string(dstPoints.size());
        putText(mapImg,text.c_str(), cvPoint(x+10,y+10),
                FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,0), 1, CV_AA);
        imshow(W_MAP,mapImg);
    }
}


int main(int argc,const char * argv[])
{
    if(argc <= 3)
    {
        cout << "usage: ./calibrate <image for calibration> <floormap> <output calibration file>" << endl;
        return -1;
    }

    img = imread(argv[1]);
    mapImg = imread(argv[2]);
    namedWindow(W_TITLE);

    setMouseCallback(W_TITLE, mouse_callback);

    imshow(W_TITLE,img);

    for(;;)
    {
        if(cvWaitKey(0)>0)
            break;
    }



    // destroyWindow(W_TITLE);


    if(points.size()<4)
    {
        cout << "Not enough points selected!" << endl;
        return -1;
    }

    namedWindow(W_MAP);

    setMouseCallback(W_MAP, mouse_callback2);

    imshow(W_MAP,mapImg);

    for(;;)
    {
        if(cvWaitKey(0)>0)
            break;
    }

    //vector<Point2f> dstPoints;
    // float x,y;
    // for(int i=0;i<points.size();i++)
    // {
    //     cout << "Enter point " << i+1 << " : ";
    //     cin >> x;
    //     cin >> y;
    //     dstPoints.push_back(Point2f(x,y));
    // }

    // calculate homography
    Mat H = findHomography(points,dstPoints,RHO);
    string filename(argv[3]);
    FileStorage fileStorage(filename,cv::FileStorage::WRITE);
    fileStorage << "homography" << H;
    cout << "Homography written to \'"
         << filename << "\'." << endl;

    cout << H.rows;
    return 0;
}

