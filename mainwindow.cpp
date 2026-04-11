#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ganttchart.h"
#include <QMessageBox>
#include <QAbstractItemView>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerTick);

    // Setup Gantt Chart inside ScrollArea
    ganttChart = new GanttChart(this);
    ui->scrollArea_gantt->setWidget(ganttChart);
    ui->scrollArea_gantt->setWidgetResizable(true);

    // Hide unused FCFS inputs
    updateSchedulerVisibility();
    connect(ui->rb_FCFS, &QRadioButton::toggled, this, &MainWindow::updateSchedulerVisibility);
    connect(ui->rb_SJF, &QRadioButton::toggled, this, &MainWindow::updateSchedulerVisibility);
    connect(ui->rb_priority, &QRadioButton::toggled, this, &MainWindow::updateSchedulerVisibility);
    connect(ui->rb_roundRobin, &QRadioButton::toggled, this, &MainWindow::updateSchedulerVisibility);

    ui->tableWidget_queue->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->btn_pause->setEnabled(false);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::updateSchedulerVisibility() {
    bool isFCFS = ui->rb_FCFS->isChecked();
    ui->spinBox_priority->setVisible(!isFCFS);
    ui->label_3->setVisible(!isFCFS); // Priority label
    ui->spinBox_quantum->setVisible(!isFCFS);
    ui->label_4->setVisible(!isFCFS); // Time Quantum label
}

void MainWindow::on_btn_addProcess_clicked() {
    Process p;
    p.id = ++processCounter;
    p.arrival = ui->spinBox_arrivalTime->value();
    p.burst = ui->spinBox_burstTime->value();

    if (p.burst <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Burst time must be > 0");
        processCounter--;
        return;
    }
    p.remaining = p.burst;
    processes.push_back(p);

    // If arrived or in past, push to FCFS queue immediately
    if (p.arrival <= currentTime) {
        readyQueue.push(processes.size() - 1);
        processes.back().status = "Ready";
    }
    updateTable();
}

void MainWindow::on_btn_start_clicked() {
    if (processes.empty()) {
        QMessageBox::information(this, "Info", "Add processes first.");
        return;
    }
    isRunning = true;
    ui->btn_pause->setEnabled(true);
    ui->btn_start->setEnabled(false);

    if (ui->rb_modeExistingOnly->isChecked()) {
        runBatch();
    } else {
        timer->start(1000); // 1 second = 1 time unit
        ui->btn_addProcess->setEnabled(true); // Allow dynamic addition
    }
}

void MainWindow::on_btn_pause_clicked() {
    timer->stop();
    isRunning = false;
    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);
}

void MainWindow::on_btn_reset_clicked() {
    timer->stop();
    isRunning = false;
    processes.clear();
    std::queue<int> empty; std::swap(readyQueue, empty);
    currentIdx = -1;
    currentTime = 0;
    processCounter = 0;
    ganttChart->clear();
    ui->tableWidget_queue->setRowCount(0);
    ui->lbl_avgWaitTime->setText("0.00");
    ui->lbl_avgTurnaroundTime->setText("0.00");
    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);
    ui->btn_addProcess->setEnabled(true);
}

void MainWindow::timerTick() {
    currentTime++;
    checkNewArrivals();

    if (currentIdx == -1 && !readyQueue.empty()) {
        currentIdx = readyQueue.front();
        readyQueue.pop();
        processes[currentIdx].status = "Running";
    }

    if (currentIdx != -1) {
        processes[currentIdx].remaining--;
        if (processes[currentIdx].remaining == 0) {
            finishProcess(currentIdx);
        }
    }

    updateTable();
    ganttChart->update();

    // Check if all processes completed
    bool allDone = !processes.empty();
    for (const auto& p : processes) {
        if (p.status != "Completed") { allDone = false; break; }
    }
    if (allDone) {
        timer->stop();
        isRunning = false;
        ui->btn_start->setEnabled(true);
        ui->btn_pause->setEnabled(false);
        calculateAverages();
    }
}

void MainWindow::runBatch() {
    // Instant execution of existing & dynamically added processes
    while (true) {
        currentTime++;
        checkNewArrivals();
        if (currentIdx == -1 && !readyQueue.empty()) {
            currentIdx = readyQueue.front();
            readyQueue.pop();
            processes[currentIdx].status = "Running";
        }
        if (currentIdx != -1) {
            processes[currentIdx].remaining--;
            if (processes[currentIdx].remaining == 0) {
                finishProcess(currentIdx);
            }
        }
        bool allDone = !processes.empty();
        for (const auto& p : processes) {
            if (p.status != "Completed") { allDone = false; break; }
        }
        if (allDone) break;
    }
    timer->stop();
    isRunning = false;
    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);
    updateTable();
    ganttChart->update();
    calculateAverages();
}

void MainWindow::checkNewArrivals() {
    for (size_t i = 0; i < processes.size(); ++i) {
        if (processes[i].arrival == currentTime && processes[i].status == "Waiting") {
            readyQueue.push(i);
            processes[i].status = "Ready";
        }
    }
}

void MainWindow::finishProcess(int idx) {
    processes[idx].completion = currentTime;
    processes[idx].tat = currentTime - processes[idx].arrival;
    processes[idx].wait = processes[idx].tat - processes[idx].burst;
    processes[idx].status = "Completed";
    ganttChart->addSegment(processes[idx].id, processes[idx].burst, currentTime - processes[idx].burst);
    currentIdx = -1;
}

void MainWindow::updateTable() {
    ui->tableWidget_queue->setRowCount(processes.size());
    for (size_t i = 0; i < processes.size(); ++i) {
        const auto& p = processes[i];
        ui->tableWidget_queue->setItem(i, 0, new QTableWidgetItem(QString("P%1").arg(p.id)));
        ui->tableWidget_queue->setItem(i, 1, new QTableWidgetItem(QString::number(p.arrival)));
        ui->tableWidget_queue->setItem(i, 2, new QTableWidgetItem(QString::number(p.burst)));
        ui->tableWidget_queue->setItem(i, 3, new QTableWidgetItem("-")); // Unused for FCFS
        ui->tableWidget_queue->setItem(i, 4, new QTableWidgetItem(QString::number(p.remaining)));
        ui->tableWidget_queue->setItem(i, 5, new QTableWidgetItem(p.status));
    }
}

void MainWindow::calculateAverages() {
    double totalWT = 0, totalTAT = 0;
    int count = 0;
    for (const auto& p : processes) {
        if (p.status == "Completed") {
            totalWT += p.wait;
            totalTAT += p.tat;
            count++;
        }
    }
    if (count > 0) {
        ui->lbl_avgWaitTime->setText(QString::number(totalWT / count, 'f', 2));
        ui->lbl_avgTurnaroundTime->setText(QString::number(totalTAT / count, 'f', 2));
    }
}