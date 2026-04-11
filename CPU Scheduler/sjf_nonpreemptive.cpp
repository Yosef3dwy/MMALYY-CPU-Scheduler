#include "process.h"
#include <vector>
#include <iostream>
using namespace std;

void sjf_nonpreemptive(vector<Process> processes) {
    JobQueue jobQueue;
    ReadyQueue readyQueue;
    //needed another vector to print gantt chart
    vector<Process> finished; 

    //push processes vector entered from user to queue
    for (auto& p : processes)
        jobQueue.push(p);
 
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;

    while (completed < n) {
        //get the top process from job to ready queue
        while (!jobQueue.empty() && jobQueue.top().arrivalTime <= currentTime)
        {
            readyQueue.push(jobQueue.top());
            jobQueue.pop();
        }
        if (readyQueue.empty()) { //CPU idle, move on to next arrived process
            currentTime = jobQueue.top().arrivalTime;
            continue;
        }
        //running process 
        Process p = readyQueue.top();
        readyQueue.pop();
        p.startTime = currentTime;
        p.finishTime = currentTime + p.burstTime;
        p.waitingTime = p.startTime - p.arrivalTime;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        currentTime = p.finishTime; //update current time to proceed well
        finished.push_back(p);
        completed++;
    }

    //printing gantt chart
    for (auto& p : finished)
        cout << " P" << p.id << " |";
    cout << "\n" << finished[0].startTime;
    for (auto& p : finished)
        cout << "    " << p.finishTime;

    //calculating avr waiting, turnaround time
    float totalWaiting = 0, totalTurnaround = 0;
    for (auto& p : finished) {
        totalWaiting += p.waitingTime;
        totalTurnaround += p.turnaroundTime;
    }
    cout << "\nAverage Waiting Time = " << totalWaiting / n;
    cout << "\nAverage Turnaround Time = " << totalTurnaround / n<<"\n";
}