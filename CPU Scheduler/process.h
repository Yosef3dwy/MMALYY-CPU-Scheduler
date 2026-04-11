#pragma once
#include <queue>
#include <vector>
struct Process {
    //inputs by user:
    int id;
    int arrivalTime;
    int burstTime;
    //needed for some calculations:
    int startTime = -1; //CPU picks it
    int finishTime = -1;  //burst time ends
    int waitingTime = 0;   //start - arrival
    int turnaroundTime = 0;   //finish - arrival
};

struct CompareArrival {
    bool operator()(const Process& a, const Process& b) {
        //odrder based on arrival time (ascending)
        return a.arrivalTime > b.arrivalTime; 
    }
};

struct CompareBurst {
    bool operator()(const Process& a, const Process& b) {
        if (a.burstTime == b.burstTime)
            return a.arrivalTime > b.arrivalTime; //if both arrived at same time: FCFS
        return a.burstTime > b.burstTime; //ordered based on bust time (ascending)
    }
};

//customize priority queue based on my logic
//arguments: element type, container, comparator
using JobQueue = std::priority_queue<Process, std::vector<Process>, CompareArrival>;
using ReadyQueue = std::priority_queue<Process, std::vector<Process>, CompareBurst>;
