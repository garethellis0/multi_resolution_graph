#include "Node.h"

namespace multi_resolution_graph {

    template<typename T>
    std::vector<std::shared_ptr<RealNode<T>>>
    Node<T>::getAllNodesInArea(Area<T>& area) {
        auto filter = [&](Node<T>& n) {
            return area.overlapsNode(n);
        };
        return this->getAllNodesThatPassFilter(filter, true, false);
    }

}
