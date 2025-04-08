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



#include "ArucoSaveMarkersDialog.h"
#include "ArucoDetector.h"

class internalCalibrationWidget : public QWidget {
    Q_OBJECT
public:
    explicit internalCalibrationWidget(QWidget *parent = nullptr);
    
private slots:

    void onCreateArukoClicked();
    void startCalibrate(bool checked);
    
signals:
    void started(int dictType, float sizeMarkers);
    void stop();

    void stopped();
    void resumeRequested();

private:
    void setupUi();
    void setupConnections();

    QComboBox *ArukoDICTSize;
    QPushButton *startButton;
    QPushButton *Reload;
    QPushButton *CreateAruko;
    QSpinBox *sizeAruco;
    ArucoSaveMarkersDialog *saveArucoDialog;



};

#endif // INTERNALCALIBRATIONWIDGET_H


