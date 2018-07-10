#include "edge.h"
namespace mtsp {
    Edge::Edge(int d, int c) {
      _dest = d;
      _cost = c;
    }

    int Edge::getDest() const {
      return _dest;
    }

    int Edge::getCost() const {
      return _cost;
    }

    void Edge::setDest(int d) {
      _dest = d;
    }

    void Edge::setCost(int c) {
      _cost = c;
    }
}
