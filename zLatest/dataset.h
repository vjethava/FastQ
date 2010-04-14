/* dataset.h ---
 *
 * Filename: dataset.h
 * Description: This header defines the dataset requirements for the interface
 * Author: Vinay Jethava
 * Created: Thu Apr  8 13:28:14 2010 (+0200)
 * Last-Updated: Sat Apr 10 16:55:28 2010 (+0200)
 *           By: Vinay Jethava
 *     Update #: 79
 * URL:
 * Keywords:
 */

/* Code: */

#ifndef DATASET_H
#define DATASET_H


#include "common.h"
#include <dai/var.h>
#include <dai/daialg.h>
#include<dai/factor.h>
#include <dai/factorgraph.h>
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
using namespace std;
using namespace dai;

#define matrix boost::numeric::ublas::matrix
class Engine;

class MyVar {
    string name;
    VarType type;
    Var* var;
    string* values;
public:

    string getName() {
        return name;
    }

    MyVar(string name, VarType varType, int label, int states) {
        this->name = name;
        this->var = new Var(label, states);
        this->type = varType;
    }

};

class MyFactor {
    string name;
    FactorType type;
    Factor* factor;
public:

    MyFactor(string _name, FactorType _type) {
        name = _name;
        type = _type;
    }

    void setFactor(Factor* factor) {
        this->factor = factor;
    }

    Factor* getFactor() {
        return factor;
    }
};

class Query {

public:
    map<string, int>* words;
    int id;
    Engine* engine;
    int count; 

public:
    Query(int _id, vector<string> _words, vector<int> _freq, Engine* engine = NULL) {
        this->words = new map<string, int>();
        assert(_words.size() > 0); 
        assert(_words.size() == _freq.size());
        int len  = (int) _words.size();
        for (int i = 0; i < len ; i++) {
            this->words->insert(make_pair<string, int>(_words[i], _freq[i]));
        }
        this->id = _id;
        this->engine = engine;
        count = -1; 
    }

    vector<string> getWords() const {
        vector<string> result;
        FOREACH(iter, *words) {
            result.push_back(iter->first);
        }
        return result;
    }
    
    string getStr() {
        stringstream ss("");
        ss << "Q: {"<<id<<": ";
        FOREACH( it, *words) {
            string w = it->first;
            int freq = it->second;
            ss<<w<<" "<<freq<<" ";
        }
        ss<<"}";
        return ss.str();
    }
};

enum CLFactorType {MLE=0, MAP, OTHER};

class ChowLiu {
    matrix<int>* instances;
    int numAttrs;
    vector<int> attrSizes;
    
    vector<double> mutualInfo;


public: /* Data for factors is here - handle with care */

    /// index of the c-l tree factors
    int* j0; // [numAttrs - 1 ]
    /// the degree of the attributes in the c-l tree
    int* s0; // [numAttrs]
    /// the probability information
    vector<matrix<double>* > mutualCounts;

public:
    void computeMutualCounts();
    void computeMutualInfo();
    void findBestFactors();

    ChowLiu(matrix<int>* _instances, vector<int>* _attrSizes = NULL) {
        this->instances = _instances;
        // This section has to be modified
        if (_attrSizes == NULL) { // original code - use from common.cpp
            numAttrs = numAttributes;
            for (int i = 0; i < numAttrs; i++) {
                attrSizes.push_back(attrsNumVals[i]);
            }

        } else {
            numAttrs = _attrSizes->size();
            attrSizes.insert(attrSizes.begin(), _attrSizes->begin(), _attrSizes->end());
        }
    }
    ~ChowLiu() {
        free(j0);
        free(s0);
    }
    inline pair<int, int> getXY(int idx) {
        return make_pair<int, int>(idx/numAttrs , idx%numAttrs);
//        pair<int, int> result;
//        result.first = idx / numAttrs;
//        result.second = idx % numAttrs;
//        return result;
    }

    inline int getI(int x, int y) {
        // if( x > y) return getI(y, x);
        return (x * numAttrs + y);
    }


private:

    double getMutualInfo(int i, int j);



};
#endif

/* dataset.h ends here */
