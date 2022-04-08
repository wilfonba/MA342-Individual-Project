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
    double alpha;
    double beta;
    double h;
    double r;
    int visualize;
} options;


void parseOptions(int argc,char** argv,options* opt) {
    int tempI;
    char tempS;
    double tempD;
    for (int i = 0;i < argc;i++) {
        if (sscanf(argv[i],"-N=%d",&tempI)==1) {
            opt->N = tempI;
        }
        else if (sscanf(argv[i],"-alpha=%lf",&tempD)==1) {
            opt->alpha = tempD;
        }
        else if (sscanf(argv[i],"-beta=%lf",&tempD)==1) {
            opt->beta = tempD;
        }
        else if (sscanf(argv[i],"-h=%lf",&tempD)==1) {
            opt->h = tempD;
        }
        else if (sscanf(argv[i],"-r=%lf",&tempD)==1) {
            opt->r = tempD;
        }
        else if (!strcmp(argv[i],"-visualize")) {
            opt->visualize = 1;
        }
    }
}

void printOptions(options* opt) {
    printf("Number of particles: %d \n",opt->N);
    printf("alpha: %f \n",opt->alpha);
    printf("beta: %f \n",opt->beta);
    printf("h: %f \n",opt->h);
    printf("r: %f \n",opt->r);
}

int plotBoundary(options* opt) {
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
}