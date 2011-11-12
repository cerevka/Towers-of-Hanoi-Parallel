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
    // Inicializace MPI.
    try {

        MPI_Init(&argc, &argv);

        // Zjisti sve ID.
        int myRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

        // Zjisti pocet procesu.
        int processesCount;
        MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

        if (myRank == 0) {
            // Inicializace vypoctu.
            cout << "Towers of Hanoi" << endl;

            // Priprav se deska.
            Board board;
            Input input(&board);
            input.parseArguments(argc, argv);

            Solver solver(&board, input.getTargetTower(), input.getMaxDepth());
            vector<Move> solution;
            //solver.solve(solution);

            // Vypise reseni.
            for (vector<Move>::const_iterator it = solution.begin(); it != solution.end(); ++it) {
                cout << *it << endl;
            }

            cout << "Solution has " << solution.size() << " steps." << endl;
            
            char buffer[BUFFER_SIZE];
            int position = 0;
            
            board.serialize(buffer, position);
            
            int dest = 1; 
            int tag = 1;
            // Vez se odesle.
            MPI_Send(buffer, position, MPI_PACKED, dest, tag, MPI_COMM_WORLD);
            
            cout << "Buffer size is: " << position << endl;
            
        } else {
            // Ostatni procesy.
            char buffer[BUFFER_SIZE];
            MPI_Status status;
            // Zprava se prijme.
            MPI_Recv(&buffer, BUFFER_SIZE, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // Zprava se rozbali.
            Board board;
            int position = 0;
            board.deserialize(buffer, position);
            
            cout << "Recieved board: " << endl << board << endl;
        }

        // Ukonci MPI.
        MPI_Finalize();
        return 0;

    } catch (const char* exception) {
        cout << exception << endl;
        MPI_Finalize();
        return 1;
    }


}

