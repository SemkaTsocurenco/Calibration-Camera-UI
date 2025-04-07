#include "InternalCalibrationWidget.h"

internalCalibrationWidget::internalCalibrationWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
}

void internalCalibrationWidget::setupUi() {
    // Настройка выбора размера ArUco-маркера
    ArukoSize = new QComboBox(this);
    ArukoSize->addItem("Метка 4х4", cv::aruco::DICT_4X4_50);
    ArukoSize->addItem("Метка 5х5", cv::aruco::DICT_5X5_100);
    ArukoSize->addItem("Метка 6х6", cv::aruco::DICT_6X6_100);
    ArukoSize->addItem("Метка 7х7", cv::aruco::DICT_7X7_50);

    // Кнопки
    Reload = new QPushButton("Перезагрузить калибровку", this);
    SaveCalibrate = new QPushButton("Сохранить результаты калибровки", this);
    CreateAruko = new QPushButton("Создать ArUco-маркеры", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("Выбор размера ArUco-маркера:", this));
    mainLayout->addWidget(ArukoSize);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(Reload);
    mainLayout->addWidget(SaveCalibrate);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(CreateAruko);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void internalCalibrationWidget::setupConnections() {
    connect(CreateAruko, &QPushButton::clicked, this, &internalCalibrationWidget::onCreateArukoClicked);
    connect(Reload, &QPushButton::clicked, this, &internalCalibrationWidget::onReloadClicked);
    connect(SaveCalibrate, &QPushButton::clicked, this, &internalCalibrationWidget::onSaveCalibrateClicked);
}

void internalCalibrationWidget::onCreateArukoClicked() {
    emit stopped();  
    saveArucoDialog = new ArucoDialog(this);
    saveArucoDialog->exec();
    emit resumeRequested();
}

void internalCalibrationWidget::onReloadClicked() {
    QMessageBox::information(this, "Перезагрузка", "Калибровка была перезагружена!");
    // здесь добавь функционал перезагрузки калибровки
    emit resumeRequested();
}

void internalCalibrationWidget::onSaveCalibrateClicked() {
    emit stopped();  
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Сохранить файл калибровки",
        QDir::homePath(),
        "Файл калибровки (*.yaml *.xml)"
    );

    if (!filePath.isEmpty()) {
        QMessageBox::information(this, "Сохранение", QString("Файл калибровки сохранён в %1").arg(filePath));
        //  сохранение файла калибровки
    }
    emit resumeRequested();
}
