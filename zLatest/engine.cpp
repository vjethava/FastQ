// engine.cpp ---
//
// Filename: engine.cpp
// Description:
// Author: Vinay Jethava
// Created: Thu Apr  8 15:14:31 2010 (+0200)
// Last-Updated: Mon Apr 12 17:33:33 2010 (+0200)
//           By: Vinay Jethava
//     Update #: 88
// URL:
// Keywords:
//

// Code:


#include "engine.h"
#include "common.h"
#include "dataset.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <dai/alldai.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <dai/varset.h>
#include <dai/exceptions.h>
#include <dai/bp.h>
#include <dai/properties.h>
#include <exception>
#include <dai/smallset.h>
#include "dataset.h"
using namespace std;

// Read from common.cpp
extern int numAttributes;
extern int attrNumVals[];
extern string* attributes[]; 

Engine::Engine() {
   
    // copying to allow modification later - this part is all that needs
    // to be changed, if moving to a different set of attributes. Currently,
    // attributes defined in common.cpp
    numAttrs = numAttributes;
    for (int i = 0; i < numAttrs; i++) {
        attrSizes.push_back(attrsNumVals[i]);
    }
    chowLiuFlag = false;
    facetVarsFlag = false;
    queryReadFlag = false;
    wordFactorsFlag = false;
    queryFactorsFlag = false;
    clFactorsFlag = false;
    sGraph= NULL;
    sFactors = NULL; 
  }
/// Reads the ARFF file name data part.
vector<vector<int> > Engine::readArff(string fileName, string idFileName) {
    vector<vector<int> > result;
    ifstream inFileStream;
    try {
        inFileStream.open(fileName.c_str());
    } catch (std::exception& e) {
        cerr << " Exception: " << e.what() << endl;
        exit(-1);
    }
    string line;
    int count = 0;
    char delimiter = ',';
    while (getline(inFileStream, line)) { // do the reading
        count++;
        vector<string> currAttrs;
        for (int i = 0; i < numAttributes - 1; i++) {
            size_t pos = line.find_first_of(delimiter);
            string lAttr = line.substr(0, pos);
            currAttrs.push_back(lAttr);
            line = line.substr(pos + 1);
        }
        // strip trailing character
        line = line.substr(0, line.length() - 1);
        currAttrs.push_back(line);

        //            if(count == 1)
        //                {
        //                    for(int i=0; i < numAttributes; i++) {
        //                        fprintf(stderr, "%s\n", currAttrs[i].c_str());
        //                    }
        //                }
        vector<int> currAttrState;
        for (int i = 0; i < numAttributes; i++) {
            const char* ca = (currAttrs[i]).c_str();
            int cl = currAttrs[i].length();
            char* ta;
            bool notFound;
            int n = attrsNumVals[i];
            int idx = 0;

            do {
                int tl = (attributes[i])[idx].length();
                ta = (char*) ((attributes[i])[idx]).c_str();
                notFound = !(strcmp(ca, ta) == 0);
                idx++;
                //    fprintf(stderr, "i: %d n: %d\n\tc: %s l: %d\n\tt: %s l: %d\n ", i, n, ca, cl, ta , tl);
            } while ((idx <= n) && notFound);
            idx--;
            if (idx >= attrsNumVals[i]) {
                fprintf(stderr, " not found: %s\n", ca);
                exit(-1);
            } else {
                currAttrState.push_back(idx);
                //    fprintf(stderr, " idx: %d\n" , idx);
            }
        }
        result.push_back(currAttrState);
    }
    instances = new matrix<int>(result.size(), result[0].size());
    for (int i = 0; i < (int) instances->size1(); i++) {
        for (int j = 0; j < (int) instances->size2(); j++) {
            instances->insert_element(i, j, result[i][j]);
        }
    }
    FPRINTF(stderr, "Lines in arff : %d \n", (int) count);
    inFileStream.close();
    // read the corresponding ids for instances
    if (idFileName != "") { // read the id count
        ifstream idStream;
        try {
            idStream.open(idFileName.c_str());
        } catch (std::exception& e) {
            cerr << "Exception: " << e.what() << endl;
            exit(-1);
        }
        string line;
        int idCount = 0;
        while (getline(idStream, line)) {
            idCount++;
            line = trim(line);
            int id = atoi(line.c_str());
            // fprintf(stderr, "%d. id= %d\n", idCount, id);
            if (instanceIdMp.find(id) != instanceIdMp.end()) {
                fprintf(stderr, "Multiple query id: %d\n", id);
            }
            instanceIdMp.insert(make_pair<int, int>(id, idCount - 1));
        }
        FPRINTF(stderr, "Lines in queryIds: %d\n", (int) count);
        ASSERT(idCount == count);
    }
    return result;
}

