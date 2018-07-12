//
// Created by geext on 7/12/18.
//

#ifndef MTSPSOLVER_DESTINATION_H
#define MTSPSOLVER_DESTINATION_H

// The struct represents destination from first vertex to the "destination" vertex
// in the Dijkstra algorithm implementation
struct Destination {
    Destination(int dest, int dist);
    int destination;
    int distance;
};


#endif //MTSPSOLVER_DESTINATION_H
