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
    void grayscaleToggled(bool enabled);
    void resolutionChanged(QSize resolution);

private:
    QComboBox *cameraSelectCombo;
    QPushButton *openVideoFileButton;
    QPushButton *closeButton;

    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    QCheckBox *grayscaleCheckBox;
    QComboBox *resolutionComboBox;

    void setupUi();
    void setupConnections();
};

#endif // VIDEOFLOWWIDGET_H
