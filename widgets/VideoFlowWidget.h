#ifndef VIDEOFLOWWIDGET_H
#define VIDEOFLOWWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>
#include "CameraFlowDialog.h"
#include "UDPFlowDialog.h"


class VideoFlowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoFlowWidget(QWidget *parent = nullptr);

signals:
    void cameraSelected(int deviceId);
    void videoFileOpened(const QString &filePath);
    void stopped();

    void brightnessChanged(int value);
    void contrastChanged(int value);
    void resolutionChanged(int value);
    void grayscaleToggled(bool enabled);
    void udpSettingsEntered(const QString &ipAddress, int port);

private:
    CameraFlowDialog *cameraFlowDialog;
    QPushButton *openCameraFlowButton;
    QPushButton *openUDPFlowButton;
    QPushButton *openVideoFileButton;
    QPushButton *closeButton;

    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    QSlider *resolutionSlider;
    QCheckBox *grayscaleCheckBox;

    void setupUi();
    void setupConnections();
    void setupCameraSource();
    void setupUdpSource();
};

#endif // VIDEOFLOWWIDGET_H
