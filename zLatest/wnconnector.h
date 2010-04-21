/* wnconnector.h --- 
 * 
 * Filename: wnconnector.h
 * Description: 
 * Author: Vinay Jethava
 * Created: Fri Apr 16 14:02:08 2010 (+0200)
 * Last-Updated: Fri Apr 16 15:41:35 2010 (+0200)
 *           By: Vinay Jethava
 *     Update #: 17
 * URL: 
 * Keywords: 
 */

/* Code: */
#ifndef WNCONNECTOR_H
#define WNCONNECTOR_H

#include "common.h"
#include <iostream>
#include <string>
#include <wn.h>
#include <mysql/mysql.h>
using namespace std;
// using namespace mysqlpp;

/**
 *  This class connects to the word net database
 */
class WnConnector {
private: // Mysql connection
    MYSQL *cnxn;
    MYSQL_RES *result;
    MYSQL_ROW row;
    int queryState;

private: // Database details
    const char* server;
    const char* user;
    const char* password;
    const char* db;

public: // result for current word
    map<string, double>* wnMp;
    stringstream queryStr; 
private:
    bool query(const char * qstr);

    inline bool query(string qStr) {
        return query(qStr.c_str());
    }
public:
    WnConnector(map<string, string>* params = NULL);

    bool queryWord(const string word);
    // XXX: ~WnConnector() some time causes problems

    ~WnConnector() {
        FPRINTF(stderr, "destroying WnConnector object\n");
        /* close connection */
        mysql_free_result(result);
        
    }
};
#endif
/* WNCONNECTOR.h ends here */
