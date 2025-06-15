#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <bits/stdc++.h>

using namespace std;

//global variables
string operation;
int last_instant,process_count;
vector<pair<char,int>> algorithms;
vector<tuple<string,int,int>> processes;
vector<vector<char>> timeline;
unordered_map<string,int> process_to_index;

//results
vector<int> finish_time;
vector<int> turn_around_time;
vector<float> norm_turn;





#endif