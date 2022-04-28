#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include "/usr/include/python2.7/Python.h"

#include "helperFuncs.c"
#include "progressBar.c"

#define PI 3.14159265358979323846

int main(int argc, char** argv) {
    // Seed RNG
    srand(get_timestamp());
    long long unsigned int start = get_timestamp();

    // Parse Settings
    options opt;
    parseOptions(argc,argv,&opt);

    FILE* fp;
    fp = fopen(opt.filename,"w");
    //printOptions(fp,&opt);

    // Initialize Boids
    boid boids[opt.N];
    initializePositions(boids,&opt);

    int nSteps = (int)(opt.tf/opt.dt)   ;
    double t = 0;
    int frameSteps = opt.frameDt/opt.dt;
    //printFrame(fp,boids,t,&opt);
    for(int i = 0;i <= nSteps;i++) {
        //printf("%d/%d\n",i,nSteps);
        if (i%frameSteps == 0) {
            print_progress(100*i/nSteps,start);
            printFrame(fp,boids,t,&opt);
        }
        moveBoids(boids,&opt);
        t = t + opt.dt;
    }
    printf("\n");
    fclose(fp);
    return 0;
}