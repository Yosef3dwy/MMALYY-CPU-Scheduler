#include "ganttchart.h"
#include <QPainter>
#include <QFontMetrics>

GanttChart::GanttChart(QWidget *parent) : QWidget(parent) {
    setMinimumHeight(100);
}

//I have changed the data type of burst, start to float to match function definition
//notice that addSegment in .h file is (int pid, float start, float end)
void GanttChart::addSegment(int pid, float start, float end) {
    segments.append({pid, start, end});
    if (end > maxTime) maxTime = end;
    update();
}

void GanttChart::clear() {
    segments.clear();
    maxTime = 0;
    update();
}

void GanttChart::paintEvent(QPaintEvent *) {
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

    for (const auto &seg : segments) {
        int xStart = seg.start * segWidth;
        int xEnd = seg.end * segWidth;
        int w = xEnd - xStart;
        if (w <= 0) continue;

        // Unique color per process
        QColor color = QColor::fromHsv((seg.pid * 47) % 360, 160, 230);
        painter.setBrush(color);
        painter.setPen(Qt::white);
        painter.drawRect(xStart, y, w, barH);

        painter.setPen(Qt::black);
        QString label = QString("P%1").arg(seg.pid);
        painter.drawText(QRect(xStart, y, w, barH), Qt::AlignCenter, label);

        // Time ticks below bar
        painter.setPen(Qt::darkGray);
        painter.drawText(xStart, y + barH + 15, QString::number(seg.start));
    }
    painter.drawText(segments.last().end * segWidth, y + barH + 15, QString::number(segments.last().end));
}