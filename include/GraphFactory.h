#ifndef THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
#define THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H

// C++ STD Includes
#include <cmath>
#include <functional>

// Boost Includes
#include <boost/optional.hpp>

// Thunderbots Includes
#include <GraphNode.h>
#include <RealNode.h>

template <typename T>
class GraphFactory {
public:

    using Radius = double;
    using Resolution = double;
    using Scale = double;

    // TODO: We may want to replace these with Thunderbots Geometry classes at some point
    // TODO: Make a note that an area has to be contiguous for stuff to work (ie it can't be a bunch of seperate circles)
    class Area {
    public:
        // TODO: Don't think we need this function anymore - remove
        /**
         * Checks if a given point is in this area
         * @param point the point we're checking if is in this area
         * @return true if the given point is in this area, false otherwise
         */
        virtual bool pointInArea(Coordinates point) = 0;

        // TODO: Don't think we need this function anymore - remove
        /**
         * Get the center point of this Area
         * @return the center point of this Area
         */
        virtual Coordinates getCenter() = 0;

        // TODO: Can we come up with a more generic impl. for this,
        // TODO: so that we can reduce the amount of work required to create a new Area subclass?
        /**
         * Checks if this area overlaps the given node
         * @param node the node we're checking if this are overlaps
         * @return
         *      `true` if this area *DOES* overlap the given node
         *      `false` if this are *DOES NOT* overlap the given node
         */
        virtual bool overlapsNode(Node<T>& node) = 0;

        /**
         * Clone this Area Object
         * @return a pointer to a clone of this Area object
         */
        virtual Area* clone() const = 0;
    };

    // TODO: Implementation should be in it's own `.cpp`
    class Circle : public Area {
    public:
        // Delete the default constructor
        Circle() = delete;
        Circle(Radius radius, Coordinates center):
                radius(radius),
                center(center)
        {}
        bool pointInArea(Coordinates point) override {
            double distance_to_center = std::sqrt(
                    std::pow(point.x - center.x, 2) + std::pow(point.x - center.x, 2)
            );
            return distance_to_center < radius;
        }
        Coordinates getCenter() override {
            return center;
        }
        // TODO: Comment met
        double sgn(double x){
            if (x < 0) { return -1; }
            else return 1;
        }
        // TODO: Def. need to test this!
        bool overlapsNode(Node<T>& node) override {
            // Get the 4 corner points of this node
            Coordinates p1 = node.getCoordinates();
            Coordinates p2 = {
                    p1.x + node.getScale(),
                    p1.y
            };
            Coordinates p3 = {
                    p1.x + node.getScale(),
                    p1.y + node.getScale()
            };
            Coordinates p4 = {
                    p1.x,
                    p1.y + node.getScale()
            };
            std::vector<Coordinates> points = {p1, p2, p3, p4};

            // This circle intersects this node if:
            // - the center of the circle lies within the node
            // - any of the corners of the node are contained in the circle
            // - the edge of the circle intersects the edge of this node

            // TODO: IS just using points here confusing? (Might want to have actual min/max x/y variables for clarity)
            // Check if center of circle is within the node
            if (getCenter().x > p1.x && getCenter().x < p4.x &&
                getCenter().y > p1.y && getCenter().y < p4.y) {
                return true;
            }

            // Check if any of the node corners are within the circle
            for (const Coordinates& point : points){
                double dx = center.x - point.x;
                double dy = center.y - point.y;
                double distance = std::sqrt(std::pow(dx,2) + std::pow(dy,2));
                if (distance < radius){
                    return true;
                }
            }

            // Check if the edge of this circle intersects any of the edges
            // of the node
            // Math from here:
            // http://mathworld.wolfram.com/Circle-LineIntersection.html
            // Construct a line between every possible pairing of points
            // (ie. all the edges of the node)
            for (int i = 0; i < points.size(); i++) {
                for (int j = 0; j < points.size(); j++) {
                    // Check to make sure we're not making a line from
                    // two of the same point
                    if (i != j){
                        double x1 = points[i].x - center.x;
                        double y1 = points[i].y - center.y;
                        double x2 = points[j].x - center.x;
                        double y2 = points[j].y - center.y;

                        double dx = x2 - x1;
                        double dy = y2 - y1;
                        double dr = std::sqrt(std::pow(dx,2) + std::pow(dy,2));
                        double D = x1*y2 - x2*y1;
                        double intersection_discriminant =
                                std::pow(radius,2)*std::pow(dr,2) - std::pow(D,2);
                        // Get at least one of the intersection points
                        Coordinates intersection;
                        intersection.x = D*dy + sgn(dy)*dx * std::sqrt(intersection_discriminant)
                                                / std::pow(dr,2);
                        intersection.y = -D*dx + std::abs(dy)*sqrt(intersection_discriminant)
                                                 / std::pow(dr,2);

                        // Make sure the intersection is on the line segment
                        // that actually makes up the node edge
                        if (intersection.x > std::min(x1,x2) && intersection.x < std::max(x1,x2) &&
                             intersection.y > std::min(y1,y2) && intersection.y < std::max(y1,y2)){
                            return true;
                        }
                    }
                }
            }

            // If none of the above checks passed, then this circle
            // does not overlap the given node
            return false;
        }
        Circle* clone() const { return new Circle(*this); };
    private:
        Radius radius;
        Coordinates center;
    };

