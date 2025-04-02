#include "newdialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

NewDialog::NewDialog(QWidget *parent) : QDialog(parent) {

    setWindowTitle("Create new image");
    setFixedSize(450, 350);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *labelWidth = new QLabel("Width:", this);
    QLabel *labelHeight = new QLabel("Height:", this);

    widthSpin = new QSpinBox(this);
    widthSpin->setRange(10, 100000);

    heightSpin = new QSpinBox(this);
    heightSpin->setRange(10, 100000);

    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    layout->addWidget(labelWidth);
    layout->addWidget(widthSpin);
    layout->addWidget(labelHeight);
    layout->addWidget(heightSpin);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    connect(okButton, &QPushButton::clicked, this, &NewDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &NewDialog::reject);

}

int NewDialog::getWidth() const {
    return widthSpin->value();
}

int NewDialog::getHeight() const {
    return heightSpin->value();
}
