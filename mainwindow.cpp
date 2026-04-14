#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ganttchart.h"
#include <QMessageBox>
#include <QAbstractItemView>
#include <cmath>

// Rounds a floating-point number to 1 decimal place
// Used to avoid precision issues during simulation time updates
float r3(float x)
{
    return std::round(x * 1000.0f) / 1000.0f;
}

float r1(float x)
{
    return std::round(x * 10.0f) / 10.0f;
}
// Initializes UI components, timer, and default states
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerTick); // connect the timer with timertick so when call timer automaticaly go to timertick

    ganttChart = new GanttChart(this);
    ui->scrollArea_gantt->setWidget(ganttChart);
    ui->scrollArea_gantt->setWidgetResizable(true);

    ui->tableWidget_queue->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->btn_pause->setEnabled(false);

    connect(ui->rb_FCFS, &QRadioButton::toggled, this, &MainWindow::updatePriorityInputState);
    connect(ui->rb_SJF, &QRadioButton::toggled, this, &MainWindow::updatePriorityInputState);
    connect(ui->rb_priority, &QRadioButton::toggled, this, &MainWindow::updatePriorityInputState);
    connect(ui->rb_roundRobin, &QRadioButton::toggled, this, &MainWindow::updatePriorityInputState);

    updatePriorityInputState();
}

MainWindow::~MainWindow()
{
    delete ui;
}
// button add
void MainWindow::on_btn_addProcess_clicked()
{

    float burst = ui->spinBox_burstTime->value();
    float arrival = ui->spinBox_arrivalTime->value();

    if (arrival < currentTime)
    {
        arrival = currentTime;
    }

    int pri = ui->spinBox_priority->value();

    if (burst <= 0)
    {
        QMessageBox::warning(this, "Error", "Burst time must be > 0");
        return;
    }

    Process p;

    p.id = ++processCounter;
    p.arrival = arrival;
    p.burst = burst;
    p.priority = pri;
    p.remaining = burst;

    p.completion = 0;
    p.wait = 0;
    p.tat = 0;

    p.actualStart = -1;
    p.status = "Waiting";

    processes.push_back(p);

    updateTable();
}
// button start
void MainWindow::on_btn_start_clicked()
{

    if (processes.size() == 0)
    {
        QMessageBox::information(this, "Info", "Add processes first");
        return;
    }

    isRunning = true;

    ui->btn_start->setEnabled(false);
    ui->btn_pause->setEnabled(true);


    // Disable the configuretion buttons
    ui->rb_FCFS->setEnabled(false);
    ui->rb_SJF->setEnabled(false);
    ui->rb_priority->setEnabled(false);
    ui->rb_roundRobin->setEnabled(false);
    ui->chk_preemptive->setEnabled(false);
    ui->rb_modeLive->setEnabled(false);
    ui->rb_modeExistingOnly->setEnabled(false);


    if (ui->rb_roundRobin->isChecked())
        QTime = ui->spinBox_quantum->value();

    if (ui->rb_modeExistingOnly->isChecked())
    {
        runBatch();
    }
    else
    {
        timer->start((int)(tickTime * 1000));  // automatically call timertick
    }
}

// button pause
void MainWindow::on_btn_pause_clicked()
{

    timer->stop();
    isRunning = false;

    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);
}

// button reset
void MainWindow::on_btn_reset_clicked()
{

    timer->stop();
    isRunning = false;

    processes.clear();

    std::queue<int> empty;
    readyQueue = empty;

    currentIdx = -1;
    currentTime = 0;
    processCounter = 0;

    QTime = 2.0f;
    timeOnCPU = 0.0f;

    ganttChart->clear();

    ui->tableWidget_queue->setRowCount(0);
    ui->lbl_avgWaitTime->setText("0.00");
    ui->lbl_avgTurnaroundTime->setText("0.00");

    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);

    // Enable the configuretion buttons
    ui->rb_FCFS->setEnabled(true);
    ui->rb_SJF->setEnabled(true);
    ui->rb_priority->setEnabled(true);
    ui->rb_roundRobin->setEnabled(true);
    ui->chk_preemptive->setEnabled(true);
    ui->rb_modeLive->setEnabled(true);
    ui->rb_modeExistingOnly->setEnabled(true);
}

