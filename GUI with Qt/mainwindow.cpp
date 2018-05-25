#include "mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QKeyEvent>
#include <fstream>
#include <QCoreApplication>
#include "Initializer.hpp"

using namespace std;

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
    m_layout = NULL;
    m_timer = NULL;
    m_carObj = NULL;
    m_data = NULL;
    m_init = NULL;
    m_rule = NULL;

    m_mode = SimulateMode;

    allCompoentInit();
    buttonInit();
    addCompoentToContainer();
    setLabel();

    gridLayoutSetup();
}

MainWindow::~MainWindow()
{
    while ( m_vAllButton.size() >= 1 )
    {
        QPushButton *obj = m_vAllButton.back();
        m_vAllButton.pop_back();
        if ( obj != NULL )
            delete obj;
    }

    while ( m_vAllCanvas.size() >= 1 )
    {
        Canvas *obj = m_vAllCanvas.back();
        m_vAllCanvas.pop_back();
        if ( obj != NULL )
            delete obj;
    }

    while ( m_vAllEditText.size() >= 1 )
    {
        QLineEdit *obj = m_vAllEditText.back();
        m_vAllEditText.pop_back();
        if ( obj != NULL )
            delete obj;
    }

    while ( m_vAllLabel.size() >= 1 )
    {
        QLabel *obj = m_vAllLabel.back();
        m_vAllLabel.pop_back();
        if ( obj != NULL )
            delete obj;
    }

    if ( m_layout != NULL )
        delete m_layout;

    if ( m_timer != NULL )
        delete m_timer;

    if ( m_carObj != NULL )
        delete m_carObj;

    if ( m_data != NULL )
        delete m_data;

    if ( m_init != NULL )
        delete m_init;

    if ( m_rule != NULL )
        delete m_rule;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_L )
    {
        if ( m_mode == SimulateMode )
            return;

        m_operateWheelAngle -= 0.5;
        m_operateWheelAngle = (m_operateWheelAngle < WHEEL_ANGLE_LEFT_MAX) ? WHEEL_ANGLE_LEFT_MAX : m_operateWheelAngle;
        m_lb_wheelAngle->setText(QString(QString::number(m_operateWheelAngle)));
    }
    else if ( event->key() == Qt::Key_R )
    {
        if ( m_mode == SimulateMode )
            return;

        m_operateWheelAngle += 0.5;
        m_operateWheelAngle = (m_operateWheelAngle > WHEEL_ANGLE_RIGHT_MAX) ? WHEEL_ANGLE_RIGHT_MAX : m_operateWheelAngle;
        m_lb_wheelAngle->setText(QString(QString::number(m_operateWheelAngle)));
    }
    else if ( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
        if ( m_mode == SimulateMode )
            return;

        if ( m_btn_startOneUnit->isEnabled() )
        {
            m_data->m_carWheelAngle = Degree(m_operateWheelAngle);
            m_carObj->m_lastFuzzyRuleResult_wheelAngle = m_operateWheelAngle;
            m_carObj->move();
            allCompoentRefresh();

            if ( isCarReachEndZone() == true )
            {
                m_btn_startUntilEnd->setEnabled(false);
                m_btn_startOneUnit->setEnabled(false);
                m_btn_stop->setEnabled(false);
                m_timer->stop();
                m_btnStartOver->setEnabled(true);
                m_btn_saveResult->setEnabled(true);
                m_btn_operateMode->setEnabled(true);
            }
        }
    }
    else if ( event->key() == Qt::Key_Space )
    {
        if ( m_mode == OperateMode )
            return;

        if ( m_btn_startOneUnit->isEnabled() )
        {
            onStartOnceButtonClick();
        }
    }
}

