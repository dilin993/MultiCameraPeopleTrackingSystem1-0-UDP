//
// Created by dilin on 10/10/17.
//

#include "graph.h"

Graph::~Graph()
{
    return;
}

void Graph::addNode(uint8_t cameraID, TrackedPoint trackedPoint)
{
    nodes.push_back(GraphNode(cameraID,trackedPoint));
}

void Graph::calculateCosts()
{
    cost.clear();
    for(auto const& node1:nodes)
    {
        std::vector<double> row;
        for(auto const& node2:nodes)
        {
            if((node1.id)==(node2.id))
                row.push_back(0);
            else
            {
                double d = sqrt(pow(node1.location.x - node2.location.x,2.0)+
                                        pow(node1.location.y - node2.location.y,2.0));
                double dh = compareHist(node1.histogram,node2.histogram,cv::HISTCMP_BHATTACHARYYA);
                dh = 10.0 / (dh+1e-10);
                d += dh;
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

Graph::Graph(double DIST_TH)
{
    this->DIST_TH = DIST_TH;
}

std::vector<cv::Point2f> Graph::getUniquePoints()
{
    std::vector<cv::Point2f> uniquePoints;

    histograms.clear();


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
            uniquePoints.push_back(cv::Point2f((p1.x+p2.x)/2,(p1.y+p2.y)/2));
            cv::Mat histogram = combineHistograms(nodes[minI].histogram,
                                              nodes[minJ].histogram);
            histograms.push_back(histogram);
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
        uniquePoints.push_back(node.location);
        histograms.push_back(node.histogram);
    }

    return uniquePoints;
}

void Graph::clear()
{
    nodes.clear();
}

void Graph::addNodes(uint8_t cameraID, std::vector<TrackedPoint> trackedPoints)
{
    for(auto const & trackedPoint: trackedPoints)
    {
        addNode(cameraID,trackedPoint);
    }
}

GraphNode::GraphNode(uint8_t id, TrackedPoint trackedPoint)
{
    this->id = id;
    this->location = trackedPoint.location;
    this->histogram = trackedPoint.histogram.clone();
}


TrackedPoint::TrackedPoint(cv::Mat histogram,cv::Point2f location,cv::Scalar color)
{
    this->histogram = histogram.clone();
    this->location = location;
    this->color = color;
}
