#ifndef ARUCODIALOG_H
#define ARUCODIALOG_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>

#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>  
#include <filesystem>            
#include <iostream>

class ArucoDialog : public QDialog{
    Q_OBJECT
public:
    explicit ArucoDialog(QDialog *parent = nullptr);

private slots:
    void save_markers();

private:
    QComboBox *Size;
    QPushButton *Save;

    void saveArucoMarkers();

};

#endif // ARUCODIALOG_H