void MainWindow::allCompoentInit()
{
    m_data = new SystemDataCenter();
    m_rule = new FuzzyRulesBase();
    m_carObj = new CarObject(m_data, m_rule);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

    m_edTxt_fileFullPath = new QLineEdit("請點選左方按鈕以輸入檔案");
    m_edTxt_fileFullPath->setReadOnly(true);

    m_edTxt_ruleSetting = new QLineEdit("模糊規則設定：預設");
    m_edTxt_ruleSetting->setReadOnly(true);

    m_btn_fileSelect = new QPushButton(QString("載入車體及軌道資訊"));
    m_btn_ruleSelect = new QPushButton(QString("載入模糊規則"));

    m_lb_init_info = new QLabel(QString("注意：\n車體及軌道資訊載入完成後即結束初始化"));

    m_mainCanvasView = new Canvas();

    m_lb_title_leftSensor = new QLabel(QString("左方感測器"));
    m_lb_title_rightSensor = new QLabel(QString("右方感測器"));
    m_lb_title_frontSensor = new QLabel(QString("前方感測器"));
    m_lb_leftDistance = new QLabel(QString("--"));
    m_lb_rightDistance = new QLabel(QString("--"));
    m_lb_frontDistance = new QLabel(QString("--"));
    m_lb_title_HD = new QLabel(QString("水平偏向"));
    m_lb_title_VDR = new QLabel(QString("垂直剩餘距離"));
    m_lb_horizontalDeviate = new QLabel(QString("--"));
    m_lb_verticalDistanceRemain = new QLabel(QString("--"));
    m_lb_ValueOfHD = new QLabel(QString("--"));
    m_lb_title_carPosition = new QLabel(QString("車體中心位置"));
    m_lb_carPosition = new QLabel(QString("--"));
    m_lb_title_FuzzyReslut = new QLabel(QString("模糊系統輸出"));
    m_lb_wheelAngle = new QLabel(QString("--"));

    m_btn_startUntilEnd = new QPushButton(QString("啟動直達終點"));
    m_btn_startOneUnit = new QPushButton(QString("單次啟動(space)"));
    m_btn_stop = new QPushButton(QString("停止"));
    m_btn_operateMode = new QPushButton(QString("切換為操作模式"));
    m_btnStartOver = new QPushButton(QString("重新開始"));
    m_btn_saveResult = new QPushButton(QString("輸出、儲存紀錄"));
}

void MainWindow::gridLayoutSetup()
{
    m_layout = new QGridLayout;

    m_layout->addWidget(m_mainCanvasView, 0, 0, 12, 5, 0);

    m_layout->addWidget(m_btn_ruleSelect, 0, 5, 1, 1, 0);
    m_layout->addWidget(m_edTxt_ruleSetting, 0, 6, 1, 2, 0);

    m_layout->addWidget(m_btn_fileSelect, 1, 5, 1, 1, 0);
    m_layout->addWidget(m_edTxt_fileFullPath, 1, 6, 1, 2, 0);

    m_layout->addWidget(m_lb_init_info, 2, 5, 1, 3, 0);

    m_layout->addWidget(m_lb_title_leftSensor, 3, 5, 1, 1, 0);
    m_layout->addWidget(m_lb_title_rightSensor, 4, 5, 1, 1, 0);
    m_layout->addWidget(m_lb_title_frontSensor, 5, 5, 1, 1, 0);

    m_layout->addWidget(m_lb_leftDistance, 3, 6, 1, 1, 0);
    m_layout->addWidget(m_lb_rightDistance, 4, 6, 1, 1, 0);
    m_layout->addWidget(m_lb_frontDistance, 5, 6, 1, 2, 0);
    m_layout->addWidget(m_lb_ValueOfHD, 3, 7, 2, 1, 0);

    m_layout->addWidget(m_lb_title_HD, 6, 5, 1, 1, 0);
    m_layout->addWidget(m_lb_title_VDR, 7, 5, 1, 1, 0);

    m_layout->addWidget(m_lb_horizontalDeviate, 6, 6, 1, 2, 0);
    m_layout->addWidget(m_lb_verticalDistanceRemain, 7, 6, 1, 2, 0);

    m_layout->addWidget(m_lb_title_carPosition, 8, 5, 1, 1, 0);
    m_layout->addWidget(m_lb_carPosition, 8, 6, 1, 2, 0);

    m_layout->addWidget(m_lb_title_FuzzyReslut, 9, 5, 1, 1, 0);
    m_layout->addWidget(m_lb_wheelAngle, 9, 6, 1, 2, 0);

    m_layout->addWidget(m_btn_startUntilEnd, 10, 5, 1, 1, 0);
    m_layout->addWidget(m_btn_startOneUnit, 11, 5, 1, 1, 0);

    m_layout->addWidget(m_btn_operateMode, 10, 6, 1, 1, 0);
    m_layout->addWidget(m_btn_stop, 11, 6, 1, 1, 0);

    m_layout->addWidget(m_btnStartOver, 10, 7, 1, 1, 0);
    m_layout->addWidget(m_btn_saveResult, 11, 7, 1, 1, 0);

    setLayout(m_layout);
}

