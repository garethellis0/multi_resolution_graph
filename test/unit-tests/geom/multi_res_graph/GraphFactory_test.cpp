// Testing Includes
#include <gtest/gtest.h>

// C++ STD Includes
#include <memory>
#include <vector>
#include <chrono>

// Thunderbots Includes
#include "geom/multi_res_graph/GraphFactory.h"
#include "geom/multi_res_graph/Rectangle.h"
#include "geom/multi_res_graph/Circle.h"

namespace {

class GraphFactoryTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

// Test that we get the expected graph from the default constructor
TEST_F(GraphFactoryTest, default_constructor){
    GraphFactory<nullptr_t> graph_factory;
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 1);
    EXPECT_EQ(generated_graph.getResolution(), 1);
}

// Test setting the top level scale of the graph
TEST_F(GraphFactoryTest, setGraphScale){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(10);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 10);
    EXPECT_EQ(generated_graph.getResolution(), 1);
}

// Test setting the top level resolution of the graph
TEST_F(GraphFactoryTest, setGraphResolution){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphTopLevelResolution(10);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 1);
    EXPECT_EQ(generated_graph.getResolution(), 10);
}

// Test that setting the size and resolution at the same time works
TEST_F(GraphFactoryTest, setGraphScale_and_setGraphResolution){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(20);
    graph_factory.setGraphTopLevelResolution(10);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    EXPECT_EQ(generated_graph.getScale(), 20);
    EXPECT_EQ(generated_graph.getResolution(), 10);
}

TEST_F(GraphFactoryTest, setMaxScaleAtPoint_0_0){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphTopLevelResolution(2);
    graph_factory.setMaxScaleAtPoint((Coordinates) {0, 0}, 0.25);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get the top-level subnodes
    std::vector<std::vector<Node<nullptr_t>*>> top_level_sub_nodes = generated_graph.getSubNodes();

    // We should have expanded the bottom left node, so it should now be a GraphNode
    Node<nullptr_t>* expanded_node = top_level_sub_nodes[0][0];
    EXPECT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node);

    EXPECT_EQ(new_graph_node->getResolution(), 2);
    EXPECT_EQ(new_graph_node->getScale(), 0.5);
}

TEST_F(GraphFactoryTest, setMaxScaleAtPoint_1_1){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphTopLevelResolution(2);
    graph_factory.setMaxScaleAtPoint((Coordinates) {1, 1}, 0.25);
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get the top-level subnodes
    std::vector<std::vector<Node<nullptr_t>*>> top_level_sub_nodes = generated_graph.getSubNodes();

    // We should have expanded the bottom left node, so it should now be a GraphNode
    Node<nullptr_t>* expanded_node = top_level_sub_nodes[1][1];
    EXPECT_EQ(typeid(GraphNode<nullptr_t>), typeid(*expanded_node));

    // Cast the expanded node to a GraphNode so we can do some more checks
    auto new_graph_node = dynamic_cast<GraphNode<nullptr_t>*>(expanded_node);

    EXPECT_EQ(new_graph_node->getResolution(), 2);
    EXPECT_EQ(new_graph_node->getScale(), 0.5);
}

// Test setting the scale for a rectangular area which is entirely within a single node
// (that is, the node does not overlay more then a single node)
TEST_F(GraphFactoryTest, setMaxScaleInArea_Rectangle_area_within_single_node){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(1);
    graph_factory.setGraphTopLevelResolution(1);

    // Set the scale for a single rectangular area that is entirely within
    // the single top level node
    Rectangle<nullptr_t> rectangle(0.1,0.1,(Coordinates){0.25,0.25});
    graph_factory.setMaxScaleInArea(rectangle, 0.51);

    // Generate the graph
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get all the RealNodes
    std::function<bool(Node<nullptr_t>&)> always_true_filter = [&](Node<nullptr_t> &n) {
        return true;
    };
    std::vector<RealNode<nullptr_t>*> all_real_nodes =
            generated_graph.getAllNodesThatPassFilter(always_true_filter);

    // The area we choose should have forced the top level node to split into
    // 4 sub-nodes
    EXPECT_EQ(4, all_real_nodes.size());
    std::vector<Coordinates> expected_coordinates = {
            (Coordinates){0,0},
            (Coordinates){0.5,0},
            (Coordinates){0,0.5},
            (Coordinates){0.5,0.5},
    };
    std::vector<Coordinates> actual_coordinates;
    for (auto& node : all_real_nodes){
        actual_coordinates.emplace_back(node->getCoordinates());
    }
    // TODO: Add in GMock for unordered vector comparison
    // NOTE: List equality checks are *order-sensitive*. If it fails, check
    // that it's just not because the order has changed (which is fine)
    EXPECT_EQ(expected_coordinates, actual_coordinates);
}

