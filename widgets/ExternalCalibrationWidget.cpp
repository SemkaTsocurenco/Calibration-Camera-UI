#include "ExternalCalibrationWidget.h"

externalCalibrationWidget::externalCalibrationWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
}

void externalCalibrationWidget::setupUi() {

    squareSize = new QDoubleSpinBox (this);
    squareSize->setRange(0, 1000);     
    squareSize->setValue(60);

    markerSize = new QDoubleSpinBox (this);
    markerSize->setRange(0, 1000);     
    markerSize->setValue(30);


    H_count = new QSpinBox (this);
    H_count->setRange(2, 20);     
    H_count->setValue(4);

    W_count = new QSpinBox (this);
    W_count->setRange(2, 20);     
    W_count->setValue(6);

    
    ArukoDICTSize = new QComboBox(this);
    ArukoDICTSize->addItem("Метка 4х4", cv::aruco::DICT_4X4_50);
    ArukoDICTSize->addItem("Метка 5х5", cv::aruco::DICT_5X5_100);
    ArukoDICTSize->addItem("Метка 6х6", cv::aruco::DICT_6X6_100);
    ArukoDICTSize->addItem("Метка 7х7", cv::aruco::DICT_7X7_50);

    
    CreateCharucoButton = new QPushButton ("Создать файл ChAruco доски" , this );
    CreateCharucoButton-> setCheckable(false);

    StartTwoHandSetCollectorButton = new QPushButton ("Ручное создание сета фотографий" , this );
    StartTwoHandSetCollectorButton-> setCheckable(true);

    StartAutoSetCollectorButton = new QPushButton ("Автоматическое создание сета фотографий" , this );
    StartAutoSetCollectorButton-> setCheckable(true);

    CreateCalibrateFileButton = new QPushButton ("Создать калибровачный файл" , this );
    CreateCalibrateFileButton-> setCheckable(false);


    QLabel* label = new QLabel("Выбор размера ChAruco доски", this);
    QLabel* labelH = new QLabel("H:", this);
    QLabel* labelW = new QLabel("W:", this);

    QHBoxLayout* layout1 = new QHBoxLayout();
    layout1->addWidget(label); // Добавляем заголовок
    layout1->addWidget(labelH); // Добавляем подпись для H
    layout1->addWidget(H_count); // Добавляем поле ввода для H
    layout1->addWidget(labelW); // Добавляем подпись для W
    layout1->addWidget(W_count); // Добавляем поле ввода для W

    QHBoxLayout* layout2 = new QHBoxLayout();
    layout2->addWidget(new QLabel("squareSize:", this)); // Добавляем подпись для H
    layout2->addWidget(squareSize); // Добавляем поле ввода для H
    layout2->addWidget(new QLabel("markerSize:", this)); // Добавляем подпись для W
    layout2->addWidget(markerSize); // Добавляем поле ввода для W



    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    
    mainLayout->addSpacing(20);
    mainLayout->addWidget(new QLabel("Выбор словаря Aruco меток:", this));
    mainLayout->addWidget(ArukoDICTSize);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(CreateCharucoButton);
    mainLayout->addWidget(StartTwoHandSetCollectorButton);
    mainLayout->addWidget(StartAutoSetCollectorButton);
    mainLayout->addWidget(CreateCalibrateFileButton);

    mainLayout->addStretch();

    setLayout(mainLayout);
}

void externalCalibrationWidget::setupConnections() {

    connect(StartTwoHandSetCollectorButton, &QPushButton::toggled, this, &externalCalibrationWidget::StartHandSetCollector);
    connect(StartAutoSetCollectorButton, &QPushButton::toggled, this, &externalCalibrationWidget::StartAutoSetCollector);

    connect(CreateCharucoButton, &QPushButton::clicked, this, &externalCalibrationWidget::onCreateChArukoClicked);
    connect(CreateCalibrateFileButton, &QPushButton::clicked, this, &externalCalibrationWidget::CreateCalibrateFile);
}

void externalCalibrationWidget::StartHandSetCollector(bool checked){
    
}

void externalCalibrationWidget::StartAutoSetCollector(bool checked){

}


void externalCalibrationWidget::onCreateChArukoClicked() {
    emit stop();  
    saveArucoDialog = new ChArucoSaveBoardDialog(this, H_count->value(), W_count->value(), squareSize->value(), markerSize->value(), ArukoDICTSize->currentData().toInt());
    saveArucoDialog->exec();
    emit resumeRequested();
}

void externalCalibrationWidget::CreateCalibrateFile(){
}


// void externalCalibrationWidget::startCalibrate(bool checked){
//     if (checked){
//         int dictType = ArukoDICTSize->currentData().toInt();
//         float size = sizeAruco->value();
//         emit started(dictType, size);  
//     } else {
//         emit stop();
//     }
// }