void MainWindow::buttonInit()
{
    connect(m_btn_fileSelect, SIGNAL(clicked()), SLOT(onFileSelectButtonClick()));
    connect(m_btn_ruleSelect, SIGNAL(clicked()), SLOT(onRuleSelectButtonClick()));
    connect(m_btn_startUntilEnd, SIGNAL(clicked()), SLOT(onStartToEndButtonClick()));
    connect(m_btn_startOneUnit, SIGNAL(clicked()), SLOT(onStartOnceButtonClick()));
    connect(m_btn_stop, SIGNAL(clicked()), SLOT(onStopButtonClick()));
    connect(m_btnStartOver, SIGNAL(clicked()), SLOT(onStartOverButtonClick()));
    connect(m_btn_saveResult, SIGNAL(clicked()), SLOT(onSaveResult()));
    connect(m_btn_operateMode, SIGNAL(clicked()), SLOT(onSwitchMode()));

    m_btn_startUntilEnd->setEnabled(false);
    m_btn_startOneUnit->setEnabled(false);
    m_btn_stop->setEnabled(false);
    m_btnStartOver->setEnabled(false);
    m_btn_saveResult->setEnabled(false);
    m_btn_operateMode->setEnabled(false);
}

void MainWindow::addCompoentToContainer()
{
    m_vAllLabel.clear();
    m_vAllLabel.push_back(m_lb_title_leftSensor);
    m_vAllLabel.push_back(m_lb_title_rightSensor);
    m_vAllLabel.push_back(m_lb_title_frontSensor);
    m_vAllLabel.push_back(m_lb_leftDistance);
    m_vAllLabel.push_back(m_lb_rightDistance);
    m_vAllLabel.push_back(m_lb_frontDistance);
    m_vAllLabel.push_back(m_lb_title_HD);
    m_vAllLabel.push_back(m_lb_title_VDR);
    m_vAllLabel.push_back(m_lb_horizontalDeviate);
    m_vAllLabel.push_back(m_lb_verticalDistanceRemain);
    m_vAllLabel.push_back(m_lb_ValueOfHD);
    m_vAllLabel.push_back(m_lb_title_carPosition);
    m_vAllLabel.push_back(m_lb_carPosition);
    m_vAllLabel.push_back(m_lb_title_FuzzyReslut);
    m_vAllLabel.push_back(m_lb_wheelAngle);
    m_vAllLabel.push_back(m_lb_init_info);

    m_vAllEditText.clear();
    m_vAllEditText.push_back(m_edTxt_fileFullPath);
    m_vAllEditText.push_back(m_edTxt_ruleSetting);

    m_vAllCanvas.clear();
    m_vAllCanvas.push_back(m_mainCanvasView);

    m_vAllButton.clear();
    m_vAllButton.push_back(m_btn_fileSelect);
    m_vAllButton.push_back(m_btn_ruleSelect);
    m_vAllButton.push_back(m_btn_startUntilEnd);
    m_vAllButton.push_back(m_btn_startOneUnit);
    m_vAllButton.push_back(m_btn_stop);
    m_vAllButton.push_back(m_btnStartOver);
    m_vAllButton.push_back(m_btn_saveResult);
    m_vAllButton.push_back(m_btn_operateMode);
}

void MainWindow::dataInit()
{
    if ( m_data->m_vTrackBorderInfo.empty() )
        m_data->m_vTrackBorderInfo = m_init->getTrackInfo();

    if ( m_data->m_vEndZoneBorderInfo.empty() )
        m_data->m_vEndZoneBorderInfo = m_init->getEndZoneInfo();

    m_data->m_carAngle = m_init->getCarInitAngle();
    m_data->m_carPosition = m_init->getCarInitCoordinate();

    QString strCarPos = QString("X = ") + QString::number(m_data->m_carPosition.x) + QString("\nY = ") + QString::number(m_data->m_carPosition.y);
    m_lb_carPosition->setText(strCarPos);

    m_data->clearCarRecord();
    m_mainCanvasView->setDataCenter(m_data);

    allCompoentRefresh();
    setMode_Stop();
    m_btn_fileSelect->setEnabled(false);
    m_btn_ruleSelect->setEnabled(false);

    fuzzySysJudgement();
}

void MainWindow::fuzzySysJudgement()
{
    m_carObj->autopilotSystemTackControl();

    float degree = m_carObj->m_lastFuzzyRuleResult_wheelAngle;
    QString str;

    str = (degree < 0) ? QString("左轉\n") : QString("右轉\n");
    degree = (degree < 0) ? -1.0*degree : degree;

    str += QString::number((degree), 'f', 2)+QString(" 度");

    m_lb_wheelAngle->setText(str);
}

