#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ganttchart.h"
#include <QMessageBox>
#include <QAbstractItemView>
#include <cmath>

// Rounds a floating-point number to 3 decimal place
// Used to avoid precision issues during simulation time updates
float r3(float x)
{
    return std::round(x * 1000.0f) / 1000.0f;
}

// Initializes UI components, timer, and default states
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    jobQueue(CompareArrivalIdx(processes)),
    priReadyQueue(ComparePriorityIdx(processes)),
    sjfReadyQueue(CompareBurstIdx(processes))
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
       ui->lineEdit_arrivalTime->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// button add
void MainWindow::on_btn_addProcess_clicked()
{

    bool burstOk, arrivalOk, priorityOk;

    float burst = ui->lineEdit_burstTime->text().toFloat(&burstOk);
    float arrival = ui->lineEdit_arrivalTime->text().toFloat(&arrivalOk);

    if (!burstOk || burst <= 0)
    {
        QMessageBox::warning(this, "Error", "Burst time must be > 0");
        return;
    }

    if (!arrivalOk || arrival < 0)
    {
        QMessageBox::warning(this, "Error", "Arrival time must be >= 0");
        return;
    }

    if (arrival < currentTime)
    {
        arrival = currentTime;
    }

    int pri = 0;

    if (ui->rb_priority->isChecked())
    {
        pri = ui->lineEdit_priority->text().toInt(&priorityOk);
        if (!priorityOk || pri<0)
        {
            QMessageBox::warning(this, "Error", "Priority must be >= 0");
            return;
        }
    }

    if (ui->rb_roundRobin->isChecked())
    {
        bool ok;
        float q = ui->lineEdit_quantum->text().toFloat(&ok);
        if (!ok || q <= 0)
        {
            QMessageBox::warning(this, "Error", "Quantum must be > 0");
            return;
        }
        QTime = q;
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
    jobQueue.push(processes.size() - 1);

    updateTable();

    if (ui->rb_roundRobin->isChecked())
    ui->lineEdit_quantum->setEnabled(false);

      ui->lineEdit_arrivalTime->clear();
ui->lineEdit_burstTime->clear();
ui->lineEdit_priority->clear();

}

// button start
void MainWindow::on_btn_start_clicked()
{

    if (processes.size() == 0 || jobQueue.size() == 0)
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
    noCompletedProcesses = 0;

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

    ui->lineEdit_arrivalTime->clear();
ui->lineEdit_burstTime->clear();
ui->lineEdit_priority->clear();
ui->lineEdit_quantum->clear();
    ui->lineEdit_arrivalTime->setFocus();
}

// live mode
void MainWindow::runStep()
{
    while (!jobQueue.empty() && processes[jobQueue.top()].arrival <= currentTime)
    {
        int arrivedIdx = jobQueue.top();
        jobQueue.pop();
        processes[arrivedIdx].status = "Ready";

        if (ui->rb_FCFS->isChecked() || ui->rb_roundRobin->isChecked()) {
            readyQueue.push(arrivedIdx);
        }
        else if (ui->rb_SJF->isChecked()) {
            sjfReadyQueue.push(arrivedIdx);

            // SJF Preemption Check
            if (ui->chk_preemptive->isChecked() && currentIdx != -1 &&
                processes[currentIdx].remaining > processes[sjfReadyQueue.top()].remaining)
            {
                processes[currentIdx].status = "Ready";
                sjfReadyQueue.push(currentIdx);
                currentIdx = -1;
            }
        }
        else if (ui->rb_priority->isChecked()) {
            priReadyQueue.push(arrivedIdx);

            // Priority Preemption Check
            if (ui->chk_preemptive->isChecked() && currentIdx != -1 &&
                processes[currentIdx].priority > processes[priReadyQueue.top()].priority)
            {
                processes[currentIdx].status = "Ready";
                priReadyQueue.push(currentIdx);
                currentIdx = -1;
            }
        }
    }

    // waiting in ready queue -> running
    if (currentIdx == -1)
    {
        if ((ui->rb_FCFS->isChecked() || ui->rb_roundRobin->isChecked()) && !readyQueue.empty()) {
            currentIdx = readyQueue.front();
            readyQueue.pop();
        }
        else if (ui->rb_SJF->isChecked() && !sjfReadyQueue.empty()) {
            currentIdx = sjfReadyQueue.top();
            sjfReadyQueue.pop();
        }
        else if (ui->rb_priority->isChecked() && !priReadyQueue.empty()) {
            currentIdx = priReadyQueue.top();
            priReadyQueue.pop();
        }

        // If we successfully pulled a process, start it
        if (currentIdx != -1) {
            processes[currentIdx].status = "Running";
            if (processes[currentIdx].actualStart == -1) {
                processes[currentIdx].actualStart = currentTime;
            }
            timeOnCPU = 0.0f; // Reset for Round Robin
        }
    }

    // process is now running
    if (currentIdx != -1)
    {
        processes[currentIdx].remaining -= (float)tickTime;

        if (ui->rb_roundRobin->isChecked())
            timeOnCPU += (float)tickTime;

        // Gantt Chart Rendering
        float sliceStart = currentTime;
        float sliceEnd = r3(currentTime + (float)tickTime);
        ganttChart->updateActiveProcess(processes[currentIdx].id, sliceStart, sliceEnd);

        // Check for Completion
        if (processes[currentIdx].remaining <= 0.0001f)
        {
            processes[currentIdx].remaining = 0;
            processes[currentIdx].completion = r3(currentTime + (float)tickTime);

            // Turnaround Time
            processes[currentIdx].tat = processes[currentIdx].completion - processes[currentIdx].arrival;

            // Waiting Time
            processes[currentIdx].wait = processes[currentIdx].tat - processes[currentIdx].burst;
            processes[currentIdx].status = "Completed";

            noCompletedProcesses++;
            currentIdx = -1;
        }

        // check for round robin time slice expiration
        else if (ui->rb_roundRobin->isChecked() && timeOnCPU >= QTime - 0.00001f)
        {
            processes[currentIdx].status = "Ready";

            // push new arrivals BEFORE re-adding current process
            float sliceEnd = r3(currentTime + (float)tickTime);

            while (!jobQueue.empty() && processes[jobQueue.top()].arrival <= sliceEnd)
            {
                int arrivedIdx = jobQueue.top();
                jobQueue.pop();
                processes[arrivedIdx].status = "Ready";
                readyQueue.push(arrivedIdx);
            }

            readyQueue.push(currentIdx);
            currentIdx = -1;
        }
    }
    // CPU is idle (no process running)
    if (currentIdx == -1 && noCompletedProcesses < processes.size())
    {
        float sliceStart = currentTime;
        float sliceEnd = r3(currentTime + (float)tickTime);

        ganttChart->updateActiveProcess(-1, sliceStart, sliceEnd);
    }
    // update time after check arrivals
    currentTime = r3(currentTime + (float)tickTime);
}

// Called automatically every timer interval
// Drives step-by-step simulation
void MainWindow::timerTick()
{
    runStep();

    updateTable();
    ganttChart->update();

    if (processes.size() == noCompletedProcesses)
    {
        timer->stop();
        isRunning = false;

        ui->btn_start->setEnabled(true);
        ui->btn_pause->setEnabled(false);

        calculateAverages();
    }
}

// non-live mode
void MainWindow::runBatch()
{
    while (processes.size() != noCompletedProcesses)
    {
        // here we need while as we don't have updates it work untill the processes was added are finished
        runStep();
    }

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
        ui->lineEdit_priority->setEnabled(true);
        ui->lineEdit_quantum->setEnabled(false);
    }
    else if (ui->rb_roundRobin->isChecked())
    {
        ui->lineEdit_priority->setEnabled(false);
        ui->lineEdit_quantum->setEnabled(processes.empty());
    }
    else
    {
        ui->lineEdit_priority->setEnabled(false);
        ui->lineEdit_quantum->setEnabled(false);
    }
}