// Testing Includes
#include <gtest/gtest.h>

// Thunderbots Includes
#include "multi_resolution_graph/GraphNode.h"
#include "multi_resolution_graph/Rectangle.h"

namespace {
class RectangleTest : public testing::Test {
protected:
    virtual void SetUp() {
    }
};

// Test if a rectangle that is above a given node overlaps
TEST_F(RectangleTest, overlapsNode_not_overlapping_rectangle_above){
    GraphNode<nullptr_t> node(1,10);
    Rectangle<nullptr_t > rectangle(10, 10, (Coordinates){9,11});
    EXPECT_FALSE(rectangle.overlapsNode(node));
}

// Test if a rectangle that is below a given node overlaps
TEST_F(RectangleTest, overlapsNode_not_overlapping_rectangle_below){
    GraphNode<nullptr_t> node(1,10);
    Rectangle<nullptr_t> rectangle(10, 10, (Coordinates){9,-11});
    EXPECT_FALSE(rectangle.overlapsNode(node));
}

// Test if a rectangle that is to the right of a given node overlaps
TEST_F(RectangleTest, overlapsNode_not_overlapping_rectangle_right){
    GraphNode<nullptr_t> node(1,10);
    Rectangle<nullptr_t> rectangle(10, 10, (Coordinates){11,9});
    EXPECT_FALSE(rectangle.overlapsNode(node));
}

// Test if a rectangle that is to the left of a given node overlaps
TEST_F(RectangleTest, overlapsNode_not_overlapping_rectangle_left){
    GraphNode<nullptr_t> node(1,10);
    Rectangle<nullptr_t> rectangle(10, 10, (Coordinates){-11,9});
    EXPECT_FALSE(rectangle.overlapsNode(node));
}

// Test a rectangle that partially overlaps a node
TEST_F(RectangleTest, ovelapsNode_partially_overlaps){
    GraphNode<nullptr_t> node(1,10);
    Rectangle<nullptr_t> rectangle(10, 10, (Coordinates){9,9});
    EXPECT_TRUE(rectangle.overlapsNode(node));
}

// Test a rectangle that fully overlaps a node
TEST_F(RectangleTest, ovelapsNode_fully_overlaps){
    GraphNode<nullptr_t> node(1,10);
    Rectangle<nullptr_t> rectangle(3, 3, (Coordinates){2,2});
    EXPECT_TRUE(rectangle.overlapsNode(node));
}

}


