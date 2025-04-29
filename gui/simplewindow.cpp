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
    setWindowIcon(QIcon(":/Logo/logo2STORMFLARE.png"));
    setWindowIconText(tr("Stormflare v0.2.0"));

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

    QVBoxLayout *layoutB = new QVBoxLayout();
    QLabel *labelSize = new QLabel("Brush Size:", this);
    sizeSpin = new QSpinBox(this);
    sizeSpin->setMaximumWidth(100);
    sizeSpin->setRange(1, 100);
    sizeSpin->setValue(brush->getSize());


    layoutB->addWidget(labelSize);
    layoutB->addWidget(sizeSpin);


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

    inner->addLayout(layoutB);


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

    newAct = new QAction(QIcon::fromTheme("document-new", QIcon(":/icons/new.svg")),
                         tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &SimpleWindow::newFile);

    openAct = new QAction(QIcon::fromTheme("document-open", QIcon(":/icons/open.svg")),
                         tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a file"));
    connect(openAct, &QAction::triggered, this, &SimpleWindow::openFile);


    saveAct = new QAction(QIcon::fromTheme("document-save", QIcon(":/icons/save.svg")),
                         tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save current file"));
    connect(saveAct, &QAction::triggered, this, &SimpleWindow::saveFile);

    saveAsAct = new QAction(QIcon::fromTheme("document-save-as", QIcon(":/icons/save-as.svg")),
                          tr("&Save As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save current file with another name!"));
    connect(saveAsAct, &QAction::triggered, this, &SimpleWindow::saveAsFile);


    exitAct = new QAction(QIcon::fromTheme("window-close", QIcon(":/icons/close.svg")),
                          tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Close);
    exitAct->setStatusTip(tr("Exit"));
    connect(exitAct, &QAction::triggered, this, &SimpleWindow::exitWindow);


    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    qbar->addMenu(fileMenu);


    editMenu = new QMenu(tr("&Edit"), qbar);

    undoAct = new QAction(QIcon::fromTheme("edit-undo", QIcon(":/icons/undo.svg")),
                         tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo an action"));
    connect(undoAct, &QAction::triggered, this, &SimpleWindow::undo);

    redoAct = new QAction(QIcon::fromTheme("edit-redo", QIcon(":/icons/redo.svg")),
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

    connect(sizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &SimpleWindow::setBrushSize);
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

void SimpleWindow::setBrushSize(int newSize){
    brush->setSize(newSize);
}

void SimpleWindow::update(){
    st->focus(canvas.getImage());

    qcanvas->setImage(canvas.getImage());
    canvas.update();
}

void SimpleWindow::undo(){
    canvas.undo();
}

void SimpleWindow::redo(){
    canvas.redo();
}

void SimpleWindow::changeColor(QColor color){
    // qDebug() << color.red() << " " << fd.getBrush().getColor().val;
    int r, g, b;
    color.getRgb(&r, &g, &b);
    brush->setColor(cv::Scalar(b,g,r));
}

void SimpleWindow::changeTool(bool clicked){

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

void SimpleWindow::onMousePressedPaint(QPoint event) {
    st->updateTool(event.x(), event.y());
    // qDebug() << "Mouse Pressed at:" << st->getBrush().getMouse().x << " , " << st->getBrush().getMouse().y;

}

void SimpleWindow::onMouseMovedPaint(QPoint event) {
    // qDebug() << "Mouse Moved to:" << event->pos();
    st->updateDraw(canvas.getDrawImage(), event.x(), event.y());
}

void SimpleWindow::onMouseReleasedPaint(QPoint event) {
    // qDebug() << "Mouse Released at:" << event->pos();
    canvas.addToHistory(st->draw(canvas.getDrawImage(), event.x(), event.y()));
    st->updateTool(-1, -1);
}

void SimpleWindow::newFile(){

    NewDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int width = dialog.getWidth();
        int height = dialog.getHeight();
        // qDebug() << "User selected width:" << width << "height:" << height;
        canvas = Canvas(height, width, cv::Scalar(255, 255, 255));
    }
    //If dialog is closed / not accepted didn't do it

}

void SimpleWindow::saveFile(){
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

void SimpleWindow::saveAsFile(){

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

void SimpleWindow::openFile(){

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



void SimpleWindow::exitWindow(){
    this->close();
}
