#ifndef ARUCODIALOG_H
#define ARUCODIALOG_H

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

class ArucoDialog : public QDialog {
    Q_OBJECT
public:
    explicit ArucoDialog(QWidget *parent = nullptr);

private slots:
    void saveMarkersToDisk();


private:
    void setupUi();  // для настройки интерфейса

    QComboBox *sizeComboBox;
    QPushButton *saveButton;
    QSpinBox *sizeAruco;

};

#endif // ARUCODIALOG_H
