/* 
 * File:   main.cpp
 * Author: Tomáš Čerevka, Adam Činčura
 *
 * Created on October 31, 2011, 3:21 PM
 */

#include <cstdlib>
#include <string>
#include <mpi.h>

using namespace std;

#define LENGTH 100

/*
 * Vstupni bod programu.
 */
int main(int argc, char** argv) {

    int myRank;
    int processesCount; 

    // Inicializace MPI.
    MPI_Init(&argc, &argv);

    // Proces ziska svuj rank (id).
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    /* Zjisti, kolik procesu bylo celkem spusteno. */
    MPI_Comm_size(MPI_COMM_WORLD, &processesCount);

    if (myRank == 0) {
        // Inicializace vypoctu nulou.        
    } else {
        // Zpracovani vypoctu.
    }

    // Ukonceni MPI.
    MPI_Finalize();

    return 0;
}

