// Testing Includes
#include <gtest/gtest.h>

// Project Includes
#include "multi_resolution_graph/GraphNode.h"
#include "multi_resolution_graph/Polygon.h"

using namespace multi_resolution_graph;

namespace {

class PolygonTest : public testing::Test { protected:
    virtual void SetUp() {
    }
};

// Test checking the overlap of a partially overlapping polygon and node
// where the corner of the node lies within the polygon
TEST_F(PolygonTest, overlapsNode_partially_overlapping_node_corner_in_polygon){
    GraphNode<nullptr_t> node(1,1);
    Polygon<nullptr_t> polygon({
       (Coordinates){0.9, 0.9},
       (Coordinates){0.9, 5},
       (Coordinates){5, 0.9}
    });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking the overlap of a partially overlapping polygon and node
// where the polygon is shaped like an L
TEST_F(PolygonTest, overlapsNode_partially_overlapping_L_shaped_polygon){
    GraphNode<nullptr_t> node(1,1);
    Polygon<nullptr_t> polygon({
       (Coordinates){0.3, 0.8},
       (Coordinates){0.4, 0.8},
       (Coordinates){0.4, 1.5},
       (Coordinates){2, 1.5},
       (Coordinates){2, 1.7},
       (Coordinates){0.3, 1.7},
    });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking the overlap of a polygon shaped like a triangle,
// with the tip of the triangle overlapping the right-hand side
// of the node
TEST_F(PolygonTest, overlapsNode_partially_overlapping_triangle_shaped_polygon){
    GraphNode<nullptr_t> node(1,1);
    Polygon<nullptr_t> polygon({
                                       (Coordinates){0.5, 0.5},
                                       (Coordinates){2, 0.1},
                                       (Coordinates){2, 0.7},
                               });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking the overlap of a polygon shaped like a square, where the entire
// polygon is within the node
TEST_F(PolygonTest, overlapsNode_square_polygon_fully_within_node){
    GraphNode<nullptr_t> node(1,1);
    Polygon<nullptr_t> polygon({
                                       (Coordinates){0.2, 0.2},
                                       (Coordinates){0.7, 0.2},
                                       (Coordinates){0.7, 0.7},
                                       (Coordinates){0.2, 0.7},
                               });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking the overlap of a polygon with a "random" shape, where the
// entire polygon is within the node
TEST_F(PolygonTest, overlapsNode_funky_polygon_fully_within_node){
    GraphNode<nullptr_t> node(1,1);
    Polygon<nullptr_t> polygon({
                                       (Coordinates){0.2, 0.2},
                                       (Coordinates){0.5, 0.1},
                                       (Coordinates){0.9, 0.5},
                                       (Coordinates){0.7, 0.9},
                                       (Coordinates){0.4, 0.4},
                                       (Coordinates){0.2, 0.7},
                               });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking the overlap of a polygon shaped like a triangle,
// with the tip of the triangle overlapping the right-hand side
// of the node
// (this differs from the similar test above because we change the node size
// to a value other then 1)
TEST_F(PolygonTest, overlapsNode_partially_overlapping_triangle_shaped_polygon_different_size_node){
    GraphNode<nullptr_t> node(1,7);
    Polygon<nullptr_t> polygon({
                                       (Coordinates){6.5, 5},
                                       (Coordinates){7.5, 6},
                                       (Coordinates){7.5, 4},
                               });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking the overlap of a "twisted" polygon , where the entire
// polygon is inside the node
TEST_F(PolygonTest, overlapsNode_twisted_polygon_overlapping_node){
    GraphNode<nullptr_t> node(1,9);
    Polygon<nullptr_t > polygon = Polygon<nullptr_t >({
        {7,3},
        {3,7},
        {4.7, 7.8},
        {1,1}
    });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking the overlap of a polygon with a "random" shape, where the
// entire polygon is outside the node
TEST_F(PolygonTest, overlapsNode_funky_polygon_fully_outside_node){
    GraphNode<nullptr_t> node(1,0.05);
    Polygon<nullptr_t> polygon({
                                       (Coordinates){0.2, 0.2},
                                       (Coordinates){0.5, 0.1},
                                       (Coordinates){0.9, 0.5},
                                       (Coordinates){0.7, 0.9},
                                       (Coordinates){0.4, 0.4},
                                       (Coordinates){0.2, 0.7},
                               });
    EXPECT_FALSE(polygon.overlapsNode(node));
}

// Test checking the overlap of a polygon shaped like a square, where the entire
// polygon is outside the node
TEST_F(PolygonTest, overlapsNode_square_polygon_fully_outside_node){
    GraphNode<nullptr_t> node(1,0.1);
    Polygon<nullptr_t> polygon({
                                       (Coordinates){0.2, 0.2},
                                       (Coordinates){0.7, 0.2},
                                       (Coordinates){0.7, 0.7},
                                       (Coordinates){0.2, 0.7},
                               });
    EXPECT_FALSE(polygon.overlapsNode(node));
}

// Test checking the overlap of a polygon shaped like a square, where the entire
// node is inside the polygon
TEST_F(PolygonTest, overlapsNode_node_fully_inside_square_polygon){
    GraphNode<nullptr_t> node(1,1);
    Polygon<nullptr_t> polygon({
                                       (Coordinates){-0.1, -0.1},
                                       (Coordinates){1.1, -0.1},
                                       (Coordinates){1.1, 1.1},
                                       (Coordinates){-0.1, 1.1},
                               });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking the overlap of a polygon with a "random" shape, where the
// entire node is inside the polygon
TEST_F(PolygonTest, overlapsNode_node_fully_inside_funky_polygon){
    GraphNode<nullptr_t> node(1,1);
    Polygon<nullptr_t> polygon({
                                       (Coordinates){-10, -10},
                                       (Coordinates){0.5, -2},
                                       (Coordinates){2, -9},
                                       (Coordinates){10, 10},
                                       (Coordinates){-20, 10},
                               });
    EXPECT_TRUE(polygon.overlapsNode(node));
}

// Test checking for when a triangular polygon completely encompasses a node
// (This was created as the result of a specific bug)
TEST_F(PolygonTest, overlaps_node_fully_inside_triangular_polygon) {
        GraphNode<nullptr_t> node(1,1);
        Polygon<nullptr_t> polygon({
                                           (Coordinates){-1, 1},
                                           (Coordinates){-1, -5},
                                           (Coordinates){10, 1},
                                   });
        EXPECT_TRUE(polygon.overlapsNode(node));
}

}

