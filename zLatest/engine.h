/* 
 * File:   Engine.h
 * Author: vjethava
 *
 * Created on 9 April, 2010, 4:01 PM
 */

#ifndef _ENGINE_H
#define	_ENGINE_H

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
class Engine {
private: // BOOK KEEPING
    ChowLiu * cl;
    WnConnector* wnc;
    Matrix<int>* instances;
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
    vector<Factor>* solution;

private: // WORDNET INTEGRATION
    /// the max depth at which to search for related words
    int  maxBfsDepth;
    /// hit per level up in the query_word-wn_word distance (in graph)
    double levelFactor;
    vector<string>* missing;
    vector<string>* found;
    map<string, double>* wnMp;
    /// computes the wordnet relations - recursive
    void updateWnMp(int level, string word, bool update=true);
    
public:
    void train(vector<Query*> * trQ); 
    inline int getQueryAttribute(Query* q, int attr) {
        return (*instances)(q->count, attr);
    }

    Matrix<int>* getInstances(vector<Query* > * cqueries);
    string getAttrStr(int attrNum, int indexInAttr, string* name=NULL);
    Engine();
    ~Engine() {   }
    void makeFactorGraph();
    void initializeFacets();
    vector<vector<int> > readArff(string fileName,
            string idFileName = "");
    void readQueries(string fileName);
    // XXX: Modified for partial data usage for training 
    void computeChowLiu(Matrix<int> * trInstances = NULL);
    void makeFacetVars();
    void makeChowLiuFactors(CLFactorType clType = MLE, double alpha = 0.5);
    void makeQueryFactors(map<string, vector<Query*>* >* ptrWqMp = NULL);
    void makeFactorForWord(const string& word, const vector<Query*>* vq = NULL);
    vector<size_t> solveForQuery(const vector<string>* word,
            const vector<int>* freq = NULL);
    vector<vector<size_t> > getQueriesMAP(
            const vector<vector<string>* > & wordVec,
            const vector<vector<int>* > & freqVec);
    void testSolver();
    void testSolver1(double trainingPct = 1.0);
    map<string, vector<Query*>* >* getWordMpForQueries(vector<Query*>* trQ);
    void init(string arffFile, string queryFile);
    void writeRes(vector<vector<size_t> >& output,
            vector<Query*>& samples,
            string dirName = "res");
};

#endif	/* _ENGINE_H */

