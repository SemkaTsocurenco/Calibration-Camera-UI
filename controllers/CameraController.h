#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include "ArucoDetector.h"
#include "ArucoSaveCalibration.h"
#include "ChArucoCalculateCalibrate.h"

class CameraController : public QObject {
    Q_OBJECT
public:

    enum SourceType {
        Camera,
        VideoFile,
        UDP,        // Добавляем поддержку UDP
        None
    };

    /**
     * @brief Режим захвата изображений
     */
    enum CaptureMode {
        MANUAL,     // Ручной режим - снимок делается по нажатию пробела, если доска обнаружена
        AUTOMATIC   // Автоматический режим - снимок делается каждые 7 секунд, если доска обнаружена
    };

    CameraController(QObject *parent = nullptr);
    ~CameraController();

    bool startCamera(int device);
    bool startVideoFile(const QString &filePath);
    bool startUDP(const QString &ipAddress, int port); // Объявление метода для UDP

    void stop();

    void internalCalibrate(int dictType, float sizeMarkers);
    void internalCalibrateStop();

    void externalStartHand(int h, int w, float sqSize, float markerSize, int dictID);
    void externalStartAuto(int h, int w, float sqSize, float markerSize, int dictID);
    void externalStopHand();
    void externalStopAuto();

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
    CaptureMode captureMode;
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
    bool externalCalibrateHand = false;
    bool externalCalibrateAuto = false;
    int H;
    int W;
    float SqSize;
    float MarkerSize;
    int DictID;

    // Другие члены класса…
};

#endif // CAMERACONTROLLER_H
