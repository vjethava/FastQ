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
#include "engine.h"
#include "wnconnector.h"
#include <cstdio>
#include <string>

int main() {
  srand(time(NULL));
  double p = 0.5;
  Engine engine;
//  engine.testSolver1(p);
  WnConnector wnc(NULL);
  wnc.queryWord("papa");
  return 0;
}

//
// dataset.cpp ends here
