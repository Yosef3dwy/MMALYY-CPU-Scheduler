#ifndef GANTTCHART_H
#define GANTTCHART_H

#include <QWidget>
#include <QVector>

struct GanttSegment {
    int pid;
    float start;
    float end;
};

class GanttChart : public QWidget {
    Q_OBJECT
public:
    explicit GanttChart(QWidget *parent = nullptr);
    void addSegment(int pid, float start, float end);
    void updateActiveProcess(int pid, float startTime, float endTime);
    void clear();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QVector<GanttSegment> segments;
    float maxTime = 0.0f;
};

#endif
