#ifndef ARUCODETECTOR_H
#define ARUCODETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <stdexcept>
#include <vector>
#include <map>

class ArucoDetector {
public:
    // Конструктор принимает изображение и тип словаря (например, cv::aruco::DICT_4X4_50)
    ArucoDetector(cv::Mat &inputImage, int dictType) {
        if (inputImage.empty()) {
            throw std::runtime_error("Изображение не найдено!");
        }
        inputImage.copyTo(image);
        dictionary = cv::aruco::getPredefinedDictionary(dictType);
        detectMarkers();
    }

    // Получение изображения с отрисованными метками
    cv::Mat getAnnotatedImage() {
        cv::Mat output;
        image.copyTo(output);
        if (!markerIds.empty()) {
            cv::aruco::drawDetectedMarkers(output, markerCorners, markerIds);
        }
        return output;
    }

    // Координаты найденных меток: id -> углы (точки)
    std::map<int, std::vector<cv::Point2f>> getMarkerCoordinates() {
        std::map<int, std::vector<cv::Point2f>> coords;
        for (size_t i = 0; i < markerIds.size(); ++i) {
            coords[markerIds[i]] = markerCorners[i];
        }
        return coords;
    }

    // Вычисляет матрицу внутренней калибровки по 4 меткам Aruco
    bool calibrateCameraFromAruco(float markerLengthMeters, cv::Size imageSize) {
        if (markerIds.size() != 4) {
            std::cerr << "Ошибка: для калибровки необходимо ровно 4 метки!" << std::endl;
            return false;
        }

        // 3D координаты углов меток в пространстве (система координат симметрична относительно центра)
        std::vector<std::vector<cv::Point3f>> objectPoints(1);
        float halfSquareSize = 0.5f; // половина квадрата (1м / 2)

        // Верхний левый угол
        objectPoints[0].push_back(cv::Point3f(-halfSquareSize, halfSquareSize, 0));
        // Верхний правый угол
        objectPoints[0].push_back(cv::Point3f(halfSquareSize, halfSquareSize, 0));
        // Нижний правый угол
        objectPoints[0].push_back(cv::Point3f(halfSquareSize, -halfSquareSize, 0));
        // Нижний левый угол
        objectPoints[0].push_back(cv::Point3f(-halfSquareSize, -halfSquareSize, 0));

        // 2D точки (центры маркеров на изображении)
        std::vector<std::vector<cv::Point2f>> imagePoints(1);
        for (const auto& corners : markerCorners) {
            cv::Point2f center(0,0);
            for (const auto& pt : corners) {
                center += pt;
            }
            center *= 0.25f;
            imagePoints[0].push_back(center);
        }

        // Начальные предположения
        cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
        distCoeffs = cv::Mat::zeros(8, 1, CV_64F);

        std::vector<cv::Mat> rvecs, tvecs;

        // Калибровка
        double rms = cv::calibrateCamera(objectPoints, imagePoints, imageSize,
                                         cameraMatrix, distCoeffs, rvecs, tvecs);

        return true;
    }

    // Получение матрицы внутренней калибровки
    cv::Mat getCameraMatrix() const {
        return cameraMatrix.clone();
    }

    // Получение коэффициентов искажений
    cv::Mat getDistCoeffs() const {
        return distCoeffs.clone();
    }

private:
    cv::Mat image;
    cv::Ptr<cv::aruco::Dictionary> dictionary;
    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;

    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;

    // Метод детекции меток
    void detectMarkers() {
        cv::aruco::detectMarkers(image, dictionary, markerCorners, markerIds);
    }
};

#endif // ARUCODETECTOR_H
