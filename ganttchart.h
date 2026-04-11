#ifndef GANTTCHART_H
#define GANTTCHART_H

#include <QWidget>
#include <QVector>

struct GanttSegment {
    int pid;
    int start;
    int end;
};

class GanttChart : public QWidget {
    Q_OBJECT
public:
    explicit GanttChart(QWidget *parent = nullptr);
    void addSegment(int pid, int burst, int start);
    void clear();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QVector<GanttSegment> segments;
    int maxTime = 0;
};

#endif // GANTTCHART_H