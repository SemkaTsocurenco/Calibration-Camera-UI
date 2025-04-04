#include "ArucoDialog.h"


void ArucoDialog::saveArucoMarkers()
{
    // Создаём элементы интерфейса
    Size = new QComboBox(this);
    Size->addItem ("Метка 4х4", cv::aruco::DICT_4X4_50);
    Size->addItem ("Метка 5х5", cv::aruco::DICT_5X5_100);
    Size->addItem ("Метка 6х6", cv::aruco::DICT_6X6_100);
    Size->addItem ("Метка 7х7", cv::aruco::DICT_7X7_50);

    Save = new QPushButton("Сохранить изображения для меток");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(Size);
    layout->addWidget(Save);
    setLayout(layout);

    // 2) Правильная форма connect:
    connect(Save, &QPushButton::clicked, this, &ArucoDialog::save_markers);
}



void ArucoDialog::save_markers(){
    int currentDictID = Size->currentData().toInt();
    cv::Ptr<cv::aruco::Dictionary> arucoDict = cv::aruco::getPredefinedDictionary(currentDictID);

    QString saveDir = QFileDialog::getExistingDirectory(
        this,
        "Выберите папку для сохранения",
        QDir::homePath() // или другой путь по умолчанию, например QDir::currentPath()
    );

    if (saveDir.isEmpty()) {
        qDebug() << "Выбор директории отменен.";
        return;
    }

    int markerSize = 200;

    for (int markerId = 0; markerId < 4; ++markerId)
    {
        cv::Mat markerImage;
        cv::aruco::drawMarker(arucoDict, markerId, markerSize, markerImage);
        QString fileName = QString("marker_%1.png").arg(markerId);
        QString fullPath = saveDir + "/" + fileName;
        if (!cv::imwrite(fullPath.toStdString(), markerImage))
        {
            qWarning() << "Ошибка при сохранении файла:" << fullPath;
            continue;
        }
        qDebug() << "Сохранён файл:" << fullPath;
    }
    
};