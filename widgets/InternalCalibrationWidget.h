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
#include "ArucoDetector.h"

class internalCalibrationWidget : public QWidget {
    Q_OBJECT
public:
    explicit internalCalibrationWidget(QWidget *parent = nullptr);
    
private slots:
    void onCreateArukoClicked();
    void onReloadClicked();
    void onSaveCalibrateClicked();

signals:
    void stopped();
    void resumeRequested();

private:
    void setupUi();
    void setupConnections();

    QComboBox *ArukoSize;
    QPushButton *Reload;
    QPushButton *SaveCalibrate;
    QPushButton *CreateAruko;

    ArucoDialog *saveArucoDialog;
    // ArucoDetector ArucoDetector;
};

#endif // INTERNALCALIBRATIONWIDGET_H