    // TODO: Implementation should be in it's own `.cpp`
    class Rectangle : public Area {
    public:
        // Delete the default constructor
        Rectangle() = delete;
        Rectangle(double width, double height, Coordinates bottom_left_point):
                width(width),
                height(height),
                bottom_left_coordinates(bottom_left_point)
        {}
        Coordinates getCenter() override {
            return (Coordinates){
                    bottom_left_coordinates.x + width/2,
                    bottom_left_coordinates.y + height/2
            };
        }
        bool pointInArea(Coordinates point) override {
            double min_x = bottom_left_coordinates.x;
            double max_x = bottom_left_coordinates.x + width;
            double min_y = bottom_left_coordinates.y;
            double max_y = bottom_left_coordinates.y + height;
            return point.x > min_x && point.x < max_x
                   && point.y > min_y && point.y < max_y;
        }
        // TODO: Test me!
        bool overlapsNode(Node<T>& node) override {
            double nx_min = node.getCoordinates().x;
            double ny_min = node.getCoordinates().y;
            double nx_max = nx_min + node.getScale();
            double ny_max = ny_min + node.getScale();
            double rx_min = bottom_left_coordinates.x;
            double ry_min = bottom_left_coordinates.y;
            double rx_max = rx_min + width;
            double ry_max = ry_min + height;
            bool x_overlap = valueInRange(nx_min, rx_min, rx_max) ||
                             valueInRange(rx_min, nx_min, nx_max);
            bool y_overlap = valueInRange(ny_min, ry_min, ry_max) ||
                             valueInRange(ry_min, ny_min, ny_max);
            return x_overlap && y_overlap;
        }
        Rectangle* clone() const { return new Rectangle(*this); };
    private:
        double width, height;
        Coordinates bottom_left_coordinates;
        // TODO: Function comment
        bool valueInRange(double value, double min, double max)
        { return (value >= min) && (value <= max); }
    };


    /**
     * Creates a GraphFactory with default values
     */
    GraphFactory();

    /**
     * Sets the max scale for the generated graph at a given point
     * @param coordinates the point at which to set the given resolution
     * @param max_scale the max scale this point must be
     */
    void setMaxScaleAtPoint(Coordinates coordinates, double max_scale);

    /**
     * Sets the max scale for the generated graph for a given area
     * @param area the area in which to set the given resolution
     * @param max_scale the max scale for the area
     */
    void setMaxScaleInArea(Area &area, double max_scale);

    /**
     * Sets the overall length/width of the graph
     * (ie. this will set the graph to be of `size x size`)
     * @param size the length and width of the graph
     */
    void setGraphScale(double size);

    /**
     * Sets the top level resolution for the graph
     * (width/height in # of nodes)
     * @param resolution the desired resolution for the top level of the graph
     */
    void setGraphTopLevelResolution(unsigned int resolution);

    /**
     * Creates a graph with the currently set parameters
     * @return a new graph with the currently set parameters
     */
    GraphNode<T> createGraph();

private:

    /**
     * Sets all nodes within the given area on the given graph to the given resolution
     * @param graph_node the graph in which we're setting the min. resolution of an area
     * @param area the area over which the max scale is to be set
     * @param max_scale the maximum scale every node in the given area must be
     */
    void setMinGraphResolutionForArea(GraphNode<T> &graph_node,
                                      Area &area,
                                      Scale max_scale);

    /**
     * Sets the max scale for the node closest to the given point
     * @param graph_node the graph in which we're setting the min. resolution of a point
     * @param coordinates the point we want to set the scale of
     * @param max_scale the maximum scale the node closest to the given point must be
     */
    void setMinGraphResolutionForPoint(GraphNode<T> &graph_node,
                                       Coordinates coordinates,
                                       Scale max_scale);

    // TODO: Better name for this variable?
    // A list of areas with a given max scale
    std::vector<std::pair<Area*, Scale>> min_resolution_areas;

    // TODO: Better name for this variable?
    // A list of points with a given max scale
    std::vector<std::pair<Coordinates, Scale>> min_resolution_points;

    // The length/width of the top level graph
    // (in terms of some unit of distance)
    double top_level_graph_scale;

    // The length/width of the top level of the graph
    // (in terms of # of nodes)
    unsigned int top_level_graph_resolution;

    // TODO: Better comment?
    // TODO: Setter function for this
    // The factor by which we subdivide nodes to build up the tree structure
    unsigned int subnode_resolution;
};

#include <GraphFactory.tpp>

#endif //THUNDERBOTS_NAVIGATOR_GRAPHFACTORY_H
