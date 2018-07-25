#include <gtest/gtest.h>
#include "priority_queue.h"
using namespace mtsp;

TEST(TestPriorityQueue, PushExtract) {
    PriorityQueue pq;
    pq.push(0, 3);
    pq.push(1, 6);
    pq.push(2, 10);
    pq.push(3, 5);
    pq.push(5, 1);
    pq.push(6, 8);
    ASSERT_EQ(5, pq.extractMin());
    ASSERT_EQ(0, pq.extractMin());
    ASSERT_EQ(3, pq.extractMin());
    ASSERT_EQ(1, pq.extractMin());
    ASSERT_EQ(6, pq.extractMin());
    ASSERT_EQ(2, pq.extractMin());
}

TEST(TestPriorityQueue, DecreasePriority) {
    PriorityQueue pq;
    pq.push(0, 3);
    pq.push(1, 6);
    pq.push(2, 10);
    pq.push(3, 5);
    pq.push(5, 1);
    pq.push(6, 8);
    pq.decreasePriority(2, 0);
    pq.decreasePriority(3, 2);
    pq.decreasePriority(6, 4);
    ASSERT_EQ(2, pq.extractMin());
    ASSERT_EQ(5, pq.extractMin());
    ASSERT_EQ(3, pq.extractMin());
    ASSERT_EQ(0, pq.extractMin());
    ASSERT_EQ(6, pq.extractMin());
    ASSERT_EQ(1, pq.extractMin());
}
