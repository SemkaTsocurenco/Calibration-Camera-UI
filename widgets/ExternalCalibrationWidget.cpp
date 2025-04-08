#include "externalCalibrationWidget.h"

externalCalibrationWidget::externalCalibrationWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
}

void externalCalibrationWidget::setupUi() {

    squareSize = new QSpinBox (this);
    squareSize->setRange(0, 1);     
    squareSize->setValue(0.3);

    markerSize = new QSpinBox (this);
    markerSize->setRange(0, 1);     
    markerSize->setValue(0.3);


    H_count = new QSpinBox (this);
    H_count->setRange(2, 20);     
    H_count->setValue(50);

    W_count = new QSpinBox (this);
    W_count->setRange(0, 1);     
    W_count->setValue(0.3);

    
    ArukoDICTSize = new QComboBox(this);
    ArukoDICTSize->addItem("Метка 4х4", cv::aruco::DICT_4X4_50);
    ArukoDICTSize->addItem("Метка 5х5", cv::aruco::DICT_5X5_100);
    ArukoDICTSize->addItem("Метка 6х6", cv::aruco::DICT_6X6_100);
    ArukoDICTSize->addItem("Метка 7х7", cv::aruco::DICT_7X7_50);


    startButton = new QPushButton("Начать калибровку", this);
    startButton->setCheckable(true);
    

    CreateAruko = new QPushButton("Создать ArUco-маркеры", this);

    sizeAruco = new QSpinBox (this);
    sizeAruco->setRange(0, 1000);     
    sizeAruco->setValue(200);     


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("Выбор размера ArUco-маркера:", this));
    mainLayout->addSpacing(20);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void externalCalibrationWidget::setupConnections() {


    connect(startButton, &QPushButton::toggled, this, &externalCalibrationWidget::startCalibrate);
    connect(CreateAruko, &QPushButton::clicked, this, &externalCalibrationWidget::onCreateArukoClicked);
}

void externalCalibrationWidget::onCreateArukoClicked() {
    emit stopped();  
    saveArucoDialog = new ArucoDialog(this, sizeAruco->value());
    saveArucoDialog->exec();
    emit resumeRequested();
}

void externalCalibrationWidget::startCalibrate(bool checked){
    if (checked){
        int dictType = ArukoDICTSize->currentData().toInt();
        float size = sizeAruco->value();
        emit started(dictType, size);  
    } else {
        emit stop();
    }
}