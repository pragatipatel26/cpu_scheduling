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




