/**
 * Vinay Jethava (vjethava@gmail.com)
 *
 * Copyright 2010
 *
 */

#include "dataset.h"
#include "common.h"
#include <cmath>
#include <cstdio>
#include <string>
#include <dai/util.h>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
using namespace std;
#define log1 std::log

void ChowLiu::computeMutualCounts() {
    for (int i = 0; i < numAttrs; i++) {
        for (int j = 0; j < numAttrs; j++) {
            int idx = getI(i, j);
            int n1 = attrSizes[i];
            int n2 = attrSizes[j];
            Matrix<double>* m;
            if (i == j) {
                m = (Matrix<double> *) new Matrix<double>(n1, 1);
            } else {
                m = (Matrix<double> *) new Matrix<double>(n1, n2);
            }
            for (unsigned int i = 0; i < m->size1(); i++) {
                for (unsigned int j = 0; j < m->size2(); j++) {
                    m->insert_element(i, j, 0.0);
                }
            }

            mutualCounts.push_back(m);
        }
    }
    // increment appropriate places 
    for (int i = 0; i < (int) instances->size1(); i++) {
        for (int j = 0; j < (int) instances->size2(); j++) {
            int idx = getI(j, j);
            int cval = (*instances)(i, j);
            assert((cval >= 0) && (cval < attrSizes[j]));
            double mcurr = (* mutualCounts[idx])(cval, 0);
            mutualCounts[idx]->insert_element(cval, 0, mcurr + 1.0);
            for (int k = j + 1; k < (int) instances->size2(); k++) {
                int i1 = getI(i, j);
                int i2 = getI(i, k);
                int mi = getI(j, k);
                int mi2 = getI(k, j);
                int c1 = (*instances)(i, j);
                int c2 = (*instances)(i, k);
                double val = (*mutualCounts[mi])(c1, c2);
                mutualCounts[mi]->insert_element(c1, c2, val + 1.0);
                mutualCounts[mi2]->insert_element(c2, c1, val + 1.0);
            }
        }
    }
    int N = instances->size1();
    for (int i = 0; i < (int) numAttrs; i++) {
        for (int j = 0; j < (int) numAttrs; j++) {
            int idx = getI(i, j);
            (*mutualCounts[idx]) /= N;
        }
    }

//    for (int i = 0; i < numAttrs; i++) {
//        for (int j = 0; j < numAttrs; j++) {
//            int idx = getI(i, j);
//            Matrix<double>* m = mutualCounts[idx];
//            cerr << *m << endl;
//        }
//    }
}

void ChowLiu::computeMutualInfo() {
    for(int i=0; i < numAttrs; i++) {
        for(int j=0; j < numAttrs; j++) {
            double cm = getMutualInfo(i, j);
            mutualInfo.push_back(cm);
      //    fprintf(stderr, "computeMutualInfo(): i: %d j: %d mi: %g\n", i, j, cm);
        }
    }
}


double ChowLiu::getMutualInfo(int x, int y) {
    double result = 0.0;
    if(x==y) {
        Matrix<double>* p = mutualCounts[getI(x, x)];
        for(int i=0; i < (int) p->size1(); i++) {
            double cp = (*p)(i, 0);
            if(cp > 0.0) {
                double t1 = (double) log1((long double) cp);
                double t2 = - cp * t1; // compute entropy as negative
                result = result + t2;
            }
        }
    } else {
        Matrix<double>* pJoint = mutualCounts[getI(x,y)];
        Matrix<double>* p1 = mutualCounts[getI(x, x)];
        Matrix<double>* p2 = mutualCounts[getI(y, y)];
        for (int i = 0; i < (int) pJoint->size1(); i++) {
            for (int j = 0; j < (int) pJoint->size2(); j++) {
                double jp = (*pJoint)(i, j);
                double sp1 = (*p1)(i, 0);
                double sp2 = (*p2)(j, 0);
                if(jp > 0.0) { // pxy > 0
                    double t1 = jp/(sp1*sp2);
                    double t2 = (double) log1( (long double) t1);
                    double t3 = jp*t2;
                    result = result + t3;
                }
            }
        }
    }
    
    return result; 
}