// live mode
void MainWindow::runStep()
{

    for (int i = 0; i < (int)processes.size(); i++)
    {
        // job queue -> ready queue
        if (processes[i].status == "Waiting" && processes[i].arrival <= currentTime)
        {
            processes[i].status = "Ready";

            if (ui->rb_FCFS->isChecked() || ui->rb_roundRobin->isChecked())
            {
                readyQueue.push(i);
            }
            else if (ui->chk_preemptive->isChecked() && (currentIdx != -1) )
            {
                // If Preemptive, recalculate the best index in SJF & Priority only!!!
                processes[currentIdx].status = "Ready";
                currentIdx = -1;
            }
        }
    }

    // waiting in ready queue -> running
    if (currentIdx == -1)
    {
        // FCFS logic
        if (ui->rb_FCFS->isChecked())
        {
            if (!readyQueue.empty())
            {
                currentIdx = readyQueue.front();
                readyQueue.pop();
                processes[currentIdx].status = "Running";
                processes[currentIdx].actualStart = currentTime;
            }
        }

        // Round Robin logic
        else if (ui->rb_roundRobin->isChecked())
        {
            if (!readyQueue.empty())
            {
                currentIdx = readyQueue.front();
                readyQueue.pop();
                processes[currentIdx].status = "Running";
                if (processes[currentIdx].actualStart == -1)
                    processes[currentIdx].actualStart = currentTime;
                timeOnCPU = 0.0f;
            }
        }

        int bestIdx = -1;
        // SJF logic
        if (ui->rb_SJF->isChecked())
        {
            // choose best (least) burst time

            for (int j = 0; j < (int)processes.size(); j++)
            {
                if (processes[j].status == "Ready")
                {
                    if (bestIdx == -1
                        || processes[j].remaining < processes[bestIdx].remaining
                        || (processes[j].remaining == processes[bestIdx].remaining
                        && processes[j].arrival < processes[bestIdx].arrival))

                        bestIdx = j;
                }
            }
        }
        // Priority logic
        else if (ui->rb_priority->isChecked())
        {
            // choose best (least) priority number

            for (int j = 0; j < (int)processes.size(); j++)
            {
                if (processes[j].status == "Ready")
                {
                    if (bestIdx == -1
                        || processes[j].priority < processes[bestIdx].priority
                        || (processes[j].priority == processes[bestIdx].priority
                        && processes[j].arrival < processes[bestIdx].arrival))

                        bestIdx = j;
                }
            }
        }

        // have chosen, now run process with least burst time or the least priority number
        if (bestIdx != -1)
        {
            currentIdx = bestIdx;
            processes[currentIdx].status = "Running";
            processes[currentIdx].actualStart = currentTime;
        }
    }

    // process is now running
    if (currentIdx != -1)
    {
        processes[currentIdx].remaining -= (float)tickTime;

        // GantChart Rendering
        float sliceStart = currentTime;
        float sliceEnd = r3(currentTime + (float)tickTime);
        ganttChart->updateActiveProcess(processes[currentIdx].id, sliceStart, sliceEnd);

        if (ui->rb_roundRobin->isChecked())
            timeOnCPU += (float)tickTime; // track time of the process

        if (processes[currentIdx].remaining <= 0.001f)
        {
            processes[currentIdx].remaining = 0;

            //as the end of the process in round robin differs from the others
            if (ui->rb_roundRobin->isChecked())
            {
                processes[currentIdx].completion = sliceEnd;
            }
            //for FCFS & (Non-preemptive)
            else
            {
                processes[currentIdx].completion = r1(currentTime);
            }

            // Process' Turnaround Time
            processes[currentIdx].tat =
                processes[currentIdx].completion - processes[currentIdx].arrival;

            // Process' Waiting Time
            processes[currentIdx].wait =
                processes[currentIdx].tat - processes[currentIdx].burst;

            processes[currentIdx].status = "Completed";

            // Flag that there is no process is running
            currentIdx = -1;
        }

        else if (ui->rb_roundRobin->isChecked() && timeOnCPU >= QTime - 0.00001f) // current process used its qtime at its turn
        {
            processes[currentIdx].status = "Ready"; // process hasn't finished yet

            for (int i = 0; i < (int)processes.size(); i++)
            {
                if (processes[i].status == "Waiting" && processes[i].arrival <= sliceEnd) // check if any processes has arrived during this qtime or were waiting
                {
                    processes[i].status = "Ready";
                    readyQueue.push(i);
                }
            }

            readyQueue.push(currentIdx); // the current process(not completed after its turn in qtime) goes at the back of the queue
            timeOnCPU = 0.0f;            // reset it for the next process
            currentIdx = -1;
        }
    }
    // update time after check arrivals
    currentTime = r3(currentTime + (float)tickTime);
}

// Called automatically every timer interval
// Drives step-by-step simulation
void MainWindow::timerTick()
{

    if (ui->rb_FCFS->isChecked() || ui->rb_SJF->isChecked() || ui->rb_priority->isChecked() || ui->rb_roundRobin->isChecked())
        runStep();

    updateTable();
    ganttChart->update();

    bool done = true;

    if (processes.size() == 0) //Race condition during Reset
        done = false;

    for (int i = 0; i < (int)processes.size(); i++)
    {

        if (processes[i].status != "Completed")
        {
            done = false;
            break;
        }
    }

    if (done == true)
    {

        timer->stop();
        isRunning = false;

        ui->btn_start->setEnabled(true);
        ui->btn_pause->setEnabled(false);

        calculateAverages();
    }
}

