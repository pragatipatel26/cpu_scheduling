#include <bits/stdc++.h>
#include "parser.h"

#define all(v) v.begin(), v.end()

using namespace std;

//global constants
const string TRACE = "trace";
const string SHOW_STATISTICS = "stats";
const string ALGORITHMS[9] = {"","FCFS","RR-","SPN","SRT","HRRN","FB-1","FB-2i","AGING"};

//sorting helper functions

//sort process based on their service time
bool sortByServiceTime(const tuple<string,int,int> &a,const tuple<string,int,int> &b){
    return(get<2>(a) < get<2>(b));
}

//sort process based on their arrival time
bool sortByArrivalTime(const tuple<string,int,int> &a,const tuple<string,int,int> &b){
    return(get<1>(a) < get<1>(b));
}

//sorting tuples based on response ratio in decreasing order.Used for Highest Response Ratio Next (HRRN) scheduling
bool descendingly_by_response_ratio(tuple<string,double,int> a,tuple<string,double,int> b){
    return(get<1>(a) > get<1>(b));
}

//priority based scheduling.sorting first by priority level(higher first).if two process have same priority,sorting by waiting time(higher first)
bool byPriorityLevel(const tuple<int,int,int> &a,const tuple<int,int,int> &b){
    if (get<0>(a) == get<0>(b)){
        return get<2>(a) > get<2>(b);
    }
    else{
        return get<0>(a) > get<0>(b);
    }
}

//Utility functions

//resetting the timeline array by setting all avlues equal to ' '
void clear_timeline(){
    for(int i=0;i<last_instant;i++){
        for(int j=0;j<process_count;j++){
            timeline[i][j] = ' ';
        }
    }
}

//functions for extracting process name,arrival time,service time and priority level from a tuple

string getProcessName(tuple<string,int,int> &a){
    return get<0>(a);
}

int getArrivalTime(tuple<string,int,int> &a){
    return get<1>(a);
}

int getServiceTime(tuple<string,int,int> &a){
    return get<2>(a);
}

int getPriorityLevel(tuple<string,int,int> &a){
    return get<2>(a);
}

//computing response ration for HRRN scheduling
//response ratio = (wait time+service time)/service time
double calculate_response_ratio(int wait_time,int service_time){
    return (wait_time+service_time)*1.0/service_time;
}

//helper function for wait time calculation
void fillInWaitTime(){
    for(int i=0;i<process_count;i++){
        int arrivalTime = getArrivalTime(processes[i]);
        for(int k=arrivalTime;k<finishTime[i];k++){
            if(timeline[k][i]!='*'){
                timeline[k][i]=".";
            }
        }
    }
}


//First Come First Service(FCFS) Scheduling algorithm
void firstComeFirstServe(){

    //start time from the arrival of first process
    int time = getArrivalTime(processes[0]);

    //iterating through all process in arrival order(not considering arrival time)
    for(int i=0;i<process_count;i++){

        int processIndex = i;
        int arrivalTime = getArrivalTime(processes[i]);
        int serviceTime = getServiceTime(processes[i]);
        
        //finish time -> when the proceess will complete its execution
        finish_time[processIndex] = (time+serviceTime);
        //turn around time -> finish time-arrival time
        turn_around_time[processIndex] = (finish_time[processIndex]-arrivalTime);
        //normalized turn around time -> turnaround time/service time
        norm_turn[processIndex] = (turn_around_time[processIndex]*1.0/serviceTime);

        //Mark timeline with '*' to indicate when the process is running
        for(int j=time;j<finish_time[processIndex];j++){
            timeline[j][processIndex] = '*';
        }

        //Mark timeline with '.' to indicate when the process is waiting
        for(int j=arrivalTime;j<time;j++){
            timeline[j][processIndex] = '.';
        }

        //update current time after process is finished
        time += serviceTime;
    }
}


//Round Robin
void roundRobin(int originalQuantum)
{
    queue<pair<int,int>>q;
    int j=0;
    if(getArrivalTime(processes[j])==0){
        q.push(make_pair(j,getServiceTime(processes[j])));
        j++;
    }
    int currentQuantum = originalQuantum;
    for(int time =0;time<last_instant;time++){
        if(!q.empty()){
            int processIndex = q.front().first;
            q.front().second = q.front().second-1;
            int remainingServiceTime = q.front().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            currentQuantum--;
            timeline[time][processIndex]='*';
            while(j<process_count && getArrivalTime(processes[j])==time+1){
                q.push(make_pair(j,getServiceTime(processes[j])));
                j++;
            }

            if(currentQuantum==0 && remainingServiceTime==0){
                finish_time[processIndex]=time+1;
                turn_around_ime[processIndex] = (finish_time[processIndex] - arrivalTime);
                norm_turn[processIndex] = (turn_around_time[processIndex] * 1.0 / serviceTime);
                currentQuantum=originalQuantum;
                q.pop();
            }else if(currentQuantum==0 && remainingServiceTime!=0){
                q.pop();
                q.push(make_pair(processIndex,remainingServiceTime));
                currentQuantum=originalQuantum;
            }else if(currentQuantum!=0 && remainingServiceTime==0){
                finish_time[processIndex]=time+1;
                turAroundTime[processIndex] = (finishTime[processIndex] - arrivalTime);
                normTurn[processIndex] = (turnAroundTime[processIndex] * 1.0 / serviceTime);
                q.pop();
                currentQuantum=originalQuantum;
            }
        }
        while(j<process_count && getArrivalTime(processes[j])==time+1){
            q.push(make_pair(j,getServiceTime(processes[j])));
            j++;
        }
    }
    fillInWaitTime();
}


int main()
{
    parse();

    return 0;
}