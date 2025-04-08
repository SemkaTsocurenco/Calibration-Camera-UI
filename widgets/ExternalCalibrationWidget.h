#ifndef EXTERNALCALIBRATIONWIDGET_H
#define EXTERNALCALIBRATIONWIDGET_H

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



// #include "ArucoDialog.h"
// #include "ArucoDetector.h"

class externalCalibrationWidget : public QWidget {
    Q_OBJECT
public:
    explicit externalCalibrationWidget(QWidget *parent = nullptr);
    
private slots:

    void onCreateChArukoClicked();
    void StartAutoSetCollector(bool checked);
    void StartHandSetCollector(bool checked);
    void CreateCalibrateFile();
    
signals:
    void startAuto();
    void startHand();
    void stopAuto();
    void stopHand();
    
    void stopped();
    void resumeRequested();

private:
    void setupUi();
    void setupConnections();

    
    QSpinBox *H_count;
    QSpinBox *W_count;
    QSpinBox *squareSize;
    QSpinBox *markerSize;

    QComboBox *ArukoDICTSize;

    QPushButton *CreateCharucoButton; 
    QPushButton *StartTwoHandSetCollectorButton; 
    QPushButton *StartAutoSetCollectorButton;
    QPushButton *CreateCalibrateFileButton;



};

#endif // EXTERNALCALIBRATIONWIDGET_H
