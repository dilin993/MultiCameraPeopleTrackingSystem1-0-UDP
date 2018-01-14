//
// Created by dilin on 11/11/17.
//

#ifndef CLIENT_FRAME_H
#define CLIENT_FRAME_H

#include <boost/serialization/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

#define IMG_WIDTH 320
#define IMG_HEIGHT 240
#define IMG_N IMG_WIDTH * IMG_HEIGHT/8 + 5


using namespace std;
using namespace cv;

typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;


struct BoundingBox
{
    int x;
    int y;
    int width;
    int height;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & x;
        ar & y;
        ar & width;
        ar & height;
    }
};

class Frame
{
public:
    vector<BoundingBox> detections;
    vector<vector<uint16_t >> histograms;
    uint16_t frameNo;
    uint8_t cameraID;
    Time timeStamp;
    boost::array<uchar, IMG_N> buffer;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & detections;
        ar & histograms;
        ar & frameNo;
        ar & cameraID;
        ar & timeStamp;
        ar & buffer;
    }

    void print()
    {
        cout << "Frame\n{";
        cout << "\tCameraID: " << (int)cameraID << "\n";
        cout << "\tFrameNO: " << frameNo << "\n";
        cout << "\tdetections=[";
        for(auto const detection:detections)
        {
            cout << "[";
            cout << detection.x << ",";
            cout << detection.y << ",";
            cout << detection.width << ",";
            cout << detection.height << "],";
        }
        cout << "]\n}\n";
    }

    void set_now()
    {
        timeStamp = Time(boost::posix_time::microsec_clock::local_time());
    }

    void setMask(Mat &binMask)
    {
        assert(binMask.rows==IMG_HEIGHT && binMask.cols==IMG_WIDTH);

        buffer[0] = (uchar)(binMask.rows & 255);
        buffer[1] = (uchar)(binMask.rows  >> 8);
        buffer[2] = (uchar)(binMask.cols & 255);
        buffer[3] = (uchar)(binMask.cols  >> 8);

        int bytePos = 4;
        uchar bitPos = 0;
        uchar temp;
        uchar bit;

        for(int i=0;i<binMask.rows;i++)
        {
            for(int j=0;j<binMask.cols;j++)
            {
                if(binMask.at<uchar>(i,j)>0)
                    bit = 1;
                else
                    bit = 0;

                if(bitPos==0)
                    temp = bit;
                else
                    temp = (temp<<1) | bit;

                bitPos++;
                if(bitPos>=8)
                {
                    buffer[bytePos] = temp;
                    bitPos = 0;
                    bytePos++;
                }
            }
        }

    }


    void getMask(Mat &binMask)
    {
        int rows = (buffer[1] << 8) + buffer[0];
        int cols = (buffer[3] << 8) + buffer[2];

        assert(rows==IMG_HEIGHT && cols==IMG_WIDTH);

        if(!binMask.data)
            binMask = Mat(rows,cols,CV_8UC1);

        int bytePos = 4;
        uchar bitPos = 0;
        uchar temp;

        for(int i=0;i<binMask.rows;i++)
        {
            for(int j=0;j<binMask.cols;j++)
            {
                temp = buffer[bytePos];
                temp = (temp & (uchar)(1<<(7-bitPos)))>>(7-bitPos);
                binMask.at<uchar>(i,j) = (uchar)(temp*255);
                bitPos++;
                if(bitPos>=8)
                {
                    bitPos = 0;
                    bytePos++;
                }
            }
        }
    }


};


#endif //CLIENT_FRAME_H
