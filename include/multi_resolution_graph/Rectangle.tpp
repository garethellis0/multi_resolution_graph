#pragma once

template <typename T>
Rectangle<T>::Rectangle(double width, double height, Coordinates bottom_left_point):
        width(width),
        height(height),
        bottom_left_coordinates(bottom_left_point)
{}

template <typename T>
bool Rectangle<T>::overlapsNode(Node<T>& node) {
    // TODO: Some comments here would probably be a good idea.....
    double nx_min = node.getCoordinates().x;
    double ny_min = node.getCoordinates().y;
    double nx_max = nx_min + node.getScale();
    double ny_max = ny_min + node.getScale();
    double rx_min = bottom_left_coordinates.x;
    double ry_min = bottom_left_coordinates.y;
    double rx_max = rx_min + width;
    double ry_max = ry_min + height;
    auto valueInRange = [&](double val, double min, double max){
        return (val >= min) && (val <= max);
    };
    bool x_overlap = valueInRange(nx_min, rx_min, rx_max) ||
                     valueInRange(rx_min, nx_min, nx_max);
    bool y_overlap = valueInRange(ny_min, ry_min, ry_max) ||
                     valueInRange(ry_min, ny_min, ny_max);
    return x_overlap && y_overlap;
}

