#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupConnections();

    // Запускаем по умолчанию камеру №0
    cameraController->startCamera(0);
}

MainWindow::~MainWindow() {
    cameraController->stop();
}

void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    internalCalibration = new internalCalibrationWidget(this);
    videoFlowWidget = new VideoFlowWidget(this);

    controlsStack = new QStackedWidget(this);
    controlsStack->addWidget(videoFlowWidget); 
    controlsStack->addWidget(internalCalibration);

    videoLabel = new QLabel(this);
    videoLabel->setStyleSheet("background-color: black;");

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(controlsStack);
    mainLayout->addWidget(videoLabel, 1, Qt::AlignCenter);

    // Ограничиваем размер окна под размер добавленных виджетов
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    createMenus();

    cameraController = new CameraController(this);

    // запускаем по умолчанию камеру 0
    cameraController->startCamera(0);
}


void MainWindow::createMenus() {
    // Создание верхнего меню
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *calibrationMenu = menuBar->addMenu("Режимы");

    actionVideoFlowSet = new QAction("Настроить параметры видео потока", this);
    actionInternalCalibration = new QAction("Внутренняя калибровка", this);
    actionExternalCalibration = new QAction("Внешняя калибровка", this);

    calibrationMenu->addAction(actionVideoFlowSet);
    calibrationMenu->addAction(actionInternalCalibration);
    calibrationMenu->addAction(actionExternalCalibration);
}

void MainWindow::setupConnections() {

    connect(videoFlowWidget, &VideoFlowWidget::cameraSelected, cameraController, &CameraController::startCamera);
    connect(videoFlowWidget, &VideoFlowWidget::videoFileOpened, cameraController, &CameraController::startVideoFile);
    connect(videoFlowWidget, &VideoFlowWidget::stopped, cameraController, &CameraController::stop);

    connect(videoFlowWidget, &VideoFlowWidget::brightnessChanged, cameraController, &CameraController::setBrightness);
    connect(videoFlowWidget, &VideoFlowWidget::contrastChanged, cameraController, &CameraController::setContrast);
    connect(videoFlowWidget, &VideoFlowWidget::grayscaleToggled, cameraController, &CameraController::enableGrayscale);
    connect(videoFlowWidget, &VideoFlowWidget::resolutionChanged, cameraController, &CameraController::setResolution);


    connect(cameraController, &CameraController::frameReady, this, [=](const QImage &image){
        videoLabel->setPixmap(QPixmap::fromImage(image));
        this->adjustSize();  
    });


    connect(cameraController, &CameraController::errorOccurred, this, [=](const QString &error){
        QMessageBox::warning(this, "Ошибка", error);
    });



    connect(internalCalibration, &internalCalibrationWidget::stopped, cameraController, &CameraController::stop);
    connect(internalCalibration, &internalCalibrationWidget::resumeRequested, this, [=](){
        if (cameraController->currentSource() == CameraController::VideoFile){
            cameraController->startVideoFile(cameraController->getFileSource()); 
        } else if (cameraController->currentSource() == CameraController::Camera){
            cameraController->startCamera(cameraController->getCameraSource()); 
        }
    });


    connect(actionVideoFlowSet, &QAction::triggered, this, &MainWindow::switchToVideoFlowSet);
    connect(actionInternalCalibration, &QAction::triggered, this, &MainWindow::switchToInternalCalibration);
    connect(actionExternalCalibration, &QAction::triggered, this, &MainWindow::switchToExternalCalibration);

}


void MainWindow::switchToInternalCalibration() {
    controlsStack->setCurrentWidget(internalCalibration);
}

void MainWindow::switchToExternalCalibration() {
    // controlsStack->setCurrentWidget(externalCalibrationWidget);
}

void MainWindow::switchToVideoFlowSet() {
    controlsStack->setCurrentWidget(videoFlowWidget);
}