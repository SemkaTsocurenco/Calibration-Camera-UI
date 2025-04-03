#ifndef FINDCAMERA_H
#define FINDCAMERA_H

#include <map>
#include <string>
#include <opencv2/opencv.hpp>




std::map<QString, QVariant> getExistCameraSet() {
    int devices = 5;
    QString camera_string_start = "Камера ";
    std::map<QString, QVariant> camera_set;
    for (int i = 0; i < devices; i++) {
        cv::VideoCapture cap(i);
        if (cap.isOpened()) {
            QString name = camera_string_start + QString::number(i);
            camera_set.insert({name, static_cast<QVariant>(i)});
        }
        cap.release();
    }
    return camera_set;
}

#endif // FINDCAMERA_H
