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


//Round Robin(RR) Scheduling Algorithm
void roundRobin(int originalQuantum)
{
    //queue holds pair of {process_index,remaining_service_time}
    queue<pair<int,int>>q;

    int j=0; //count the number of process as they arrive

    //push all the process that arrive at time zero into the queue
    if(getArrivalTime(processes[j])==0){
        q.push(make_pair(j,getServiceTime(processes[j])));
        j++;
    }

    //remaining quantum for current process
    int currentQuantum = originalQuantum;

    for(int time =0;time<last_instant;time++){//simulation will run from 0 to last instant
        
        //if there is any process in queue, execute it
        if(!q.empty()){
            int processIndex = q.front().first;

            //decrease remaining service time by 1
            q.front().second = q.front().second-1;
            int remainingServiceTime = q.front().second;

            //original process info 
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);

            //decrease quantum by 1
            currentQuantum--;

            //Mark execution in timeline
            timeline[time][processIndex]='*';

            //if any new process arrived at current time push it into the queue
            while(j<process_count && getArrivalTime(processes[j])==time+1){
                q.push(make_pair(j,getServiceTime(processes[j])));
                j++;
            }

            //Case 1: current process is completed as well as quantum is over
            if(currentQuantum==0 && remainingServiceTime==0){
                finish_time[processIndex]=time+1;
                turn_around_time[processIndex] = (finish_time[processIndex] - arrivalTime);
                norm_turn[processIndex] = (turn_around_time[processIndex] * 1.0 / serviceTime);
                currentQuantum=originalQuantum;
                q.pop();
            }
            
            //Case 2: current process is not completed but quantum is over
            else if(currentQuantum==0 && remainingServiceTime!=0){
                q.pop();
                q.push(make_pair(processIndex,remainingServiceTime));
                currentQuantum=originalQuantum;
            }
            
            //Case 3: current process is completed but quantum is not over
            else if(currentQuantum!=0 && remainingServiceTime==0){
                finish_time[processIndex]=time+1;
                turn_around_time[processIndex] = (finish_time[processIndex] - arrivalTime);
                norm_turn[processIndex] = (turn_around_time[processIndex] * 1.0 / serviceTime);
                q.pop();
                currentQuantum=originalQuantum;
            }
        }

        //check for newly arrived process(if queue is empty)
        while(j<process_count && getArrivalTime(processes[j])==time+1){
            q.push(make_pair(j,getServiceTime(processes[j])));
            j++;
        }
    }

    //fill '.' for waiting time in timeline matrix wherever applicable
    fillInWaitTime();
}


//Shortest Process Next(Non-Preemptive SJF Scheduling Algorithm)
void shortestProcessNext()
{
    //priority queue to store {Service time,Process index}
    //ensures smallest service time job is selected next
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; 

    int j = 0; //index to track the number of process which have been added to queue

    for (int i = 0; i < last_instant; i++)
    {
        //Add all the process that have arrived at or before current time 'i' to the queue
        while(j<process_count && getArrivalTime(processes[j]) <= i){
            pq.push(make_pair(getServiceTime(processes[j]), j));
            j++;
        }

        //if atleast one process is ready to run
        if (!pq.empty())
        {
            //select the process with shortest sevice time
            int processIndex = pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();

            //mark waiting time as '.' in timeline
            int temp = arrivalTime;
            for (; temp < i; temp++)
                timeline[temp][processIndex] = '.';

            //mark execution time as '*' in timeline
            temp = i;
            for (; temp < i + serviceTime; temp++)
                timeline[temp][processIndex] = '*';

            //calculate process metrics
            finish_time[processIndex] = (i + serviceTime);
            turn_around_time[processIndex] = (finish_time[processIndex] - arrivalTime);
            norm_turn[processIndex] = (turn_around_time[processIndex] * 1.0 / serviceTime);

            //jump to the end of current process execution
            i = temp - 1;
        }
    }
}


