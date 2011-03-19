// dataset.cpp ---
//
// Filename: dataset.cpp
// Description:
// Author: Vinay Jethava
// Created: Thu Apr  8 13:45:25 2010 (+0200)
// Last-Updated: Thu Apr  8 16:39:54 2010 (+0200)
//           By: Vinay Jethava
//     Update #: 34
// URL:
// Keywords:
//

// Code:

#include "dataset.h"
#include "fwengine.h"
#include "wnconnector.h"
#include <engine.h>
#include <matrix.h>
#include <cstdio>
#include <string>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <getopt.h>
using namespace std;

string plotFileName;
int tester0(double p, bool useWN, bool onlyCL, bool onlyWords, int samples) {
    FwEngine e;
    int n = e.testSolver(p, useWN, onlyCL, onlyWords, samples);
    return n;
}

int matPlot(int N, int* num) {
    Engine *ep;
    mxArray* T = NULL;
    if (!(ep = engOpen("\0"))) {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        return EXIT_FAILURE;
    }
    stringstream ss("");
    ss<<"plotter(T,\'"<<plotFileName<<"\');";
    T = mxCreateNumericMatrix(N, 1, mxINT32_CLASS, mxREAL);
    memcpy((void*) mxGetPr(T), (void*) num, N*sizeof(int));
    engPutVariable(ep, "T", T);
    engEvalString(ep, ss.str().c_str());
    engClose(ep); 
}

int main(int argc, char** argv) {
    double p = 0.9;
    int samples = 0;
    stringstream ss("");
    if(argc >= 2) {
        p = (double) atof(argv[1]);
    }

    if(argc >= 3) {
        samples = (int) atoi(argv[2]);
    }
    cout << "num = "<< samples <<endl;
    vector<int> num;
    ss << "cmp_" << p << "_" << samples;
    plotFileName = ss.str();
    if(plotFileName.find('.') != string::npos) {
        int pos = (int) plotFileName.find('.');
        plotFileName = plotFileName.substr(0, pos) + "p" + plotFileName.substr(pos+1);
    }
    // TEST CHOW-LIU ???
    num.push_back(tester0(p, false, true, false, samples));
    // TEST WORDS
    num.push_back(tester0(p, false, false, true, samples));
    // TEST WORDS + WN
    num.push_back(tester0(p, true, false, true, samples));
    // TEST CHOW-LIU + WORDS
    num.push_back(tester0(p, false, false, false, samples));
     // TEST CHOW-LIU + WORDS + WN
    num.push_back(tester0(p, true, false, false, samples));
    ofstream tStream;
    tStream.open("T.txt");
    for(int i=0; i < num.size(); i++) {
        cout <<" num: "<< num[i] <<endl;
        tStream<< num[i]<<endl; 
    }

    int N2 = num.size();
    int toPassMatlab[N2];
    fi(0, N2) toPassMatlab[i] = num[i]; 
    matPlot(N2, (int*) toPassMatlab);
    ss.str("");
    ss << "display "<<plotFileName<<".png &";
//    system(ss.str().c_str());
    return 0; 
}
//
// dataset.cpp ends here
