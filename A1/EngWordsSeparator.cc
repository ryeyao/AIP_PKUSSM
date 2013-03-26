#include<iostream>
#include<fstream>
#include<string.h>
#include<map>
#include<vector>

using namespace std;

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

   while (index < content.length()) {
       switch(content[index]) {
           case ' ':
           case '\t':
           case '\r':
           case '\n':
               index++;
               if (is_new_word) {
                   wordsList.push_back(a_word);
                   a_word = "";
                   is_new_word = false;
               }
               continue;

           default:
               is_new_word = true;
               a_word += content[index];
               index++;
       }
   }

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


int main() {

    string content = getContentStr("test/testFile1");
    vector<string> words = getWords(content);
    map<string, int> wordsCount = parseAndCount(words);
    
    for (map<string, int>::iterator ii = wordsCount.begin(); ii != wordsCount.end(); ++ii) {
        cout << (*ii).first << "[" << (*ii).second << "]" << endl;
    }
   // cout<<content<<endl;
    return 0;
}


