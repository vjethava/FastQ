// dataset.cpp ---
// Filename: dataset.cpp
// Description:
// Author: Vinay Jethava
// Created: Thu Apr  8 13:45:25 2010 (+0200)
// Last-Updated: Wed Jan 26 23:12:13 2011 (+0100)
//           By: Vinay Jethava
//     Update #: 102
// URL:
// Keywords:
//
// Change Log:
// 19-Jan-2011    Vinay Jethava
//    Last-Updated: Wed Jan 19 13:04:45 2011 (+0100) #64 (Vinay Jethava)
//    Revision number 2 just for fun.
// 19-Jan-2011    Vinay Jethava
//    Last-Updated: Wed Jan 19 13:04:09 2011 (+0100) #63 (Vinay Jethava)
//    Trying out changelog using make-revision
// 19-Jan-2011    Vinay Jethava
//    Post WWW version
//

// Code:

#include "dataset.h"
#include "fwengine.h"
#include "wnconnector.h"
// #include <engine.h>
// #include <matrix.h>
#include <cstdio>
#include <string>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <getopt.h>
using namespace std;
ofstream legFile;
string plotFileName;


void writeNums(vector<int>& num) {
  ofstream tStream;
  tStream.open("T.txt");

  for(int i=0; i < num.size(); i++) {
    //  cout <<" num: "<< num[i] <<endl;
    tStream<< num[i]<<endl;
  }
  tStream.close();
}

int tester0(double p, bool useWN, bool onlyCL, bool onlyWords, int samples) {
  FwEngine e;
  int n = e.testSolver(p, useWN, onlyCL, onlyWords, samples);
  legFile<<useWN<<" " <<onlyCL<< " "<<onlyWords<<"\n";
  return n;
}


/// this implements the 10-30-50 test profiling
int test2() {
  ofstream pStream, p2Stream;
  pStream.open("P.txt");
  p2Stream.open("P2.txt");
  /// the random id of the simulation
  vector<int> num;
  /// % of training data
  double p;
  /// maximum number of test samples
  int samples = 1000; // for remaining to use as test
  int MM = 4; 
  int N = 4;
  double myAxP[10]={0.70, 0.90, 0.95, 0.99};
  // for(int i=0; i < N; i++) {
  //   myAxP[i] = 0.01 + 0.02*(i);
  // }
  for(int j=0; j < MM; j++) {  
  for(int i=0; i < N; i++) {
    fprintf(stderr, "i: %d of %d j: %d of %d\n\n", i, N, j, MM); 
	    fflush(stderr); 
    p = myAxP[i];
    // p = 0.1 + 0.2*i;
    pStream << p <<endl;
    // // TEST WORDS
    p2Stream<<p <<endl;
    num.push_back(tester0(p, false, false, true, samples));
    // TEST WORDS + WN
    p2Stream<<p <<endl;
    num.push_back(tester0(p, true, false, true, samples));
    // // TEST CHOW-LIU + WORDS
    //p2Stream<<p <<endl;
    //num.push_back(tester0(p, false, false, false, samples));
    // // TEST CHOW-LIU + WORDS + WN
    p2Stream<<p <<endl;
    num.push_back(tester0(p, true, false, false, samples));
  }
  }
  // ofstream trial;
  /* FILE* tfile = fopen("T.txt", "w");
     FOREACH(i, num)
     fprintf(tfile, "%d\n", *i);
     fclose(tfile); */
  writeNums(num);
  pStream.close();
  p2Stream.close();
}


int main(int argc, char *argv[]) {
  legFile.open("mylegend.txt", ios::out);

  test2();
  legFile.close();
  return 0;
}


// pair<double, int> getLineSearchParams(double p=0.5) {
//     pair<double, int> result;
//     double accuracy = 0.0;
//     for(int depth=1; depth <= 5; depth++)
//      {
//          for(double lfactor = 1.0; lfactor >= 0.1; lfactor= lfactor - 0.1)
//          {
//              FwEngine e;
//              vector<double> cacc;
//          }
//      }
//     return result;
// }
// int matPlot(int N, int* num) {

//     system("rm -f *.png");
//     Engine *ep;
//     mxArray* T = NULL;
//     if (!(ep = engOpen("\0"))) {
//         fprintf(stderr, "\nCan't start MATLAB engine\n");
//         return EXIT_FAILURE;
//     }
//     stringstream ss("");
//     ss<<"plotter(T,\'"<<plotFileName<<"\');";
//     T = mxCreateNumericMatrix(N, 1, mxINT32_CLASS, mxREAL);
//     memcpy((void*) mxGetPr(T), (void*) num, N*sizeof(int));
//     // engPutVariable(ep, "T", T);
//     engEvalString(ep, "load(\'T.txt\');");
//     engEvalString(ep, ss.str().c_str());
//     engClose(ep);
//     system("mv *.fig result/");

//     ss.str("");
//     ss << "ristretto "<< plotFileName<< "_topic.png &";
//     system(ss.str().c_str());
// }

// int test1(int argc, char** argv) {
//     legFile.open("mylegend.txt", ios::out);
//     double p = 0.9;
//     int samples = 0;
//     stringstream ss("");
//     if(argc >= 2) {
//         p = (double) atof(argv[1]);
//     }

//     if(argc >= 3) {
//         samples = (int) atoi(argv[2]);
//     }
//     cout << "num = "<< samples <<endl;
//     vector<int> num;
//     ss << "cmp_" << p << "_" << samples;
//     plotFileName = ss.str();
//     if(plotFileName.find('.') != string::npos) {
//         int pos = (int) plotFileName.find('.');
//         plotFileName = plotFileName.substr(0, pos) + "p" + plotFileName.substr(pos+1);
//     }

//     // TEST CHOW-LIU ???
//     //   num.push_back(tester0(p, false, true, false, samples));
//     // TEST WORDS
//     num.push_back(tester0(p, false, false, true, samples));
//     // TEST WORDS + WN
//     num.push_back(tester0(p, true, false, true, samples));
//     // TEST CHOW-LIU + WORDS
//     num.push_back(tester0(p, false, false, false, samples));
//     // TEST CHOW-LIU + WORDS + WN
//     num.push_back(tester0(p, true, false, false, samples));
//     ofstream tStream;
//     tStream.open("T.txt");
//     for(int i=0; i < num.size(); i++) {
//         //  cout <<" num: "<< num[i] <<endl;
//         tStream<< num[i]<<endl;
//     }

//     int N2 = num.size();
//     int toPassMatlab[N2];
//     fi(0, N2) toPassMatlab[i] = num[i];
//     // matPlot(N2, (int*) toPassMatlab);
//     legFile.close();
//     tStream.close();
//     return 0;
// }

//
// main.cpp ends here
