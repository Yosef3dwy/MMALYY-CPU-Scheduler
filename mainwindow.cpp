#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ganttchart.h"
#include <QMessageBox>
#include <QAbstractItemView>
#include <cmath>

float r1(float x) {
    return std::round(x * 10.0f) / 10.0f;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerTick);

    ganttChart = new GanttChart(this);
    ui->scrollArea_gantt->setWidget(ganttChart);
    ui->scrollArea_gantt->setWidgetResizable(true);

    ui->tableWidget_queue->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->btn_pause->setEnabled(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_btn_addProcess_clicked() {

    float burst = ui->spinBox_burstTime->value();
    float arrival = ui->spinBox_arrivalTime->value();

    if (burst <= 0) {
        QMessageBox::warning(this, "Error", "Burst time must be > 0");
        return;
    }

    Process p;

    p.id = ++processCounter;
    p.arrival = arrival;
    p.burst = burst;
    p.remaining = burst;

    p.completion = 0;
    p.wait = 0;
    p.tat = 0;

    p.actualStart = -1;
    p.status = "Waiting";

    processes.push_back(p);

    updateTable();
}

void MainWindow::on_btn_start_clicked() {

    if (processes.size() == 0) {
        QMessageBox::information(this, "Info", "Add processes first");
        return;
    }

    isRunning = true;

    ui->btn_start->setEnabled(false);
    ui->btn_pause->setEnabled(true);

    if (ui->rb_modeExistingOnly->isChecked()) {
        runBatch();
    } else {
        timer->start(100);
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

    std::queue<int> empty;
    readyQueue = empty;

    currentIdx = -1;
    currentTime = 0;
    processCounter = 0;

    ganttChart->clear();

    ui->tableWidget_queue->setRowCount(0);
    ui->lbl_avgWaitTime->setText("0.00");
    ui->lbl_avgTurnaroundTime->setText("0.00");

    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);
}

void MainWindow::runStep() {

    currentTime = currentTime + 0.1f;
    currentTime = r1(currentTime);

    for (int i = 0; i < (int)processes.size(); i++) {

        if (processes[i].status == "Waiting" &&
            processes[i].arrival <= currentTime) {

            readyQueue.push(i);
            processes[i].status = "Ready";
        }
    }

    if (currentIdx == -1 && !readyQueue.empty()) {

        currentIdx = readyQueue.front();
        readyQueue.pop();

        processes[currentIdx].status = "Running";
        processes[currentIdx].actualStart = currentTime;
    }

    if (currentIdx != -1) {

        processes[currentIdx].remaining -= 0.1f;

        if (processes[currentIdx].remaining <= 0.0001f) {

            processes[currentIdx].remaining = 0;

            float exactFinish =
                processes[currentIdx].actualStart +
                processes[currentIdx].burst;

            processes[currentIdx].completion = r1(exactFinish);

            processes[currentIdx].tat =
                processes[currentIdx].completion - processes[currentIdx].arrival;

            processes[currentIdx].wait =
                processes[currentIdx].tat - processes[currentIdx].burst;

            processes[currentIdx].status = "Completed";

            ganttChart->addSegment(
                processes[currentIdx].id,
                r1(processes[currentIdx].actualStart),
                processes[currentIdx].completion
                );

            currentIdx = -1;
        }
    }
}

void MainWindow::timerTick() {

    if (ui->rb_FCFS->isChecked()) {
        runStep();
    }

    updateTable();
    ganttChart->update();

    bool done = true;

    if (processes.size() == 0) {
        done = false;
    }

    for (int i = 0; i < (int)processes.size(); i++) {

        if (processes[i].status != "Completed") {
            done = false;
            break;
        }
    }

    if (done == true) {

        timer->stop();
        isRunning = false;

        ui->btn_start->setEnabled(true);
        ui->btn_pause->setEnabled(false);

        calculateAverages();
    }
}

void MainWindow::runBatch() {

    while (true) {

        if (ui->rb_FCFS->isChecked()) {
            runStep();
        }

        bool done = true;

        if (processes.size() == 0) {
            done = false;
        }

        for (int i = 0; i < (int)processes.size(); i++) {

            if (processes[i].status != "Completed") {
                done = false;
                break;
            }
        }

        if (done == true) {
            break;
        }
    }

    timer->stop();
    isRunning = false;

    ui->btn_start->setEnabled(true);
    ui->btn_pause->setEnabled(false);

    updateTable();
    ganttChart->update();
    calculateAverages();
}

void MainWindow::updateTable() {

    ui->tableWidget_queue->setRowCount(processes.size());

    for (int i = 0; i < (int)processes.size(); i++) {

        Process p = processes[i];

        ui->tableWidget_queue->setItem(i, 0, new QTableWidgetItem("P" + QString::number(p.id)));
        ui->tableWidget_queue->setItem(i, 1, new QTableWidgetItem(QString::number(p.arrival, 'f', 2)));
        ui->tableWidget_queue->setItem(i, 2, new QTableWidgetItem(QString::number(p.burst, 'f', 2)));
        ui->tableWidget_queue->setItem(i, 3, new QTableWidgetItem("-"));
        ui->tableWidget_queue->setItem(i, 4, new QTableWidgetItem(QString::number(p.remaining, 'f', 2)));
        ui->tableWidget_queue->setItem(i, 5, new QTableWidgetItem(p.status));
    }
}

void MainWindow::calculateAverages() {

    float totalWT = 0;
    float totalTAT = 0;
    int count = 0;

    for (int i = 0; i < (int)processes.size(); i++) {

        if (processes[i].status == "Completed") {

            totalWT += processes[i].wait;
            totalTAT += processes[i].tat;
            count++;
        }
    }

    if (count > 0) {

        ui->lbl_avgWaitTime->setText(QString::number(totalWT / count, 'f', 2));
        ui->lbl_avgTurnaroundTime->setText(QString::number(totalTAT / count, 'f', 2));
    }
}

void MainWindow::updateSchedulerVisibility() {}
