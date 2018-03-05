#ifndef THUNDERBOTS_NAVIGATOR_CIRCLE_H
#define THUNDERBOTS_NAVIGATOR_CIRCLE_H

#include "Area.h"

template <typename T>
class Circle : public Area<T> {
public:
    using Radius = double;

    // Delete the default constructor
    Circle() = delete;

    /**
     * Construct a Circle with given values
     * @param radius the radius of the circle
     * @param center the coordinates of the center of the circle
     */
    Circle(Radius radius, Coordinates center);

    // TODO: Def. need to test this!
    bool overlapsNode(Node<T>& node) override;

    Circle* clone() const { return new Circle(*this); };
private:
    Radius radius;
    Coordinates center;
};

#include "Circle.tpp"

#endif //THUNDERBOTS_NAVIGATOR_CIRCLE_H
