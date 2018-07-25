#ifndef MTSP_PRIORITY_QUEUE_H
#define MTSP_PRIORITY_QUEUE_H
#include <vector>
#include <utility>

namespace mtsp {

class PriorityQueue {
public:
    void push(std::size_t value, unsigned long priority);
    std::size_t extractMin();
    void decreasePriority(std::size_t value, unsigned long new_priority);
    std::size_t size() const;

private:
    std::vector<std::pair<std::size_t, unsigned long>> _data;

    void restoreDown(std::size_t parent);
    void restoreUp(std::size_t index);;
};

}

#endif  // MTSP_PRIORITY_QUEUE_H
