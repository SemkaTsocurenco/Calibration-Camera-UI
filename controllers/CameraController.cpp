#include "CameraController.h"

CameraController::CameraController(QObject *parent)
    :QObject(parent),
    timer(this),
    sourceType(None)
{
    connect(&timer, &QTimer::timeout, this, &CameraController::grabFrame);
}

CameraController::~CameraController() {
    stop();
}

CameraController::SourceType CameraController::currentSource() const {
    return sourceType;
}

bool CameraController::startCamera(int device){
    stop();
    cap.open(device);
    if (!cap.isOpened()){
        emit errorOccurred("Не удалось открыть камеру №" + QString::number(device));
        return false;
    }
    
    timer.start(33);
    sourceType = Camera;
    emit started(Camera);
    return true;
}

bool CameraController::startVideoFile(const QString &filePath){
    stop();
    cap.open(filePath.toStdString());
    if (!cap.isOpened()){
        emit errorOccurred("Не удалось открыть видео " + filePath);
        return false;
    }
    timer.start(33);
    sourceType = VideoFile;
    emit started(VideoFile);
    return true;
}

void CameraController::grabFrame() {
    cv::Mat frame;
    cap >> frame;

    if (frame.empty()) {
        // Если видео закончилось, останавливаем таймер
        if (sourceType == VideoFile) {
            stop();
        } else {
            emit errorOccurred("Пустой кадр с камеры");
        }
        return;
    }
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows,
               static_cast<int>(frame.step), QImage::Format_RGB888);

    emit frameReady(img.copy());
}


void CameraController::stop() {
    if (cap.isOpened()) {
        timer.stop();
        cap.release();
        sourceType = None;
        emit stopped();
    }
}
