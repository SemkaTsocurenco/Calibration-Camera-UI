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
    resize(900, 600);

    // Центральный виджет
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Элементы управления (левая панель)
    controlsStack = new QStackedWidget(this);
    // internalCalibrationWidget = new InternalCalibrationWidget(this);
    // externalCalibrationWidget = new ExternalCalibrationWidget(this);
    // controlsStack->addWidget(internalCalibrationWidget);
    // controlsStack->addWidget(externalCalibrationWidget);

    // Видео-панель (правая панель)
    videoLabel = new QLabel(this);
    videoLabel->setStyleSheet("background-color: black;");

    // Выбор камеры и файла
    cameraSelectCombo = new QComboBox(this);
    cameraSelectCombo->addItem("Камера 0", 0);
    cameraSelectCombo->addItem("Камера 1", 1);
    cameraSelectCombo->addItem("Камера 2", 2);
    cameraSelectCombo->addItem("Камера 3", 3);

    openVideoFileButton = new QPushButton("Открыть видеофайл", this);
    CloseButton = new QPushButton("Стоп", this);

    // Компоновка левой панели
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(cameraSelectCombo);
    leftLayout->addWidget(openVideoFileButton);
    leftLayout->addWidget(CloseButton);
    leftLayout->addWidget(controlsStack);
    leftLayout->addStretch();

    // Главная горизонтальная компоновка
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(videoLabel, 1, Qt::AlignCenter);

    // Настройка меню
    createMenus();

    // Инициализация CameraController
    cameraController = new CameraController(this);
}

void MainWindow::createMenus() {
    // Создание верхнего меню
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *calibrationMenu = menuBar->addMenu("Режимы калибровки");

    actionInternalCalibration = new QAction("Внутренняя калибровка", this);
    actionExternalCalibration = new QAction("Внешняя калибровка", this);

    calibrationMenu->addAction(actionInternalCalibration);
    calibrationMenu->addAction(actionExternalCalibration);
}

void MainWindow::setupConnections() {
    // Переключение источника видео по выбору камеры
    connect(cameraSelectCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index){
        int deviceId = cameraSelectCombo->itemData(index).toInt();
        cameraController->startCamera(deviceId);
    });

    // Открытие видеофайла
    connect(openVideoFileButton, &QPushButton::clicked, this, [=](){
        cameraController->stop();
        QString filePath = QFileDialog::getOpenFileName(this,
                                "Выберите видеофайл",
                                QString(),
                                "Видео (*.mp4 *.avi *.mov *.mkv)");
        if (!filePath.isEmpty()) {
            cameraController->startVideoFile(filePath);
        }
    });

    connect(CloseButton, &QPushButton::clicked, this, [=](){
        cameraController->stop();

    });

    // Отображение кадров в QLabel
    connect(cameraController, &CameraController::frameReady, this, [=](const QImage &image){
        videoLabel->setPixmap(QPixmap::fromImage(image));
    });

    // Переключение режимов через меню
    connect(actionInternalCalibration, &QAction::triggered, this, &MainWindow::switchToInternalCalibration);
    connect(actionExternalCalibration, &QAction::triggered, this, &MainWindow::switchToExternalCalibration);

    // Обработка ошибок
    connect(cameraController, &CameraController::errorOccurred, this, [=](const QString &error){
        QMessageBox::warning(this, "Ошибка", error);
    });
}

void MainWindow::switchToInternalCalibration() {
    // controlsStack->setCurrentWidget(internalCalibrationWidget);
}

void MainWindow::switchToExternalCalibration() {
    // controlsStack->setCurrentWidget(externalCalibrationWidget);
}
