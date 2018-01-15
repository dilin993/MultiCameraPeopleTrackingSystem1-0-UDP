#include "Graph.h"

TrackedPoint::TrackedPoint(uint8_t cameraID,
                           cv::MatND histogram,
                           cv::Point2f location,
                           cv::Scalar color):
    cameraID(cameraID),
    histogram(histogram),
    location(location),
    color(color)
{

}

Graph::Graph(double DIST_TH):
    DIST_TH(DIST_TH)
{

}

void Graph::addNode(TrackedPoint trackedPoint)
{
    nodes.push_back(trackedPoint);
}

void Graph::addNodes(std::vector<TrackedPoint> trackedPoints)
{
    for(auto trackedPoint:trackedPoints)
    {
       nodes.push_back(trackedPoint);
    }
}

std::vector<TrackedPoint> Graph::getUniquePoints()
{
    std::vector<TrackedPoint> uniquePoints;


    double minPosVal = DBL_MAX;
    int minI,minJ;

    while(minPosVal>0)
    {

        minPosVal = DBL_MAX;
        minI = -1;
        minJ = -1;

        calculateCosts(); // calculate cost matrix

        for(int i=0;i<nodes.size();i++)
        {
            for(int j=0;j<nodes.size();j++)
            {
                if(cost[i][j]>0 && cost[i][j]<minPosVal)
                {
                    minI = i;
                    minJ = j;
                    minPosVal = cost[i][j];
                }
            }
        }


        if(minI>-1 && minJ>-1)
        {
            cv::Point2f p1 = nodes[minI].location;
            cv::Point2f p2 = nodes[minJ].location;
            cv::Point2f p3((p1.x+p2.x)/2,(p1.y+p2.y)/2);
            cv::Mat hist = combineHistograms(nodes[minI].histogram,
                                         nodes[minJ].histogram);
            cv::Scalar color = combineColor(nodes[minI].color,
                                        nodes[minJ].color);
            TrackedPoint trp(nodes[minI].cameraID,
                             hist,
                             p3,
                             color);
            nodes.push_back(trp);
            if(minI>minJ)
            {
                nodes.erase(nodes.begin()+minI);
                nodes.erase(nodes.begin()+minJ);
            }
            else
            {
                nodes.erase(nodes.begin()+minJ);
                nodes.erase(nodes.begin()+minI);
            }
        }
        else
        {
            minPosVal = 0;
        }
    }

    for(auto const &node:nodes)
    {
        uniquePoints.push_back(node);
    }

    return uniquePoints;
}

void Graph::clear()
{
    nodes.clear();
}

void Graph::calculateCosts()
{
    cost.clear();
    for(auto const& node1:nodes)
    {
        std::vector<double> row;
        for(auto const& node2:nodes)
        {
            if((node1.cameraID)==(node2.cameraID))
                row.push_back(0);
            else
            {
                double d;
                d = averageError(node1.location,
                                 node2.location,
                                 node1.histogram,
                                 node2.histogram);
                if(d<DIST_TH)
                    row.push_back(d);
                else
                    row.push_back(0);

            }
        }
        cost.push_back(row);
    }
}

cv::Mat Graph::combineHistograms(cv::Mat &hist1, cv::Mat &hist2)
{
    cv::Mat hist = 0.5*hist1 + 0.5*hist2;
    return hist;
}

cv::Scalar Graph::combineColor(cv::Scalar color1, cv::Scalar color2)
{
    return cv::Scalar(std::max(color1.val[0],color2.val[0]),
            std::max(color1.val[1],color2.val[1]),
            std::max(color1.val[2],color2.val[2]));
}

double Graph::averageError(cv::Point2f a, cv::Point2f b, cv::MatND histA, cv::MatND histB)
{
    double constGaus = 1.0/sqrt(2*M_PI*var_m);
    double dist = pow(a.x-b.x,2.0);
    dist += pow(a.y-b.y,2.0);
    dist /= (2.0*var_m);
    double w_d = constGaus * exp(-dist);

    double w_c = compareHist(histA,histB,cv::HISTCMP_BHATTACHARYYA);
    w_c = 1 - w_c;

    double w =  (1-alpha)*w_d + alpha * w_c;
    return 1 - w;
}
