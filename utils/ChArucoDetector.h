#ifndef CHARUCO_CALIBRATOR_H
#define CHARUCO_CALIBRATOR_H

#include <opencv2/aruco.hpp>
#include <opencv2/aruco/charuco.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdexcept>

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
    CharucoCalibrator(int squaresX, int squaresY, float squareLength, float markerLength, int dictionaryId) {
        if (squaresX < 2 || squaresY < 2) {
            throw std::runtime_error("Доска Charuco должна иметь как минимум 2 квадрата по каждой стороне!");
        }
        dictionary = cv::aruco::getPredefinedDictionary(dictionaryId);
        board = cv::aruco::CharucoBoard::create(squaresX, squaresY, squareLength, markerLength, dictionary);
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
            // Если маркеры не найдены, возвращаем false
            return false;
        }

        // Интерполяция углов Charuco доски
        cv::Mat currentCharucoCorners, currentCharucoIds;
        cv::aruco::interpolateCornersCharuco(markerCorners, markerIds, gray, board, currentCharucoCorners, currentCharucoIds);

        if (currentCharucoCorners.total() < 4) {
            // Недостаточно углов для калибровки
            return false;
        }

        // Сохраняем найденные углы и их идентификаторы
        std::vector<cv::Point2f> corners;
        std::vector<int> ids;
        corners.assign((cv::Point2f*)currentCharucoCorners.datastart, (cv::Point2f*)currentCharucoCorners.dataend);
        ids.assign((int*)currentCharucoIds.datastart, (int*)currentCharucoIds.dataend);

        allCharucoCorners.push_back(corners);
        allCharucoIds.push_back(ids);
        return true;
    }

    /**
     * @brief Производит калибровку камеры на основе добавленных изображений.
     * @param imageSize Размер изображений, с которых производилась калибровка
     * @param rmsError Возвращает значение RMS ошибки калибровки
     * @return true, если калибровка выполнена успешно, иначе бросает исключение.
     */
    bool calibrate(cv::Size imageSize, double &rmsError) {
        if (allCharucoCorners.empty()) {
            throw std::runtime_error("Нет данных для калибровки. Добавьте хотя бы одно изображение!");
        }

        std::vector<cv::Mat> rvecs, tvecs;

        // Производим калибровку с помощью функции cv::aruco::calibrateCameraCharuco
        rmsError = cv::aruco::calibrateCameraCharuco(allCharucoCorners, allCharucoIds, board, imageSize,
                                                      cameraMatrix, distCoeffs, rvecs, tvecs);
        return true;
    }

    /// Получение внутренней матрицы камеры (копия матрицы)
    cv::Mat getCameraMatrix() const {
        return cameraMatrix.clone();
    }

    /// Получение коэффициентов искажений (копия коэффициентов)
    cv::Mat getDistCoeffs() const {
        return distCoeffs.clone();
    }

    /// Получение количества изображений, добавленных для калибровки
    size_t getNumCalibrationImages() const {
        return allCharucoCorners.size();
    }

    /**
     * @brief Сохраняет матрицу калибровки и коэффициенты искажений в YAML файл.
     * @param filename Имя файла для сохранения
     * @return true, если данные успешно сохранены, иначе false.
     */
    bool saveCalibration(const std::string &filename) const {
        cv::FileStorage fs(filename, cv::FileStorage::WRITE);
        if (!fs.isOpened()) {
            std::cerr << "Ошибка при открытии файла для записи!" << std::endl;
            return false;
        }

        // Записываем матрицу камеры и коэффициенты искажений
        fs << "cameraMatrix" << cameraMatrix;
        fs << "distCoeffs" << distCoeffs;

        fs.release();
        return true;
    }

private:
    // Словарь ArUco для создания Charuco доски
    cv::Ptr<cv::aruco::Dictionary> dictionary;
    // Настроенная Charuco доска
    cv::Ptr<cv::aruco::CharucoBoard> board;

    // Вектор векторов углов (для каждого изображения)
    std::vector<std::vector<cv::Point2f>> allCharucoCorners;
    // Вектор векторов id углов (для каждого изображения)
    std::vector<std::vector<int>> allCharucoIds;

    // Результаты калибровки
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;
};

#endif // CHARUCO_CALIBRATOR_H
