#include "canvaswidget.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>

CanvasWidget::CanvasWidget(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this)), pixmapItem(new QGraphicsPixmapItem()) {

    scene->addItem(pixmapItem);
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);

    setDragMode(QGraphicsView::NoDrag); // Disable built-in dragging
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void CanvasWidget::setImage(const cv::Mat &image) {
    QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    pixmapItem->setPixmap(QPixmap::fromImage(img.rgbSwapped()));
    scene->setSceneRect(pixmapItem->boundingRect());
}

cv::Mat CanvasWidget::getImage() {
    QPixmap pixmap = pixmapItem->pixmap();
    return cv::Mat(pixmap.height(), pixmap.width(), CV_8UC4, const_cast<uchar*>(pixmap.toImage().bits())).clone();
}

// **ZOOMING FUNCTION (Mouse Wheel)**
void CanvasWidget::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor); // Zoom in
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Zoom out
}

// **START PANNING WHEN MIDDLE MOUSE BUTTON IS PRESSED**
void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        isPanning = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor); // Change cursor to indicate panning
    }
    QGraphicsView::mousePressEvent(event);
}

// **PAN THE VIEW WHEN MOVING THE MOUSE WHILE MIDDLE BUTTON IS PRESSED**
void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isPanning) {
        QPointF delta = mapToScene(event->pos()) - mapToScene(lastMousePos);
        translate(delta.x(), delta.y());
        lastMousePos = event->pos();
    }
    QGraphicsView::mouseMoveEvent(event);
}

// **STOP PANNING WHEN MIDDLE MOUSE BUTTON IS RELEASED**
void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor); // Restore normal cursor
    }
    QGraphicsView::mouseReleaseEvent(event);
}
