//
// Created by gareth on 04/01/18.
//
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <GraphNode.h>
#include <GraphFactory.h>
#include <iostream>
#include <chrono>

// Create the mat
int window_resolution = 800;
//TODO: This should be neither a raw pointer, nor a global variable
GraphNode<int>* graphNode;
cv::Mat globalImage;

template <typename T>
cv::Mat drawNode(Node<T>& node, int window_resolution);

template <typename T>
cv::Mat drawRealNode(RealNode<T> node, int window_resolution){
    cv::Mat image = cv::Mat::zeros(window_resolution, window_resolution, CV_8UC3);
    rectangle(image, {0,0}, {window_resolution,window_resolution},
              cv::Scalar(0, 255, 0), 0.05 * window_resolution);
    return image;
}

template <typename T>
cv::Mat drawGraphNode(GraphNode<T> node, int window_resolution){
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

template <typename T>
cv::Mat drawNode(Node<T>& node, int window_resolution){
    if (typeid(node) == typeid(GraphNode<T>)){
        return drawGraphNode(static_cast<GraphNode<T>&>(node), window_resolution);
    }
    if (typeid(node) == typeid(RealNode<T>)){
        return drawRealNode(static_cast<RealNode<T>&>(node), window_resolution);
    }
    // Should never get here
    BOOST_ASSERT(false);
}

void callBack(int event, int y, int x, int flags, void* userdata){
    if (event == CV_EVENT_LBUTTONUP) {
        // Convert window coordinates to graph coordinates
        Coordinates coordinates = {
                (x / (double)window_resolution) * graphNode->getScale(),
                (y / (double)window_resolution) * graphNode->getScale(),
        };
        graphNode->changeResolutionOfClosestNode(coordinates, 2);
        globalImage = drawNode(*graphNode, window_resolution);
        imshow("Graph", globalImage);
        cvSetMouseCallback("Graph", callBack, NULL);
        cv::waitKey(0);
    }
}


int main(){

    GraphFactory<int> graph_factory;
    graph_factory.setGraphScale(100);
    graph_factory.setGraphTopLevelResolution(4);

    GraphFactory<int>::Rectangle rectangle =
            GraphFactory<int>::Rectangle(0,0,(Coordinates){0,0});
    rectangle = GraphFactory<int>::Rectangle(10,10,(Coordinates){10,10});
    graph_factory.setMaxScaleInArea(rectangle, 0.5);
    rectangle = GraphFactory<int>::Rectangle(10,10,(Coordinates){30,40});
    graph_factory.setMaxScaleInArea(rectangle, 1);
    rectangle = GraphFactory<int>::Rectangle(30,10,(Coordinates){60,40});
    graph_factory.setMaxScaleInArea(rectangle, 1);
    rectangle = GraphFactory<int>::Rectangle(10,60,(Coordinates){70,40});
    graph_factory.setMaxScaleInArea(rectangle, 0.5);
    rectangle = GraphFactory<int>::Rectangle(5,10,(Coordinates){80,80});
    graph_factory.setMaxScaleInArea(rectangle, 0.5);

    GraphFactory<int>::Circle circle =
            GraphFactory<int>::Circle(0,(Coordinates){0,0});
    circle = GraphFactory<int>::Circle(10,(Coordinates){40,40});
    graph_factory.setMaxScaleInArea(circle, 0.5);
    circle = GraphFactory<int>::Circle(10,(Coordinates){70,70});
    graph_factory.setMaxScaleInArea(circle, 0.5);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto graphNode_val = graph_factory.createGraph();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time to generate graph = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
    std::cout << "Done generating" << std::endl;
    graphNode = &graphNode_val;

    // Draw the graph over the cv::Mat
    globalImage = drawNode(*graphNode, window_resolution);
    // Display the graph
    imshow("Graph", globalImage);
    cvSetMouseCallback("Graph", callBack, NULL);
    cv::waitKey(0);
    return 0;
}
