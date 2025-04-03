#include <QApplication>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include "ArucoDetector.h"
#include "FindCamera.h"


// Пример приложения на Qt для отображения видеопотока с камеры с возможностью выбора устройства.
class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent), cap(0) {

        // Инициализируем все элементы 
        cap_select = new QComboBox(this);
        cap_select->addItem ("Камера 0 ", 0);
        cap_select->addItem ("Камера 1 ", 1);
        cap_select->addItem ("Камера 2 ", 2);
        cap_select->addItem ("Камера 3 ", 3);



        aruco_dict_select = new QComboBox(this);
        aruco_dict_select->addItem ("Метка 4х4", cv::aruco::DICT_4X4_50);
        aruco_dict_select->addItem ("Метка 5х5", cv::aruco::DICT_5X5_100);
        aruco_dict_select->addItem ("Метка 6х6", cv::aruco::DICT_6X6_100);
        aruco_dict_select->addItem ("Метка 7х7", cv::aruco::DICT_7X7_50);


        QVBoxLayout *vertical_combo_boxes = new QVBoxLayout ();
        vertical_combo_boxes->addWidget(cap_select);
        vertical_combo_boxes->addWidget(aruco_dict_select);


        videoLabel = new QLabel;
        videoLabel->setFixedSize(640, 480);

        QVBoxLayout *vertical_combo_video = new QVBoxLayout ();
        vertical_combo_video->addWidget(videoLabel);


        QHBoxLayout *horisontal_cam_and_select = new QHBoxLayout ();
        horisontal_cam_and_select->addLayout(vertical_combo_boxes);
        horisontal_cam_and_select->addLayout(vertical_combo_video);



        setLayout(horisontal_cam_and_select);
        // Добавляем обработку смены камеры 
        connect(cap_select, SIGNAL(currentIndexChanged(int)), this, SLOT(onCameraChanged(int)));

        // Обнавление кадра по таймеру 
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
        timer->start(60); // 60 мс (~ 60 FPS)
    
    }
    ~MainWindow() {
        if (cap.isOpened()) {
            cap.release();
        }
    }   
public slots:

    void onCameraChanged(int index){
        int device = cap_select->itemData(index).toInt();
        if (cap.isOpened()){
            cap.release();
        }
        cap.open(device);
    }

void updateFrame(){
    cv::Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        int dictType = aruco_dict_select->currentData().toInt();

        try {
            ArucoDetector detector(frame, dictType);
            cv::Mat annotated = detector.getAnnotatedImage();
            auto coordinates = detector.getMarkerCoordinates();
            
            QImage qimg(annotated.data, annotated.cols, annotated.rows, annotated.step, QImage::Format_RGB888);
            videoLabel->setPixmap(QPixmap::fromImage(qimg));
        } catch (const std::exception &e) {
            qDebug("Ошибка обработки изображения: %s", e.what());
        }
    }
}


private:
    cv::VideoCapture cap;
    QComboBox *cap_select;
    QComboBox *aruco_dict_select;
    QPushButton *button_go_calibrate;
    QLabel *videoLabel;
    QTimer *timer;
};





#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