//Shortest Remaining Time First(Preemptive SJF Scheduling Algorithm)
void shortestRemainingTime()
{
    //priority queue to store {Remaining time,Process index}
    //ensures process with shortest remaining time is selected next
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    int j = 0; //index to track which process have arrived

    for (int i = 0; i < last_instant; i++)
    {
        //add newly arrived process at current time 'i' to the queue
        while(j<process_count &&getArrivalTime(processes[j]) == i){
            pq.push(make_pair(getServiceTime(processes[j]), j));
            j++;
        }

        //atleast on process is ready to execute
        if (!pq.empty())
        {
            //select the process with shortest remaining time
            int processIndex = pq.top().second;
            int remainingTime = pq.top().first;
            pq.pop();///temporarily being removed from queue


            int serviceTime = getServiceTime(processes[processIndex]);
            int arrivalTime = getArrivalTime(processes[processIndex]);

            // Mark this time unit as execution for the selected process
            timeline[i][processIndex] = '*';

            if (remainingTime == 1) // process finished
            {
                //process finishes at time i+1
                finish_time[processIndex] = i + 1;
                turn_around_time[processIndex] = (finish_time[processIndex] - arrivalTime);
                norm_turn[processIndex] = (turn_around_time[processIndex] * 1.0 / serviceTime);
            }
            else
            {
                // Reinsert the process with decremented remaining time
                pq.push(make_pair(remainingTime - 1, processIndex));
            }
        }
    }

    // Fill waiting time ('.') for all other slots in the timeline
    fillInWaitTime();
}


//Highest Response Ratio Next (HRNN) Scheduling Algorithm - Non Preemptive
void highestResponseRatioNext() 
{

    // Vector of tuple <process_name, process_response_ratio, time_in_service> for processes that are in the ready queue
    vector<tuple<string, double, int>> present_processes;

    int j=0; //index to track newly arrived process

    for (int current_instant = 0; current_instant < last_instant; current_instant++)
    {
        //add newly arrived processes to the ready queue
        while(j<process_count && getArrivalTime(processes[j])<=current_instant){
            present_processes.push_back(make_tuple(getProcessName(processes[j]), 1.0, 0));
            j++;
        }

        //calculate response ratio for every process
        for (auto &proc : present_processes)
        {
            string process_name = get<0>(proc);
            int process_index = process_to_index[process_name];
            int wait_time = current_instant - getArrivalTime(processes[process_index]);
            int service_time = getServiceTime(processes[process_index]);
            get<1>(proc) = calculate_response_ratio(wait_time, service_time);// RR = (W + S) / S
        }

        //sort present processes by highest to lowest response ratio
        sort(all(present_processes), descendingly_by_response_ratio);

        //atleast one process is ready to execute
        if (!present_processes.empty())
        {
            int process_index = process_to_index[get<0>(present_processes[0])];

            //run the selected process non-preemptively until completion
            while(current_instant<last_instant && get<2>(present_processes[0]) != getServiceTime(processes[process_index])){
                timeline[current_instant][process_index]='*';
                current_instant++;
                get<2>(present_processes[0])++;
            }

            //step back since outer loop will increment current_instant again
            current_instant--;

            //remove completed process from the ready queue
            present_processes.erase(present_processes.begin());
            finish_time[process_index] = current_instant + 1;
            turn_around_time[process_index] = finish_time[process_index] - getArrivalTime(processes[process_index]);
            norm_turn[process_index] = (turn_around_time[process_index] * 1.0 / getServiceTime(processes[process_index]));
        }
    }
    //fill remaining timeline slots with '.' for waiting periods
    fillInWaitTime();
}


//Feedback Queue Level 1 (FB-1) Scheduling Algorithm - Preemptive
void feedbackQ1()
{
    //priority queue of pair {priority level,process index}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; 
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service time

    int j=0; //index to track newly arrived process

    //insert the first process if it arrives at time 0
    if(getArrivalTime(processes[0])==0){
        pq.push(make_pair(0,j));
        remainingServiceTime[j]=getServiceTime(processes[j]);
        j++;
    }

    for(int time =0;time<last_instant;time++){

        //execute one unit of time for the process at the front of the queue
        if(!pq.empty()){
            int priorityLevel = pq.top().first;
            int processIndex =pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();

            //add new arrivals at time = time + 1
            while(j<process_count && getArrivalTime(processes[j])==time+1){
                    pq.push(make_pair(0,j));
                    remainingServiceTime[j]=getServiceTime(processes[j]);
                    j++;
            }

            //mark the process as running at current time
            remainingServiceTime[processIndex]--;
            timeline[time][processIndex]='*';

            if(remainingServiceTime[processIndex]==0){
                finish_time[processIndex]=time+1;
                turn_around_time[processIndex] = (finish_time[processIndex] - arrivalTime);
                norm_turn[processIndex] = (turn_around_time[processIndex] * 1.0 / serviceTime);
            }
            
            else{

                //reinsert process into queue:
                //increase priority if other processes are present
                if(pq.size()>=1)
                    pq.push(make_pair(priorityLevel+1,processIndex));
                else
                    pq.push(make_pair(priorityLevel,processIndex));
            }

        }

        //add any new processes that arrive at time = current + 1
        while(j<process_count && getArrivalTime(processes[j])==time+1){
                pq.push(make_pair(0,j));
                remainingServiceTime[j]=getServiceTime(processes[j]);
                j++;
        }

    }

    //fill remaining timeline slots with '.' for waiting periods
    fillInWaitTime();
}


