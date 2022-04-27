#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include "/usr/include/python2.7/Python.h"

#include "helperFuncs.c"

#define PI 3.14159265358979323846

int main(int argc, char** argv) {
    // Seed RNG
    srand(987654321);

    // Parse Settings
    options opt;
    parseOptions(argc,argv,&opt);

    FILE* fp;
    fp = fopen(opt.filename,"w");
    printOptions(fp,&opt);

    // Initialize Boids
    boid boids[opt.N];
    initializePositions(boids,&opt);

    int nSteps = opt.tf/opt.dt - 1;
    double t = 0;
    int frameSteps = opt.frameDt/opt.dt;
    printFrame(fp,boids,t,&opt);
    /*for(int i = 0;i < nSteps;i++) {
        moveBoids(boids,&opt);
        if (i%frameSteps == 0) {
            printFrame(fp,boids,t,&opt);
        }
    }*/

    fclose(fp);
    return 0;
}