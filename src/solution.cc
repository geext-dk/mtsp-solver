//
// Created by geext on 7/8/18.
//

#include "solution.h"

namespace mtsp {

    Solution::Solution(std::vector<unsigned long> &shops_array, std::vector<unsigned long> &paths_lengths)
            : _number_of_cars(paths_lengths.size()), _number_of_shops(shops_array.size()),
              _shops_array(shops_array), _paths_lengths(paths_lengths) {
    }

    Solution::Solution(unsigned long number_of_shops, unsigned long number_of_cars) : _number_of_shops(number_of_shops),
                                                                                      _number_of_cars(number_of_cars) {
        regeneratePathsLengths();
        regenerateShopsArray();
    }

    const std::vector<unsigned long>& Solution::getShopsArray() const {
        return _shops_array;
    }

    const std::vector<unsigned long>& Solution::getPathsLengths() const {
        return _paths_lengths;
    }

    Solution&& Solution::copy() const {
        return std::move(Solution(_shops_array, _paths_lengths));
    }

    void Solution::regeneratePathsLengths() {
        // random generator
        std::random_device random_device;
        std::default_random_engine random_engine(random_device());

        _paths_lengths.clear();
        _paths_lengths.resize(_number_of_cars, 0);

        std::vector<int> assigned(_number_of_cars, 0);
        int number_of_assigned_shops = 0;
        for (unsigned long i = 0; i < _number_of_cars - 1; ++i) {
            // upper bound to generate number of shops assigned to the car
            // constraint: at least 2 shops per car
            // so I reserve 2 shops for every car left to be assigned excluding the current processed car
            // 2 * (_number_of_cars - (i + 1)) <- this
            // then I subtract the number of already assigned shops (we can't assign them as they are already assigned)
            unsigned long upper_bound = _number_of_shops - 2 * (_number_of_cars - (i + 1)) - number_of_assigned_shops;
            std::uniform_int_distribution<unsigned long> generator(2, upper_bound);
            unsigned long random_number_of_shops = generator(random_engine);
            // then I assign the
            unsigned long random_car = firstFreeRandom(assigned, _number_of_cars - i);
            _paths_lengths[random_car] = random_number_of_shops;
            assigned[random_car] = 1;
            number_of_assigned_shops += random_number_of_shops;
        }

        unsigned long last_car;
        for (last_car = 0; last_car < _number_of_cars; ++last_car) {
            if (assigned[last_car] == 0) {
                break;
            }
        }

        _paths_lengths[last_car] = _number_of_shops - number_of_assigned_shops;
    }

    void Solution::regenerateShopsArray() {
        _shops_array.clear();
        _shops_array.resize(_number_of_shops);
        std::vector<int> assigned(_number_of_shops, 0);
        for (unsigned long i = 0; i < _number_of_shops; ++i) {
            unsigned long random_shop = firstFreeRandom(assigned, _number_of_shops - i);
            assigned[random_shop] = 1;
            _shops_array[i] = random_shop;
        }
    }
}