/*template <typename ReadyQueueType>
void MainWindow::executeNonPreemptive(std::vector<Process> &processes, GanttChart *ganttChart)
{
    JobQueue jobQueue;
    ReadyQueueType readyQueue;

    for (auto &p : processes)
        jobQueue.push(p);

    float time = 0;

    while (!jobQueue.empty() || !readyQueue.empty())
    {
        while (!jobQueue.empty() && jobQueue.top().arrival <= time)
        {
            readyQueue.push(jobQueue.top());
            jobQueue.pop();
        }

        // CPU idle, move on to next arrived process
        if (readyQueue.empty())
        {
            time = jobQueue.top().arrival;
            continue;
        }

        // Running process
        Process p = readyQueue.top();
        readyQueue.pop();
        p.actualStart = time;
        p.completion = r3(time + p.burst);
        p.tat = p.completion - p.arrival;
        p.wait = p.tat - p.burst;
        time = p.completion;

        // Update processes vector to reflect on GUI
        for (auto &vp : processes)
        {
            if (vp.id == p.id)
            {
                vp = p;
                vp.status = "Completed";
                vp.remaining = 0;
                break;
            }
        }
        ganttChart->addSegment(p.id, p.actualStart, p.completion);
    }
}*/

// non-live mode
void MainWindow::runBatch()
{
    // SJF (Non-Preemptive) logic
    // if (ui->rb_SJF->isChecked() && !ui->chk_preemptive->isChecked())
    // {
    //     executeNonPreemptive<ReadyQueue_SJF>(processes, ganttChart);
    // }

    // FCFS & RR logic
    // else if (ui->rb_FCFS->isChecked() || ui->rb_roundRobin->isChecked() || ui->rb_priority->isChecked() || ui->rb_SJF->isChecked())
    // {
    while (true)
    {
        // here we need while as we don't have updates it work untill the processes  was added are finished
        runStep();
        bool done = true;
        if (processes.size() == 0)
        {
            done = false;
        }
        for (int i = 0; i < (int)processes.size(); i++)
        {
            if (processes[i].status != "Completed")
            {
                done = false;
                break;
            }
        }
        if (done == true)
            break;
    }
    // }

    timer->stop();
    isRunning = false;

    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);

    updateTable();
    ganttChart->update();
    calculateAverages();
}

// Updates process table with current simulation state
void MainWindow::updateTable()
{

    ui->tableWidget_queue->setRowCount(processes.size());

    for (int i = 0; i < (int)processes.size(); i++)
    {

        Process p = processes[i];

        ui->tableWidget_queue->setItem(i, 0, new QTableWidgetItem("P" + QString::number(p.id)));
        ui->tableWidget_queue->setItem(i, 1, new QTableWidgetItem(QString::number(p.arrival, 'f', 2)));
        ui->tableWidget_queue->setItem(i, 2, new QTableWidgetItem(QString::number(p.burst, 'f', 2)));

        if (ui->rb_priority->isChecked())
        {
            ui->tableWidget_queue->setItem(i, 3, new QTableWidgetItem(QString::number(p.priority)));
        }
        else
        {
            ui->tableWidget_queue->setItem(i, 3, new QTableWidgetItem("-"));
        }

        ui->tableWidget_queue->setItem(i, 4, new QTableWidgetItem(QString::number(p.remaining, 'f', 2)));
        ui->tableWidget_queue->setItem(i, 5, new QTableWidgetItem(p.status));
    }
}

// Calculates average waiting time and turnaround time
void MainWindow::calculateAverages()
{

    float totalWT = 0;
    float totalTAT = 0;
    int count = 0;

    for (int i = 0; i < (int)processes.size(); i++)
    {

        if (processes[i].status == "Completed")
        {

            totalWT += processes[i].wait;
            totalTAT += processes[i].tat;
            count++;
        }
    }

    if (count > 0)
    {

        ui->lbl_avgWaitTime->setText(QString::number(totalWT / count, 'f', 2)); 
        ui->lbl_avgTurnaroundTime->setText(QString::number(totalTAT / count, 'f', 2));
    }
}

void MainWindow::updatePriorityInputState() {

    if (ui->rb_priority->isChecked())
    {
        ui->spinBox_priority->setEnabled(true);
        ui->spinBox_quantum->setEnabled(false);
    }
    else if (ui->rb_roundRobin->isChecked())
    {
        ui->spinBox_priority->setEnabled(false);
        ui->spinBox_quantum->setEnabled(true);
    }
    else
    {
        ui->spinBox_priority->setEnabled(false);
        ui->spinBox_quantum->setEnabled(false);
    }
}
