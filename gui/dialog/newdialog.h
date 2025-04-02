#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QSpinBox>

class NewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewDialog(QWidget *parent = nullptr);

    int getWidth() const;
    int getHeight() const;

private:
    QSpinBox *widthSpin;
    QSpinBox *heightSpin;
};

#endif // NEWDIALOG_H
