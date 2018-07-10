//
// Created by geext on 7/8/18.
//

#ifndef MTSPSOLVER_MTSP_SOLVER_H
#define MTSPSOLVER_MTSP_SOLVER_H

namespace mtsp {
    class MtspSolver {

    public:
        const static int MaximumSameResults = 10000;
        const static int InitialNumber = 1;
        const static int MutationNumber = InitialNumber * 4;
        const static int CrossoverNumber = InitialNumber * 2;
        const static int SelectionGroupSize = (InitialNumber + CrossoverNumber + MutationNumber) / InitialNumber;
    };
}


#endif //MTSPSOLVER_MTSP_SOLVER_H