void ChowLiu::findBestFactors() {

    /**
     * The commented code is the boost equivalent which is not
     * working for some reason.
     */
    
//    using namespace boost;
//    typedef adjacency_list< vecS, vecS, undirectedS,
//            property<vertex_distance_t, int> , property<edge_weight_t, double> > Graph;
//    typedef std::pair<int, int> E;
//
//    vector<pair<double, int> > tv;
////    for(int i=0; i < numAttrs; i++) {
////        for (int j = i+1; j < numAttrs; j++) {
////            int idx = getI(i, j);
////            double val = mutualInfo[idx];
////            pair<double, int> cp = make_pair<double, int>(val, idx);
////            tv.push_back(cp);
////        }
////    }
////    sort(tv.begin(), tv.end(), PRCMP<double, int>(false));
//    Matrix<double> wtMatrix(numAttrs, numAttrs);
//
//    E edges[numAttrs*numAttrs];
//    double weights[numAttrs*numAttrs];
//    for (int i = 0; i < numAttrs; i++) {
//        for(int j=0; j < numAttrs; j++) {
//            int idx = getI(i, j);
//            double wt = mutualInfo[getI(i, j)];
//            wtMatrix(i, j) = wt;
//            edges[idx] = std::pair<int, int>(i, j);
//            weights[idx] =1000.0 - wt;
//        }
//    }
//    int num_nodes = numAttrs;
//    Graph g(edges, edges+sizeof(edges)/sizeof(E), weights, num_nodes);
//    std::vector < graph_traits < Graph >::vertex_descriptor > p(num_vertices(g));
//    prim_minimum_spanning_tree(g, &p[0]);
//    for (std::size_t i = 0; i != p.size(); ++i)
//        if (p[i] != i)
//            std::cout << "parent[" << i << "] = " << p[i] << std::endl;
//        else
//            std::cout << "parent[" << i << "] = no parent" << std::endl;
//  getchar();
    
//    for(vector<pair<double, int> >::iterator it = tv.begin(); it != tv.end(); ++it) {
//        fprintf(stderr, "tv: %d val: %g idx: %d\n", count++, it->first, it->second);
//    }
    
    // Naive Prim implementation
    Matrix<double> wtMatrix(numAttrs, numAttrs);
    set<int> unassigned, assigned;
    vector<pair<int, int> > edges;
    for(int i=0; i < numAttrs; i++) {
        unassigned.insert(i);
        for(int j=0; j < numAttrs; j++) {
            int idx = getI(i, j);
            wtMatrix (i, j) = mutualInfo[idx];
        }
    }

    for(int n=0; n < numAttrs -1 ; n++) {
        if(assigned.size() == 0) {
            int toAssign = rand() % numAttrs;
            unassigned.erase(toAssign);
            assigned.insert(toAssign);
        }
        pair<int, int> cedge(-1, -1);
        double mval = 0.0;
        FOREACH(it1, unassigned) {
            FOREACH(it2, assigned) {
                double cval = wtMatrix(*it1, *it2);
                if(cval > mval) {
                    mval = cval;
                    cedge.first = *it1;
                    cedge.second = *it2;
                }
            }
        }
        assert(cedge.first > -1);
        assert(cedge.second > -1);
   //     fprintf(stderr, "Found edge (%d, %d) wt: %g\n", cedge.first, cedge.second, mval);
        assigned.insert(cedge.first);
        unassigned.erase(cedge.first);
        edges.push_back(cedge);
    }

    int count = 0;
    s0 = new int[numAttrs];
    j0 = new int[numAttrs-1];
    for(int i=0; i < numAttrs-1; i++) {
       s0[i] = 0;
       j0[i] = 0;
    }
    s0[numAttrs - 1] = 0;
    for(int i = 0; i < numAttrs-1; i++) {
     
        pair<int, int> idx = edges[i]; 
        s0[idx.first] = s0[idx.first] + 1 ;
        s0[idx.second] = s0[idx.second] +1 ; 
        j0[i] = getI(idx.first, idx.second);
  //      fprintf(stderr, "CL edge: (%d, %d)\n", idx.first , idx.second);
    }
    
//   for (int i = 0; i < numAttrs; i++) {
//        fprintf(stderr, "i: %d s0: %d\n", i, s0[i]);
//    }
//    getchar();

}

