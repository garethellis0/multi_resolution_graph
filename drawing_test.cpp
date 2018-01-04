//
// Created by gareth on 04/01/18.
//
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <GraphNode.h>

cv::Mat drawNode(Node& node, int window_resolution);

cv::Mat drawRealNode(RealNode node, int window_resolution){
    cv::Mat image = cv::Mat::zeros(window_resolution, window_resolution, CV_8UC3);
    rectangle(image, {0,0}, {window_resolution,window_resolution},
              cv::Scalar(0, 255, 0), 0.05 * window_resolution);
    return image;
}

cv::Mat drawGraphNode(GraphNode node, int window_resolution){
    cv::Mat image = cv::Mat::zeros(window_resolution, window_resolution, CV_8UC3);
    for (int rowIndex = 0; rowIndex < node.getResolution(); rowIndex++){
        for (int colIndex = 0; colIndex < node.getResolution(); colIndex++){
            cv::Mat subNode = drawNode(*node.getSubNodes()[rowIndex][colIndex],
                             window_resolution/node.getResolution());
            cv::Rect roi(
                    window_resolution/node.getResolution() * rowIndex,
                    window_resolution/node.getResolution() * colIndex,
                    window_resolution/node.getResolution(),
                    window_resolution/node.getResolution()
            );
            cv::Mat imgRoi(image, roi);
            subNode.copyTo(imgRoi);
        }
    }
    return image;
}

cv::Mat drawNode(Node& node, int window_resolution){
    if (typeid(node) == typeid(GraphNode)){
        return drawGraphNode(static_cast<GraphNode&>(node), window_resolution);
    }
    if (typeid(node) == typeid(RealNode)){
        return drawRealNode(static_cast<RealNode&>(node), window_resolution);
    }
    // Should never get here
    BOOST_ASSERT(false);
}


int main(){
    // Create the mat
    int window_resolution = 600;

    // Draw the graph over the cv::Mat
    GraphNode graphNode(10);
    auto subNodes = graphNode.getSubNodes();
    graphNode.increaseResolutionOfNode(subNodes[0][0], 3);
    cv::Mat image = drawNode(graphNode, window_resolution);
    // Display the graph
    imshow("Graph", image);
    cv::waitKey(0);
    return 0;
}

