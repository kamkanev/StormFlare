#ifndef SIMPLEWINDOW_H
#define SIMPLEWINDOW_H

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <utils/tools/shapetool.h>
#include <utils/tools/freedrawing.h>
#include <utils/tools/rectdrawing.h>
#include <utils/tools/linetool.h>
#include <utils/tools/circletool.h>

#include "utils/canvas/canvas.h"

#include <QWidget>
#include <QMouseEvent>
#include <QShortcut>
#include <QPushButton>
#include <QMenuBar>
#include <QFileDialog>

#include "gui/qcanvas.h"
#include "include/QtColorWidgets/ColorDialog"
#include "gui/dialog/newdialog.h"

class SimpleWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SimpleWindow(QWidget *parent = nullptr);
    static QImage MatToQPixmap(const cv::Mat &image);

protected:
    Canvas canvas = Canvas(500, 500, cv::Scalar(255, 255, 255));
    QCanvas* qcanvas = new QCanvas(this);

    Brush* brush = new Brush();

    //tools
    FreeDrawing fd = FreeDrawing(true);
    RectDrawing rd = RectDrawing(true);
    CircleTool cl = CircleTool(true);
    LineTool lt = LineTool(true);

    ShapeTool* st = &fd;

    // QShortcut *scFreeDraw = new QShortcut(QKeySequence("Ctrl+Shift+F"), this);
    // QShortcut *scRectTool = new QShortcut(QKeySequence("Ctrl+Shift+R"), this);
    // QShortcut *scCircleTool = new QShortcut(QKeySequence("Ctrl+Shift+C"), this);
    // QShortcut *scLineTool = new QShortcut(QKeySequence("Ctrl+Shift+L"), this);

    // QShortcut *scUndo = new QShortcut(QKeySequence("Ctrl+Z"), this);
    // QShortcut *scRedo = new QShortcut(QKeySequence("Ctrl+Y"), this);

    //tools buttons
    QPushButton* fdBtn = new QPushButton("Free");
    QPushButton* rdBtn = new QPushButton("Rect");
    QPushButton* clBtn = new QPushButton("Circle");
    QPushButton* ltBtn = new QPushButton("Line");
    QPushButton* rgbBtn = new QPushButton("RGB");

    //color dialog
    color_widgets::ColorDialog* clrPick = new color_widgets::ColorDialog(this);

    //menus and bar
    QMenuBar *qbar;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    //sub menu actions
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;

private:
    void createMenus();
    void createActions();
    void setTools();

    QString fileName = NULL;

    QLabel* fileLabel = new QLabel();

