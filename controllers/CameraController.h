
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H


#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <QImage>
#include <QPixmap>
#include <QTimer>


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

    SourceType currentSource() const;

signals:
    void frameReady(const QImage &image);
    void started(SourceType type);
    void stopped();
    void errorOccurred(const QString &error);

private slots:
    void grabFrame();

public slots:
    void setBrightness(int value);
    void setContrast(int value);
    void enableGrayscale(bool enabled);
    void setResolution(QSize resolution);

private:
    cv::VideoCapture cap;
    QTimer timer;    
    SourceType sourceType;
    int brightness = 50;
    int contrast = 50;
    bool grayscaleEnabled = false;
    QSize targetResolution = QSize(640, 480);
    cv::Mat applySettings(const cv::Mat &frame);
};


#endif // CAMERACONTROLLER_H