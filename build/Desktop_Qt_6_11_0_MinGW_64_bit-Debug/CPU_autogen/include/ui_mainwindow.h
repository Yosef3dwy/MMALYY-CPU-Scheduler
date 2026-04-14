/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *AddNewProcess;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QSpinBox *spinBox_burstTime;
    QSpinBox *spinBox_priority;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *spinBox_arrivalTime;
    QLabel *label;
    QPushButton *btn_addProcess;
    QSpinBox *spinBox_quantum;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget_queue;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_5;
    QRadioButton *rb_FCFS;
    QRadioButton *rb_SJF;
    QRadioButton *rb_priority;
    QRadioButton *rb_roundRobin;
    QCheckBox *chk_preemptive;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *rb_modeLive;
    QRadioButton *rb_modeExistingOnly;
    QVBoxLayout *verticalLayout_4;
    QPushButton *btn_start;
    QPushButton *btn_pause;
    QPushButton *btn_reset;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_6;
    QScrollArea *scrollArea_gantt;
    QWidget *widget_ganttCanvas;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_5;
    QLabel *lbl_avgWaitTime;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer;
    QLabel *label_6;
    QLabel *lbl_avgTurnaroundTime;
    QLabel *label_10;
    QSpacerItem *horizontalSpacer_2;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(777, 758);
        QIcon icon;
        icon.addFile(QString::fromUtf8("Icon.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        AddNewProcess = new QGroupBox(centralwidget);
        AddNewProcess->setObjectName("AddNewProcess");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AddNewProcess->sizePolicy().hasHeightForWidth());
        AddNewProcess->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(AddNewProcess);
        gridLayout->setObjectName("gridLayout");
        label_4 = new QLabel(AddNewProcess);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 0, 2, 1, 1);

        spinBox_burstTime = new QSpinBox(AddNewProcess);
        spinBox_burstTime->setObjectName("spinBox_burstTime");

        gridLayout->addWidget(spinBox_burstTime, 1, 0, 1, 1);

        spinBox_priority = new QSpinBox(AddNewProcess);
        spinBox_priority->setObjectName("spinBox_priority");

        gridLayout->addWidget(spinBox_priority, 1, 4, 1, 1);

        label_2 = new QLabel(AddNewProcess);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_3 = new QLabel(AddNewProcess);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 0, 4, 1, 1);

        spinBox_arrivalTime = new QSpinBox(AddNewProcess);
        spinBox_arrivalTime->setObjectName("spinBox_arrivalTime");

        gridLayout->addWidget(spinBox_arrivalTime, 1, 1, 1, 1);

        label = new QLabel(AddNewProcess);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 1, 1, 1);

        btn_addProcess = new QPushButton(AddNewProcess);
        btn_addProcess->setObjectName("btn_addProcess");
        btn_addProcess->setMinimumSize(QSize(0, 45));

        gridLayout->addWidget(btn_addProcess, 2, 1, 1, 2);

        spinBox_quantum = new QSpinBox(AddNewProcess);
        spinBox_quantum->setObjectName("spinBox_quantum");

        gridLayout->addWidget(spinBox_quantum, 1, 2, 1, 1);


        verticalLayout->addWidget(AddNewProcess);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        tableWidget_queue = new QTableWidget(groupBox_2);
        if (tableWidget_queue->columnCount() < 6)
            tableWidget_queue->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_queue->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_queue->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_queue->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_queue->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_queue->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_queue->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget_queue->setObjectName("tableWidget_queue");
        tableWidget_queue->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_2->addWidget(tableWidget_queue);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        horizontalLayout = new QHBoxLayout(groupBox_3);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        rb_FCFS = new QRadioButton(groupBox_3);
        buttonGroup = new QButtonGroup(MainWindow);
        buttonGroup->setObjectName("buttonGroup");
        buttonGroup->addButton(rb_FCFS);
        rb_FCFS->setObjectName("rb_FCFS");
        rb_FCFS->setChecked(true);

        verticalLayout_5->addWidget(rb_FCFS);

        rb_SJF = new QRadioButton(groupBox_3);
        buttonGroup->addButton(rb_SJF);
        rb_SJF->setObjectName("rb_SJF");

        verticalLayout_5->addWidget(rb_SJF);

        rb_priority = new QRadioButton(groupBox_3);
        buttonGroup->addButton(rb_priority);
        rb_priority->setObjectName("rb_priority");

        verticalLayout_5->addWidget(rb_priority);

        rb_roundRobin = new QRadioButton(groupBox_3);
        buttonGroup->addButton(rb_roundRobin);
        rb_roundRobin->setObjectName("rb_roundRobin");

        verticalLayout_5->addWidget(rb_roundRobin);

        chk_preemptive = new QCheckBox(groupBox_3);
        chk_preemptive->setObjectName("chk_preemptive");

        verticalLayout_5->addWidget(chk_preemptive);


        horizontalLayout->addLayout(verticalLayout_5);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        rb_modeLive = new QRadioButton(groupBox_3);
        buttonGroup_2 = new QButtonGroup(MainWindow);
        buttonGroup_2->setObjectName("buttonGroup_2");
        buttonGroup_2->addButton(rb_modeLive);
        rb_modeLive->setObjectName("rb_modeLive");
        rb_modeLive->setChecked(true);

        verticalLayout_3->addWidget(rb_modeLive);

        rb_modeExistingOnly = new QRadioButton(groupBox_3);
        buttonGroup_2->addButton(rb_modeExistingOnly);
        rb_modeExistingOnly->setObjectName("rb_modeExistingOnly");

        verticalLayout_3->addWidget(rb_modeExistingOnly);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        btn_start = new QPushButton(groupBox_3);
        btn_start->setObjectName("btn_start");

        verticalLayout_4->addWidget(btn_start);

        btn_pause = new QPushButton(groupBox_3);
        btn_pause->setObjectName("btn_pause");

        verticalLayout_4->addWidget(btn_pause);

        btn_reset = new QPushButton(groupBox_3);
        btn_reset->setObjectName("btn_reset");

        verticalLayout_4->addWidget(btn_reset);


        horizontalLayout->addLayout(verticalLayout_4);


        verticalLayout->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName("groupBox_4");
        verticalLayout_6 = new QVBoxLayout(groupBox_4);
        verticalLayout_6->setObjectName("verticalLayout_6");
        scrollArea_gantt = new QScrollArea(groupBox_4);
        scrollArea_gantt->setObjectName("scrollArea_gantt");
        scrollArea_gantt->setFrameShape(QFrame::Shape::NoFrame);
        scrollArea_gantt->setWidgetResizable(true);
        widget_ganttCanvas = new QWidget();
        widget_ganttCanvas->setObjectName("widget_ganttCanvas");
        widget_ganttCanvas->setGeometry(QRect(0, 0, 727, 83));
        horizontalLayout_3 = new QHBoxLayout(widget_ganttCanvas);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        scrollArea_gantt->setWidget(widget_ganttCanvas);

        verticalLayout_6->addWidget(scrollArea_gantt);


        verticalLayout->addWidget(groupBox_4);

        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName("groupBox_5");
        horizontalLayout_2 = new QHBoxLayout(groupBox_5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName("label_5");
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        label_5->setFont(font);

        horizontalLayout_2->addWidget(label_5);

        lbl_avgWaitTime = new QLabel(groupBox_5);
        lbl_avgWaitTime->setObjectName("lbl_avgWaitTime");
        lbl_avgWaitTime->setFont(font);

        horizontalLayout_2->addWidget(lbl_avgWaitTime);

        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName("label_8");
        label_8->setFont(font);

        horizontalLayout_2->addWidget(label_8);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_6 = new QLabel(groupBox_5);
        label_6->setObjectName("label_6");
        label_6->setFont(font);

        horizontalLayout_2->addWidget(label_6);

        lbl_avgTurnaroundTime = new QLabel(groupBox_5);
        lbl_avgTurnaroundTime->setObjectName("lbl_avgTurnaroundTime");
        lbl_avgTurnaroundTime->setFont(font);

        horizontalLayout_2->addWidget(lbl_avgTurnaroundTime);

        label_10 = new QLabel(groupBox_5);
        label_10->setObjectName("label_10");
        label_10->setFont(font);

        horizontalLayout_2->addWidget(label_10);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addWidget(groupBox_5);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "CPU Scheduler Simulator", nullptr));
        AddNewProcess->setTitle(QCoreApplication::translate("MainWindow", "Add New Process", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Time Quantum", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Burst Time", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Priority", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Arrival Time", nullptr));
        btn_addProcess->setText(QCoreApplication::translate("MainWindow", "Add Process", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Process Queue & Live Status", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_queue->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "PID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_queue->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Arrival", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_queue->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Burst", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_queue->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Priority", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_queue->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Remaining Burst", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_queue->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Scheduler Control Desk", nullptr));
        rb_FCFS->setText(QCoreApplication::translate("MainWindow", "FCFS", nullptr));
        rb_SJF->setText(QCoreApplication::translate("MainWindow", "SJF", nullptr));
        rb_priority->setText(QCoreApplication::translate("MainWindow", "Priority", nullptr));
        rb_roundRobin->setText(QCoreApplication::translate("MainWindow", "Round Robin", nullptr));
        chk_preemptive->setText(QCoreApplication::translate("MainWindow", "Preemptive Mode", nullptr));
        rb_modeLive->setText(QCoreApplication::translate("MainWindow", "Live Simulation", nullptr));
        rb_modeExistingOnly->setText(QCoreApplication::translate("MainWindow", "Exisiting processes only", nullptr));
        btn_start->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        btn_pause->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        btn_reset->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Live Gantt Chart", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Average Waiting Time:", nullptr));
        lbl_avgWaitTime->setText(QCoreApplication::translate("MainWindow", "0.00", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "units", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Average Turnaround Time:", nullptr));
        lbl_avgTurnaroundTime->setText(QCoreApplication::translate("MainWindow", "0.00", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "units", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
