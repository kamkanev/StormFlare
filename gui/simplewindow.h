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

    //brush stuff
    QSpinBox* sizeSpin;

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
    void setBrushSize(int);

    void update();

    void undo();

    void redo();

    void changeColor(QColor color);

    void changeTool(bool clicked);

    void onMousePressedPaint(QPoint event);

    void onMouseMovedPaint(QPoint event);

    void onMouseReleasedPaint(QPoint event);

    void newFile();

    void saveFile();

    void saveAsFile();

    void openFile();

    void exitWindow();
};

#endif // SIMPLEWINDOW_H