// Test setting the scale for a circular area which is entirely within a single node
// (that is, the node does not overlay more then a single node)
TEST_F(GraphFactoryTest, setMaxScaleInArea_Circle_area_within_single_node){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(1);
    graph_factory.setGraphTopLevelResolution(1);

    // Set the scale for a single circular area that is entirely within
    // the single top level node
    Circle<nullptr_t> circle(0.1, (Coordinates){0.1,0.1});
    graph_factory.setMaxScaleInArea(circle, 0.51);

    // Generate the graph
    GraphNode<nullptr_t> generated_graph = graph_factory.createGraph();

    // Get all the RealNodes
    std::function<bool(Node<nullptr_t>&)> always_true_filter = [&](Node<nullptr_t> &n) {
        return true;
    };
    std::vector<RealNode<nullptr_t>*> all_real_nodes =
            generated_graph.getAllNodesThatPassFilter(always_true_filter);

    // The area we choose should have forced the top level node to split into
    // 4 sub-nodes
    EXPECT_EQ(4, all_real_nodes.size());
    std::vector<Coordinates> expected_coordinates = {
            (Coordinates){0,0},
            (Coordinates){0.5,0},
            (Coordinates){0,0.5},
            (Coordinates){0.5,0.5},
    };
    std::vector<Coordinates> actual_coordinates;
    for (auto& node : all_real_nodes){
        actual_coordinates.emplace_back(node->getCoordinates());
    }
    // TODO: Add in GMock for unordered vector comparison
    // NOTE: List equality checks are *order-sensitive*. If it fails, check
    // that it's just not because the order has changed (which is fine)
    EXPECT_EQ(expected_coordinates, actual_coordinates);
}

// Test setting the area of several Rectangles, some overlapping
TEST_F(GraphFactoryTest, setMaxScaleInArea_several_rectangles){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(10);
    graph_factory.setGraphTopLevelResolution(1);

    // Choose some circular areas with associated resolutions
    std::vector<std::pair<Rectangle<nullptr_t>, double>> areas_and_scales = {
            // Rectangle entirely within Graph
            {
                    {3, 4, (Coordinates){2,3}},
                    0.3
            },
            // Rectangle of higher resolution entirely within Graph
            // and slightly overlapping the first
            {
                    {4, 4, (Coordinates){1,1}},
                    0.1
            },
            // Rectangle that extends outside the Graph
            {
                    {10, 20, (Coordinates){8,8}},
                    0.2
            },
            // Rectangle of higher resolution then the initial graph
            {
                    {3, 3, (Coordinates){5,5}},
                    1.4
            },
    };

    // Set the resolution of all the areas
    for (const auto& area_scale_pair : areas_and_scales){
        Rectangle<nullptr_t> area = area_scale_pair.first;
        double scale = area_scale_pair.second;
        graph_factory.setMaxScaleInArea(area, scale);
    }

    // Generate the Graph and get all the RealNodes
    GraphNode<nullptr_t> graph = graph_factory.createGraph();
    std::vector<RealNode<nullptr_t>*> nodes = graph.getAllSubNodes();

    for (RealNode<nullptr_t>* node : nodes){
        // Check if this node is within any of the areas we set
        for (const auto& area_scale_pair : areas_and_scales){
            Rectangle<nullptr_t> area = area_scale_pair.first;
            double scale = area_scale_pair.second;

            if (area.overlapsNode(*node)){
                // If this node is within one of the areas we set the resolution for,
                // check that the scale is equal to or less then the requested value
                EXPECT_LE(node->getScale(), scale);
            }
        }
    }

}

