/* 
 * File:   Engine.h
 * Author: vjethava
 *
 * Created on 9 April, 2010, 4:01 PM
 */

#ifndef _ENGINE_H
#define	_ENGINE_H

#include "common.h"
#include <vector>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <dai/factorgraph.h>
#include <dai/var.h>
#include <dai/index.h>
#include <dai/exceptions.h>
#include <map>
#include "dataset.h"
using namespace std;
using namespace dai;
/// The main class for fastweb
class Engine {
private: // BOOK KEEPING
    ChowLiu * cl;
   
    matrix<int>* instances;
    vector<Query*> queries;
    map<string, vector<Query*>* > wordToQueryMp;
    map<int, int> instanceIdMp;
    
private: // DATASET (change for new attributes)
    int numAttrs;
    vector<int> attrSizes;

private: // FACTOR GRAPH CONSTRUCTION
    vector<Var*> facetVars;
    vector<Factor*> clFactors;
    vector<Factor*> queryFactors;
    map<string, vector<Factor*>* > wordFactors;

private: // FLAGS
    bool chowLiuFlag;
    bool facetVarsFlag;
    bool queryReadFlag;
    bool wordFactorsFlag;
    bool queryFactorsFlag;
    bool clFactorsFlag;
public: // SOLUTION FOR NEW QUERY
    FactorGraph* sGraph;
    vector<Factor>* sFactors;
    

public:
    Engine();
    ~Engine() {   }
    void makeFactorGraph();
    void initializeFacets();
    vector<vector<int> > readArff(string fileName, string idFileName = "");
    void readQueries(string fileName);
    void computeChowLiu();
    void makeFacetVars();
    void makeChowLiuFactors(CLFactorType clType = MLE, double alpha = 0.5);
    void makeQueryFactors(map<string, vector<Query*>* >* ptrWqMp = NULL);
    void makeFactorForWord(const string& word, const vector<Query*>* vq = NULL);
    void solveForQuery(const vector<string>* word, const vector<int>* freq = NULL);
    void testSolver();
    
};

#endif	/* _ENGINE_H */

