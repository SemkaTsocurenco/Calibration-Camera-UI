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
    ArucoDetector(const cv::Mat &inputImage, int dictType) {
        if (inputImage.empty()) {
            throw std::runtime_error("Изображение не найдено!");
        }
        inputImage.copyTo(image);
        // Получаем выбранный словарь на основе переданного значения
        dictionary = cv::aruco::getPredefinedDictionary(dictType);
        detectMarkers();
    }

    // Метод для получения изображения с отрисованными метками
    cv::Mat getAnnotatedImage() {
        cv::Mat output;
        image.copyTo(output);
        if (!markerIds.empty()) {
            cv::aruco::drawDetectedMarkers(output, markerCorners, markerIds);
        }
        return output;
    }

    // Метод возвращает координаты найденных меток: id -> вектор углов (точек)
    std::map<int, std::vector<cv::Point2f>> getMarkerCoordinates() {
        std::map<int, std::vector<cv::Point2f>> coords;
        for (size_t i = 0; i < markerIds.size(); ++i) {
            coords[markerIds[i]] = markerCorners[i];
        }
        return coords;
    }

private:
    cv::Mat image;
    cv::Ptr<cv::aruco::Dictionary> dictionary;
    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;

    // Метод детекции меток
    void detectMarkers() {
        cv::aruco::detectMarkers(image, dictionary, markerCorners, markerIds);
    }
};

#endif // ARUCODETECTOR_H
