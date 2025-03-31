#include <QApplication>
#include <QComboBox>
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
        // Инициализация элементов интерфейса
        comboBox = new QComboBox(this);
        // Добавляем несколько вариантов камер (индексы 0, 1, 2, 3)
        comboBox->addItem("Камера 0", 0);
        comboBox->addItem("Камера 1", 1);
        comboBox->addItem("Камера 2", 2);
        comboBox->addItem("Камера 3", 3);

        videoLabel = new QLabel(this);
        videoLabel->setFixedSize(640, 480); // Размер отображаемого видео

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(comboBox);
        layout->addWidget(videoLabel);
        setLayout(layout);

        // При смене выбранного элемента выпадающего списка вызываем слот смены камеры
        connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCameraChanged(int)));

        // Таймер для регулярного обновления кадра с камеры
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
        timer->start(30); // Интервал ~30 мс (примерно 30 FPS)
    }

    ~MainWindow() {
        if (cap.isOpened()) {
            cap.release();
        }
    }

public slots:
    // Слот для обновления кадра и отображения его в QLabel
    void updateFrame() {
        if (cap.isOpened()) {
            cv::Mat frame;
            cap >> frame;
            if (!frame.empty()) {
                // Конвертируем из BGR (формат по умолчанию в OpenCV) в RGB
                cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
                QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
                videoLabel->setPixmap(QPixmap::fromImage(qimg));
            }
        }
    }

    // Слот для смены устройства камеры
    void onCameraChanged(int index) {
        int device = comboBox->itemData(index).toInt();
        if (cap.isOpened()) {
            cap.release();
        }
        cap.open(device);
    }

private:
    cv::VideoCapture cap;
    QComboBox *comboBox;
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
