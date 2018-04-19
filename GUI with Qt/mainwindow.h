#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "canvas.h"
#include "SystemDataCenter.h"
#include "CarObject.hpp"
#include "Initializer.hpp"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    Canvas *m_mainCanvasView;

    QLineEdit *m_edTxt_fileFullPath;
    QPushButton *m_btn_fileSelect;

    QLabel *m_lb_title_leftSensor, *m_lb_title_rightSensor, *m_lb_title_frontSensor;
    QLabel *m_lb_leftDistance, *m_lb_rightDistance, *m_lb_frontDistance;
    QLabel *m_lb_title_HD, *m_lb_title_VDR;
    QLabel *m_lb_horizontalDeviate, *m_lb_ValueOfHD;
    QLabel *m_lb_verticalDistanceRemain;
    QLabel *m_lb_title_carPosition, *m_lb_carPosition;

    QPushButton *m_btn_startUntilEnd, *m_btn_startOneUnit, *m_btn_stop, *m_btnStartOver, *m_btn_saveResult;

    vector<QLabel*> m_vAllLabel;
    vector<QLineEdit*> m_vAllEditText;
    vector<QPushButton*> m_vAllButton;
    vector<Canvas*> m_vAllCanvas;

    CarObject *m_carObj;
    QTimer *m_timer;
    SystemDataCenter *m_data;
    Initializer *m_init;

    void keyPressEvent(QKeyEvent *event);

protected:
    void allCompoentInit();
    void gridLayoutSetup();
    void buttonInit();
    void addCompoentToContainer();
    void allCompoentRefresh();
    bool isCarReachEndZone();
    void setMode_Running();
    void setMode_Stop();
    void doMove();
    void dataInit();

private slots:
    void setLabel();
    void onFileSelectButtonClick();
    void onStartToEndButtonClick();
    void onStartOnceButtonClick();
    void onStartOverButtonClick();
    void onStopButtonClick();
    void onTimeOut();
    void onSaveResult();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