void MainWindow::onFileSelectButtonClick()
{
    QString strOutput = QFileDialog::getOpenFileName(this, QString("select input file"), "", QString("Text Files (*.txt)"));

    m_init = new Initializer(strOutput.toStdString());
    initStatus status = m_init->initCheck();
    if ( status == TI_NO_ERROR )
    {
        dataInit();
    }
    else
    {
        strOutput = QString("init fail, error code : " + QString::number((int)status));
    }

    m_edTxt_fileFullPath->setText(strOutput);
}

void MainWindow::onRuleSelectButtonClick()
{
    QString strPath = QFileDialog::getOpenFileName(this, QString("select input file"), "", QString("Text Files (*.txt)"));
    bool bFlag = m_rule->loadRuleSetting(strPath.toStdString());
    m_edTxt_ruleSetting->setText(bFlag==true?"成功載入設定":"載入設定失敗，將套用預設設定");
}

void MainWindow::onStartToEndButtonClick()
{
    setMode_Running();
    doMove();
    m_timer->start(200);
}

void MainWindow::onStartOnceButtonClick()
{
    if ( m_mode == OperateMode )
    {
        QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        keyPressEvent(event);
        delete event;
        return;
    }
    doMove();
}

void MainWindow::onStopButtonClick()
{
    m_timer->stop();
    setMode_Stop();
}

void MainWindow::onStartOverButtonClick()
{
    m_btnStartOver->setEnabled(false);
    m_btn_saveResult->setEnabled(false);
    m_btn_operateMode->setEnabled(false);
    dataInit();
}

void MainWindow::onTimeOut()
{
    doMove();
}

void MainWindow::onSaveResult()
{
    QString fileFullPath6 = QFileDialog::getSaveFileName(this, QString("Save File"), "C:/train6D.txt", QString("Text (*.txt)"));
    QString fileFullPath4 = QFileDialog::getSaveFileName(this, QString("Save File"), "C:/train4D.txt", QString("Text (*.txt)"));

    ofstream ofs6(fileFullPath6.toStdString().c_str(), ios::out);
    ofstream ofs4(fileFullPath4.toStdString().c_str(), ios::out);

    for ( vector<Record>::iterator it=m_data->m_vRecord.begin() ; it != m_data->m_vRecord.end() ; it++ )
    {
        Record rec = (*it);
        string str1 = QString::number(rec.pt.x, 'f', 7).toStdString();
        string str2 = QString::number(rec.pt.y, 'f', 7).toStdString();
        string str3 = QString::number(rec.distanceF, 'f', 7).toStdString();
        string str4 = QString::number(rec.distanceR, 'f', 7).toStdString();
        string str5 = QString::number(rec.distanceL, 'f', 7).toStdString();
        string str6 = QString::number(rec.wheelAngle, 'f', 7).toStdString();

        ofs6 << str1 << " " << str2 << " " << str3 << " " << str4 << " " << str5 << " " << str6 << endl;
        ofs4 << str3 << " " << str4 << " " << str5 << " " << str6 << endl;
    }
}

void MainWindow::onSwitchMode()
{
    onStartOverButtonClick();

    switch ( m_mode )
    {
    case SimulateMode:
        m_mode = OperateMode;
        m_btn_startOneUnit->setText(QString("單次啟動(enter)"));
        m_btn_startUntilEnd->setEnabled(false);
        m_lb_title_FuzzyReslut->setText(QString("使用者操作方向盤角度\n使用按鍵 L R 操作"));
        m_btn_operateMode->setText(QString("切換為模擬模式"));
        m_operateWheelAngle = 0.0;
        m_lb_wheelAngle->setText(QString(QString::number(m_operateWheelAngle)));
        break;
    case OperateMode:
        m_mode = SimulateMode;
        m_btn_startOneUnit->setText(QString("單次啟動(space)"));
        m_btn_startUntilEnd->setEnabled(true);
        m_lb_title_FuzzyReslut->setText(QString("模糊系統輸出"));
        m_btn_operateMode->setText(QString("切換為操作模式"));
        break;
    }
}

void MainWindow::doMove()
{
    m_carObj->move();
    allCompoentRefresh();

    if ( isCarReachEndZone() == true || m_carObj->collisionCheck() )
    {
        m_btn_startUntilEnd->setEnabled(false);
        m_btn_startOneUnit->setEnabled(false);
        m_btn_stop->setEnabled(false);
        m_timer->stop();
        m_btnStartOver->setEnabled(true);
        m_btn_saveResult->setEnabled(true);
        m_btn_operateMode->setEnabled(true);
    }

    // 移動完後馬上啟動判斷，讓新的輸出角度先顯示在畫面上.
    fuzzySysJudgement();
}