void Engine::readQueries(string fileName) {
    ifstream inFileStream;
    inFileStream.open(fileName.c_str());
    string line;
    int count = 0;
    while (getline(inFileStream, line)) {
        count++;
        line = trim(line);
        vector<string> token1;
        tokenize(line, token1, "\t");

        if (token1.size() == 2) { // the corresponding count 
            int id = atoi(token1[0].c_str());
            line = token1[1];
            vector<string> tokens;
            tokenize(line, tokens);
            // check that at least one word is present with freq, 
            if ((tokens.size() > 0) && (tokens.size() % 2 == 0)) {
                vector<string> words;
                vector<int> freq;

                enum State {
                    WORD = 0, FREQ
                };
                State state = WORD;
                for (int i = 0; i < (int) tokens.size(); i++) {
                    switch (state) {
                        case WORD:
                            words.push_back(tokens[i]);
                            break;
                        case FREQ:
                            int cfreq = atoi(tokens[i].c_str());
                            freq.push_back(cfreq);
                            break;
                    }
                    state = (State) (1 - state);
                }

                assert(words.size() > 0);
                assert(words.size() == freq.size());
                //                fprintf(stderr, "%d: %s\n", count, line.c_str());
                //                cout << getStr<string > (words) << endl;
                //                cout << getStr<int>(freq) << endl;
                //                if ((count >= 4910) && (count <= 4920)) cin.get();

                Query* query = new Query(id, words, freq, this);
                query->count = count - 1; // FIXME: Hack to handle multiple ids
                this->queries.push_back(query);
                // Insert the query corresponding to the word in wordToQueryMp
                for (int i = 0; i < (int) words.size(); i++) {
                    typedef map<string, vector<Query*>* > SqMp;
                    typedef map<string, vector<Query*>* >::iterator SqMpIter;
                    SqMpIter mIt = wordToQueryMp.find(words[i]);
                    if (mIt == wordToQueryMp.end()) {
                        vector<Query*>* ptrQryVec = new vector<Query*>();
                        wordToQueryMp.insert(
                                make_pair<string, vector<Query*>* >(words[i],
                                ptrQryVec));
                    }
                    mIt = wordToQueryMp.find(words[i]);
                    mIt->second->push_back(query);

                    //                    pair < map<string, vector<Query*>* >::iterator, bool> wqPair;
                    //                    wqPair = wordToQueryMp.insert(
                    //                            make_pair<string, vector<Query*>* >(words[i],
                    //                            new vector<Query*>()));
                    //                    map<string, vector<Query*>* >::iterator wqIter = wqPair.first;
                    //                    wqIter->second->push_back(query);
                }

            }
        }

    }
    // NOTE: have a query corresponding to each instance
    assert(count == (int) instances->size1());
    fprintf(stderr, "Total number of words: %d\n", (int) this->wordToQueryMp.size());
    inFileStream.close();
    queryReadFlag = true;
}

/// Computes the Chow Liu Factors.

void Engine::computeChowLiu() {
    cl = new ChowLiu(this->instances);
    cl->computeMutualCounts();
    cl->computeMutualInfo();
    cl->findBestFactors();
    chowLiuFlag = true;
}

/// Initializes the facet variables

void Engine::makeFacetVars() {
    for (int i = 0; i < numAttrs; i++) {
        int states = attrSizes[i];
        Var* cVar = new Var(i, (size_t) states);
        facetVars.push_back(cVar);
    }
    facetVarsFlag = true;
}

/// Initializes the chow-liu factorization

