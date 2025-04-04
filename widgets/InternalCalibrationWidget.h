#ifndef INTERNALCALIBRATIONWIDGET_H
#define INTERNALCALIBRATIONWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>

#include "ArucoDialog.h"



class internalCalibrationWidget : public QWidget{
    Q_OBJECT
public:
    explicit internalCalibrationWidget(QWidget *parent = nullptr);

public slots:

private slots:

private:
    QComboBox *ArukoSize;
    QMessageBox *Message;
    QPushButton *Reload;
    QPushButton *SaveCalibrate;
    
    QPushButton *CreateAruko;
    ArucoDialog *saveArucoDialog;

};


#endif // INTERNALCALIBRATIONWIDGET_H