void MainWindow::setMode_Running()
{
    m_btn_startUntilEnd->setEnabled(false);
    m_btn_startOneUnit->setEnabled(false);
    m_btn_stop->setEnabled(true);
}

void MainWindow::setMode_Stop()
{
    m_btn_startUntilEnd->setEnabled(true);
    m_btn_startOneUnit->setEnabled(true);
    m_btn_stop->setEnabled(false);
}

void MainWindow::setLabel()
{
    for ( vector<QLabel*>::iterator it=m_vAllLabel.begin() ; it != m_vAllLabel.end() ; it++ )
    {
        QLabel *label = (*it);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("border-width: 1px; border-style: solid; border-color: rgb(181, 181, 181);");

        QFont font("Arial", 14, QFont::Bold);
        label->setFont(font);
    }
}

void MainWindow::allCompoentRefresh()
{
    Geometry groObj;

    QString strCarPos = QString("X = ") + QString::number(m_data->m_carPosition.x, 'f', 4) + QString("\nY = ") + QString::number(m_data->m_carPosition.y, 'f', 4);
    m_lb_carPosition->setText(strCarPos);

    // left
    m_data->m_intersectPt_L = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() + 45.0);
    float disL = groObj.getDistance(m_data->m_intersectPt_L, m_data->m_carPosition);
    m_lb_leftDistance->setText(QString::number(disL, 'f', 4));

    // right
    m_data->m_intersectPt_R = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() - 45.0);
    float disR = groObj.getDistance(m_data->m_intersectPt_R, m_data->m_carPosition);
    m_lb_rightDistance->setText(QString::number(disR, 'f', 4));

    // front
    m_data->m_intersectPt_F = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree());
    float disF = groObj.getDistance(m_data->m_intersectPt_F, m_data->m_carPosition);
    m_lb_frontDistance->setText(QString::number(disF, 'f', 4));

    // HD, value
    float disHD = disL-disR;
    if ( disHD < 0 )
        m_lb_ValueOfHD->setText(QString("偏左\n\n"+QString::number(fabs(disHD), 'f', 4)));
    else if ( disHD > 0 )
        m_lb_ValueOfHD->setText(QString("偏右\n\n"+QString::number(fabs(disHD), 'f', 4)));
    else
        m_lb_ValueOfHD->setText(QString("無水平偏移"));

    // HD, rule
    string str="";
    vector<HorizontalDeviate> vHD = m_rule->search_if_XisA(disL-disR);
    for ( vector<HorizontalDeviate>::iterator itHD=vHD.begin() ; itHD != vHD.end() ; itHD++ )
    {
        HorizontalDeviate A = (*itHD);
        str += m_rule->getDebugString_HD(A);
        if ( itHD+1 != vHD.end() )
            str += "\n";
    }
    m_lb_horizontalDeviate->setText(QString::fromStdString(str));

    // VDR
    str = "";
    vector<VerticalDistanceRemain> vVDR = m_rule->search_if_YisB(disF);
    for ( vector<VerticalDistanceRemain>::iterator itVDR=vVDR.begin() ; itVDR != vVDR.end() ; itVDR++ )
    {
        VerticalDistanceRemain B = (*itVDR);
        str += m_rule->getDebugString_VDR(B);
        if ( itVDR+1 != vVDR.end() )
            str += "\n";
    }
    m_lb_verticalDistanceRemain->setText(QString::fromStdString(str));

    // add record
    Record rec = Record(m_data->m_carPosition, disL, disR, disF, m_carObj->m_lastFuzzyRuleResult_wheelAngle);
    m_data->addCarNewRecord(rec);

    m_mainCanvasView->repaint();
}

bool MainWindow::isCarReachEndZone()
{
    float xMax=-1000, xMin=1000, yMax=-1000, yMin=1000;

    for ( vector<Point>::iterator it=m_data->m_vEndZoneBorderInfo.begin() ; it != m_data->m_vEndZoneBorderInfo.end() ; it++ )
    {
        Point pt = (*it);

        if ( pt.x > xMax )
            xMax = pt.x;

        if ( pt.x < xMin )
            xMin = pt.x;

        if ( pt.y > yMax )
            yMax = pt.y;

        if ( pt.y < yMin )
            yMin = pt.y;
    }

    if ( m_data->m_carPosition.x >= xMin && m_data->m_carPosition.x <= xMax )
    {
        if ( m_data->m_carPosition.y >= yMin && m_data->m_carPosition.y <= yMax )
            return true;
    }

    return false;
}
