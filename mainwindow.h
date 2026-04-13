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
    float remaining;
    float completion = -1.0f;
    float wait = 0.0f;
    float tat = 0.0f;
    float actualStart = -1.0f;  
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
    void runStep();
    void updateTable();
    void calculateAverages();
    void runBatch();
    void finishProcess(int idx);
    void checkNewArrivals();
    void startNextProcess();

    Ui::MainWindow *ui;
    QTimer *timer;
    std::vector<Process> processes;
    std::queue<int> readyQueue;
    int currentIdx = -1;
    float currentTime = 0.0f;
    int processCounter = 0;
    bool isRunning = false;
    GanttChart *ganttChart;
};

#endif 
