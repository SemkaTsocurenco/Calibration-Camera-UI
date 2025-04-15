#ifndef CAMERAFLOWDIALOG_H
#define CAMERAFLOWDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

class CameraFlowDialog : public QDialog {
    Q_OBJECT

public:
    explicit CameraFlowDialog(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setupUi();
        setupConnections();
    }

signals:
    void cameraSelected(int deviceId);

private:
    QComboBox *cameraSelectCombo;
    QPushButton *ok;
    int cameraId = 0;

    // Метод для настройки UI
    void setupUi(){
        // Создаём QComboBox и заполняем его элементами с привязанными данными
        cameraSelectCombo = new QComboBox(this);
        cameraSelectCombo->addItem("Камера 0", 0);
        cameraSelectCombo->addItem("Камера 1", 1);
        cameraSelectCombo->addItem("Камера 2", 2);
        cameraSelectCombo->addItem("Камера 3", 3);

        ok = new QPushButton("OK", this);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(cameraSelectCombo);
        layout->addSpacing(20);
        layout->addWidget(ok);
    }

    // Метод для подключения сигналов и слотов
    void setupConnections(){
        connect(ok, &QPushButton::clicked, this, [this](){
            cameraId = cameraSelectCombo->currentData().toInt();
            emit cameraSelected(cameraId);
        });
    }
};

#endif // CAMERAFLOWDIALOG_H