//Feedback Queue Level 2 with Increasing Quantum (FB-2i) Scheduling Algorithm
void feedbackQ2i()
{
    //pair of priority level and process index; lower priority means higher scheduling priority
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; 
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service time

    int j=0; //index to track newly arrived process

    //insert the first process if it arrives at time 0
    if(getArrivalTime(processes[0])==0){
        pq.push(make_pair(0,j));
        remainingServiceTime[j]=getServiceTime(processes[j]);
        j++;
    }

    for(int time =0;time<last_instant;time++){

        //if the process is ready to run
        if(!pq.empty()){
            int priorityLevel = pq.top().first;
            int processIndex =pq.top().second;
            int arrivalTime = getArrivalTime(processes[processIndex]);
            int serviceTime = getServiceTime(processes[processIndex]);
            pq.pop();

            //add new arrivals at time = time + 1
            while(j<process_count && getArrivalTime(processes[j])<=time+1){
                    pq.push(make_pair(0,j));
                    remainingServiceTime[j]=getServiceTime(processes[j]);
                    j++;
            }

            //assign time quantum based on current priority level
            int currentQuantum = pow(2,priorityLevel);
            int temp = time;

            //execute till completion of process or current quantum
            while(currentQuantum && remainingServiceTime[processIndex]){
                currentQuantum--;
                remainingServiceTime[processIndex]--;
                timeline[temp++][processIndex]='*';
            }

            //if the process is finished
            if(remainingServiceTime[processIndex]==0){
                finish_time[processIndex]=temp;
                turn_around_time[processIndex] = (finish_time[processIndex] - arrivalTime);
                norm_turn[processIndex] = (turn_around_time[processIndex] * 1.0 / serviceTime);
            }
            //if not finished,increase priority
            else{
                if(pq.size()>=1)
                    pq.push(make_pair(priorityLevel+1,processIndex));
                else
                    pq.push(make_pair(priorityLevel,processIndex));
            }

            //move simulation time forward by the number of executed units
            time = temp-1;
        }
        
        //add any other new arrivals at time = time + 1
        while(j<process_count && getArrivalTime(processes[j])<=time+1){
                pq.push(make_pair(0,j));
                remainingServiceTime[j]=getServiceTime(processes[j]);
                j++;
        }
    }
    //fill remaining timeline slots with '.' for waiting periods
    fillInWaitTime();
}


//Aging Scheduling Algorithm - Priority based with dynamic priority adjustment
void aging(int originalQuantum)
{
    vector<tuple<int,int,int>>v; //tuple of priority level, process index and total waiting time
    int currentProcess=-1; //currently running process (-1 = none)

    int j=0; //index to track newly arrived process

    for(int time =0;time<last_instant;time++){

        //add newly arrived process to ready queue
        while(j<process_count && getArrivalTime(processes[j])<=time){
            v.push_back(make_tuple(getPriorityLevel(processes[j]),j,0));
            j++;
        }

        //apply aging: update priority levels and wait times
        for(int i=0;i<v.size();i++){

            //reset priority and wait time for the currently running process
            if(get<1>(v[i])==currentProcess){
                get<2>(v[i])=0;
                get<0>(v[i])=getPriorityLevel(processes[currentProcess]);
            }
            //increase priority (i.e., decrease priority value) for waiting processes
            else{
                get<0>(v[i])++;
                get<2>(v[i])++;
            }
        }
        //sort all processes by their updated priority level
        sort(v.begin(),v.end(),byPriorityLevel);

        //select process with highest priority level(lowest value)
        currentProcess=get<1>(v[0]);
        int currentQuantum = originalQuantum;

        //run the selected process for one quantum or until time runs out        
        while(currentQuantum-- && time<last_instant){
            timeline[time][currentProcess]='*';
            time++;
        }
        time--; //adjust time because outer loop will increment it again
    }
    //fill remaining timeline slots with '.' for waiting periods
    fillInWaitTime();
}

//function to print the name of the algorithm being used
void printAlgorithm(int algorithm_index)
{
    int algorithm_id = algorithms[algorithm_index].first - '0';
    if(algorithm_id==2)
        cout << ALGORITHMS[algorithm_id] <<algorithms[algorithm_index].second <<endl;
    else
        cout << ALGORITHMS[algorithm_id] << endl;
}

