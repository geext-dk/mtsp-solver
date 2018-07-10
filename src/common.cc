#include "common.h"

unsigned long mtsp::firstFreeRandom(std::vector<int> &assigned, unsigned long number_of_free)
{
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_int_distribution<unsigned long> uniform_dist(0, number_of_free);
    // get random number
    unsigned long random_position = uniform_dist(random_engine);

    int k = -1;
    while (k != random_position) {
        ++k;
        if (assigned[k]) {
            ++random_position;
        }
    }

    return random_position;
}


std::tuple<std::unordered_map<int, int>, std::unordered_map<int, int>>
mtsp::dijkstra(std::unordered_map<int, std::vector<Edge>> &adjacancy_list, int from)
{
    // maps are used because vertices numbers can be greater than their amount
    std::unordered_map<int, int> distance;
    std::unordered_map<int, int> previous;

    auto cmp = [](Edge &left, Edge &right) {
        return left.getCost() > right.getCost();
    };
    // priority queue
    std::priority_queue<Edge, std::vector<Edge>, decltype(cmp)> queue(cmp);

    queue.push(Edge(from, 0));
    distance[from] = 0;
    previous[from] = -1;
    while (!queue.empty()) {
        Edge u = queue.top();
        queue.pop();
        if (distance.find(u.getDest()) != distance.end() && u.getCost() > distance[u.getDest()]) {
            continue;
        }

        for (Edge v : adjacancy_list[u.getDest()]) {
            Edge next = Edge(v.getDest(), u.getCost() + v.getCost());

            if (distance.find(next.getDest()) == distance.end() || next.getCost() < distance[next.getDest()]) {
                queue.push(next);
                distance[next.getDest()] = next.getCost();
                previous[next.getDest()] = u.getDest();
            }
        }
    }
    return std::tie(distance, previous);
}