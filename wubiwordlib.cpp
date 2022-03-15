#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

using namespace std;
vector<string> tagetData;
map<string, string> wordCodeMap;
map<string, string> codeWordMap;
set<string> code;
int checkWeight = 3000000;

// 获取单字库
void HandleAWord(string line) {
  size_t index = line.find_first_of("\t");
  if (index < 4)
    tagetData.push_back(line);
}

// 获取存在简还有全码的字
void HandleAWordHaveFull(string line) {
  if (line.length() < 2)
    return;
  size_t index = line.find_first_of("\t");
  string word = line.substr(0, index);
  string temp = line.substr(index + 1, line.length());
  size_t index2 = temp.find_first_of("\t");
  string key = line.substr(index, index2 + 1);

  if (wordCodeMap.find(word) == wordCodeMap.end()) {
    wordCodeMap.insert(pair<string, string>(word, key));
    tagetData.push_back(line);
    //cout << word << " 1 " << key << endl;
  }
  else {

    //cout << word << " 2 " << key << endl;
  }
}
string currentKey = "";
bool b = false;
string lastKey = "";
vector<string> lastlineList;
string lastLine;
// 删除重码
int GetWeight(string line)
{
  size_t index = line.find_first_of("\t");
  string word = line.substr(0, index);
  string temp = line.substr(index + 1, line.length());
  size_t index2 = temp.find_first_of("\t");
  string key = line.substr(index + 1, index2);

  string temp2 = temp.substr(index2 + 1, line.length());
  size_t index3 = temp2.find_first_of("\t");
  string weightStr = "";
  if (index3 != string::npos)
    weightStr = temp2.substr(0, index3);
  else
    weightStr = temp2;
  int weight = atoi(weightStr.c_str());
  return weight;
}

string GetLine() {
  string lastLine = lastlineList[0];
  for (int i = 1; i < lastlineList.size(); i++) {
    if (GetWeight(lastLine) < GetWeight(lastlineList[i])) {
      lastLine = lastlineList[i];
    }
  }
  return lastLine;
}

vector<string> GetLines() {
  string line = lastlineList[0];
  int max = 0;
  for (int i = 1; i < lastlineList.size(); i++) {
    if (GetWeight(line) < GetWeight(lastlineList[i])) {
      max = i;
      line = lastlineList[i];
    }
  }

  vector<string> lines;
  for (int i = 0; i < lastlineList.size(); i++) {
    line = lastlineList[i];
    if (max != i) {
      line = "#" + line;
    }
    lines.push_back(line);
  }

  return lines;
}

void HandleACodeHaveWords(string line) {
  if (line.length() < 2)
    return;
  size_t index = line.find_first_of("\t");
  string word = line.substr(0, index);
  string temp = line.substr(index + 1, line.length());
  size_t index2 = temp.find_first_of("\t");
  string key = line.substr(index + 1, index2);

  string temp2 = temp.substr(index2 + 1, line.length());
  size_t index3 = temp2.find_first_of("\t");
  string weightStr = "";
  if (index3 != string::npos)
    weightStr = temp2.substr(0, index3);
  else
    weightStr = temp2;

  int weight = atoi(weightStr.c_str());
  //// 填加
  if (currentKey != key) {
    if (lastlineList.size() > 0) {
      tagetData.push_back(GetLine());
      //vector<string> lines = GetLines();
      //for (size_t i = 0; i < lines.size(); i++)
      //{
      //  tagetData.push_back(lines[i]);
      //}
    }

    lastlineList.clear();
    lastlineList.push_back(line);
    currentKey = key;
  }
  else {
    lastlineList.push_back(line);
  }
}

void HandleBigWeightWords(string line) {
  if (line.length() < 2)
    return;
  size_t index = line.find_first_of("\t");
  string word = line.substr(0, index);
  string temp = line.substr(index + 1, line.length());
  size_t index2 = temp.find_first_of("\t");
  string key = line.substr(index + 1, index2);

  string temp2 = temp.substr(index2 + 1, line.length());
  size_t index3 = temp2.find_first_of("\t");
  string weightStr = "";
  if (index3 != string::npos)
    weightStr = temp2.substr(0, index3);
  else
    weightStr = temp2;

  int weight = atoi(weightStr.c_str());
  if (weight == 0 || weight > checkWeight || key.length() < 3)
  {
    tagetData.push_back(line);
  }
}

int main()
{
  ifstream ifile;
  ifile.open("e:/wubi86update.dict.yaml");
  //ifile.open("e:/config2.txt");
  if (!ifile.is_open()) {
    cout << "open error" << endl;
  }

  int startLine = 25;
  int currentLine = 1;
  string lineData;
  string word;


  while (getline(ifile, lineData)) {
    if (currentLine < startLine) {
      tagetData.push_back(lineData);
      currentLine++;
    }
    else {
      //HandleAWord(lineData);
      //HandleAWordHaveFull(lineData);
      //HandleACodeHaveWords(lineData);
      HandleBigWeightWords(lineData);
    }
  }
  ifile.close();

  ofstream ofile;
  ofile.open("e:/temp.txt");
  for (size_t i = 0; i < tagetData.size(); i++)
  {
    ofile << tagetData[i].c_str() << endl;
  }

  //for (set<string>::iterator it = code.begin(); it != code.end(); it++)
  //{
  //  ofile << *it << endl;
  //}
  ofile.close();
  return 0;
}
