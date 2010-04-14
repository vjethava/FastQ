/* common.h ---
 *
 * Filename: common.h
 * Description:
 * Author: Vinay Jethava
 * Created: Thu Apr  8 13:29:49 2010 (+0200)
 * Last-Updated: Thu Apr  8 16:39:27 2010 (+0200)
 *           By: Vinay Jethava
 *     Update #: 37
 * URL:
 * Keywords:
 */

/* Code: */
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cstdio>

using namespace std;

// loop macros
#define fi(a, b) for(int i=a; i < b; i++)
#define fj(a, b) for(int j=a; j < b; j++)
#define fk(a, b) for(int k=a; k < b; k++)
#define FOREACH(it,c) for(typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
// print error messages or not

#ifdef DEBUG
#define PRINTF printf
#define FPRINTF fprintf
#define ASSERT assert
#else
#define PRINTF //
#define FPRINTF //
#define ASSERT //
#endif

#ifdef VERBOSE
#define VPRINTF printf
#define VFPRINTF fprintf
#define VASSERT assert
#else
#define VPRINTF //
#define VFPRINTF //
#define VASSERT //
#endif

// what are the int, float, string
typedef double DOUBLE;
typedef int INT;
typedef string STRING;
typedef double FLOAT;

// macros definitions to ease coding
typedef vector<STRING > VS;
typedef vector<INT > VI;
typedef vector<DOUBLE > VD;

typedef vector<vector<STRING> > VVS;
typedef vector<vector<DOUBLE> > VVD;
typedef vector<vector<INT> > VVI;
typedef vector<DOUBLE>::iterator VD_ITER;
typedef vector<STRING>::iterator VS_ITER;
typedef vector<INT>::iterator VI_ITER;



template<typename T> string getStr(vector<T> v) {
    stringstream ss("");
    ss << "{";
    for (typeof ((v).begin()) iter = (v).begin(); iter != (v).end(); ++iter) {
        ss << " " << (*iter);
    }
    ss << " }";
    return ss.str();
}

/// splits the string based on delimiters
vector<string> split(string ip, char delim);
/// strips beginning and trailing white spaces
string trim(string str);

/// pair comparator

template <typename T, typename C> struct PRCMP {
    bool sortAscending;

    PRCMP(bool sortAscending = true) {
        this->sortAscending = sortAscending;
    }

    bool operator() (const pair<T, C>& p1, const pair<T, C>& p2) const {
        if (sortAscending) {
            if (p1.first == p2.first) return p1.second < p2.second;
            else return p1.first < p2.first;
        } else {
            if (p1.first == p2.first) return p1.second > p2.second;
            else return p1.first > p2.first;
        }
    }
};


/// vector comparator - lexical

template <typename T> struct VCMP {

    bool operator ()(const vector<T>& v1, const vector<T>& v2) const {
        if (v1.size() != v2.size())
            return (v1.size() < v2.size());
        else
            for (int i = 0; i < v1.size(); i++) {
                if (v1[i] != v2[i])
                    return v1[i] < v2[i];
            }
        return false;
    }
};

/// timer class that ticks as required

class Timer {
public:
    time_t tb;
    time_t tc;
    double totTimeSpent;
    double last;
    int incr;
    FILE* fout;
public:

    Timer(int _incr = 1, FILE* _fout = stderr) {
        tb = time(NULL);
        incr = _incr;
        fout = _fout;
    }

    void tick() {
        tc = time(NULL);
        totTimeSpent = difftime(tc, tb);
        double elapsed = totTimeSpent - last;
        if (elapsed - last > incr) {
            last += incr;
            fprintf(fout, ".");
        }
    }
};

enum VarType {
    FACET, WORD
};

enum FactorType {
    QUERY, INSTANCE
};

extern int numAttributes;
extern string specificity[]; // = {"medium", "specific", "broad"};
extern string task[]; // =  {"inf", "notinf", "both"};
extern string objective[]; // = {"resource", "action"};
extern string authosen[]; // = {"yes", "no"};
extern string spatialsen[]; // = {"yes", "no"};
extern string timesen[]; // = {"yes", "no"};
extern string polywords[]; // =  {"yes", "no"};
extern string genre[]; // = {"reference","news","business","community"};
extern string topic[]; // = {"Adults-Sex", "Arts-Culture", "Finance",
//   "Cars-Transportation", "Computers-Internet", "Education",
//   "Entertainment-Music-Games", "Health", "Home-Garden",
//   "Politics-Government", "Religion-Belief-Systems",
//   "Science-Mathematics", "Social-Science", "Sports",
//   "Technology-Electronic", "Travel", "Beauty-Style",
//   "Industrial-Goods-Services", "Undefined", "Work", "Food-Drink",
//  "Charity", "Person"};
extern int attrsNumVals[]; // = {3, 3, 2, 2, 2, 2, 2, 4, 23};
extern string* attributes[];
//= {specificity, task, objective, authosen, 
//   spatialsen, timesen, polywords, genre, topic};

void tokenize(const string& str, vector<string>& tokens,
        const string& delimiters = " ");


#endif
/* common.h ends here */
