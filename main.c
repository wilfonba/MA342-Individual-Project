#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

#include "helperFuncs.c"



void workerStuff(options* opt) {
    int a = 1;
}



void rootStuff(options* opt) {

    printOptions(opt);
}



int main(int argc, char** argv) {

    options opt;
    parseOptions(argc,argv,&opt);

    MPI_Init(&argc,&argv);
    
    int myRank;
    int numProcs;

    MPI_Comm_size(MPI_COMM_WORLD,&numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myRank);
    printf("My Rank is %2d, numProcs = %d\n",myRank,numProcs);

    if (myRank == 0) {
        rootStuff(&opt);
    } else {
        workerStuff(&opt);
    }

    MPI_Finalize();
    return 0;
}