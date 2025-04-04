#ifndef ARUCODIALOG_H
#define ARUCODIALOG_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>


class ArucoDialog : public QDialog{
    Q_OBJECT
public:
    explicit ArucoDialog(QDialog *parent != nullptr);
    ~ArucoDialog();

private slots:
    
private:
    QComboBox *Size;
    QPushButton *Save;

}

#endif // ARUCODIALOG_H