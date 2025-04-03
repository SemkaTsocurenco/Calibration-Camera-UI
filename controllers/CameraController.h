
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

private:
    cv::VideoCapture cap;
    QTimer timer;    
    SourceType sourceType;
};


#endif // CAMERACONTROLLER_H