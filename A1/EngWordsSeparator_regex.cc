#include<iostream>
#include<fstream>
#include<string.h>
#include<map>
#include<vector>
#include<regex.h>
#include<boost/regex.hpp>

using namespace std;
using namespace boost;

string getContentStr(char* path) {

    string content, line;
    ifstream in_file(path);

    while(!in_file.eof()) {
        getline(in_file, line);
        content += line;
    }

    in_file.close();
    return content;
}

vector<string> getWords(string content) {

   vector<string> wordsList;
   int index = 0;
   int wordIndex = 0;
   bool is_new_word = false;
   string a_word;
   regex re("[\t\r\n]|[\b]|[\W ]|[\?.,]?[ ]+");
   sregex_token_iterator first(content.begin(), content.end(), re, -1), last;
   wordsList = {first, last};
   return wordsList;
}

map<string, int> parseAndCount(vector<string> content) {
    map<string, int> wordsCount;
    for (vector<string>::iterator ii = content.begin(); ii != content.end(); ++ii) {
        string curr_word = (*ii); 
        wordsCount[curr_word] = wordsCount[curr_word] + 1;
    }
    
    return wordsCount;
    
}


int main(int argc, char** argv) {

    char* path;

    if (argc != 2) {
        cout<<"usage: parse_regex [file]"<<endl;
        return 0;
    }
    path = argv[1];
    string content = getContentStr(path);
    vector<string> words = getWords(content);
    map<string, int> wordsCount = parseAndCount(words);
    
    for (map<string, int>::iterator ii = wordsCount.begin(); ii != wordsCount.end(); ++ii) {
        cout << (*ii).first << "[" << (*ii).second << "]" << endl;
    }
   // cout<<content<<endl;
    return 0;
}