//fnction to print process names
void printProcesses()
{
    cout << "Process    ";
    for (int i = 0; i < process_count; i++)
        cout << "|  " << getProcessName(processes[i]) << "  ";
    cout << "|\n";
}

//function to print arrival times
void printArrivalTime()
{
    cout << "Arrival    ";
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ",getArrivalTime(processes[i]));
    cout<<"|\n";
}

//function to print service times and mean
void printServiceTime()
{
    cout << "Service    |";
    for (int i = 0; i < process_count; i++)
        printf("%3d  |",getServiceTime(processes[i]));
    cout << " Mean|\n";
}

//function to print finish times
void printFinishTime()
{
    cout << "Finish     ";
    for (int i = 0; i < process_count; i++)
        printf("|%3d  ",finish_time[i]);
    cout << "|-----|\n";
}

//function to print turnaround times and their average
void printTurnAroundTime()
{
    cout << "Turnaround |";
    int sum = 0;
    for (int i = 0; i < process_count; i++)
    {
        printf("%3d  |",turn_around_time[i]);
        sum += turn_around_time[i];
    }
    if((1.0 * sum / turn_around_time.size())>=10)
		printf("%2.2f|\n",(1.0 * sum / turn_around_time.size()));
    else
	 	printf(" %2.2f|\n",(1.0 * sum / turn_around_time.size()));
}

//function to print normalized turnaround times and their average
void printNormTurn()
{
    cout << "NormTurn   |";
    float sum = 0;
    for (int i = 0; i < process_count; i++)
    {
        if( norm_turn[i]>=10 )
            printf("%2.2f|",norm_turn[i]);
        else
            printf(" %2.2f|",norm_turn[i]);
        sum += norm_turn[i];
    }

    if( (1.0 * sum / norm_turn.size()) >=10 )
        printf("%2.2f|\n",(1.0 * sum / norm_turn.size()));
	else
        printf(" %2.2f|\n",(1.0 * sum / norm_turn.size()));
}

//function to print all statistics (used when operation == SHOW_STATISTICS)
void printStats(int algorithm_index)
{
    printAlgorithm(algorithm_index);
    printProcesses();
    printArrivalTime();
    printServiceTime();
    printFinishTime();
    printTurnAroundTime();
    printNormTurn();
}

//function to print Gantt timeline for TRACE mode
void printTimeline(int algorithm_index)
{
    for (int i = 0; i <= last_instant; i++)
        cout << i % 10<<" ";
    cout <<"\n";
    cout << "------------------------------------------------\n";
    for (int i = 0; i < process_count; i++)
    {
        cout << getProcessName(processes[i]) << "     |";
        for (int j = 0; j < last_instant; j++)
        {
            cout << timeline[j][i]<<"|";
        }
        cout << " \n";
    }
    cout << "------------------------------------------------\n";
}

//core function to run the selected scheduling algorithm
void execute_algorithm(char algorithm_id, int quantum,string operation)
{
    switch (algorithm_id)
    {
    case '1':
        if(operation==TRACE)cout<<"FCFS  ";
        firstComeFirstServe();
        break;
    case '2':
        if(operation==TRACE)cout<<"RR-"<<quantum<<"  ";
        roundRobin(quantum);
        break;
    case '3':
        if(operation==TRACE)cout<<"SPN   ";
        shortestProcessNext();
        break;
    case '4':
        if(operation==TRACE)cout<<"SRT   ";
        shortestRemainingTime();
        break;
    case '5':
        if(operation==TRACE)cout<<"HRRN  ";
        highestResponseRatioNext();
        break;
    case '6':
        if(operation==TRACE)cout<<"FB-1  ";
        feedbackQ1();
        break;
    case '7':
        if(operation==TRACE)cout<<"FB-2i ";
        feedbackQ2i();
        break;
    case '8':
        if(operation==TRACE)cout<<"Aging ";
        aging(quantum);
        break;
    default:
        break;
    }
}


int main()
{
    parse(); //parse input file or data

    for (int idx = 0; idx < (int)algorithms.size(); idx++)
    {
        clear_timeline();//reset timeline before each algorithm run
        execute_algorithm(algorithms[idx].first, algorithms[idx].second,operation);
        if (operation == TRACE)
            printTimeline(idx);
        else if (operation == SHOW_STATISTICS)
            printStats(idx);
        cout << "\n";
    }

    return 0;
}