private slots:
    void update(){
        st->focus(canvas.getImage());

        qcanvas->setImage(canvas.getImage());
        canvas.update();
    }

    void undo(){
        canvas.undo();
    }

    void redo(){
        canvas.redo();
    }

    void changeColor(QColor color){
        // qDebug() << color.red() << " " << fd.getBrush().getColor().val;
        int r, g, b;
        color.getRgb(&r, &g, &b);
        brush->setColor(cv::Scalar(b,g,r));
    }

    void changeTool(bool clicked){

        if(!clicked){
            QPushButton *buttonWidget = qobject_cast<QPushButton*>(sender());
            if (!buttonWidget)
                return;

            if(buttonWidget->text().toStdString() == "Free"){
                st = &fd;

                //qDebug() << buttonWidget ->text();
            } else if(buttonWidget->text().toStdString() == "Rect"){
                st = &rd;
            }else if(buttonWidget->text().toStdString() == "Circle"){
                st = &cl;
            } else if (buttonWidget->text().toStdString() == "Line"){
                st = &lt;
            }

            // qDebug() << buttonWidget ->text() << " " << fdBtn->text();
        }

    }

    void onMousePressedPaint(QPoint event) {
        st->updateTool(event.x(), event.y());
        // qDebug() << "Mouse Pressed at:" << st->getBrush().getMouse().x << " , " << st->getBrush().getMouse().y;

    }

    void onMouseMovedPaint(QPoint event) {
        // qDebug() << "Mouse Moved to:" << event->pos();
        st->updateDraw(canvas.getDrawImage(), event.x(), event.y());
    }

    void onMouseReleasedPaint(QPoint event) {
        // qDebug() << "Mouse Released at:" << event->pos();
        canvas.addToHistory(st->draw(canvas.getDrawImage(), event.x(), event.y()));
        st->updateTool(-1, -1);
    }

    void newFile(){

        NewDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            int width = dialog.getWidth();
            int height = dialog.getHeight();
            qDebug() << "User selected width:" << width << "height:" << height;
            canvas = Canvas(height, width, cv::Scalar(255, 255, 255));
        }
        //If dialog is closed / not accepted didn't do it

    }

    void saveFile(){
        //dialog where to save

        if(fileName.isNull()) {
            QFileDialog dialog(this);
            dialog.setAcceptMode(QFileDialog::AcceptSave);
            dialog.setNameFilters({"PNG Images (*.png)", "JPG Images (*.jpg)", "XMP Images (*.xpm)"});

            if (dialog.exec() == QDialog::Accepted) {
                fileName = dialog.selectedFiles().first();  // Get the chosen file path
                QString selectedFilter = dialog.selectedNameFilter();  // Get the selected file type

                if (!fileName.endsWith(".png", Qt::CaseInsensitive) &&
                    !fileName.endsWith(".jpg", Qt::CaseInsensitive) &&
                    !fileName.endsWith(".xpm", Qt::CaseInsensitive)) {

                    if (selectedFilter.contains("*.png")) {
                        fileName += ".png";
                    } else if (selectedFilter.contains("*.jpg")) {
                        fileName += ".jpg";
                    } else if (selectedFilter.contains("*.xpm")) {
                        fileName += ".xpm";
                    }
                }

                // qDebug() << "Saving to:" << fileName;
                imwrite(fileName.toStdString(), canvas.getImage());

                fileLabel->setText(fileName);
            }

        }else {
            imwrite(fileName.toStdString(), canvas.getImage());
        }

    }

    void saveAsFile(){

        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setNameFilters({"PNG Images (*.png)", "JPG Images (*.jpg)", "XMP Images (*.xpm)"});

        if (dialog.exec() == QDialog::Accepted) {
            fileName = dialog.selectedFiles().first();  // Get the chosen file path
            QString selectedFilter = dialog.selectedNameFilter();  // Get the selected file type

            if (!fileName.endsWith(".png", Qt::CaseInsensitive) &&
                !fileName.endsWith(".jpg", Qt::CaseInsensitive) &&
                !fileName.endsWith(".xpm", Qt::CaseInsensitive)) {

                if (selectedFilter.contains("*.png")) {
                    fileName += ".png";
                } else if (selectedFilter.contains("*.jpg")) {
                    fileName += ".jpg";
                } else if (selectedFilter.contains("*.xpm")) {
                    fileName += ".xpm";
                }
            }

            // qDebug() << "Saving to:" << fileName;
            imwrite(fileName.toStdString(), canvas.getImage());

            fileLabel->setText(fileName);
        }

    }

    void openFile(){

        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setNameFilters({"PNG Images (*.png)", "JPG Images (*.jpg)", "XMP Images (*.xpm)"});

        if (dialog.exec() == QDialog::Accepted) {
            fileName = dialog.selectedFiles().first();  // Get the chosen file path
            QString selectedFilter = dialog.selectedNameFilter();  // Get the selected file type

            //For now checks what the file ends with
            if (fileName.endsWith(".png", Qt::CaseInsensitive) ||
                fileName.endsWith(".jpg", Qt::CaseInsensitive) ||
                fileName.endsWith(".xpm", Qt::CaseInsensitive)) {


                // qDebug() << "Opening from:" << fileName;
                cv::Mat oImg = cv::imread(fileName.toStdString());
                canvas = Canvas(oImg);

                fileLabel->setText(fileName);
            }


        }

    }



    void exitWindow(){
        this->close();
    }
};

#endif // SIMPLEWINDOW_H