// Test setting the area of several circles, some overlapping
TEST_F(GraphFactoryTest, setMaxScaleInArea_several_circles){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(10);
    graph_factory.setGraphTopLevelResolution(1);

    // Choose some circular areas with associated resolutions
    std::vector<std::pair<Circle<nullptr_t>, double>> areas_and_scales = {
            // Circle entirely within Graph
            {
                    {3, (Coordinates){2,3}},
                    0.3
            },
            // Circle of higher resolution entirely within Graph
            // and slightly overlapping the first
            {
                    {2, (Coordinates){3,2}},
                    0.1
            },
            // Circle that extends outside the Graph
            {
                    {10, (Coordinates){8,8}},
                    0.2
            },
            // Circle of higher resolution then the initial graph
            {
                    {3, (Coordinates){5,5}},
                    1.4
            },
    };

    // Set the resolution of all the areas
    for (const auto& area_scale_pair : areas_and_scales){
        Circle<nullptr_t> area = area_scale_pair.first;
        double scale = area_scale_pair.second;
        graph_factory.setMaxScaleInArea(area, scale);
    }

    // Generate the Graph and get all the RealNodes
    GraphNode<nullptr_t> graph = graph_factory.createGraph();
    std::vector<RealNode<nullptr_t>*> nodes = graph.getAllSubNodes();

    for (RealNode<nullptr_t>* node : nodes){
        // Check if this node is within any of the areas we set
        for (const auto& area_scale_pair : areas_and_scales){
            Circle<nullptr_t> area = area_scale_pair.first;
            double scale = area_scale_pair.second;

            if (area.overlapsNode(*node)){
                // If this node is within one of the areas we set the resolution for,
                // check that the scale is equal to or less then the requested value
                EXPECT_LE(node->getScale(), scale);
            }
        }
    }

}

// Test setting the area of several circles and rectangles, some overlapping
TEST_F(GraphFactoryTest, setMaxScaleInArea_several_circles_and_rectangles){
    GraphFactory<nullptr_t> graph_factory;
    graph_factory.setGraphScale(10);
    graph_factory.setGraphTopLevelResolution(1);

    // Create some Areas with associated resolutions
    // (Note that pointers have to be used here because we can't use
    // the abstract "Area" class as a type in a double, because the compiler
    // won't know how to allocate it)
    std::vector<std::pair<Area<nullptr_t>*, double>> areas_and_scales {
            // Create a simple rectangle totally enclosed in the graph
            {
                    new Rectangle<nullptr_t>(2,2, {1,1}),
                    0.3
            },
            // Create a Circle of higher resolution partially overlapping the Rectangle
            {
                    new Circle<nullptr_t>(1,{1,1}),
                    0.1
            },
            // Create a Circle extending off the side of the graph
            {
                    new Circle<nullptr_t>(5,{9,5}),
                    0.1
            },
            // Create a Rectangle overlapping the circle we just created
            // and extending off the side of the graph
            {
                    new Rectangle<nullptr_t>(5,2, {8,5}),
                    0.5
            },
    };

    // Set the resolution of all the areas
    for (const auto& area_scale_pair : areas_and_scales){
        Area<nullptr_t>* area = area_scale_pair.first;
        double scale = area_scale_pair.second;
        graph_factory.setMaxScaleInArea(*area, scale);
    }

    // Generate the Graph and get all the RealNodes
    GraphNode<nullptr_t> graph = graph_factory.createGraph();
    std::vector<RealNode<nullptr_t>*> nodes = graph.getAllSubNodes();

    for (RealNode<nullptr_t>* node : nodes){
        // Check if this node is within any of the areas we set
        for (const auto& area_scale_pair : areas_and_scales){
            Area<nullptr_t>* area = area_scale_pair.first;
            double scale = area_scale_pair.second;

            if (area->overlapsNode(*node)){
                // If this node is within one of the areas we set the resolution for,
                // check that the scale is equal to or less then the requested value
                EXPECT_LE(node->getScale(), scale);
            }
        }
    }

}

