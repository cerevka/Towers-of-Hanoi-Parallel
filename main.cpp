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

    int my_rank;
    int p;
    int dest;
    int tag = 1;
    int source;
    int length;
    MPI_Status status;
    char message[LENGTH];
    int m = 100;

    /* start up MPI */
    MPI_Init(&argc, &argv);

    /* find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* find out number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank != 0) {
        /* create message */
        sprintf(message, "Greetings from process %d!", my_rank);
        dest = 0;
        /* length of message is strlen(message)+1 to send '\0' */
        /* this is blocking send */
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    } else {
        /* my_rank == 0 */
        printf("There are %d processes.\n", p);
        for (source = 1; source < p; source++) {
            /* blocking receive */
            MPI_Recv(message, LENGTH, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            /* getting info about received message */
            MPI_Get_count(&status, MPI_CHAR, &length);
            printf("Message status: source=%d, tag=%d, error=%d, length=%d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR, length);
            /* message */
            printf("%s\n", message);
        }
    }

    /* shut down MPI */
    MPI_Finalize();

    return 0;
}

