#include "common.h"

std::default_random_engine& mtsp::getRandomEngine() {
    static std::random_device r;
    static std::default_random_engine e;
    try {
        e = std::default_random_engine(r());
    } catch(std::exception& ex) {
        std::cerr << "Error creating a random engine. Falling back to a time-based seed" << std::endl;
        e = std::default_random_engine(
                static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch().count()));
    }
    return e;
}

std::size_t mtsp::firstFreeRandom(const std::vector<unsigned> &assigned, std::size_t number_of_free)
{
    // get random number (the distribution generates a random number that can be equal to the right boundary)
    std::uniform_int_distribution<std::size_t> uniform_dist(0, number_of_free - 1);
    std::size_t random_position = uniform_dist(getRandomEngine());

    int k = -1;
    while (k != random_position) {
        ++k;
        if (assigned[k]) {
            ++random_position;
        }
    }

    return random_position;
}


std::tuple<std::unordered_map<std::size_t, std::size_t>, std::unordered_map<std::size_t, std::size_t>>
mtsp::dijkstra(const AdjacencyList &adjacency_list, std::size_t from)
{

    // maps are used because the vertices numbers can be greater than their amount
    std::unordered_map<std::size_t, std::size_t> distance;
    std::unordered_map<std::size_t, std::size_t> previous;

    auto cmp = [](const Destination &first, const Destination &second) {
        return first.distance < second.distance;
    };
    // priority queue
    std::priority_queue<Destination, std::vector<Destination>, decltype(cmp)> queue(cmp);

    queue.push(Destination(from, 0));
    distance[from] = 0;
    previous[from] = 0;  // TODO: define the value to distinguish a source from other vertices
                         // (A source cannot have a previous vertex)
    while (!queue.empty()) {
        Destination u = queue.top();
        queue.pop();
        if (distance.find(u.destination) != distance.end() && u.distance > distance[u.destination]) {
            continue;
        }

        for (const EndVertex &v : adjacency_list.getEdges(u.destination)) {
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