void Engine::makeChowLiuFactors(CLFactorType clType, double alpha) {
    assert(chowLiuFlag == true);
    assert(facetVarsFlag == true);
    assert(queryReadFlag == true);
    for (int i = 0; i < numAttrs - 1; i++) {
        int idx = cl->j0[i];
        matrix<double>* cProb = cl->mutualCounts[idx];
        pair<int, int> pi1 = cl->getXY(idx);
        VarSet* cVarSet = new VarSet(*facetVars[pi1.first], *facetVars[pi1.second]);
       // fprintf(stderr, "facetVars[%d] states: %d\n", pi1.first, (int) facetVars[pi1.first]->states());
       // fprintf(stderr, "facetVars[%d] states: %d\n", pi1.second, (int) facetVars[pi1.second]->states());
        Factor* cFactor = new Factor(*cVarSet);
        map<Var, size_t> state;

        for (int i = 0; i < attrSizes[pi1.first]; i++) {
            for (int j = 0; j < attrSizes[pi1.second]; j++) {
                state.clear();
                state.insert(make_pair<Var, size_t > (*(facetVars[pi1.first]), (size_t) i));
                state.insert(make_pair<Var, size_t > (*(facetVars[pi1.second]), (size_t) j));
                size_t linState = calcLinearState(*cVarSet, state);
                // fprintf(stderr, " linState: %d i: %d j: %d\n" , (int ) linState, i, j);
                switch (clType) {
                        // MLE approach
                    case MLE:
                        (*cFactor)[linState] = (*cProb)(i, j);
                        break;
                    case MAP:
                        (*cFactor)[linState] = (0.5 * (*cProb)(i, j)
                                + 0.5 / ((double) cVarSet->nrStates()));
                        break;
                    case OTHER:
                        (*cFactor)[linState] = (alpha * (*cProb)(i, j)
                                + (1 - alpha) / ((double) cVarSet->nrStates()));
                        break;
                }
            }
        }
        clFactors.push_back(cFactor);
        //        cerr<<*cFactor << endl<< (*cProb)<<endl<<endl;
        //        cerr << *cVarSet <<  "states: "<< cVarSet->nrStates() <<endl;
    }
    this->clFactorsFlag = true; 
}


/// make the factors corresponding to the queries

void Engine::makeQueryFactors(map<string, vector<Query*>* >* ptrWordQueryMp) {
    
    if (ptrWordQueryMp == NULL) {
        ptrWordQueryMp = &(this->wordToQueryMp);
    }
    typedef map<string, vector<Query*> *> WqMp;
    typedef map<string, vector<Query*> *>::iterator WqMpIter;
    for (WqMpIter it = (ptrWordQueryMp)->begin(); it != (ptrWordQueryMp)->end(); it++) {
        string word = it->first;
        vector<Query*>* vq = it->second;
        this->makeFactorForWord(word, vq);
    }

}

void Engine::makeFactorForWord(const string& word, const vector<Query*>* vq) {
    typedef vector<Query*>::const_iterator VqIter;
    map<string, vector<Factor*>* >::iterator sfvIter = wordFactors.find(word);
    if (sfvIter == wordFactors.end()) { // introduce a new factors
        vector<Factor*>* cVec = new vector<Factor*>();
        for (int i = 0; i < this->numAttrs; i++) {
            Factor* vFactor = new Factor(*facetVars[i]);
            //fprintf(stderr, "Created factor %d for %s with size %d\n", i,
            //        word.c_str(), (int) vFactor->states());

            /// this has the global statistics for the current facet
            int mystates = (int) facetVars[i]->states();
            double facetTotCounts[mystates];
            for (int kl = 0; kl < mystates; kl++) {
                facetTotCounts[kl] = 1.0;
            }

            double cSum = (double) mystates;
            if (vq != NULL) { // handles the missing word in DB
                for (VqIter vqIter = vq->begin(); vqIter != vq->end(); vqIter++) {
                    Query* cQuery = (*vqIter);

                    map<string, int>::iterator it = cQuery->words->find(word);
                    if (it == cQuery->words->end()) {
                        cerr << cQuery->getStr() << " " << word << "count: " << cQuery->count << endl;
                        exit(-1);
                    }
                    int wFreq = it->second;
                    //     fprintf(stderr, "Analyzing query: %d for word: %s freq: %d\n", cQuery->id, word.c_str(), wFreq);
                    for (int w = 0; w < wFreq; w++) { // count per word freq in query
                        int cFacetAssignment = (*instances)(cQuery->count, i);
                        assert(cFacetAssignment < mystates);
                        assert(cFacetAssignment >= 0);
                        facetTotCounts[cFacetAssignment] = facetTotCounts[cFacetAssignment] + 1.0;
                        cSum = cSum + 1.0;
                    }
                }
            } else {
                // TODO: Wordnet has to fit in here. 
                int dummy = 1;
            }
            // normalized factor;
            for (int j = 0; j < mystates; j++) {
                double val = ((double) facetTotCounts[j]) * 1.0 / ((double) cSum);
                //    fprintf(stderr, "Assigning state %d P = %g\n", j, val );
                (*vFactor)[j] = val;
            }
            cVec->push_back(vFactor);
            // cout<< *vFactor << "P: "<< vFactor->p()<<  endl ;
            // cin.get();
        }
        wordFactors.insert(make_pair<string, vector<Factor*> *>(word, cVec));
    }
}

