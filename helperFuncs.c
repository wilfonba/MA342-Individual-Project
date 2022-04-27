typedef struct {
    int N;
    double H;
    double W;
    double separation;
    double separationStrength;
    double alignmentStrength;
    double cohesionStrength;
    double maxV;
    double dt;
    double tf;
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
        } else if (sscanf(argv[i],"-alignmentStrength=%le",&tempD)==1) {
            opt->alignmentStrength = tempD;
        } else if (sscanf(argv[i],"-cohesionStrength=%le",&tempD)==1) {
            opt->cohesionStrength = tempD;
        } else if (sscanf(argv[i],"-maxV=%le",&tempD) == 1) {
            opt->maxV = tempD;
        } else if (sscanf(argv[i],"-dt=%le",&tempD) == 1) {
            opt->dt = tempD;
        } else if (sscanf(argv[i],"-tf=%le",&tempD)==1) {
            opt->tf = tempD;
        } else if (sscanf(argv[i],"-frameDt=%le",&tempD)==1) {
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

void vectorAdd(double* x,double* y,double* result) {
    result[0] = x[0] + y[0];
    result[1] = x[1] + y[1];
}

void vectorSubtract(double* x,double* y,double* result) {
    result[0] = x[0] - y[0];
    result[1] = x[1] - y[1];
}

double vectorNorm(double* x) {
    return sqrt(x[0]*x[0] + x[1]*x[1]);
}

void boundPosition(boid boidLim,options* opt) {
    double velocityCorrection = 5.0;
    if (boidLim.pX < 0) {
        boidLim.vX = velocityCorrection;
    } else if (boidLim.pX > opt->W) {
        boidLim.vX = -velocityCorrection;
    } else if (boidLim.pY < 0) {
        boidLim.vY =  velocityCorrection;
    } else if (boidLim.pY > opt->H) {
        boidLim.vY = -velocityCorrection;
    }
}

void limitVelocity(boid boidLim,options* opt) {
    double v[2] = {boidLim.vX,boidLim.vY};
    if (vectorNorm(v) > opt->maxV) {
        unitVector(v[0],v[1],v);
        boidLim.vX = v[0]*opt->maxV;
        boidLim.vY = v[1]*opt->maxV;
    }
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
    fprintf(fp,"Separation Strength: %f\n",opt->separationStrength);
    fprintf(fp,"Alignment Strength: %f\n",opt->alignmentStrength);
    fprintf(fp,"Cohesion Strength: %f\n",opt->cohesionStrength);
    fprintf(fp,"Max Velocity: %f\n",opt->maxV);
    fprintf(fp,"Final Time: %f\n",opt->tf);
    fprintf(fp,"Delta t: %f\n",opt->dt);
    fprintf(fp,"Frame Delta t: %f\n",opt->frameDt);
    fprintf(fp,"Filename: %s\n",opt->filename);
    fprintf(fp,"#,vX,vY,pX,pY\n");
}

void alignment(boid* boids,options* opt,double* vAlignment,int i) {
    //printf("In Alignment %d\n",i);
    double vi[2];
    double vj[2];
    double v[2] = {0,0};
    int N = opt->N;
    vi[0] = boids[i].vX;
    vi[1] = boids[i].vY;

    for (int j = 0;j < N;j++) {
        vj[0] = boids[j].vX;
        vj[1] = boids[j].vY;
        if (i!=j) {
            vectorAdd(v,vj,v);
        }
    }
    v[0]/=(N-1);
    v[1]/=(N-1);
    vAlignment[0] = v[0]*opt->alignmentStrength;
    vAlignment[1] = v[1]*opt->alignmentStrength;
}

void cohesion(boid* boids,options* opt,double* vCohesion,int i) {
    //printf("In Cohesion %d\n",i);
    double pc[2] = {0,0};
    double pj[2];
    int N = opt->N;
    for(int j = 0;j < N;j++) {
        pj[0] = boids[j].pX;
        pj[1] = boids[j].pY;
        if (j != i) {
            vectorAdd(pc,pj,pc);
        }
    }

    pc[0]/=(N-1);
    pc[1]/=(N-1);

    pj[0] = boids[i].pX;
    pj[1] = boids[i].pX;

    vectorSubtract(pc,pj,vCohesion);
    vCohesion[0]*=opt->cohesionStrength;
    vCohesion[1]*=opt->cohesionStrength;
}

void separation(boid* boids,options* opt,double* vSeparation,int i) {
    //printf("In Separation %d\n",i);
    double c[2] = {0,0};
    double pi[2];
    double pj[2];
    int N = opt->N;
    double distance[2];
    double L;

    pi[0] = boids[i].pX;
    pi[1] = boids[i].pY;

    for(int j = 0;j < N;j++) {
        pj[0] = boids[j].pX;
        pj[1] = boids[j].pY;
        if(j!=i) {
            vectorSubtract(pi,pj,distance);
            L = vectorNorm(distance);
            if (L < opt->separation) {
                vectorSubtract(c,distance,c);
            }
        }
    }
    c[0]*=opt->separationStrength;
    c[1]*=opt->separationStrength;
    vSeparation[0] = c[0];
    vSeparation[1] = c[1];
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
        //printf("Moving Boid %d\n",i);
        alignment(boids,opt,vAlignment,i);
        cohesion(boids,opt,vCohesion,i);
        separation(boids,opt,vSeparation,i);
        boids[i].vX+=(vAlignment[0] + vCohesion[0] + vSeparation[0]);
        boids[i].vY+=(vAlignment[1] + vCohesion[1] + vSeparation[1]);
        boundPosition(boids[i],opt);
        limitVelocity(boids[i],opt);
        boids[i].pX+=(boids[i].vX*opt->dt);
        boids[i].pY+=(boids[i].vX*opt->dt);
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
