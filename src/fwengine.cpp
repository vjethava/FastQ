// engine.cpp ---
//
// Filename: engine.cpp
// Description:
// Author: Vinay Jethava
// Created: Thu Apr  8 15:14:31 2010 (+0200)
// Last-Updated: Wed Apr 14 22:05:23 2010 (+0200)
//           By: Vinay Jethava
//     Update #: 93
// URL:
// Keywords:
//

// Code:


#include "fwengine.h"
#include "common.h"
#include "dataset.h"
#include <sstream>
#include <iostream>
#include <fstream>

#include <dai/varset.h>
#include <dai/exceptions.h>
#include <dai/bp.h>
#include <dai/properties.h>
#include <exception>
#include <dai/smallset.h>
#include "dataset.h"
using namespace std;
const int MAX_DEPTH = 5;
// #define DEBUG_INF
// #define ONLY_CL
// #define ONLY_WORDS

// Read from common.cpp
extern int numAttributes;
extern int attrNumVals[];
extern string* attributes[];
extern string attrNames[];

FwEngine::FwEngine() {

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
    sGraph = NULL;
    sFactors = NULL;
    solution = new vector<Factor > ();
    // FIXME: initialization using params defined in wnconnector.cpp
    wnc = new WnConnector();
    maxBfsDepth = 3;
    levelFactor = 0.5;
    missingWN = new vector<string > ();
    missingDB = new vector<string > ();
    foundMp = new map<string, int>();
    wnMp = new map<string, double>();

    pTestSolver = 1.0;
    unseenWords = 0;

    onlyCL = false;
    onlyWords = false;
    useWN = true;
}
// TODO: Take into account queryWord() result status

int FwEngine::updateWnMp(int level, string word, bool update) {
    stringstream ss("");
    ss << word << " " << level;
    //SHOWFUNC(ss.str().c_str());
    typedef map<string, double> MpSD;
    if ((level == 0) || (update == true)) // looking at a top level word
        wnMp->clear();
    double clFactor = pow(levelFactor, (double) level);
    // printf("\tclFactor: %.2f\n", clFactor);
    if (level < maxBfsDepth) {
        /// query WN for neighbours of current word
        bool qres = wnc->queryWord(word);
        if (qres) {
            map<string, double>* cwordMp = wnc->wnMp;
            vector<string> newWordsInLvl;
            for (map<string, double>::iterator iterMp = cwordMp->begin();
                    iterMp != cwordMp->end();
                    ++iterMp) { // look at the neighbours of current word in WN
                string cw = iterMp->first;
                double cd = iterMp->second;
                MpSD::iterator wIter = wnMp->find(cw);
                if (wIter == wnMp->end()) { // found a new word
                    double cmult = cd * clFactor;
                    wnMp->insert(make_pair<string, double>(cw, cmult));
                    newWordsInLvl.push_back(cw);

                    //                    fprintf(stderr, "\tfound %s at level %d\n", cw.c_str(), level);
                }
            }
            //     fprintf(stderr, "Calling for %d new words\n", (int) newWordsInLvl.size());

            for (VS_ITER wIter = newWordsInLvl.begin(); wIter != newWordsInLvl.end(); ++wIter) {
                string nword = *wIter;
                int retval = this->updateWnMp(level + 1, nword, false);
            }
        }
    }
    return 0;
}

string FwEngine::getAttrStr(int attrCount, int indexInAttr, string* name) {
    // FIXME: clear attrNames dependency
    assert((attrCount < numAttrs) && (attrCount >= 0));
    assert((indexInAttr >= 0) && (indexInAttr < attrSizes[attrCount]));
    if (name != NULL)
        *name = attrNames[attrCount];
    return attributes[attrCount][indexInAttr];
}
/// Reads the ARFF file name data part.

