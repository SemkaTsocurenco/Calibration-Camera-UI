#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>


#include <QApplication>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <QImage>
#include <QMessageBox>

#include "VideoFlowWidget.h"
#include "CameraController.h"
#include "ArucoDetector.h"
#include "InternalCalibrationWidget.h"
#include "ExternalCalibrationWidget.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupUi();
    void setupConnections();
    void switchToInternalCalibration();
    void switchToExternalCalibration();
    void switchToVideoFlowSet();

private:
    QWidget *centralWidget;
    QLabel *videoLabel;

    // Управление камерой и видео
    CameraController *cameraController;


    // Левая панель с переключаемыми виджетами
    QStackedWidget *controlsStack;
    internalCalibrationWidget *internalCalibration;
        // ExternalCalibrationWidget *externalCalibrationWidget;
    VideoFlowWidget *videoFlowWidget;

    // Меню и действия
    QMenuBar *menuBar;
    QAction *actionInternalCalibration;
    QAction *actionExternalCalibration;
    QAction *actionVideoFlowSet;

    void createMenus();
};

#endif // MAINWINDOW_H
