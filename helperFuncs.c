typedef struct {
    double pX;
    double pY;
    double vX;
    double vY;
    float radius;
    float density;
} grain;

typedef struct {
    int N;
} options;


void parseOptions(int argc,char** argv,options* opt) {
    int tempI;
    char tempS;
    for (int i = 0;i < argc;i++) {
        if (sscanf(argv[i],"-N=%d",&tempI)==1) {
            opt->N = tempI;
        }
    }
}

void printOptions(options* opt) {
    printf("Number of particles: %d \n",opt->N);
}