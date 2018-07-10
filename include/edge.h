#ifndef MTSPSOLVER_EDGE_H
#define MTSPSOLVER_EDGE_H
namespace mtsp {

    class Edge {
    public:
        Edge(int d, int c);

        int getDest() const;
        int getCost() const;
        void setDest(int d);
        void setCost(int c);

    private:
        int _dest;
        int _cost;
    };
}

#endif  // MTSPSOLVER_EDGE_H