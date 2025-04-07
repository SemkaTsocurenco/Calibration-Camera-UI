#include "ArucoSaveCalibration.h"

ArucoSaveCalibration::ArucoSaveCalibration(cv::Mat matrix, cv::Mat dist, QWidget *parent)
    : QDialog(parent)
{
    Matrix = matrix;
    Dist = dist;
    setupUi(); // Настраиваем интерфейс
}

void ArucoSaveCalibration::setupUi()
{


    saveButton = new QPushButton("Сохранить матрицу калибровки", this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(saveButton);

    setLayout(layout);

    connect(saveButton, &QPushButton::clicked, this, &ArucoSaveCalibration::saveMatrix);
}

void ArucoSaveCalibration::saveMatrix()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Сохранить файл калибровки",
        QDir::homePath(),
        "Файл калибровки (*.yaml *.xml)"
    );

    if (!filePath.isEmpty()) {
        cv::FileStorage fs(filePath.toStdString(), cv::FileStorage::WRITE);
        if (!fs.isOpened()) {
            std::cerr << "Ошибка открытия файла для сохранения!" << std::endl;
        }
        fs << "cameraMatrix" << Matrix;
        fs << "distCoeffs" << Dist;
        fs.release();
        QMessageBox::information(this, "Сохранение", QString("Файл калибровки сохранён в %1").arg(filePath));

    }
}
