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
    int arrival;
    int burst;
    int remaining;
    int completion = -1;
    int wait = 0;
    int tat = 0;
    QString status = "Waiting";
};

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
    void updateTable();
    void calculateAverages();
    void runBatch();
    void finishProcess(int idx);
    void checkNewArrivals();

    Ui::MainWindow *ui;
    QTimer *timer;
    std::vector<Process> processes;
    std::queue<int> readyQueue; // FCFS Queue
    int currentIdx = -1;
    int currentTime = 0;
    int processCounter = 0;
    bool isRunning = false;
    GanttChart *ganttChart;
};

#endif // MAINWINDOW_H