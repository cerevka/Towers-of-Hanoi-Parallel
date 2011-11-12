/* 
 * File:   main.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on October 31, 2011, 3:21 PM
 */

#include <cstdlib>
#include <string>

#include <mpi.h>

#include "Input.h"
#include "Solver.h"

using namespace std;

/*
 * Vstupni bod programu.
 */
int main(int argc, char** argv) {
    try {
        MPI_Init(&argc, &argv);
        int myRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        int processesCount;
        MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

        if (myRank == 0) {
            cout << "Towers of Hanoi" << endl;
            
            // Priprav se deska.
            Board board;
            Input input(&board);
            input.parseArguments(argc, argv);

            Solver solver(&board, input.getTargetTower(), input.getMaxDepth());
            vector<Move> solution;
            solver.solve(solution);

            // Vypise reseni.
            for (vector<Move>::const_iterator it = solution.begin(); it != solution.end(); ++it) {
                cout << *it << endl;
            }

            cout << "Solution has " << solution.size() << " steps." << endl;
        } else {

        }

        MPI_Finalize();


        return 0;

    } catch (const char* exception) {
        cout << exception << endl;
        MPI_Finalize();
        return 1;
    }


}

