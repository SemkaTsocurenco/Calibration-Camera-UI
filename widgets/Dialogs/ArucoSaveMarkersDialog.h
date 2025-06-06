#ifndef ARUCOSAVEMARKERSDIALOG_H
#define ARUCOSAVEMARKERSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>



#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>

class ArucoSaveMarkersDialog : public QDialog {
    Q_OBJECT
public:
    explicit ArucoSaveMarkersDialog(QWidget *parent = nullptr, float sizeAruco = 200);

private slots:
    void saveMarkersToDisk();


private:
    void setupUi();  // для настройки интерфейса

    QComboBox *sizeComboBox;
    QPushButton *saveButton;
    QSpinBox *sizeAruco;
    float sizeMarkers;

};

#endif // ARUCOSAVEMARKERSDIALOG_H
