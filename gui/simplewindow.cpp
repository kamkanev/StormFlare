#include "simplewindow.h"

#include <iostream>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QImage>
#include <QGuiApplication>  // For QGuiApplication
#include <QApplication>  // For QGuiApplication
#include <QTimer>
#include <QMenuBar>

#include "gui/qcanvas.h"

SimpleWindow::SimpleWindow(QWidget *parent)
    : QWidget(parent)
{
    setTools();

    //Set Icon
    setWindowTitle(tr("Stormflare"));
    setWindowIcon(QIcon("resources/Logo/logo2STORMFLARE.png"));
    setWindowIconText(tr("Stormflare v0.1.2a"));

    // this->move(screen()->availableGeometry().center());

    qbar = new QMenuBar(this);

    auto outer = new QVBoxLayout(this);

    auto endl = new QLabel("");

    outer->addWidget(endl);

    if(fileName.isNull()){
        fileLabel->setText(tr("untitled"));
    }else{
        fileLabel->setText(fileName);
    }

    outer->addWidget(fileLabel);


    //Added canvas image
    //QImage qim = SimpleWindow::MatToQPixmap(canvas.getImage());
    qcanvas->setImage(canvas.getImage());

    qcanvas->setStyleSheet("border: 1px solid black; background-color: #767676;");

    outer->addWidget(qcanvas);

    //Test new canvas
    // auto cWid = new CanvasWidget(this);

    // cWid->setImage(cv::imread("lenna.png"));

    // outer->addWidget(cWid);

    // auto noteEdit = new QTextEdit(this);
    // outer->addWidget(noteEdit);
    auto btncols = new QVBoxLayout;
    auto drawbtns = new QHBoxLayout;
    btncols->addLayout(drawbtns);

    drawbtns->addWidget(fdBtn);
    drawbtns->addWidget(rdBtn);
    drawbtns->addWidget(clBtn);
    drawbtns->addWidget(ltBtn);

    drawbtns->addWidget(rgbBtn);


    connect(rgbBtn, &QPushButton::clicked, [=]{
        clrPick->show();
    });

    auto inner = new QHBoxLayout;
    outer->addLayout(inner);

    inner->addLayout(btncols);


    // auto clearBtn = new QPushButton("Clear");
    // inner->addWidget(clearBtn);

    // auto saveBtn = new QPushButton("Save");
    // inner->addWidget(saveBtn);
    createActions();


    createMenus(); //<- problem


    QTimer *timer = new QTimer(qcanvas);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);


}

void SimpleWindow::setTools(){
    fd.brushPointer = brush;
    rd.brushPointer = brush;
    cl.brushPointer = brush;
    lt.brushPointer = brush;
}

void SimpleWindow::createMenus(){



    fileMenu = new QMenu(tr("&File"), qbar);

    newAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                         tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &SimpleWindow::newFile);

    saveAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
                         tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save current file"));
    connect(saveAct, &QAction::triggered, this, &SimpleWindow::saveFile);

    saveAsAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
                          tr("&Save As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save current file with another name!"));
    connect(saveAsAct, &QAction::triggered, this, &SimpleWindow::saveAsFile);


    exitAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::WindowClose),
                          tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Close);
    exitAct->setStatusTip(tr("Exit"));
    connect(exitAct, &QAction::triggered, this, &SimpleWindow::exitWindow);


    fileMenu->addAction(newAct);
    // fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    qbar->addMenu(fileMenu);


    editMenu = new QMenu(tr("&Edit"), qbar);

    undoAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo),
                         tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo an action"));
    connect(undoAct, &QAction::triggered, this, &SimpleWindow::undo);

    redoAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::EditRedo),
                         tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo an action"));
    connect(redoAct, &QAction::triggered, this, &SimpleWindow::redo);

    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    qbar->addMenu(editMenu);

}

void SimpleWindow::createActions(){
    connect(qcanvas, &QCanvas::mousePressedPaint, this, &SimpleWindow::onMousePressedPaint);
    connect(qcanvas, &QCanvas::mouseMovedPaint, this, &SimpleWindow::onMouseMovedPaint);
    connect(qcanvas, &QCanvas::mouseReleasedPaint, this, &SimpleWindow::onMouseReleasedPaint);

    connect(fdBtn, &QPushButton::clicked, this, &SimpleWindow::changeTool);
    connect(rdBtn, &QPushButton::clicked, this, &SimpleWindow::changeTool);
    connect(clBtn, &QPushButton::clicked, this, &SimpleWindow::changeTool);
    connect(ltBtn, &QPushButton::clicked, this, &SimpleWindow::changeTool);
    connect(clrPick, &color_widgets::ColorDialog::colorSelected, this, &SimpleWindow::changeColor);

    // connect(scRedo, SIGNAL(activated()), this, SLOT(redo()));
    // connect(scUndo, SIGNAL(activated()), this, SLOT(undo()));
}

QImage SimpleWindow::MatToQPixmap(const cv::Mat &image) {
    // Check if image is empty
    if (image.empty()) {
        std::cerr << "Empty image" << std::endl;
        return QImage();
    }

    // Convert the cv::Mat to QImage
    QImage qimage;

    // For color images (CV_8UC3, RGB format)
    if (image.type() == CV_8UC3) {
        // cv::Mat rgb;
        // cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB); // Convert BGR to RGB
        //image.copyTo(rgb);
        cv::Mat rgb(image.size(), image.type());
        int from_to[] = {0, 2, 1, 1, 2, 0}; // BGR to RGB mapping
        cv::mixChannels(&image, 1, &rgb, 1, from_to, 3);
        qimage = QImage(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
    }
    // For grayscale images (CV_8UC1)
    else if (image.type() == CV_8UC1) {
        qimage = QImage(image.data, image.cols, image.rows, static_cast<int>(image.step), QImage::Format_Grayscale8);
    }
    // Handle other types if needed (e.g., CV_8UC4, CV_16UC3, etc.)
    else {
        std::cerr << "Unsupported image format" << std::endl;
        return QImage();
    }

    // Convert QImage to QPixmap
    //QPixmap pixmap = QPixmap::fromImage(qimage);

    // Check if pixmap conversion is successful
    // if (pixmap.isNull()) {
    //     std::cerr << "Failed to convert cv::Mat to QPixmap." << std::endl;
    // }

    return qimage;
}
