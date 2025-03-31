#include <QApplication>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>

// Пример приложения на Qt для отображения видеопотока с камеры с возможностью выбора устройства.
class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent), cap(0) {

        // Инициализируем все элементы 
        cap_select = new QComboBox(this);
        cap_select->addItem("Камера 0", 0);
        cap_select->addItem("Камера 1", 1);
        cap_select->addItem("Камера 2", 2);
        cap_select->addItem("Камера 3", 3);

        videoLabel = new QLabel;
        videoLabel->setFixedSize(640, 480);

        QHBoxLayout *horisontal_cam_and_select = new QHBoxLayout (this);
        horisontal_cam_and_select->addWidget(cap_select);
        horisontal_cam_and_select->addWidget(videoLabel);

        // Добавляем обработку смены камеры 
        connect(cap_select, SIGNAL(currentIndexChanged(int)), this, SLOT(onCameraChanged(int)));

        // Обнавление кадра по таймеру 
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
        timer->start(30); // 30 мс (~ 30 FPS)
    
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
            QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            videoLabel->setPixmap(QPixmap::fromImage(qimg));
        }
    }

private:
    cv::VideoCapture cap;
    QComboBox *cap_select;
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