vector<vector<int> > FwEngine::readArff(string fileName, string idFileName) {
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
    instances = new Matrix<int>(result.size(), result[0].size());
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

void FwEngine::readQueries(string fileName) {
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
                    S_WORD = 0, S_FREQ
                };
                State state = S_WORD;
                for (int i = 0; i < (int) tokens.size(); i++) {
                    switch (state) {
                        case S_WORD:
                            words.push_back(tokens[i]);
                            break;
                        case S_FREQ:
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
                // FIXME: Hack to handle multiple ids
                query->count = count - 1;
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

void FwEngine::computeChowLiu(Matrix<int> * trInstances) {

    if (trInstances == NULL) {
        trInstances = this->instances;
    }
    cl = new ChowLiu(trInstances);
    cl->computeMutualCounts();
    cl->computeMutualInfo();
    cl->findBestFactors();
    chowLiuFlag = true;
}

/// Initializes the facet variables

void FwEngine::makeFacetVars() {
    for (int i = 0; i < numAttrs; i++) {
        int states = attrSizes[i];
        Var* cVar = new Var(i, (size_t) states);
        facetVars.push_back(cVar);
    }
    facetVarsFlag = true;
}

/// Initializes the chow-liu factorization

void FwEngine::makeChowLiuFactors(CLFactorType clType, double alpha) {
    assert(chowLiuFlag == true);
    assert(facetVarsFlag == true);
    assert(queryReadFlag == true);
    for (int i = 0; i < numAttrs - 1; i++) {
        int idx = cl->j0[i];
        Matrix<double>* cProb = cl->mutualCounts[idx];
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
        //   cerr << *cFactor << endl << (*cProb) << endl << endl;


    }

    /// Make the singleton factors;
    for (int i = 0; i < numAttrs; i++) {
        int cdegree = cl->s0[i];
        if (cdegree != 1) { // introduce a singleton factor.
            int idx = cl->getI(i, i);
            VarSet* cVarSet = new VarSet(*facetVars[i]);
            Factor* cFactor = new Factor(*cVarSet);
            Matrix<double>* cProb = cl->mutualCounts[idx];
            for (int si = 0; si < attrSizes[i]; si++) {
                double p = (*cProb)(si, 0);
                double v = ((p == 0.0) ? 0.0 : pow(p, 1.0 - cdegree));
                (*cFactor)[si] = v;
            }
            cFactor->normalize();
            //    cerr << *cFactor << endl << (*cProb) << endl << endl;


            clFactors.push_back(cFactor);
        }
        // cerr<<attrNames[i]<<" degree: "<<cdegree<<"\n";

    }

    this->clFactorsFlag = true;
}


/// make the factors corresponding to the queries

void FwEngine::makeQueryFactors(map<string, vector<Query*>* >* ptrWordQueryMp) {

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

void FwEngine::makeFactorForWord(const string& word, const vector<Query*>* vq) {

    typedef vector<Query*>::const_iterator VqIter;
    map<string, vector<Factor*>* >::iterator sfvIter = wordFactors.find(word);
    if (sfvIter == wordFactors.end()) { // introduce a new factors
        FPRINTF(stderr, "makeFactorForWord(%s)\n ", word.c_str());
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

            
            bool wordInDB = false;
            bool wordInWN = true;
            
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
            } else if (i == 0) {
                unseenWords++;
                if ((onlyCL == false) && (useWN)) {
                    // TODO: Wordnet has to fit in here.
                    // printf("Encountered new word %s, calling wordnet\n", word.c_str());
                    maxBfsDepth = 0; // note this is dynamically changing based on search requirement
                    while ((maxBfsDepth < MAX_DEPTH) && (!wordInDB) && (wordInWN)) {
                        maxBfsDepth = maxBfsDepth + 1;
                        int level = 0;
                        bool update = true;
                        // call to wordnet
                        updateWnMp(level, word, update);
                        if (wnMp->empty())
                            wordInWN = false;
                        else {
                            stringstream ss("");
                            ss << "found " << wnMp->size() << " words for " << word << " max depth " << maxBfsDepth;
                            //SHOWFUNC(ss.str().c_str());

                            FOREACH(itMp, (*wnMp)) {
                                string cw = itMp->first;
                                double cmult = itMp->second;
                                MSVpFp::iterator wIt = wordFactors.find(cw);
                                if (wIt != wordFactors.end()) { // note this should only update the factors once.
                                    wordInDB = true;
                                    FPRINTF(stderr, "\t%s %.2f\n", cw.c_str(), cmult);

                                    FOREACH(fpIt, *(wIt->second)) {
                                        Factor* cfactor = *fpIt;
                                        Factor* wnfactor = new Factor(cfactor->vars(), cfactor->p());
                                        //// FIXME: Hack to amplify effect of WN
                                        (*wnfactor) *= (1.0 * cmult);
                                        cVec->push_back(wnfactor);
                                    }
                                }
                            }
                        }
                    }
                    if (wordInWN == false) { // word not found in
                        missingWN->push_back(word);
                    } else if (wordInDB == false) {
                        missingDB->push_back(word);
                    }
                }
            }
            if ((vq != NULL) || (wordInDB == false)) {
                // normalized factor;
                for (int j = 0; j < mystates; j++) {
                    //// FIXME: not normalizing word count probs

                    double val = ((double) facetTotCounts[j]) * 1.0; // / ((double) cSum);
                    //    fprintf(stderr, "Assigning state %d P = %g\n", j, val );
                    if(cSum > 0.0)
                        (*vFactor)[j] = val + 1e-6;
                    else
                        (*vFactor)[j] = 1.0;
                }
                cVec->push_back(vFactor);
            }
            // cout<< *vFactor << "P: "<< vFactor->p()<<  endl ;
            // cin.get();
        }
        wordFactors.insert(make_pair<string, vector<Factor*> *>(word, cVec));
    }
}

// actually does a top-level compute

vector<size_t> FwEngine::solveForQuery(const vector<string>* words, const vector<int>* freq, map<string, vector<Query* > * >* sqMpPtr) {
    if (sqMpPtr == NULL) {
        sqMpPtr = &wordToQueryMp;
    }

    assert((freq == NULL) || (words->size() == freq->size()));
    assert(words->size() > 0);
    typedef map<string, vector<Query* >* > SqMp;
    typedef SqMp::iterator SqMpIter;
    typedef map<string, vector<Factor* >* > WfMp;
    typedef WfMp::iterator WfMpIter;

    // CLEAR THE PAST 
    if (sFactors == NULL) {
        sFactors = new vector<Factor > ();
    } else {
        sFactors->clear();
    }

    if (sGraph != NULL) {
        FactorGraph* tGraph = sGraph;
        sGraph = NULL;
        delete(tGraph);
    }
    // make the factors
    for (int wi = 0; wi < words->size(); wi++) {
        string cword = words->at(wi);
        int cfreq = ((freq == NULL) ? (1) : (freq->at(wi)));
        SqMpIter mIter = sqMpPtr->find(cword);
        vector<Query*>* pVecQ;
        if (mIter == sqMpPtr->end())
            pVecQ = NULL;
        else
            pVecQ = mIter->second;
        makeFactorForWord(cword, pVecQ);
    }

    // start making the graph

    // get the Chow Liu factors;
    if (!clFactorsFlag)
        /// FIXME: Hack to try out MAP ChowLiu Factors
        this->makeChowLiuFactors(MLE);

    FOREACH(iter, this->clFactors) {
        Factor* ofPtr = (*iter);
        Factor newFactor(ofPtr->vars(), ofPtr->p());
        // FIXME: Hack to investigate only words
        if (onlyWords == false) {
            sFactors->push_back(newFactor);
        }
    }
    FPRINTF(stderr, "solveForQuery(): Post add-CL factorSize: %d\n", (int) sFactors->size());
    int wi = 0;

    FOREACH(iter, *words) {
        string cword = *iter;
        WfMpIter wIter = wordFactors.find(cword);
        assert(wIter != wordFactors.end());
        int cfreq = ((freq == NULL) ? (1) : (freq->at(wi)));
        for (int jj = 0; jj < cfreq; jj++) {
            // handling multiple occurrences of word in query
            vector<Factor*> *vFactor = wIter->second;
            for (int i = 0; i < numAttrs; i++) {
                Factor* pcFact = vFactor->at(i);
                Factor newFactor(pcFact->vars(), pcFact->p());
                //// FIXME: Hack to amplify effect of words
                //               newFactor *= 10.0;
                //// FIXME: Hack to investigate only CL factor graph
                if (onlyCL == false) {
                    sFactors->push_back(newFactor);
                }
            }
        }
        FPRINTF(stderr, "solveForQuery() post Word-add factor size: %d\n", (int) sFactors->size());
        wi++;
    }
    sGraph = new FactorGraph(*sFactors);
    ofstream dotStream;
    dotStream.open("sample.dot");
    sGraph->printDot(dotStream);
    sGraph->WriteToFile("sample.fg");

    PropertySet opts;

    size_t maxiter = 1000;
    Real tol = 1e-6;
    size_t verb = 0;
    opts.Set("maxiter", maxiter);
    opts.Set("tol", tol);
    opts.Set("verbose", verb);
    BP bpEngine(*sGraph, opts("updates", string("SEQFIX"))("inference", string("MAXPROD"))("logdomain", false));
    bpEngine.init();
    bpEngine.run();

    BP bp2(*sGraph, opts("updates", string("SEQFIX"))("inference", string("SUMPROD"))("logdomain", false));
    bp2.init();
    bp2.run();
    //        for (size_t i = 0; i < sGraph->nrVars(); i++) {
    //            //  cout<< "var: "<< attrNames[(int) i]<< " size: " << attrSizes[i]<<endl;
    //            cout<< bp2.belief(sGraph->var(i))<<endl;
    //        }
    solution->clear();
    return bpEngine.findMaximum();
}

vector<vector<size_t> > FwEngine::getQueriesMAP(const vector<vector<string>* >& wordVec,
        const vector<vector<int>* >& freqVec) {

    typedef vector<vector<size_t> > VVSz;
    typedef vector<vector<string>* > VVpStr;
    typedef vector<vector<int>*> VVpInt;

    assert(wordVec.size() > 0);
    assert((freqVec.size() == 0) || (freqVec.size() == wordVec.size()));

    VVSz result;

    int num = wordVec.size();
    for (int i = 0; i < num; i++) {
        vector<int>* cf = NULL;
        if (freqVec.size() > 0)
            cf = freqVec[i];
        vector<string>* cw = wordVec[i];
        vector<size_t> cres = solveForQuery(cw, cf);
        result.push_back(cres);
    }
    return result;
}

/**
 * Writes out the results in selected directory for direct use in matlab
 */
int FwEngine::writeRes(vector<vector<size_t> >& output, vector<Query*>& samples, string dirName) {
    srand(time(NULL));
    int expNum = rand() % 1000;
    int numT = samples.size();
    // result = 1 => correctly classified
    vector<int> result;
    // hamming distance between output/true
    vector<int> hamming;
    stringstream dirCmd(""), qName(""), oName(""), tName(""), aName(""), hName("");
    ofstream rqQuery, rqOut, rqTrue, rqAcc, rqHamming, rqStats;

    dirCmd << "mkdir -p " << dirName << "/" << expNum;
    system(dirCmd.str().c_str());
    ifstream checker; // checks whether to write in header
    checker.open("runs.log");
    if (checker.is_open() == false) { // create a new file

        rqStats.open("runs.log", ios::out);
        rqStats << "date time tr% acc wn acc1 acc2 acc3 acc4 acc5 acc6 acc7 acc8 acc9";
        rqStats << " depth unseen missWN missDB";
        rqStats << " onlyCL onlyWords expNum lvlFactor";
        rqStats << "\n";
        rqStats.close();
    } else {
        checker.close();
    }

    rqStats.open("runs.log", ios::app);
    qName << dirName << "/" << expNum << "/" << "query.txt";
    rqQuery.open(qName.str().c_str());

    oName << dirName << "/" << expNum << "/" << "out.txt";
    rqOut.open(oName.str().c_str());

    tName << dirName << "/" << expNum << "/" << "true.txt";
    rqTrue.open(tName.str().c_str());

    aName << dirName << "/" << expNum << "/" << "accuracy.txt";
    rqAcc.open(aName.str().c_str());

    hName << dirName << "/" << expNum << "/" << "hamming.txt";
    rqHamming.open(hName.str().c_str());

    // compute the statistics
    double overallCorrect = 0.0;
    int accV[numAttrs];
    double sumV[numAttrs];
    for (int i = 0; i < numAttrs; i++) {
        sumV[i] = 0.0;
    }
    for (int i = 0; i < output.size(); i++) {
        rqQuery << samples[i]->getStr() << "\n";
        bool correct = true;
        int idx = 0;
        int hammingCount = 0;
        // compare attribute by attribute
        FPRINTF(stderr, "testSolver() i: %d\n", i);
        while (idx < numAttrs) {

            int xOut = (int) output[i][idx];
            int xTrue = (int) getQueryAttribute(samples[i], idx);
            // XXX: Modification to write indices instead of names
            // rqOut << getAttrStr(idx, xOut) << "\t";
            //rqTrue << getAttrStr(idx, xTrue) << "\t";
            rqOut << xOut << "\t";
            rqTrue << xTrue << "\t";
            if (xOut != xTrue) {
                hammingCount++;
                correct = false;
                accV[idx] = (0);
            } else {
                accV[idx] = (1);
            }

            sumV[idx] = sumV[idx] + accV[idx];
            idx++;
        }
        overallCorrect += (int) correct;
        rqOut << "\n";
        rqTrue << "\n";
        rqAcc << ((int) correct);
        for (int i1 = 0; i1 < numAttrs; i1++) {
            rqAcc << " " << accV[i1];
        }
        rqAcc << "\n";
        rqHamming << hammingCount << "\n";
    }
    rqStats << getTimeStr() << " ";
    rqStats << pTestSolver << " ";

    double cAcc = ((double) overallCorrect) / ((double) numT) * 100.0;
    fprintf(stderr, "Accuracy: %g [", cAcc);
    rqStats << cAcc << " ";

    rqStats << useWN << " ";
    for (int i1 = 0; i1 < numAttrs; i1++) {
        double cAcc1 = ((double) sumV[i1]) * 1.0 / ((double) numT)*100.0;
        rqStats << cAcc1 << " ";
        fprintf(stderr, " %g ", cAcc1);
    }
    fprintf(stderr, "] ");
    if(useWN) {
        fprintf(stderr, "wn: %d of %d\n", (int) missingWN->size(), unseenWords); 
    } else
        fprintf(stderr, "\n");
    rqStats << MAX_DEPTH << " ";
    rqStats << unseenWords << " ";

    rqStats << missingWN->size() << " ";
    rqStats << missingDB->size() << " ";
    rqStats << onlyCL << " ";
    rqStats << onlyWords << " ";
    rqStats << expNum << " ";
    rqStats << levelFactor << " ";
    rqStats << "\n";
    rqTrue.close();
    rqAcc.close();
    rqHamming.close();
    rqOut.close();
    rqQuery.close();
    rqStats.close();
    fprintf(stderr, "FwEngine::writeRes() finished for %d test queries\n", numT);
    return expNum;
}

/**
 * Runs tests using a portion of the data for training
 *
 * @arg p  fraction of data to use for training
 *
 */
int FwEngine::testSolver(double p, bool useWN, bool onlyCL, bool onlyWords, int num) {

    this->useWN = useWN;
    this->onlyCL = onlyCL;
    this->onlyWords = onlyWords;
    pTestSolver = p;
    stringstream ss("");
    ss << "p=" << p <<" WN="<<useWN << " onlyCL="<<onlyCL<< " onlyWords="<< onlyWords <<" N="<<num;
    SHOWFUNC(ss.str().c_str());
    // TODO: Incomplete method for partial tr-tst of data
    srand(time(NULL));
    init("data/classifieds.txt", "data/Matrix");
    int N = queries.size();
    typedef vector<Query* > VQ;
    typedef vector<Query* >* VQP;
    typedef pair<VQP, VQP > PrVQP;
    PrVQP prVqp = getRndSelection< Query* >(& this->queries, p);
    VQP trQ = prVqp.first;
    VQP tstQ = prVqp.second;
    if(p == 1.0) {
        assert(num > 0);
        tstQ = trQ; 
    }
    MSVpQp * trWordQueryMpPtr = train(trQ);
    // SHOWFUNC("Finished training\n");
    vector<vector<string> * > wv;
    vector<vector<int> *> fv;
    vector<vector<size_t> > output;
    int testCount = 0;
    int testSize = tstQ->size();
    Query* sample;
    //    fprintf(stderr, "testSolver: num = %d\n", num);
    if (num == 0) {
        FOREACH(iterQ, *tstQ) {
            stringstream ss("");
          //  ss << "test query " << testCount++ << " of " << testSize;
            sample = (*iterQ);
            vector<string>* cw = new vector<string > (sample->getWords());
            if (cw->size() == 0) {
                cerr << sample->id << "\n";
                cin.get();
            } else {
                const char* cshow = (getStr<string > (*cw)).c_str();
                FPRINTF(stderr, "%d: %s\n", sample->id, cshow);
                vector<size_t> cres = solveForQuery(cw, NULL, trWordQueryMpPtr);
                output.push_back(cres);
            }
        }
        return writeRes(output, *tstQ, "result");
    } else {
        assert(num > 0);
        vector<Query*>* res11 = new vector<Query*>();
        int nTest = tstQ->size();
        num = (int) min((int) nTest, num); 
        for (int i = 0; i < num; i++) {
            sample = (*tstQ)[rand()%nTest];
            stringstream ss("");
            ss << "test query " << testCount++ << " of " << testSize;
          //  SHOWFUNC(ss.str().c_str());
            
            vector<string>* cw = new vector<string > (sample->getWords());
            if (cw->size() == 0) {
                cerr << sample->id << "\n";
                cin.get();
            } else {
                const char* cshow = (getStr<string > (*cw)).c_str();
                FPRINTF(stderr, "%d: %s\n", sample->id, cshow);
                vector<size_t> cres = solveForQuery(cw, NULL, trWordQueryMpPtr);
                output.push_back(cres);
            }
            res11->push_back(sample);
        }
        return writeRes(output, *res11, "result");

    }
}

Matrix<int>* FwEngine::getInstances(vector<Query* > * cqueries) {
    int n1 = cqueries->size();
    int n2 = numAttributes;
    Matrix<int> * result = new Matrix<int>(n1, n2);
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            int cattr = getQueryAttribute(cqueries->at(i), j);
            result->insert_element(i, j, cattr);
        }
    }
    return result;
}

