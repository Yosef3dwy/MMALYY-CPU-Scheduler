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
    float chechpointStart = -1.0f;
    bool isStarted = false;
    QString status = "Waiting";
};

//comparators needed for SJF logic
struct CompareArrival {
    bool operator()(const Process& a, const Process& b) {
        return a.arrival > b.arrival; //odrder based on arrival time (ascending)
    }
};

struct CompareBurst {
    bool operator()(const Process& a, const Process& b) {
        if (a.burst == b.burst)
            return a.arrival > b.arrival; //if both arrived at same time: FCFS
        return a.burst > b.burst; //ordered based on bust time (ascending)
    }
};
//customize priority queue based on SJF logic
//arguments: element type, container, comparator
using JobQueue   = std::priority_queue<Process, std::vector<Process>, CompareArrival>;
using ReadyQueue_SJF = std::priority_queue<Process, std::vector<Process>, CompareBurst>;

class GanttChart;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_addProcess_clicked();
    void on_btn_start_clicked();
    void on_btn_pause_clicked();
    void on_btn_reset_clicked();
    void timerTick();
    void updateSchedulerVisibility();

private:
    void runStep(); // for FCFS & SJF
    void updateTable();
    void calculateAverages();
    void runBatch();
    void finishProcess(int idx);
    void checkNewArrivals();
    void startNextProcess();

    Ui::MainWindow *ui;
    QTimer *timer;
    std::vector<Process> processes; //used by FCFS and SJF also
    std::queue<int> readyQueue; // for FCFS
    int currentIdx = -1;
    float currentTime = 0.0f;
    int processCounter = 0;
    bool isRunning = false;
    GanttChart *ganttChart;
};

#endif 
