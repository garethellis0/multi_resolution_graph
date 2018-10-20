#pragma once

#include "Area.h"

// TODO: Implementation should be in it's own `.cpp`
template<typename T>
class Rectangle : public Area<T> {
public:
    // Delete the default constructor
    Rectangle() = delete;

    /**
     * Creates a Rectangle with given values
     * @param width the width of the Rectangle
     * @param height the height of the Rectangle
     * @param bottom_left_point the Coordinates of the bottom left point
     * of the rectangle
     */
    Rectangle(double width, double height, Coordinates bottom_left_point);

    bool overlapsNode(Node<T> &node) override;

    std::shared_ptr<Area<T>> clone() const {
        return std::make_shared<Rectangle<T>>(*this);
    };

private:
    double width, height;
    Coordinates bottom_left_coordinates;
};

#include "Rectangle.tpp"
