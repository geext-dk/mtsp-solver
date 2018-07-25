#include "util.h"

namespace mtsp {

static bool is_random_assigned = false;

std::default_random_engine& getRandomEngine() {
    static std::default_random_engine e;
    if (!is_random_assigned) {
        try {
            std::random_device r;
            e = std::default_random_engine(r());
        } catch (std::exception&) {
            std::cerr << "Error creating a random engine. Falling back to a "
                         "time-based seed" << std::endl;
            e = std::default_random_engine(static_cast<std::size_t>(
                        std::chrono::system_clock::now()
                            .time_since_epoch()
                            .count()));
        }
        is_random_assigned = true;
    }
    return e;
}

std::size_t firstFreeRandom(const std::vector<unsigned> &is_assigned,
                            std::size_t number_of_free) {
    std::size_t random_position = getRandomInteger<std::size_t>(0,
                                                                number_of_free);
    std::size_t k = 0;
    // dunno how to merge the zero case in the loop
    if (is_assigned.at(0) == 0) {
        if (random_position == 0) {
            return 0;
        }
    } else {
        ++random_position;
    }

    while (k != random_position) {
        ++k;
        if (is_assigned.at(k) == 1) {
            ++random_position;
        }
    }

    return random_position;
}

std::tuple<std::unordered_map<std::size_t, unsigned long>,
           std::unordered_map<std::size_t, std::size_t>>
dijkstra(const AdjacencyList &adjacency_list, std::size_t from) {

    // maps because the vertices numbers can be greater than their amount
    std::unordered_map<std::size_t, unsigned long> distance;
    std::unordered_map<std::size_t, std::size_t> previous;

    PriorityQueue pq;
    for (auto &pair : adjacency_list) {
        previous[pair.first] = 0;
        distance[pair.first] = std::numeric_limits<std::size_t>::max();
        pq.push(pair.first, std::numeric_limits<std::size_t>::max());
    }
    pq.decreasePriority(from, 0);
    distance[from] = 0;

    while (pq.size() != 0) {
        std::size_t u = pq.extractMin();
        for (const EndVertex &v : adjacency_list.getIncidentEdges(u)) {
            unsigned long alt = distance[u] + v.weight;
            if (alt < distance[v.vertex]) {
                distance[v.vertex] = alt;
                pq.decreasePriority(v.vertex, alt);
                previous[v.vertex] = u;
            }
        }
    }

    return std::make_tuple(std::move(distance), std::move(previous));
}

}
