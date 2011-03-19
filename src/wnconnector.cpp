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

const char* SERVER = "localhost";
const char* USER = "root";
const char* PASSWORD = "root";
const char* DB = "eswn";

WnConnector::WnConnector(map<string, string>* params) :
queryStr("") {
    if (params != NULL) {
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


}

bool WnConnector::query(const char* myQuery) {
    bool qres1 = false;
    if (mysql_query(cnxn, myQuery)) {
        fprintf(stderr, "WnConnector::query(): %s\n", mysql_error(cnxn));
        qres1 =  false;
    } else {
        result = mysql_store_result(cnxn);
        qres1 =  true;
    }
    
    return qres1;
}

/**
 * Function that queries WN for word synsets
 */

// TODO: Fix for utf16

bool WnConnector::queryWord(const string word1) {

//    wstring word(word1.length(), L' ');
//    std::copy(word1.begin(), word1.end(), word.begin());
//    wcout << L"queryWord(" << word << L")" << endl;
//  SHOWFUNC(word1.c_str());
    // ESTABLISH MYSQL CONNECTION
    cnxn = mysql_init(NULL);
    if (cnxn == NULL) {
        fprintf(stderr,
                "WnConnector::WnConnector() could not connect to db:\n\t%u: %s\n"
                , mysql_errno(cnxn), mysql_error(cnxn));
        exit(-1);
    }
    if (!mysql_real_connect(cnxn, server, user, password, db, 0, NULL, 0)) {
        fprintf(stderr, "WnConnector() %s\n", mysql_error(cnxn));
        exit(-1);
    }
    wnMp->clear(); // clear for storing current word
    bool qres0; 
    int num_fields = 0;
    int num_rows = 0;
    queryStr.str("") ; 
    queryStr << "SELECT A.offset,B.offset,B.word, A.relation,A.csco ";
    queryStr << "FROM eswn_variant B, (SELECT E.sourceSynset,F.word,F.offset,E.relation,E.csco ";
    queryStr << "FROM eswn_relation E,eswn_variant F WHERE ";
    queryStr << "E.targetSynset = F.offset AND E.relation <> 'near_antonym' ";
    queryStr << "AND E.relation <> 'has_holo_part' AND F.word = ";
    queryStr << "\"";
    queryStr << word1 << "\" ";
    queryStr << ") A WHERE B.offset = A.sourceSynset";

    string mqstring = queryStr.str();
    bool qres1;
    if (mysql_query(cnxn, mqstring.c_str())) {
        fprintf(stderr, "WnConnector::query(): %s\n", mysql_error(cnxn));
        qres1 =  false;
    } else {
        result = mysql_store_result(cnxn);
        qres1 =  true;
    }
    
    bool qres = qres1;
    if (qres) {
        num_fields = mysql_num_fields(result);
        num_rows = (int) result->row_count;
  //      printf("MySQL Tables in mysql database: %d\n", num_rows);
        while ((row = mysql_fetch_row(result)) != NULL) {
            string word;
            double num;
            for (int i = 0; i < num_fields; i++) {
                if (i == 2) { // the word
                    word = string(row[i]);
                } else if (i == 4) {
                    int n121 = atoi(row[i]);
                    num = ((double) n121) / 100.0;
                    num = min(1.0, num);
                    num = max(0.0, num);
                }
            }
            wnMp->insert(make_pair<string, double>(word, num));
       //     printf("w: %s n: %.2f\n", word.c_str(), num);
        }
        qres0 = (num_rows > 0);
    } else {
        qres0 =  false;
    }

    mysql_close(cnxn);
    return qres; 
}

//
// wnconnector.cpp ends here
