#include "ArucoSaveMarkersDialog.h"

ArucoSaveMarkersDialog::ArucoSaveMarkersDialog(QWidget *parent, float size)
    : QDialog(parent)
{
    sizeMarkers = size;
    setupUi(); // Настраиваем интерфейс
}

void ArucoSaveMarkersDialog::setupUi()
{
    // Создание элементов интерфейса
    sizeComboBox = new QComboBox(this);
    sizeComboBox->addItem("Метки 4х4", cv::aruco::DICT_4X4_50);
    sizeComboBox->addItem("Метки 5х5", cv::aruco::DICT_5X5_100);
    sizeComboBox->addItem("Метки 6х6", cv::aruco::DICT_6X6_100);
    sizeComboBox->addItem("Метки 7х7", cv::aruco::DICT_7X7_50);

    saveButton = new QPushButton("Сохранить изображения меток", this);

    sizeAruco = new QSpinBox (this);
    sizeAruco->setRange(0, 1000);     
    sizeAruco->setValue(sizeMarkers);        

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Размер меток (мм):", this));
    layout->addWidget(sizeAruco);
    layout->addWidget(new QLabel("Словарь меток:", this));
    layout->addWidget(sizeComboBox);
    layout->addSpacing(20);
    layout->addWidget(saveButton);

    setLayout(layout);

    // Связываем кнопку сохранения с методом сохранения маркеров
    connect(saveButton, &QPushButton::clicked, this, &ArucoSaveMarkersDialog::saveMarkersToDisk);
}

void ArucoSaveMarkersDialog::saveMarkersToDisk()
{
    int currentDictID = sizeComboBox->currentData().toInt();
    cv::Ptr<cv::aruco::Dictionary> arucoDict = cv::aruco::getPredefinedDictionary(currentDictID);

    QString saveDir = QFileDialog::getExistingDirectory(
        this,
        "Выберите папку для сохранения маркеров",
        QDir::homePath()
    );

    if (saveDir.isEmpty()) {
        qDebug() << "Выбор директории отменён.";
        return;
    }

    int markerSize =  sizeAruco->value();

    for (int markerId = 0; markerId < 4; ++markerId) {
        cv::Mat markerImage;
        cv::aruco::drawMarker(arucoDict, markerId, markerSize, markerImage);

        QString fileName = QString("marker_%1.png").arg(markerId);
        QString fullPath = saveDir + "/" + fileName;

        if (!cv::imwrite(fullPath.toStdString(), markerImage)) {
            qWarning() << "Ошибка при сохранении файла:" << fullPath;
            continue;
        }

        qDebug() << "Сохранён файл:" << fullPath;
    }
}
