#include "priority_queue.h"
#include <stdexcept>

namespace mtsp {

void PriorityQueue::push(std::size_t value, unsigned long priority) {
    std::size_t index = _data.size();
    _data.push_back(std::make_pair(value, priority));

    // restore the heap after the insertion
    restoreUp(index);
}

std::size_t PriorityQueue::extractMin() {
    std::size_t ret = _data.at(0).first;
    _data[0] = _data.back();
    _data.pop_back();
    
    // restore the heap after the extraction
    restoreDown(0);

    return ret;
}

void PriorityQueue::decreasePriority(std::size_t value,
                                     unsigned long new_priority) {
    for (std::size_t i = 0; i < _data.size(); ++i) {
        if (_data[i].first == value) {
            if (new_priority > _data[i].second) {
                throw std::invalid_argument("A new priority must be lower"
                                            " than the current priority");
            }
            _data[i].second = new_priority;
            restoreUp(i);
            break;
        }
    }
}

void PriorityQueue::restoreDown(std::size_t parent) {
    std::size_t minimum_index = parent;
    bool swapped = true;

    while (swapped) {
        std::size_t first_child = parent * 2 + 1;
        std::size_t second_child = parent * 2 + 2;
        if (first_child < _data.size()
                && _data[first_child].second < _data[minimum_index].second) {
            minimum_index = first_child;
        }
        if (second_child < _data.size() 
                && _data[second_child].second < _data[minimum_index].second) {
            minimum_index = second_child;
        }

        if (minimum_index != parent) {
            std::swap(_data[parent], _data[minimum_index]);
            parent = minimum_index;
        } else {
            swapped = false;
        }
    }
}

void PriorityQueue::restoreUp(std::size_t index) {
    while (index != 0) {
        std::size_t parent = (index + 1) / 2 - 1;
        if (_data.at(parent).second > _data.at(index).second) {
            std::swap(_data.at(parent), _data.at(index));
            index = parent;
        } else {
            break;
        }
    }
}

}
