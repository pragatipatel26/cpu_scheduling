# Implementation of CPU Scheduling Algorithm in C++.

## Need for CPU Scheduling Algorithm
CPU Scheduling algorithm in operating system is used to decide which process get to use CPU at a particualr time
Main purpose of CPU scheduling is to maximize the CPU Utilization and minimize the response and waiting time for process.

## Terminology 
### Arrival Time
The time at which the process arrives in the ready queue.
### Completion Time
The time at which the process completes its execution.
### Burst Time
Time required by a process for CPU execution.
### Turn Around Time
Time Difference between completion time and arrival time.
### Waiting Time
Time Difference between turn around time and burst time.
### Response Time
Time from when a process is submitted until the first response is produced, not when the process completes.

## Types of CPU Scheduling
### Preemptive Scheduling
The operating system can interrupt an ongoing process to allocate CPU time to another process mainly due to priority or time sharing policies.Process switches from running state to ready state.Algorithms based on preemptive scheduling are Round Robin (RR) , Shortest Remaining Time First (SRTF) , Priority (preemptive version) , etc.
### Non-Preemptive Scheduling
The operating system cannot interrupt a running process.Once a process gets the CPU, it keeps running until it finishes or enters the waiting state.Algorithms based on non-preemptive scheduling are: First Come First Serve, Shortest Job First (SJF basically non preemptive) and Priority (nonpreemptive version) , etc.

# Algorithms

### First Come First Serve (FCFS)
 In First Come First Serve (FCFS) CPU scheduling algorithm, processes are executed in the order of their arrival without preemption. It calculates important scheduling metrics such as finish time, turnaround time, and normalized turnaround time for each process. A timeline is generated to visualize process execution, marking running time with '*' and waiting time with '.'. FCFS can suffer from performance issues when long processes delay the execution of shorter ones, a problem known as the convoy effect.

### Round Robin with varying time quantum (RR) 
Round Robin (RR) with variable quantum is a scheduling algorithm that uses a time-sharing approach to divide CPU time among processes. In this version of RR, the quantum (time slice) is not fixed and can be adjusted depending on the requirements of the processes. Shorter processes can be handled with smaller time slices, while longer ones get more time, helping reduce overall waiting time and improving CPU utilization.
The scheduler works in a time-sharing manner, maintaining a queue of processes. Each process gets a turn on the CPU; if it doesn’t finish within its time slice, it goes to the back of the queue. As the simulation runs, the code tracks execution and waiting times, and generates a visual timeline showing exactly when each process was running ('*') or waiting ('.').
This dynamic quantum approach helps prevent starvation and balances fairness with performance—especially in systems where process burst times vary significantly.

### Shortest Process Next (SPN)
Shortest Process Next (SPN) scheduling algorithm, a non-preemptive strategy that always selects the process with the shortest burst time. It uses a priority queue to keep track of ready processes and picks the shortest one at each step. As each process arrives, it's added to the queue, and the CPU jumps directly to execute the selected process. The simulation skips idle time and handles processes in a way that minimizes average waiting time, though longer processes may get delayed if shorter ones keep arriving.

### Shortest Remaining Time (SRT)
Shortest Remaining Time (SRT) scheduling algorithm, is a preemptive version of Shortest Job First. At every time unit, the process with the least remaining execution time is selected. If a new process arrives with a shorter remaining time, it immediately preempts the currently running process. The code uses a priority queue to track remaining times and updates execution decisions in real-time. This approach helps minimize average waiting time and adapts well even when burst times aren't fully known in advance.

### Highest Response Ratio Next (HRRN)
Highest Response Ratio Next (HRRN) scheduling algorithm, is a non-preemptive method that selects the next process based on its response ratio. The response ratio balances both waiting time and service time, giving preference to processes that have waited longer. At each step, the process with the highest response ratio is selected and run until completion. The simulation uses a dynamic list to track waiting processes and updates priorities in real time to minimize overall waiting time while avoiding starvation.

### Feedback Queue Level 1 (FB-1)
Feedback Queue Level 1 (FB-1) scheduling algorithm, is a preemptive strategy that dynamically adjusts process priority. Each process starts with the highest priority, and if it doesn’t finish in one go, it is reinserted into the queue with a lower priority—unless it’s the only one waiting. The algorithm uses a priority queue and tracks remaining service time to decide execution order. It favors short jobs but also ensures longer ones eventually get CPU time, making it fair and responsive in multitasking environments.

### Feedback with Varying Time Quantum (FBV)
Feedback Queue with Varying Time Quantum (FB-2i) scheduling algorithm, is a preemptive strategy that uses multiple priority levels—each with its own time quantum. Processes start at the highest priority with the shortest quantum, and if not completed, they are moved to lower priorities with longer quanta. The quantum increases exponentially (like 1, 2, 4, ...), allowing short processes to finish quickly while giving longer processes more time as needed. This approach balances responsiveness and fairness, adapting to different process lengths efficiently.

### Aging
Aging scheduling algorithm, is a priority-based preemptive strategy that dynamically adjusts process priorities to prevent starvation. Each process starts with an initial priority, and if it waits in the queue, its priority improves over time (i.e., the priority value decreases). This aging mechanism ensures that even lower-priority processes eventually get CPU time. The simulation selects the process with the highest (lowest numerical) priority at each step and executes it for a fixed quantum. Priorities are continuously updated to keep scheduling fair and responsive.

# Input Format
Line 1: "trace" or "stats"
Line 2: a comma-separated list telling which CPU scheduling policies to be analyzed/visualized along with their parameters, if applicable. Each algorithm is represented by a number as listed in the introduction section and as shown in the attached testcases. Round Robin and Aging have a parameter specifying the quantum q to be used. Therefore, a policy entered as 2-4 means Round Robin with q=4. Also, policy 8-1 means Aging with q=1.
FCFS (First Come First Serve)
RR (Round Robin)
SPN (Shortest Process Next)
SRT (Shortest Remaining Time)
HRRN (Highest Response Ratio Next)
FB-1, (Feedback where all queues have q=1)
FB-2i, (Feedback where q= 2i)
Aging
Line 3: An integer specifying the last instant to be used in your simulation and to be shown on the timeline.

Line 4: An integer specifying the number of processes to be simulated.

Line 5: Start of description of processes. Each process is to be described on a separate line. For algorithms 1 through 7, each process is described using a comma-separated list specifying:

1- String specifying a process name
2- Arrival Time
3- Service Time

Note: For Aging algorithm (algorithm 8), each process is described using a comma-separated list specifying:

1- String specifying a process name
2- Arrival Time
3- Priority

Processes are assumed to be sorted based on the arrival time. If two processes have the same arrival time, then the one with the lower priority is assumed to arrive first.
