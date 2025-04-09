#ifndef CHARUCO_CALIBRATOR_H
#define CHARUCO_CALIBRATOR_H

#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdexcept>
#include <algorithm> // Для std::min
#include <filesystem> // Для работы с директориями
#include <iostream>

namespace fs = std::filesystem;

class CharucoCalibrator {
public:
    /**
     * @brief Конструктор, задающий параметры Charuco доски.
     * @param squaresX Количество квадратов по горизонтали (должно быть >=2)
     * @param squaresY Количество квадратов по вертикали (должно быть >=2)
     * @param squareLength Длина стороны квадрата (в метрах)
     * @param markerLength Длина стороны маркера (в метрах)
     * @param dictionaryId Идентификатор словаря (например, cv::aruco::DICT_4X4_50)
     */
    CharucoCalibrator(int squaresX, int squaresY, float squareLength, 
                     float markerLength, int dictionaryId) 
        : squaresX(squaresX), squaresY(squaresY), 
          squareLength(squareLength), markerLength(markerLength),
          dictionaryId(dictionaryId) 
    {
        if (squaresX < 2 || squaresY < 2) {
            throw std::runtime_error("Доска Charuco должна иметь как минимум 2 квадрата по каждой стороне!");
        }
        dictionary = cv::aruco::getPredefinedDictionary(dictionaryId);
        board = cv::aruco::CharucoBoard::create(squaresX, squaresY, squareLength, 
                                              markerLength, dictionary);
    }

    /**
     * @brief Загружает изображения из директории и добавляет их для калибровки.
     * @param dirPath Путь к директории с изображениями
     * @return Количество успешно загруженных изображений
     */
    size_t loadCalibrationImagesFromDirectory(const std::string& dirPath) {
        size_t imagesLoaded = 0;

        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                try {
                    cv::Mat image = cv::imread(entry.path().string());
                    imageSize = image.size();
                    if (addCalibrationImage(image)) {
                        imagesLoaded++;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Ошибка при обработке изображения: " << entry.path() << " - " << e.what() << std::endl;
                }
            }
        }

        return imagesLoaded;
    }

    /**
     * @brief Обрабатывает изображение для извлечения углов Charuco доски.
     * @param image Исходное изображение для калибровки
     * @return true, если изображение содержит достаточное количество углов для калибровки, иначе false.
     */
    bool addCalibrationImage(const cv::Mat &image) {
        if (image.empty()) {
            throw std::runtime_error("Пустое изображение!");
        }

        cv::Mat gray;
        if (image.channels() == 3) {
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = image;
        }

        // Детекция маркеров
        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners;
        cv::aruco::detectMarkers(gray, dictionary, markerCorners, markerIds);
        
        if (markerIds.empty()) {
            return false;
        }

        // Интерполяция углов Charuco доски
        cv::Mat currentCharucoCorners, currentCharucoIds;
        cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, gray, board, 
                                           currentCharucoCorners, currentCharucoIds);

        bool validPose = false;
        if (!cameraMatrix.empty()) {
            validPose = cv::aruco::estimatePoseCharucoBoard(
                currentCharucoCorners, currentCharucoIds, board,
                cameraMatrix, distCoeffs, rvec, tvec
            );
        }

        if (currentCharucoCorners.total() < 4) {
            return false;
        }

        // Получаем данные из currentCharucoCorners как вектор cv::Point2f
        std::vector<cv::Point2f> corners;
        for (int i = 0; i < currentCharucoCorners.rows; ++i) {
            corners.push_back(cv::Point2f(currentCharucoCorners.at<float>(i, 0), currentCharucoCorners.at<float>(i, 1)));
        }

        // Получаем данные из currentCharucoIds как вектор int
        std::vector<int> ids;
        for (int i = 0; i < currentCharucoIds.rows; ++i) {
            ids.push_back(currentCharucoIds.at<int>(i, 0));
        }

        allCharucoCorners.push_back(corners);
        allCharucoIds.push_back(ids);

        return true;
    }

    /**
     * @brief Производит калибровку камеры на основе добавленных изображений.
     * @param imageSize Размер изображений, с которых производилась калибровка
     * @param rmsError Возвращает значение RMS ошибки калибровки
     * @return true, если калибровка выполнена успешно, иначе false.
     */
    bool calibrate( double &rmsError) {
        if (allCharucoCorners.empty()) {
            throw std::runtime_error("Нет данных для калибровки!");
        }

        rvecs.clear();
        tvecs.clear();

        rmsError = cv::aruco::calibrateCameraCharuco(
            allCharucoCorners, allCharucoIds, board, imageSize,
            cameraMatrix, distCoeffs, rvecs, tvecs
        );

        // Проверка допустимой ошибки (пример порога)
        const double MAX_RMS_ERROR = 2.0;
        return rmsError < MAX_RMS_ERROR;
    }

    /// Получение внутренней матрицы камеры
    cv::Mat getCameraMatrix() const { return cameraMatrix.clone(); }

    /// Получение коэффициентов искажений
    cv::Mat getDistCoeffs() const { return distCoeffs.clone(); }

    /// Количество калибровочных изображений
    size_t getNumCalibrationImages() const { return allCharucoCorners.size(); }

    /// Сброс накопленных данных
    void resetCalibrationData() {
        allCharucoCorners.clear();
        allCharucoIds.clear();
        rvecs.clear();
        tvecs.clear();
        cameraMatrix.release();
        distCoeffs.release();
    }

    /**
     * @brief Сохраняет калибровочные данные в файл
     * @param filename Имя YAML/XML файла
     */
    bool saveCalibration(const std::string &filename) const {
        cv::FileStorage fs(filename, cv::FileStorage::WRITE);
        if (!fs.isOpened()) return false;

        fs << "camera_matrix" << cameraMatrix
           << "distortion_coefficients" << distCoeffs;
        return true;
    }

private:
    // Параметры доски
    int squaresX, squaresY;
    float squareLength, markerLength;
    int dictionaryId;

    // Структуры OpenCV
    cv::Ptr<cv::aruco::Dictionary> dictionary;
    cv::Ptr<cv::aruco::CharucoBoard> board;
    
    // Данные калибровки
    std::vector<std::vector<cv::Point2f>> allCharucoCorners;
    std::vector<std::vector<int>> allCharucoIds;
    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;
    cv::Mat rvec, tvec; // Для оценки позы
    cv::Size imageSize ;
};

#endif // CHARUCO_CALIBRATOR_H
