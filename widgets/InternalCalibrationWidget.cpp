#include "InternalCalibrationWidget.h"

internalCalibrationWidget::internalCalibrationWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
}

void internalCalibrationWidget::setupUi() {
    // Настройка выбора размера ArUco-маркера
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
    mainLayout->addWidget(ArukoDICTSize);
    mainLayout->addWidget(sizeAruco);

    mainLayout->addSpacing(10);
    mainLayout->addWidget(startButton);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(CreateAruko);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void internalCalibrationWidget::setupConnections() {


    connect(startButton, &QPushButton::toggled, this, &internalCalibrationWidget::startCalibrate);
    connect(CreateAruko, &QPushButton::clicked, this, &internalCalibrationWidget::onCreateArukoClicked);
}

void internalCalibrationWidget::onCreateArukoClicked() {
    emit stopped();  
    saveArucoDialog = new ArucoDialog(this, sizeAruco->value());
    saveArucoDialog->exec();
    emit resumeRequested();
}

void internalCalibrationWidget::startCalibrate(bool checked){
    if (checked){
        int dictType = ArukoDICTSize->currentData().toInt();
        float size = sizeAruco->value();
        emit started(dictType, size);  
    } else {
        emit stop();
    }
}