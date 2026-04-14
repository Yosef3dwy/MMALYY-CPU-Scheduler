#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <queue>
#include <vector>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Process {
    int id;
    float arrival;
    float burst;
    int priority = 0;
    float remaining;
    float completion = -1.0f;
    float wait = 0.0f;
    float tat = 0.0f;
    float actualStart = -1.0f;
    QString status = "Waiting";
};


struct CompareArrivalIdx {
    const std::vector<Process>& procs;
    CompareArrivalIdx(const std::vector<Process>& p) : procs(p) {}

    bool operator()(int a, int b) {
        return procs[a].arrival > procs[b].arrival;
    }
};

struct CompareBurstIdx {
    const std::vector<Process>& procs;
    CompareBurstIdx(const std::vector<Process>& p) : procs(p) {}

    bool operator()(int a, int b) {
        if (procs[a].burst == procs[b].burst)
            return procs[a].arrival > procs[b].arrival;
        return procs[a].burst > procs[b].burst;
    }
};

struct ComparePriorityIdx {
    const std::vector<Process>& procs;
    ComparePriorityIdx(const std::vector<Process>& p) : procs(p) {}

    bool operator()(int a, int b) {
        if (procs[a].priority == procs[b].priority)
            return procs[a].arrival > procs[b].arrival;
        return procs[a].priority > procs[b].priority;
    }
};



//customize priority queue based on SJF logic
//arguments: element type, container, comparator
using JobQueue   = std::priority_queue<int, std::vector<int>, CompareArrivalIdx>;
using ReadyQueue_SJF = std::priority_queue<int, std::vector<int>, CompareBurstIdx>;
using ReadyQueue_Priority = std::priority_queue<int, std::vector<int>, ComparePriorityIdx>;

class GanttChart;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    const float tickTime = 0.005; // in seconds
    ~MainWindow();

private slots:
    void on_btn_addProcess_clicked();
    void on_btn_start_clicked();
    void on_btn_pause_clicked();
    void on_btn_reset_clicked();
    void timerTick();
    void updatePriorityInputState();

private:
    void runStep();

    void updateTable();
    void calculateAverages();
    void runBatch();
    void finishProcess(int idx);
    void checkNewArrivals();
    void startNextProcess();

    template <typename ReadyQueueType>
    void executeNonPreemptive(std::vector<Process>& processes, GanttChart* ganttChart);

    Ui::MainWindow *ui;
    QTimer *timer;
    std::vector<Process> processes; //used by FCFS and SJF also


    JobQueue jobQueue;
    ReadyQueue_Priority priReadyQueue;
    ReadyQueue_SJF sjfReadyQueue;
    int noCompletedProcesses = 0;

    std::queue<int> readyQueue; // for FCFS
    int currentIdx = -1;
    float currentTime = 0.0f;
    int processCounter = 0;
    bool isRunning = false;
    GanttChart *ganttChart;

    float QTime = 2.0f; //q time for round robin
    float timeOnCPU = 0.0f; //to check how long each process been running from the q time

};

#endif 
