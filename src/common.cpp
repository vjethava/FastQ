#include "common.h"
int numAttributes = 9;
string specificity[3]= {"medium","specific","broad"};

string  task[3] = {"inf", "notinf", "both"};
string  objective[2] = {"resource", "action"};
string  authosen[2] = {"yes", "no"};
string  spatialsen[2] = {"yes", "no"};
string timesen[2] = {"yes", "no"};
string  polywords[2] =  {"yes", "no"};
string  genre[4] = {"reference","news","business","community"};
string  topic[23] = {"Adults-Sex", "Arts-Culture", "Finance",
                       "Cars-Transportation", "Computers-Internet", "Education",
                       "Entertainment-Music-Games", "Health", "Home-Garden",
                       "Politics-Government", "Religion-Belief-Systems",
                       "Science-Mathematics", "Social-Science", "Sports",
                       "Technology-Electronic", "Travel", "Beauty-Style",
                       "Industrial-Goods-Services", "Undefined", "Work", "Food-Drink",
                       "Charity", "Person"};
int attrsNumVals[9] = {3, 3, 2, 2, 2, 2, 2, 4, 23};
string* attributes[9] = {specificity, task, objective,
    authosen, spatialsen, timesen, polywords, genre, topic};
string attrNames[9] = {"specificity", "task", "objective", "authosen", "spatialsen"
    , "timesen", "polywords", "genre", "topic" };

wstring getWstr(string word1) {
    wstring word(word1.length(), L' ');
    std::copy(word1.begin(), word1.end(), word.begin());
    return word;
}

string getTimeStr() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [15];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 15, "%d/%m %H:%M:%S", timeinfo);
    string result(buffer);
    return result;
}
vector<string> split(string ip, char delim) {
    vector<string> result;
    int pos = 0;
    while(ip.find(delim) != string::npos) {
        pos = ip.find(delim);
        string curr = ip.substr(0, pos);
        ip = ip.substr(pos+1);
        result.push_back(curr);
    }
    ASSERT(ip.size() > 0); // last char is not a delim
    result.push_back(ip); // last part
    return result;
}


string trim(string str) {
    char const* delims = " \t\r\n";
    // trim leading whitespace
    string::size_type notwhite = str.find_first_not_of(delims);
    str.erase(0, notwhite);

    // trim trailing whitespace
    notwhite = str.find_last_not_of(delims);
    str.erase(notwhite + 1);
    return str; 
}

void tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
