#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <GraphNode.h>
#include <GraphFactory.h>
#include <iostream>
#include <chrono>
#include <Rectangle.h>
#include <Circle.h>

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
                    window_resolution/node.getResolution() * colIndex,
                    window_resolution/node.getResolution() * rowIndex,
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
                (y / (double)window_resolution) * graphNode->getScale(),
                (x / (double)window_resolution) * graphNode->getScale(),
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
    graph_factory.setGraphScale(9);
    graph_factory.setGraphTopLevelResolution(1);

    Rectangle<int> rectangle =
            Rectangle<int>(0,0,(Coordinates){0,0});
    // Base Field
    rectangle = Rectangle<int>(6,9,(Coordinates){1.5,0});
    graph_factory.setMaxScaleInArea(rectangle, 0.2);
    // Defensive Areas
    rectangle = Rectangle<int>(2,1,(Coordinates){3.5,0});
    graph_factory.setMaxScaleInArea(rectangle, 0.1);
    rectangle = Rectangle<int>(2,1,(Coordinates){3.5,8});
    graph_factory.setMaxScaleInArea(rectangle, 0.1);
//    rectangle = Rectangle<int>(3,2,(Coordinates){4.5,0});
//    graph_factory.setMaxScaleInArea(rectangle, 0.1);
//    rectangle = Rectangle<int>(1,1,(Coordinates){4.5,4.5});
//    graph_factory.setMaxScaleInArea(rectangle, 0.05);
//    rectangle = Rectangle<int>(1,1,(Coordinates){1,1});
//    graph_factory.setMaxScaleInArea(rectangle, 0.05);
//    rectangle = Rectangle<int>(1,1,(Coordinates){7,6});
//    graph_factory.setMaxScaleInArea(rectangle, 0.05);
    //rectangle = Rectangle<int>(10,10,(Coordinates){10,10});
    //graph_factory.setMaxScaleInArea(rectangle, 0.5);
    //rectangle = Rectangle<int>(10,10,(Coordinates){30,40});
    //graph_factory.setMaxScaleInArea(rectangle, 1);
    //rectangle = Rectangle<int>(30,10,(Coordinates){60,40});
    //graph_factory.setMaxScaleInArea(rectangle, 1);
    //rectangle = Rectangle<int>(10,60,(Coordinates){70,40});
    //graph_factory.setMaxScaleInArea(rectangle, 0.5);
    //rectangle = Rectangle<int>(5,10,(Coordinates){80,80});
    //graph_factory.setMaxScaleInArea(rectangle, 0.5);

    Circle<int> circle =
            Circle<int>(0,(Coordinates){0,0});
    // Robots
    circle = Circle<int>(0.2,(Coordinates){7,3});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){3,7});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){4.7,7.8});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){4.2,0.6});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){3,2});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){3.7,2.7});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){4.5,2.4});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){6,4});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){5.4,3.8});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){5,6});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){4.8,6.4});
    graph_factory.setMaxScaleInArea(circle, 0.05);
    circle = Circle<int>(0.2,(Coordinates){3,4.5});
    graph_factory.setMaxScaleInArea(circle, 0.05);

    /*
    std::vector<std::pair<Area<int>*, double>> areas_and_scales;

    // We're basically just randomly generating lots of different
    // Rectangles and Circles
    for (int i = 1; i < 10; i++){
        for (int j = 1; j < 10; j++){
            areas_and_scales.emplace_back(
                    std::make_pair(
                            new Rectangle<int>(i*3, j*2, {i*2,i*3}),
                            10/i
                    ));
            areas_and_scales.emplace_back(
                    std::make_pair(
                            new Circle<int>(i*2, {i*2, 100 - i*3}),
                            10/j
                    ));
            areas_and_scales.emplace_back(
                    std::make_pair(
                            new Rectangle<int>(i*2, j*3, {i*2,i*3}),
                            10/j
                    ));
            areas_and_scales.emplace_back(
                    std::make_pair(
                            new Circle<int>(i*2, {100 - i*2, 100 - i*3}),
                            1/j
                    ));
        }
    }

    // Set the resolution of all the areas
    for (const auto& area_scale_pair : areas_and_scales){
        Area<int>* area = area_scale_pair.first;
        double scale = area_scale_pair.second;
        graph_factory.setMaxScaleInArea(*area, scale);
    }
     */

    // Generate the Graph
    std::cout << "Starting generating" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto graphNode_val = graph_factory.createGraph();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Done generating" << std::endl;
    std::cout << "Time to generate graph = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
    graphNode = &graphNode_val;

    // Figure out how many nodes we generated
    auto all_nodes = graphNode->getAllSubNodes();
    std::cout << "We generated this many nodes: " << all_nodes.size() << std::endl;

    // Draw the graph over the cv::Mat
    globalImage = drawNode(*graphNode, window_resolution);
    // Display the graph
    imshow("Graph", globalImage);
    cvSetMouseCallback("Graph", callBack, NULL);
    cv::waitKey(0);
    return 0;
}
