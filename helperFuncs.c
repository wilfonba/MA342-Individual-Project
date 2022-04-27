typedef struct {
    int N;
    double H;
    double W;
    double separation;
    double alignment;
    double cohesion;
    double separationStrength;
    double alignmentStrength;
    double cohesionStrength;
    double maxV;
    double dt;
    int tf;
    double frameDt;
    char filename[25];
} options;

typedef struct{
    double vX;
    double vY;
    double pX;
    double pY;
} boid;


void parseOptions(int argc,char** argv,options* opt) {
    int tempI;
    char tempS;
    double tempD;
    for (int i = 0;i < argc;i++) {
        if (sscanf(argv[i],"-N=%d",&tempI)==1) {
            opt->N = tempI;
        } else if (sscanf(argv[i],"-W=%le",&tempD)==1) {
            opt->W = tempD;
        } else if (sscanf(argv[i],"-H=%le",&tempD)==1) {
            opt->H = tempD;
        } else if (sscanf(argv[i],"-separation=%le",&tempD)==1) {
            opt->separation = tempD;
        } else if (sscanf(argv[i],"-separationStrength=%le",&tempD)==1) {
            opt->separationStrength = tempD;
        } else if (sscanf(argv[i],"-alignment=%le",&tempD)==1) {
            opt->alignment = tempD;
        } else if (sscanf(argv[i],"-alignmentStrength=%le",&tempD)==1) {
            opt->alignmentStrength = tempD;
        } else if (sscanf(argv[i],"-cohesion=%le",&tempD)==1) {
            opt->cohesion = tempD;
        } else if (sscanf(argv[i],"-cohesionStrength=%le",&tempD)==1) {
            opt->cohesionStrength = tempD;
        } else if (sscanf(argv[i],"-maxV=%le",&tempD) == 1) {
            opt->maxV = tempD;
        } else if (sscanf(argv[i],"-dt=%le",&tempD) == 1) {
            opt->dt = tempD;
        } else if (sscanf(argv[i],"-tf=%i",&tempI)==1) {
            opt->tf = tempI;
        } else if (sscanf(argv[i],"-frameDt=%d",&tempI)==1) {
            opt->frameDt = tempD;
        }
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(opt->filename,"%02d-%02d-%d_%02d-%02d-%02d.out",tm.tm_mon + 1, tm.tm_mday,tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void unitVector(double x,double y,double* dir) {
    double length = sqrt(x*x + y*y);
    dir[0] = x/length;
    dir[1] = y/length;

}

void initializePositions(boid* boids,options* opt) {
    double HScale;
    double WScale;
    double VScale;
    double velocity;
    double dir[2];

    HScale = opt->H/RAND_MAX;
    WScale = opt->W/RAND_MAX;
    VScale = opt->maxV/RAND_MAX;

    for(int i = 0;i < opt->N;i++) {
        unitVector(rand()-RAND_MAX/2,rand()-RAND_MAX/2,dir);
        velocity = rand()*VScale;
        boids[i].vX = velocity*dir[0];
        boids[i].vY = velocity*dir[1];
        boids[i].pX = rand()*WScale;
        boids[i].pY = rand()*HScale;
        //printf("%d: %f %f %f %f \n",i,boids[i].vX,boids[i].vY,boids[i].pX,boids[i].pY);
    }
}   

void printOptions(FILE* fp,options* opt) {
    fprintf(fp,"N: %d\n",opt->N);
    fprintf(fp,"H: %f\n",opt->H);
    fprintf(fp,"W: %f\n",opt->W);
    fprintf(fp,"Separation: %f\n",opt->separation);
    fprintf(fp,"Alignment: %f\n",opt->alignment);
    fprintf(fp,"Cohesion: %f\n",opt->cohesion);
    fprintf(fp,"Separation Strength: %f\n",opt->separationStrength);
    fprintf(fp,"Alignment Strength: %f\n",opt->alignmentStrength);
    fprintf(fp,"Cohesion Strength: %f\n",opt->cohesionStrength);
    fprintf(fp,"Max Velocity: %f\n",opt->maxV);
    fprintf(fp,"Final Time: %i\n",opt->tf);
    fprintf(fp,"Delta t: %f\n",opt->dt);
    fprintf(fp,"Frame Delta t: %f\n",opt->frameDt);
    fprintf(fp,"Filename: %s\n",opt->filename);
    fprintf(fp,"#,vX,vY,pX,pY\n");
}

void alignment(boid* boids,options* opt,double* vAlignment) {

}

void cohesion(boid* boids,options* opt,double* vAlignment) {
    
}

void separation(boid* boids,options* opt,double* vAlignment) {
    
}

void printFrame(FILE* fp,boid* boids, double t,options* opt) {
    fprintf(fp,"\n\n");
    fprintf(fp,"t = %f\n",t);
    for(int i = 0;i < opt->N;i++) {
        fprintf(fp,"%d,%f,%f,%f,%f \n",i,boids[i].vX,boids[i].vY,boids[i].pX,boids[i].pY);
    }
}

void moveBoids(boid* boids,options* opt) {
    double vAlignment[2], vCohesion[2], vSeparation[2];
    for(int i = 0;i < opt->N;i++) { 
        alignment(boids,opt,vAlignment);
        cohesion(boids,opt,vCohesion);
        separation(boids,opt,vSeparation);
    }  
}


/*int plotBoundary(options* opt) {
    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue;
    Py_Initialize();

        PyRun_SimpleString("import os");
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
        pName = PyUnicode_FromString("pythonHelpers");
        pModule = PyImport_Import(pName);
        PyErr_Print(); 
        Py_DECREF(pName);
        
        if (pModule != NULL) {
            pFunc = PyObject_GetAttrString(pModule,"plotBoundary");
            if (pFunc && PyCallable_Check(pFunc)) {
                pArgs = PyTuple_New(4);
                pValue = PyFloat_FromDouble(opt->alpha);
                PyTuple_SetItem(pArgs, 0, pValue);
                pValue = PyFloat_FromDouble(opt->beta);
                PyTuple_SetItem(pArgs, 1, pValue);
                pValue = PyFloat_FromDouble(opt->h);
                PyTuple_SetItem(pArgs, 2, pValue);
                pValue = PyFloat_FromDouble(opt->r);
                PyTuple_SetItem(pArgs, 3, pValue);
                
                pValue = PyObject_CallObject(pFunc, pArgs);
                Py_DECREF(pArgs);
                if (pValue != NULL) {
                    //printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                    Py_DECREF(pValue);
                }
                else {
                    Py_DECREF(pFunc);
                    Py_DECREF(pModule);
                    PyErr_Print();
                    fprintf(stderr,"Call failed\n");
                    return 1;
                }
            } else {
               printf("errors in plotBoundary\n");
            }
        }
    Py_Finalize();
}*/
