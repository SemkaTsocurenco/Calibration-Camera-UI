#include "VideoFlowWidget.h"



VideoFlowWidget::VideoFlowWidget(QWidget *parent)
    : QWidget{parent}, cameraFlowDialog(new CameraFlowDialog(this))
{
    setupUi();
    setupConnections();
}


void VideoFlowWidget::setupUi(){



    openCameraFlowButton = new QPushButton("Принять видео с камеры", this);
    openUDPFlowButton = new QPushButton("Принять видео по UDP", this);
    openVideoFileButton = new QPushButton("Открыть видеофайл", this);
    closeButton = new QPushButton("Стоп", this);
    grayscaleCheckBox = new QCheckBox("Чёрно-белое изображение", this);



    brightnessSlider = new QSlider(Qt::Horizontal, this);
    contrastSlider = new QSlider(Qt::Horizontal, this);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(50);
    contrastSlider->setRange(0, 100);
    contrastSlider->setValue(50);


    resolutionSlider = new QSlider(Qt::Horizontal, this);
    resolutionSlider-> setRange(1, 20);
    resolutionSlider-> setValue(10);
    resolutionSlider->setTickPosition(QSlider::TicksBothSides);
    resolutionSlider->setTickInterval(5);
    resolutionSlider->setSingleStep(2.5);


    QVBoxLayout *layout = new QVBoxLayout(this);
    // layout->addWidget(cameraSelectCombo);

    layout->addWidget(openCameraFlowButton);
    layout->addWidget(openUDPFlowButton);
    layout->addWidget(openVideoFileButton);
    layout->addWidget(closeButton);

    layout->addSpacing(15);
    layout->addWidget(new QLabel("Яркость:", this));
    layout->addWidget(brightnessSlider);
    layout->addWidget(new QLabel("Контраст:", this));
    layout->addWidget(contrastSlider);
    layout->addWidget(grayscaleCheckBox);
    layout->addWidget(new QLabel("Масштаб:", this));
    layout->addWidget(resolutionSlider);

    layout->addStretch();
};

void VideoFlowWidget::setupConnections(){
    connect(openCameraFlowButton, &QPushButton::clicked, this, &VideoFlowWidget::setupCameraSource);
    connect(openUDPFlowButton, &QPushButton::clicked, this, &VideoFlowWidget::setupUdpSource);


    connect(openVideoFileButton, &QPushButton::clicked, this, [=](){
        emit stopped();  
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
    connect(resolutionSlider, &QSlider::valueChanged, this, &VideoFlowWidget::resolutionChanged);
    connect(grayscaleCheckBox, &QCheckBox::toggled, this, &VideoFlowWidget::grayscaleToggled);

    connect(cameraFlowDialog, &CameraFlowDialog::cameraSelected, this, &VideoFlowWidget::cameraSelected);
};



void VideoFlowWidget::setupCameraSource(){
    cameraFlowDialog->exec(); // Показываем уже созданный диалог
}

void VideoFlowWidget::setupUdpSource(){
    UdpFlowDialog *udpDialog = new UdpFlowDialog(this);
    connect(udpDialog, &UdpFlowDialog::udpSettingsEntered, this, [this](const QString &ipAddress, int port){

        emit udpSettingsEntered(ipAddress, port);
    });
    udpDialog->exec();
    udpDialog->deleteLater();
}
