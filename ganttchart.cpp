#include "ganttchart.h"
#include <QPainter>
#include <QFontMetrics>
#include <cmath>

GanttChart::GanttChart(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(100);
}


void GanttChart::addSegment(int pid, float start, float end) {
    segments.append({pid, start, end});
    if (end > maxTime) maxTime = end;
    update();
}

void GanttChart::updateActiveProcess(int pid, float startTime, float endTime) {
    // If the last segment belongs to the same process and there is no gap in time, extend it
    if (!segments.isEmpty() && segments.last().pid == pid && std::abs(segments.last().end - startTime) < 0.01f) {
        segments.last().end = endTime;
    } else {
        // Otherwise (new process or after idle time), append a new segment
        segments.append({pid, startTime, endTime});
    }

    // Update max time and trigger a repaint
    if (endTime > maxTime) {
        maxTime = endTime;
    }
    update();
}

void GanttChart::clear() {
    segments.clear();
    maxTime = 0;
    update();
}

void GanttChart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (segments.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "Gantt Chart will appear here");
        return;
    }

    int height = this->height();
    int barH = 50;
    int y = (height - barH) / 2;

    int segWidth = width() / (maxTime + 1);
    if (segWidth < 1) segWidth = 1;

    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    for (int i = 0; i < segments.size(); i++)
    {
        const auto &seg = segments[i];

        int xStart = seg.start * segWidth;
        int xEnd   = seg.end * segWidth;
        int w      = xEnd - xStart;
        if (w <= 0) continue;

        QColor color;

        if (seg.pid == -1)
            color = Qt::lightGray;   // idle
        else
            // Unique color per process
            color = QColor::fromHsv((seg.pid * 47) % 360, 160, 230);

        painter.setBrush(color);
        painter.setPen(Qt::black);
        painter.drawRect(xStart, y, w, barH);

        if (w > 25)
        {
            QString label;

            if (seg.pid == -1)
                label = "Idle";
            else
                label = QString("P%1").arg(seg.pid);

            painter.setPen(Qt::black);
            painter.drawText(QRect(xStart, y, w, barH),
                             Qt::AlignCenter,
                             label);
        }

        // time ticks below bar
        painter.setPen(Qt::darkGray);

        painter.drawText(xStart,y + barH + 15,QString::number(seg.start));

        // draw end time only for last segment
        if (i == segments.size() - 1)
            painter.drawText(xEnd - 10, y + barH + 15,QString::number(seg.end));
    }
}
