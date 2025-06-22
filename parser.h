#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <bits/stdc++.h>

using namespace std;

//Parsing is process of reading and understanding input.
//This code will parse the data we are working with.
//It also holds global variables we parse into.

/*input-
stats
1,2-1,2-4,3,4,5,6,7,2-2
20
5
A,0,3
B,2,6
C,4,4
D,6,5
E,8,2
*/

//operation->trace or stats; last_instant->maxm simulation time input will rum; process_count->number of process to simulate

string operation;

int last_instant, process_count;
vector<pair<char,int>> algorithms;
vector<tuple<string,int,int>> processes;
vector<vector<char>> timeline;
unordered_map<string,int> process_to_index;

//results
//finish_time=completion time; 

vector<int> finish_time;
vector<int> turn_around_time;
vector<float> norm_turn;

//parses algorithm configuration -> 1,2-1,2-4,3,4,5,6,7,2-2
void parse_algorithms(string algorithm_chunk)
{
    stringstream stream(algorithm_chunk);
    while(stream.good())
    {
        string temp_str;
        getline(stream,temp_str,',');
        stringstream ss(temp_str);
        getline(ss,temp_str,'-');
        char algorithm_id=temp_str[0];
        getline(ss,temp_str,'-');
        int quantum = temp_str.size()>=1 ? stoi(temp_str) : -1;
        algorithms.push_back(make_pair(algorithm_id,quantum));
    }
}

//parses -> A,0,3 to get process name,arrival time and service time(burst time)
void parse_processes()
{
    string process_chunk, process_name;
    int process_arrival_time, process_service_time;
    for(int i=0; i<process_count; i++)
    {
        cin>>process_chunk;
        stringstream stream(process_chunk);
        string temp_str;
        getline(stream, temp_str, ',');
        process_name = temp_str;
        getline(stream, temp_str, ',');
        process_arrival_time = stoi(temp_str);
        getline(stream, temp_str, ',');
        process_service_time = stoi(temp_str);

        processes.push_back(make_tuple(process_name,process_arrival_time,process_service_time));
        process_to_index[process_name] = i;
    }
}

//teke in input for parsing
void parse()
{
    string algorithm_chunk;
    cin>>operation>>algorithm_chunk>>last_instant>>process_count;
    parse_algorithms(algorithm_chunk);
    parse_processes();
    finish_time.resize(process_count);
    turn_around_time.resize(process_count);
    norm_turn.resize(process_count);
    timeline.resize(last_instant);
    for(int i=0; i<last_instant; i++){
        for(int j=0; j<process_count; j++){
            timeline[i].push_back(' ');
        }
    }
}



#endif //PARSER_H_INCLUDED