#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QGridLayout>
#include "canvas.h"
#include "SystemDataCenter.h"
#include "CarObject.hpp"
#include "Initializer.hpp"
#include "FuzzyRulesBase.hpp"

typedef enum enMode
{
    SimulateMode, OperateMode
}Mode;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    QGridLayout *m_layout;

    Canvas *m_mainCanvasView;

    QLineEdit *m_edTxt_fileFullPath, *m_edTxt_ruleSetting;
    QPushButton *m_btn_fileSelect, *m_btn_ruleSelect;

    QLabel *m_lb_init_info;

    QLabel *m_lb_title_leftSensor, *m_lb_title_rightSensor, *m_lb_title_frontSensor;
    QLabel *m_lb_leftDistance, *m_lb_rightDistance, *m_lb_frontDistance;
    QLabel *m_lb_title_HD, *m_lb_title_VDR;
    QLabel *m_lb_horizontalDeviate, *m_lb_ValueOfHD;
    QLabel *m_lb_verticalDistanceRemain;
    QLabel *m_lb_title_carPosition, *m_lb_carPosition;
    QLabel *m_lb_title_FuzzyReslut, *m_lb_wheelAngle;

    QPushButton *m_btn_startUntilEnd, *m_btn_startOneUnit, *m_btn_stop, *m_btnStartOver, *m_btn_saveResult;
    QPushButton *m_btn_operateMode;

    vector<QLabel*> m_vAllLabel;
    vector<QLineEdit*> m_vAllEditText;
    vector<QPushButton*> m_vAllButton;
    vector<Canvas*> m_vAllCanvas;

    CarObject *m_carObj;
    QTimer *m_timer;
    SystemDataCenter *m_data;
    Initializer *m_init;

    FuzzyRulesBase *m_rule;

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
    void fuzzySysJudgement();

    Mode m_mode;
    float m_operateWheelAngle;

private slots:
    void setLabel();
    void onFileSelectButtonClick();
    void onRuleSelectButtonClick();
    void onStartToEndButtonClick();
    void onStartOnceButtonClick();
    void onStartOverButtonClick();
    void onStopButtonClick();
    void onTimeOut();
    void onSaveResult();
    void onSwitchMode();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
