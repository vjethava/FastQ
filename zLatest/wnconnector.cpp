// wnconnector.cpp ---
//
// Filename: wnconnector.cpp
// Description:
// Author: Vinay Jethava
// Created: Fri Apr 16 14:37:17 2010 (+0200)
// Last-Updated: Fri Apr 16 15:51:56 2010 (+0200)
//           By: Vinay Jethava
//     Update #: 18
// URL:
// Keywords:
//

// Code:
#include "common.h"
#include "wnconnector.h"
#include <mysql/mysql.h>
using namespace std;

const char* SERVER="localhost";
const char* USER="root";
const char* PASSWORD="root";
const char* DB="eswn";


WnConnector::WnConnector(map<string, string>* params) {
    if(params != NULL) {
        assert(params->find("server") != params->end());
        assert(params->find("host") != params->end());
        assert(params->find("user") != params->end());
        assert(params->find("password") != params->end());
        server = params->find("server")->second.c_str();
        user = params->find("user")->second.c_str();
        password = params->find("password")->second.c_str();
        db = params->find("db")->second.c_str();
    } else {
        user = USER;
        server = SERVER;
        password = PASSWORD;
        db = DB;
    }
    wnMp = new map<string, double>();
    cnxn = mysql_init(NULL);
    if(cnxn == NULL) {
        fprintf(stderr,
                "WnConnector::WnConnector() could not connect to db:\n\t%u: %s\n"
                , mysql_errno(cnxn), mysql_error(cnxn));
        exit(-1);
    }
    if(!mysql_real_connect(cnxn, server, user, password, db, 0, NULL, 0)) {
        fprintf(stderr, "WnConnector() %s\n", mysql_error(cnxn));
        exit(-1);
    } 
}

void WnConnector::query(const char* myQuery) {
    if (mysql_query(cnxn, myQuery)) {
        fprintf(stderr, "WnConnector::query(): %s\n", mysql_error(cnxn));
        exit(-1);
    }
    result = mysql_store_result(cnxn);
}

/**
 * Function that queries WN for word synsets
 */
bool WnConnector::queryWord(const string& word) {
    SHOWFUNC(word.c_str());
    wnMp->clear(); // clear for storing current word

    stringstream queryStr("");
    queryStr << "SELECT A.offset,B.offset,B.word, A.relation,A.csco ";
    queryStr << "FROM eswn_variant B, (SELECT E.sourceSynset,F.word,F.offset,E.relation,E.csco ";
    queryStr << "FROM eswn_relation E,eswn_variant F WHERE ";
    queryStr << "E.targetSynset = F.offset AND E.relation <> 'near_antonym' ";
    queryStr << "AND E.relation <> 'has_holo_part' AND F.word = ";
    queryStr << "\""<<word << "\" ";
    queryStr << ") A WHERE B.offset = A.sourceSynset";
    const char* myquery = queryStr.str().c_str();
   // fprintf(stderr, "query:\n\t %s", myquery);
    query(myquery);
    
    int num_fields = mysql_num_fields(result);
    int num_rows = (int) result->row_count;


    printf("MySQL Tables in mysql database: %d\n", num_rows);
    while ((row = mysql_fetch_row(result)) != NULL) {
        string word;
        double num;
        for (int i = 0; i < num_fields; i++) {
            if(i== 2) { // the word
                word = string(row[i]);
            } else if(i == 4) {
                int n121 = atoi(row[i]);
                num =  ((double) n121)/100.0;
                num = min(1.0, num);
                num = max(0.0, num);
            }
        }
        wnMp->insert(make_pair<string, double>(word, num) );
        printf("w: %s n: %.2f\n", word.c_str(), num);
    }
    return (num_rows > 0);
}

//
// wnconnector.cpp ends here