// Test a case similar to a typical field generation for autonomous robot soccer (UBC Thunderbots)
TEST_F(GraphFactoryTest, ubc_thunderbots_field_generation){
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

    // TODO: Should not be printing out in tests
    // Generate the Graph
    std::cout << "Starting generating" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto graphNode_val = graph_factory.createGraph();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Done generating" << std::endl;
    std::cout << "Time to generate graph = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
    auto graphNode = &graphNode_val;

    // Figure out how many nodes we generated
    auto all_nodes = graphNode->getAllSubNodes();
    std::cout << "We generated this many nodes: " << all_nodes.size() << std::endl;
}

// TODO: Scale back this test a bit so it runs in computationally feasible time
// Test setting many Rectangles and Circles of very high resolution
// over a very large graph
//TEST_F(GraphFactoryTest, setMaxScaleInArea_many_rectangles_and_circles) {
//    GraphFactory<nullptr_t> graph_factory;
//    graph_factory.setGraphScale(100);
//    graph_factory.setGraphTopLevelResolution(2);
//
//    // Create some Areas with associated resolutions
//    // (Note that pointers have to be used here because we can't use
//    // the abstract "Area" class as a type in a double, because the compiler
//    // won't know how to allocate it)
//    std::vector<std::pair<Area<nullptr_t>*, double>> areas_and_scales;
//
//    // We're basically just randomly generating lots of different
//    // Rectangles and Circles
//    for (double i = 1; i < 10; i++){
//        for (double j = 1; j < 10; j++){
//            areas_and_scales.emplace_back(
//                    std::make_pair(
//                            new Rectangle<nullptr_t>(i*3, j*2, {i*2,i*3}),
//                            10/i
//                    ));
//            areas_and_scales.emplace_back(
//                    std::make_pair(
//                            new Circle<nullptr_t>(i*2, {i*2, 100 - i*3}),
//                            10/j
//                    ));
//            areas_and_scales.emplace_back(
//                    std::make_pair(
//                            new Rectangle<nullptr_t>(i*2, j*3, {i*2,i*3}),
//                            10/j
//                    ));
//            areas_and_scales.emplace_back(
//                    std::make_pair(
//                            new Circle<nullptr_t>(i*2, {100 - i*2, 100 - i*3}),
//                            1/j
//                    ));
//        }
//    }
//
//
//    // Set the resolution of all the areas
//    for (const auto& area_scale_pair : areas_and_scales){
//        Area<nullptr_t>* area = area_scale_pair.first;
//        double scale = area_scale_pair.second;
//        graph_factory.setMaxScaleInArea(*area, scale);
//    }
//
//    // Generate the Graph and get all the RealNodes
//    GraphNode<nullptr_t> graph = graph_factory.createGraph();
//    std::vector<RealNode<nullptr_t>*> nodes = graph.getAllSubNodes();
//
//    for (RealNode<nullptr_t>* node : nodes){
//        // Check if this node is within any of the areas we set
//        for (const auto& area_scale_pair : areas_and_scales){
//            Area<nullptr_t>* area = area_scale_pair.first;
//            double scale = area_scale_pair.second;
//
//            if (area->overlapsNode(*node)){
//                // If this node is within one of the areas we set the resolution for,
//                // check that the scale is equal to or less then the requested value
//                EXPECT_LE(node->getScale(), scale);
//            }
//        }
//    }
//}

// TODO: Some performance tests - pretty performance sensitive component (but performance is dependent on the docker container CI is running in)

}
