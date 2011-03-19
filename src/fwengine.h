/* 
 * File:   Engine.h
 * Author: vjethava
 *
 * Created on 9 April, 2010, 4:01 PM
 */

#ifndef _FW_ENGINE_H
#define	_FW_ENGINE_H

#include "common.h"
#include "wnconnector.h"
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
class FwEngine {

private: // TYPES
    typedef vector<Query*> VQp;
    typedef vector<Query*>* VpQp;
    typedef map<string, vector<Query*>* > MSVpQp;
    typedef vector<Factor> VF;
    typedef vector<Factor*> VFp;
    typedef map<string, vector<Factor* > *> MSVpFp;
    typedef vector<Var*> VVarp;

private: // BOOK KEEPING
    ChowLiu * cl;
    WnConnector* wnc;
    Matrix<int>* instances;
    VQp queries;
    MSVpQp wordToQueryMp;
    map<int, int> instanceIdMp;

private:
    /// just to keep track of % training data - do not use externally
    double pTestSolver;
    int unseenWords;
private: // DATASET (change for new attributes)
    int numAttrs;
    vector<int> attrSizes;

private: // FACTOR GRAPH CONSTRUCTION
    VVarp facetVars;
    VFp clFactors;
    VFp queryFactors;
    MSVpFp wordFactors;

private: // FLAGS
    bool chowLiuFlag;
    bool facetVarsFlag;
    bool queryReadFlag;
    bool wordFactorsFlag;
    bool queryFactorsFlag;
    bool clFactorsFlag;

private: // ALGORITHM FLAGS
    bool onlyCL;
    bool useWN;
    bool onlyWords;

public: // SOLUTION FOR NEW QUERY
    FactorGraph* sGraph;
    VF* sFactors;
    VF* solution;

public: // WORDNET INTEGRATION
    /// the max depth at which to search for related words
    int  maxBfsDepth;
    /// hit per level up in the query_word-wn_word distance (in graph)
    double levelFactor;
    /// (overall) list of words which were not found in wn
    VS *missingWN, *missingDB;
    /// (overall) map of the depth at which the words were found in wn
    map<string, int>* foundMp;
    /// list of current words neighbours
    map<string, double>* wnMp;
    /// computes the wordnet relations - recursive
    int updateWnMp(int level, string word, bool update=true);
    
public:
    MSVpQp* train(vector<Query*> * trQ);
    inline int getQueryAttribute(Query* q, int attr) {
        return (*instances)(q->count, attr);
    }

    Matrix<int>* getInstances(vector<Query* > * cqueries);
    string getAttrStr(int attrNum, int indexInAttr, string* name=NULL);
    FwEngine();
    ~FwEngine() {   }
    void makeFactorGraph();
    void initializeFacets();
    VVI readArff(string fileName,
            string idFileName = "");
    void readQueries(string fileName);
    // XXX: Modified for partial data usage for training 
    void computeChowLiu(Matrix<int> * trInstances = NULL);
    void makeFacetVars();
    void makeChowLiuFactors(CLFactorType clType = MLE, double alpha = 0.5);
    void makeQueryFactors(MSVpQp* ptrWqMp = NULL);
    void makeFactorForWord(const string& word, const VQp* vq = NULL);
    vector<size_t> solveForQuery(const vector<string>* word,
            const vector<int>* freq = NULL, MSVpQp* sqMpPtr=NULL);
    vector<vector<size_t> > getQueriesMAP(
            const vector<vector<string>* > & wordVec,
            const vector<vector<int>* > & freqVec);
    int testSolver(double trainingPct = 1.0, bool useWN = true,
            bool onlyCL = false, bool onlyWords = false, int num=0);
    MSVpQp* getWordMpForQueries(vector<Query*>* trQ);
    void init(string arffFile, string queryFile);
    int writeRes(vector<vector<size_t> >& output,
            VQp & samples,
            string dirName = "res");
};

#endif	/* _ENGINE_H */