map<string, vector<Query*>* >* FwEngine::getWordMpForQueries(vector<Query*>* trQ) {
    typedef map<string, vector<Query* > * > MSVQP;
    typedef vector<Query*> VQ;
    MSVQP* res = new MSVQP();

    FOREACH(iterQ, *trQ) {
        vector<string> words = (*iterQ)->getWords();
        for (int i = 0; i < (int) words.size(); i++) {
            typedef map<string, vector<Query*>* > SqMp;
            typedef map<string, vector<Query*>* >::iterator SqMpIter;
            SqMpIter mIt = res->find(words[i]);
            if (mIt == res->end()) {
                vector<Query*>* ptrQryVec = new vector<Query*>();
                res->insert(
                        make_pair<string, vector<Query*>* >(words[i],
                        ptrQryVec));
                FPRINTF(stderr, "adding %s for query %d\n", words[i].c_str(), (*iterQ)->id);
            }
            mIt = res->find(words[i]);
            mIt->second->push_back(*iterQ);
        }
    }
    return res;
}

void FwEngine::init(string arffFile, string queryFile) {
    SHOWFUNC("reads arff and query files");
    srand(time(NULL));
    readArff(arffFile);
    readQueries(queryFile);
    fprintf(stderr, "FwEngine::init() read arff and query files\n");
}

