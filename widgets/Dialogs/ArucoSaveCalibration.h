#ifndef ARUCOSAVECALIBRATION_H
#define ARUCOSAVECALIBRATION_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>
#include <iostream>
#include <QMessageBox>




#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>

class ArucoSaveCalibration : public QDialog {
    Q_OBJECT
public:
    explicit ArucoSaveCalibration(cv::Mat matrix, cv::Mat dist, QWidget *parent = nullptr);
private slots:
    void saveMatrix();


private:
    void setupUi(); 
    QPushButton *saveButton;
    cv::Mat Matrix;
    cv::Mat Dist;
};

#endif // ARUCOSAVECALIBRATION_H
