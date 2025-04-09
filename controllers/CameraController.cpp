#include "CameraController.h"
#include <iostream>

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

// CameraController::CaptureMode CameraController::currentMode() const {
//     return captureMode;
// }


bool CameraController::startCamera(int device){
    stop();
    Device = device;
    cap.open(device);
    if (!cap.isOpened()){
        emit errorOccurred("Не удалось открыть камеру №" + QString::number(device));
        return false;
    }
    
    timer.start(10);
    sourceType = Camera;
    emit started(Camera);
    return true;
}

bool CameraController::startVideoFile(const QString &filePath){
    stop();
    VideoName = filePath;
    cap.open(filePath.toStdString());
    if (!cap.isOpened()){
        emit errorOccurred("Не удалось открыть видео " + filePath);
        return false;
    }
    timer.start(10);
    sourceType = VideoFile;
    emit started(VideoFile);
    return true;
}

void CameraController::grabFrame() {

    cap >> frame;
    if (frame.empty()) {
        if (sourceType == VideoFile) {
            stop();
        } else {
            emit errorOccurred("Пустой кадр с камеры");
        }
        return;
    }

    frame = applySettings(frame);
    if (calibration) {
        ArucoDetector detector(frame, dictType);
        frame = detector.getAnnotatedImage();
        bool success = detector.calibrateCameraFromAruco(sizeMarkers, frame.size());
        if (success){ 
            auto matrix  = detector.getCameraMatrix();
            auto dist = detector.getDistCoeffs();
            saveCalibration = new ArucoSaveCalibration(matrix, dist, nullptr);
            saveCalibration->exec();
        }
    }

    if (externalCalibrateHand){
        CharucoCalibrator charucoCalibrator (H, W, SqSize, MarkerSize, DictID);
        frame = charucoCalibrator.captureCalibrationSet(CharucoCalibrator::MANUAL, "images", frame);

    }
    if (externalCalibrateAuto){
        CharucoCalibrator charucoCalibrator (H, W, SqSize, MarkerSize, DictID);
        frame = charucoCalibrator.captureCalibrationSet(CharucoCalibrator::AUTOMATIC, "images", frame);
    }
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows,
               static_cast<int>(frame.step), QImage::Format_RGB888);

    emit frameReady(img.copy());
}


void CameraController::setBrightness(int value) { brightness = value; }
void CameraController::setContrast(int value) { contrast = value; }
void CameraController::setResolution(int value) { targetResolution = value; }
void CameraController::enableGrayscale(bool enabled) { grayscaleEnabled = enabled; }
const QString& CameraController::getFileSource() { return VideoName; }
int CameraController::getCameraSource() { return Device; }


cv::Mat CameraController::applySettings(const cv::Mat &frame) {
    cv::Mat processed = frame.clone();

    // Изменение размера кадра
    if (processed.cols != frame.cols*(float(targetResolution)/10) || processed.rows != frame.rows*(float(targetResolution)/10)) {
        cv::resize(processed, processed, cv::Size( frame.cols*(float(targetResolution)/10),  frame.rows*(float(targetResolution)/10)));
    }

    // Применение яркости и контраста
    processed.convertTo(processed, -1, contrast / 50.0, brightness - 50);

    // Чёрно-белый фильтр
    if (grayscaleEnabled) {
        cv::cvtColor(processed, processed, cv::COLOR_BGR2GRAY);
        cv::cvtColor(processed, processed, cv::COLOR_GRAY2RGB);
    }

    return processed;
}


void CameraController::internalCalibrate(int dType, float sMarkers){
    dictType = dType;
    sizeMarkers = sMarkers;
    calibration = true;
}
void CameraController::internalCalibrateStop() { calibration = false ; } 



void CameraController::externalStartHand(int h, int w, float sqSize, float markerSize, int dictID){
    H = h;
    W = w;
    SqSize = sqSize;
    MarkerSize = markerSize;
    DictID = dictID;
    externalCalibrateHand = true;   
    externalCalibrateAuto = false;   

}
void CameraController::externalStartAuto(int h, int w, float sqSize, float markerSize, int dictID){
    H = h;
    W = w;
    SqSize = sqSize;
    MarkerSize = markerSize;
    DictID = dictID;    
    externalCalibrateAuto = true;   
    externalCalibrateHand = false;   
}

void CameraController::externalStopHand(){externalCalibrateHand = false;}
void CameraController::externalStopAuto(){externalCalibrateAuto = false;}


void CameraController::stop() {
    if (cap.isOpened()) {
        timer.stop();
        cap.release();
        emit stopped();
    }
}
