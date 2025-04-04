#include "VideoFlowWidget.h"



VideoFlowWidget::VideoFlowWidget(QWidget *parent)
    : QWidget{parent}
{
    setupUi();
    setupConnections();
}

void VideoFlowWidget::setupUi(){
       // Выбор камеры и файла
    cameraSelectCombo = new QComboBox(this);
    cameraSelectCombo->addItem("Камера 0", 0);
    cameraSelectCombo->addItem("Камера 1", 1);
    cameraSelectCombo->addItem("Камера 2", 2);
    cameraSelectCombo->addItem("Камера 3", 3);

    resolutionComboBox = new QComboBox(this);
    resolutionComboBox->addItem("640x480", QSize(640, 480));
    resolutionComboBox->addItem("800x600", QSize(800, 600));
    resolutionComboBox->addItem("1280x720", QSize(1280, 720));

    openVideoFileButton = new QPushButton("Открыть видеофайл", this);
    closeButton = new QPushButton("Стоп", this);
    grayscaleCheckBox = new QCheckBox("Чёрно-белое изображение", this);

    brightnessSlider = new QSlider(Qt::Horizontal, this);
    contrastSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(50);
    contrastSlider->setRange(0, 100);
    contrastSlider->setValue(50);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(cameraSelectCombo);
    layout->addWidget(openVideoFileButton);
    layout->addWidget(closeButton);

    layout->addSpacing(15);
    layout->addWidget(new QLabel("Яркость:", this));
    layout->addWidget(brightnessSlider);
    layout->addWidget(new QLabel("Контраст:", this));
    layout->addWidget(contrastSlider);
    layout->addWidget(grayscaleCheckBox);
    layout->addWidget(new QLabel("Разрешение:", this));
    layout->addWidget(resolutionComboBox);

    layout->addStretch();
};

void VideoFlowWidget::setupConnections(){
    connect(cameraSelectCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        emit cameraSelected(cameraSelectCombo->itemData(index).toInt());
    });

    connect(openVideoFileButton, &QPushButton::clicked, this, [=](){
        QString filePath = QFileDialog::getOpenFileName(this,
                                "Выберите видеофайл",
                                QString(),
                                "Видео (*.mp4 *.avi *.mov *.mkv)");
        if (!filePath.isEmpty())
            emit videoFileOpened(filePath);
    });

    connect(closeButton, &QPushButton::clicked, this, &VideoFlowWidget::stopped);

    connect(brightnessSlider, &QSlider::valueChanged, this, &VideoFlowWidget::brightnessChanged);
    connect(contrastSlider, &QSlider::valueChanged, this, &VideoFlowWidget::contrastChanged);
    connect(grayscaleCheckBox, &QCheckBox::toggled, this, &VideoFlowWidget::grayscaleToggled);
    connect(resolutionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        emit resolutionChanged(resolutionComboBox->itemData(index).toSize());
    });
};


