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
#include "WorkMessage.h"

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

    
          
            // Priprav se deska.
            Board board;
            Input input(&board);
            input.parseArguments(argc, argv);

            Solver solver(&board, input.getTargetTower(), input.getMaxDepth());
            vector<Move> solution;
              if(myRank==0){    
            cout << "Towers of Hanoi" << endl;
            input.printTask();
            }
            
            
            solver.init();
            solver.solve(solution);

            // Vypise reseni.
             if (myRank == 0) {
            for (vector<Move>::const_iterator it = solution.begin(); it != solution.end(); ++it) {
                cout << *it << endl;
            }

            cout << "Solution has " << solution.size() << " steps." << endl;
             }
//            //=== Testovani serializace ===
//            
//            // Vytvori se sekvence tahu.
//            vector<Move> moves;
//            Move move1(0, 2, 2);
//            Move move2(3, 2, 1);
//            Move move3(3, 0, 7);
//            Move move4(1, 3, 5);
//            moves.push_back(move1);
//            moves.push_back(move2);
//            moves.push_back(move3);
//            moves.push_back(move4);
//            
//            Move move5(2, 3, 1);
//            
//            SpaceItem spaceItem(board, moves, move5);
//            WorkMessage message;
//            message.addItem(spaceItem);
//            message.addItem(spaceItem);
//            message.addItem(spaceItem);
//            
//            char buffer[BUFFER_SIZE];
//            int position = 0;
//            message.serialize(buffer, position);
//            
//            cout << endl << "I will send (" << position  << "B): " << endl;
//            cout << message << endl;
//            
//            int dest = 1;
//            int tag = 0;
//            MPI_Send(buffer, position, MPI_PACKED, dest, tag, MPI_COMM_WORLD);
//                    
            
       
            
//            char buffer[BUFFER_SIZE];
//            WorkMessage message;
//            MPI_Status status;
//            
//            MPI_Recv(buffer, BUFFER_SIZE, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//            
//            int position = 0;
//            message.deserialize(buffer, position);
//            
//            cout << "I received (" << position << "B): " << endl;
//            cout << message << endl;
//            

       

        MPI_Finalize();


        return 0;

    } catch (const char* exception) {
        cout << exception << endl;
        MPI_Finalize();
        return 1;
    }


}

