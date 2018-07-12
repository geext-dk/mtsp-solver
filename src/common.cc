#include "common.h"

std::default_random_engine& mtsp::getRandomEngine() {
    static std::random_device r;
    static std::default_random_engine e;
    try {
        e = std::default_random_engine(r());
    } catch(std::exception& ex) {
        std::cerr << "Error creating a random engine. Falling back to a time-based seed" << std::endl;
        e = std::default_random_engine(
                static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    }
    return e;
}

unsigned long mtsp::firstFreeRandom(const std::vector<int> &assigned, unsigned long number_of_free)
{
    // get random number (the distribution generates a random number that can be equal to the right boundary)
    std::uniform_int_distribution<unsigned long> uniform_dist(0, number_of_free - 1);
    unsigned long random_position = uniform_dist(getRandomEngine());

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
mtsp::dijkstra(const std::unordered_map<int, std::vector<EndVertex>> &adjacency_list, int from)
{

    // maps are used because the vertices numbers can be greater than their amount
    std::unordered_map<int, int> distance;
    std::unordered_map<int, int> previous;

    auto cmp = [](const Destination &first, const Destination &second) {
        return first.distance < second.distance;
    };
    // priority queue
    std::priority_queue<Destination, std::vector<Destination>, decltype(cmp)> queue(cmp);

    queue.push(Destination(from, 0));
    distance[from] = 0;
    previous[from] = -1;
    while (!queue.empty()) {
        Destination u = queue.top();
        queue.pop();
        if (distance.find(u.destination) != distance.end() && u.distance > distance[u.destination]) {
            continue;
        }

        for (const EndVertex &v : adjacency_list.at(u.destination)) {
            Destination next(v.vertex, u.distance + v.weight);

            if (distance.find(next.destination) == distance.end() || next.distance < distance[next.destination]) {
                queue.push(next);
                distance[next.destination] = next.distance;
                previous[next.destination] = u.destination;
            }
        }
    }
    return std::make_tuple(std::move(distance), std::move(previous));
}