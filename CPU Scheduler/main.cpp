#include "process.h"
#include <iostream>
using namespace std;

void sjf_nonpreemptive(vector<Process> processes);

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "P" << processes[i].id << " Arrival Time: ";
        cin >> processes[i].arrivalTime;
        cout << "P" << processes[i].id << " Burst Time: ";
        cin >> processes[i].burstTime;
    }

    sjf_nonpreemptive(processes);
    return 0;
}