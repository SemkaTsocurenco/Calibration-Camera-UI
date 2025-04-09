#ifndef CHARUCOSAVECALIBRATION_H
#define CHARUCOSAVECALIBRATION_H

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <QImage>
#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/opencv.hpp>
#include "ChArucoСalculateCalibrate.h"

class ChArukoSaveCalibration : public QDialog {
    Q_OBJECT

public:
    // Конструктор
    ChArukoSaveCalibration(QWidget *parent, int H, int W, float sSize, float mSize, int dict);


private:
    // Элементы UI
    QSpinBox *H_count;
    QSpinBox *W_count;
    QDoubleSpinBox *SquareSize;
    QDoubleSpinBox *MarkerSize;
    QComboBox *ArukoDICTSize;
    QPushButton *SaveOnDisk;
    QLabel *boardImageLabel;


    // Переменные для параметров доски
    int h_count;
    int w_count;
    float squareSizeValue;
    float markerSizeValue;
    int dictSize;



    // Метод для настройки UI
    void setupUi();

    // Метод для обновления миниатюрного изображения доски
    void updateBoardImage();

    // Метод для сохранения изображения доски на диск
    void CalculateCalibration();

};

#endif // CHARUCOSAVECALIBRATION_H
