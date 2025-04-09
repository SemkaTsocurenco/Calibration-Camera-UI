#include "ChArukoSaveCalibration.h"
#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>

ChArukoSaveCalibration::ChArukoSaveCalibration(QWidget *parent, int H, int W, float sSize, float mSize, int dict)
    : QDialog(parent)
{
    h_count = H;
    w_count = W;
    squareSizeValue = sSize;
    markerSizeValue = mSize;
    dictSize = dict;

    setupUi();
}

void ChArukoSaveCalibration::setupUi() {


    SquareSize = new QDoubleSpinBox (this);
    SquareSize->setRange(0, 1000);     
    SquareSize->setValue(squareSizeValue);

    MarkerSize = new QDoubleSpinBox (this);
    MarkerSize->setRange(0, 1000);     
    MarkerSize->setValue(markerSizeValue);


    H_count = new QSpinBox(this);
    H_count->setRange(2, 20);     
    H_count->setValue(h_count);

    W_count = new QSpinBox(this);
    W_count->setRange(2, 20);     
    W_count->setValue(w_count);
    ArukoDICTSize = new QComboBox(this);
    ArukoDICTSize->addItem("Метка 4x4", cv::aruco::DICT_4X4_50);
    ArukoDICTSize->addItem("Метка 5x5", cv::aruco::DICT_5X5_100);
    ArukoDICTSize->addItem("Метка 6x6", cv::aruco::DICT_6X6_100);
    ArukoDICTSize->addItem("Метка 7x7", cv::aruco::DICT_7X7_50);

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
    layout2->addWidget(SquareSize); // Добавляем поле ввода для H
    layout2->addWidget(new QLabel("markerSize:", this)); // Добавляем подпись для W
    layout2->addWidget(MarkerSize); // Добавляем поле ввода для W


    SaveOnDisk = new QPushButton("Создать калибровочную матрицу");
    // Лейаут для миниатюрного изображения
    boardImageLabel = new QLabel(this);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(new QLabel("Выбор словаря Aruco меток:", this));
    mainLayout->addWidget(ArukoDICTSize);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(boardImageLabel, 1 , Qt::AlignCenter); // Добавляем миниатюру изображения
    mainLayout->addWidget(SaveOnDisk);


    // // Связываем кнопку сохранения с методом сохранения маркеров
    connect(SaveOnDisk, &QPushButton::clicked, this, &ChArukoSaveCalibration::CalculateCalibration);

    // Связываем изменение параметров с обновлением миниатюрного изображения
    connect(SquareSize, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ChArukoSaveCalibration::updateBoardImage);
    connect(MarkerSize, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ChArukoSaveCalibration::updateBoardImage);
    connect(H_count, QOverload<int>::of(&QSpinBox::valueChanged), this, &ChArukoSaveCalibration::updateBoardImage);
    connect(W_count, QOverload<int>::of(&QSpinBox::valueChanged), this, &ChArukoSaveCalibration::updateBoardImage);
    connect(ArukoDICTSize, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ChArukoSaveCalibration::updateBoardImage);
    updateBoardImage(); // Первоначальное обновление миниатюры


}


void ChArukoSaveCalibration::updateBoardImage() {
    int currentDictID = ArukoDICTSize->currentData().toInt();
    cv::Ptr<cv::aruco::Dictionary> arucoDict = cv::aruco::getPredefinedDictionary(currentDictID);

    // Generate Charuco board
    cv::Ptr<cv::aruco::CharucoBoard> board = cv::aruco::CharucoBoard::create(W_count->value(), H_count->value(), SquareSize->value(), MarkerSize->value(), arucoDict);

    // Generate the board image
    cv::Mat boardImage;
    board->draw(cv::Size(W_count->value() * SquareSize->value() , H_count->value() * SquareSize->value()), boardImage, 10, 1);

    // Check if boardImage is valid
    if (boardImage.empty()) {
        std::cerr << "Error: Generated board image is empty!" << std::endl;
        return;
    }

    // Convert OpenCV image to QImage
    cv::cvtColor(boardImage, boardImage, cv::COLOR_BGR2RGB);
    QImage img(boardImage.data, boardImage.cols, boardImage.rows, boardImage.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(img);
    boardImageLabel->setPixmap(pixmap);  
}


void ChArukoSaveCalibration::CalculateCalibration() {




    QString imagesDir = QFileDialog::getExistingDirectory(
        this,
        "Выберите папку с изображениями",
        QDir::homePath()
    );

    if (imagesDir.isEmpty()) {
        qDebug() << "Выбор директории отменён.";
        return;
    }

    QString saveDir = QFileDialog::getSaveFileName(
        this,
        "Выберите папку для сохранения калибровки",
        QDir::homePath(),
        "Файл калибровки (*.yaml *.xml)"
    );

    if (saveDir.isEmpty()) {
        qDebug() << "Выбор файлa отменён.";
        return;
    }

    int currentDictID = ArukoDICTSize->currentData().toInt();
    CharucoCalibrator calibrator (W_count->value(), H_count->value(), SquareSize->value(), MarkerSize->value(), currentDictID);
    auto cout_of_image = calibrator.loadCalibrationImagesFromDirectory(imagesDir.toStdString());
    double error;
    if (calibrator.calibrate(error)){
        if (calibrator.saveCalibration(saveDir.toStdString())){
            // QMessageBox::information(this, "Успех", "Доска успешно сохранена!");

            qDebug() << "Изоюражений найдено:  " << cout_of_image << "\nОшибка калибровки: " << error<<"\nФайл калибровки успешно сохранён в " <<  saveDir;
        } else {
            qDebug() << "Калибровка проведена успешно но выбран невеный файл для сохранения";
        }
    } else {
        qDebug() << "Калибровка не проведена";
    }

}


