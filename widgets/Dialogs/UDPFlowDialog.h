#ifndef UDPFLOWDIALOG_H
#define UDPFLOWDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

class UdpFlowDialog : public QDialog {
    Q_OBJECT

public:
    explicit UdpFlowDialog(QWidget *parent = nullptr)
        : QDialog(parent)
    {
        setupUi();
        setupConnections();
    }

signals:
    // Сигнал, передающий введённые настройки UDP (IP-адрес и порт)
    void udpSettingsEntered(const QString &ipAddress, int port);

private:
    QLineEdit *ipLineEdit;
    QSpinBox *portSpinBox;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(){
        setWindowTitle("Настройки UDP");

        // Формируем макет для ввода данных
        QFormLayout *formLayout = new QFormLayout;

        // Создаем поле ввода для IP адреса
        ipLineEdit = new QLineEdit(this);
        ipLineEdit->setPlaceholderText("Введите IP адрес");

        // Устанавливаем валидатор для проверки корректности IP адреса
        QRegularExpression ipRegex(
            "^(25[0-5]|2[0-4]\\d|[01]?\\d?\\d)(\\.(25[0-5]|2[0-4]\\d|[01]?\\d?\\d)){3}$"
        );
        QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
        ipLineEdit->setValidator(ipValidator);

        // Создаем поле для ввода порта с ограничением диапазона от 1 до 65535
        portSpinBox = new QSpinBox(this);
        portSpinBox->setRange(1, 65535);
        portSpinBox->setValue(12345); // Значение по умолчанию

        formLayout->addRow("IP адрес:", ipLineEdit);
        formLayout->addRow("Порт:", portSpinBox);

        // Кнопки подтверждения и отмены
        okButton = new QPushButton("OK", this);
        cancelButton = new QPushButton("Отмена", this);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);

        // Основной макет диалога
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addLayout(formLayout);
        mainLayout->addLayout(buttonLayout);

        setLayout(mainLayout);
    }

    void setupConnections(){
        // При нажатии OK проверяем ввод и отправляем сигнал с данными
        connect(okButton, &QPushButton::clicked, this, [this](){
            // Если IP адрес введён неверно, показываем сообщение об ошибке
            if (!ipLineEdit->hasAcceptableInput()) {
                QMessageBox::warning(this, "Ошибка", "Введите корректный IP адрес!");
                return;
            }
            emit udpSettingsEntered(ipLineEdit->text(), portSpinBox->value());
            accept();
        });
        // При нажатии отмена просто закрываем диалог
        connect(cancelButton, &QPushButton::clicked, this, &UdpFlowDialog::reject);
    }
};

#endif // UDPFLOWDIALOG_H