// actually does a top-level compute

void Engine::solveForQuery(const vector<string>* words, const vector<int>* freq) {

    assert((freq == NULL) || (words->size() == freq->size()));
    assert(words->size() > 0);
    typedef map<string, vector<Query* >* > SqMp;
    typedef SqMp::iterator SqMpIter;
    typedef map<string, vector<Factor* >* > WfMp;
    typedef WfMp::iterator WfMpIter; 

    // CLEAR THE PAST 
     if(sFactors == NULL) {
        sFactors = new vector<Factor>();
    } else {
        sFactors->clear();
    }

    if(sGraph != NULL) {
        FactorGraph* tGraph = sGraph;
        sGraph = NULL;
        delete(tGraph);
    }
    // make the factors
    for (int wi = 0; wi < words->size(); wi++) {
        string cword = words->at(wi);
        int cfreq = ((freq == NULL)?(1):(freq->at(wi)));
        SqMpIter mIter = wordToQueryMp.find(cword);
        vector<Query*>* pVecQ;
        if(mIter == wordToQueryMp.end())
            pVecQ = NULL;
        else
            pVecQ = mIter->second;
        makeFactorForWord(cword, pVecQ);
    }
    // start making the graph
   
    // get the Chow Liu factors;
    if(!clFactorsFlag)
        this->makeChowLiuFactors();

    FOREACH(iter, this->clFactors) {
        Factor* ofPtr = (*iter);
        Factor newFactor(ofPtr->vars(), ofPtr->p());
        sFactors->push_back(newFactor);
    }

    FOREACH(iter, *words) {
        string cword = *iter; 
        WfMpIter wIter = wordFactors.find(cword);
        assert(wIter != wordFactors.end());
        vector<Factor*> *vFactor = wIter->second;
        for(int i=0 ; i < numAttrs; i++) {
            Factor* pcFact = vFactor->at(i);
            Factor newFactor(pcFact->vars(), pcFact->p() );
            sFactors->push_back(newFactor);
        }
    }
    sGraph = new FactorGraph(*sFactors) ;
    ofstream dotStream;
    dotStream.open("sample.dot");
    sGraph->printDot(dotStream);
    sGraph->WriteToFile("sample.fg");
    
    PropertySet opts;

    size_t maxiter = 1000;
    Real tol = 1e-6;
    size_t verb = 1;


    opts.Set("maxiter", maxiter);
    opts.Set("tol", tol);
    opts.Set("verbose", verb);


    BP bpEngine(*sGraph, opts("updates",string("SEQFIX"))("inference",string("SUMPROD"))("logdomain",false));
    bpEngine.init();
    bpEngine.run();
}

void Engine::testSolver() {
  readArff("data/classifieds.txt");
  readQueries("data/Matrix");
  computeChowLiu();
  makeFacetVars();
  makeChowLiuFactors();
  makeQueryFactors();
  char choice;
  int numQ = (int) queries.size();
  do {
        Query* sample = queries[rand()%numQ];

        vector<string> words = sample->getWords();
        cout<<" Query: "<<getStr<string>(words)<<endl;
        solveForQuery(&words);
        cin.get(choice);

  } while( (choice == 'y' ) || (choice == 'Y'));
}
//
// engine.cpp ends here