map<string, vector<Query* > * >* FwEngine::train(vector<Query* > * trQ) {

    typedef Matrix<int> MI;
    MI* trI = getInstances(trQ);
    this->computeChowLiu(trI);
    makeFacetVars();
    map<string, vector<Query*> * >* cMp = getWordMpForQueries(trQ);
    makeQueryFactors(cMp);
    // fprintf(stderr, "FwEngine::train() trained on %d queries\n", (int) trQ->size());
    return cMp;
}


//// DEPRECATED: see testSolver1()
//void FwEngine::testSolver() {
//    srand(time(NULL));
//    readArff("data/classifieds.txt");
//    readQueries("data/Matrix");
//    computeChowLiu();
//    makeFacetVars();
//    makeChowLiuFactors();
//    makeQueryFactors();
//    char choice;
//    int numQ = (int) queries.size();
//    int numT = 500;
//    int counter = 0;
//    // FIXME: DEBUG_INF allows manual querying
//#ifdef DEBUG_INF
//    do {
//        Query* sample = queries[rand() % numQ];
//        stringstream ro(""), rt(""), ra(""), rh("");
//        vector<string> words = sample->getWords();
//        cout << "\nQuery: " << getStr<string > (words) << endl;
//        vector<size_t> res = solveForQuery(&words);
//        cout<< "\n"<<getStr<size_t>(res)<<endl;
//        // cout<<"\n Enter choice (y/n): ";
//        int hammingCount = 0;
//        bool correct = true;
//        for(int idx = 0; idx < numAttrs ; idx++) {
//            int xOut =  (int) res[idx];
//            int xTrue = (int) getQueryAttribute(sample, idx);
//            ro<< getAttrStr(idx, xOut)<<"\t";
//            rt<< getAttrStr(idx, xTrue)<<"\t";
//            if(xOut != xTrue) {
//                hammingCount++;
//                correct = false;
//            }
//        }
//
//        ro << "\n";
//        rt << "\n";
//        ra << correct<<"\n";
//        rh << hammingCount<<"\n";
//        cout<<endl <<ro.str()<< rt.str()<< "accuracy: " << ra.str()<< "hamming: "<< rh.str();
//        getchar();
//        counter++;
//    } while(counter < numT );
//    // while ((choice == 'y') || (choice == 'Y'));
//#else
//    vector<vector<string> * > wv;
//    vector<vector<int> *> fv;
//    vector<Query*> samples;
//
//    for (int i = 0; i < numT; i++) {
//        Query* sample = queries[rand() % numQ];
//        vector<string>* cw = new vector<string>(sample->getWords());
//        wv.push_back(cw);
//        samples.push_back(sample);
//    }
//
//    vector<vector<size_t> > output = getQueriesMAP(wv, fv);
//
//    // result = 1 => correctly classified
//    vector<int> result;
//    // hamming distance between output/true
//    vector<int> hamming;
//
//    ofstream rqQuery, rqOut, rqTrue, rqAcc, rqHamming;
//    rqQuery.open("res/query.txt");
//    rqOut.open("res/out.txt");
//    rqTrue.open("res/true.txt");
//    rqAcc.open("res/accuracy.txt");
//    rqHamming.open("res/hamming.txt");
//    // compute the statistics
//    double overallCorrect = 0.0;
//    int accV[numAttrs];
//    double sumV[numAttrs];
//    for(int i=0; i < numAttrs; i++) {
//        sumV[i] = 0.0;
//    }
//    for(int i=0; i < output.size(); i++) {
//        rqQuery<<samples[i]->getStr()<<"\n";
//        bool correct = true;
//        int idx = 0;
//        int hammingCount = 0;
//        // compare attribute by attribute
//        FPRINTF(stderr, "testSolver() i: %d\n", i);
//        while(idx < numAttrs) {
//
//            int xOut =  (int) output[i][idx];
//            int xTrue = (int) getQueryAttribute(samples[i], idx);
//            rqOut<< getAttrStr(idx, xOut)<<"\t";
//            rqTrue<< getAttrStr(idx, xTrue)<<"\t";
//            if(xOut != xTrue) {
//                hammingCount++;
//                correct = false;
//                accV[idx] = (0);
//            } else {
//                accV[idx] = (1);
//            }
//
//            sumV[idx] = sumV[idx] + accV[idx];
//            idx++;
//        }
//        overallCorrect += (int) correct;
//        rqOut<<"\n";
//        rqTrue<<"\n";
//        rqAcc<<((int) correct);
//        for(int i1=0; i1 < numAttrs; i1++) {
//            rqAcc<<" "<<accV[i1];
//        }
//        rqAcc<<"\n";
//        rqHamming<<hammingCount<<"\n";
//    }
//
//    double cAcc = ((double) overallCorrect) / ((double) numT) * 100.0;
//    fprintf(stderr, "Accuracy: %g [", cAcc);
//    for (int i1 = 0; i1 < numAttrs; i1++) {
//        double cAcc1 = ((double) sumV[i1]) * 1.0 / ((double) numT)*100.0;
//        fprintf(stderr, " %g ", cAcc1);
//    }
//    fprintf(stderr, "]\n");
//
//
//    rqTrue.close();
//    rqAcc.close();
//    rqHamming.close();
//    rqOut.close();
//    rqQuery.close();
//    FPRINTF(stderr, "Hi There! finished querying.\n");
//#endif
//}
//
// engine.cpp ends here



