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


using namespace std;
using namespace cv;

typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;

//namespace boost {
//    namespace archive {
//
//        namespace chrn = boost::chrono;
//
//        template<class Archive, typename clock>
//        void load(Archive& ar, chrn::time_point<clock>& tp, unsigned)
//        {
//            chrn::milliseconds::rep millis;
//            ar & millis;
//            tp = chrn::time_point<clock>(chrn::milliseconds(millis));
//        }
//
//        template<class Archive, typename clock>
//        void save(Archive& ar, chrn::time_point<clock> const& tp, unsigned)
//        {
//            chrn::milliseconds::rep millis = chrn::duration_cast<chrn::milliseconds>(tp.time_since_epoch()).count();
//            ar & millis;
//        }
//
//        template<class Archive, typename clock>
//        inline void serialize(Archive & ar, chrn::time_point<clock>& tp, unsigned version)
//        {
//            boost::serialization::split_free(ar, tp, version);
//        }
//    }
//}


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

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & detections;
        ar & histograms;
        ar & frameNo;
        ar & cameraID;
        ar & timeStamp;
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

};


#endif //CLIENT_FRAME_H
