
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H


#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include "ArucoDetector.h"
#include "ArucoSaveCalibration.h"


class CameraController : public QObject {
    Q_OBJECT
public:

    enum SourceType {
        Camera,
        VideoFile,
        None
    };

    CameraController(QObject *parent = nullptr);
    ~CameraController();

    bool startCamera(int device);
    bool startVideoFile(const QString &filePath);
    void stop();

    void internalCalibrate(int dictType, float sizeMarkers);

    void internalCalibrateStop();

    const QString& getFileSource();
    int getCameraSource();

    SourceType currentSource() const;

signals:
    void frameReady(const QImage &image);
    void started(SourceType type);
    void stopped();
    void errorOccurred(const QString &error);
    void internalCalibrateSave();


private slots:
    void grabFrame();

public slots:
    void setBrightness(int value);
    void setContrast(int value);
    void setResolution(int value);
    void enableGrayscale(bool enabled);

private:
    cv::VideoCapture cap;
    cv::Mat frame;
    QTimer timer;    
    SourceType sourceType;
    int brightness = 50;
    int contrast = 50;
    int targetResolution = 10;
    bool grayscaleEnabled = false;
    cv::Mat applySettings(const cv::Mat &frame);
    QString VideoName;
    int Device;
    int dictType;
    float sizeMarkers;
    bool calibration = false;
    ArucoSaveCalibration *saveCalibration;
    // ArucoDetector ArucoDetector;
};


#endif // CAMERACONTROLLER_H