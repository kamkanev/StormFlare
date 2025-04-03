#include "qcanvas.h"

#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>

QCanvas::QCanvas(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this)), pixmapItem(new QGraphicsPixmapItem()) {

    scene->addItem(pixmapItem);
    setScene(scene);

    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);

    setDragMode(QGraphicsView::NoDrag); // Disable built-in dragging
    // setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void QCanvas::setImage(const cv::Mat &image) {
    QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    pixmapItem->setPixmap(QPixmap::fromImage(img.rgbSwapped()));
    scene->setSceneRect(pixmapItem->boundingRect());
}

// **ZOOMING FUNCTION (Mouse Wheel)** //
void QCanvas::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor); // Zoom in
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor); // Zoom out
}

void QCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        isPanning = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor); // Change cursor to indicate panning
    } else if (event->button() == Qt::LeftButton) {

        QPointF scenePoint = mapToScene(event->pos());
        emit mousePressedPaint(scenePoint.toPoint());
    }
}

void QCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (isPanning) {
        QPointF delta = mapToScene(event->pos()) - mapToScene(lastMousePos);
        translate(delta.x(), delta.y());
        lastMousePos = event->pos();
    }else{

        QPointF scenePoint = mapToScene(event->pos());
        emit mouseMovedPaint(scenePoint.toPoint());
    }

}

void QCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor); // Restore normal cursor
    } else if (event->button() == Qt::LeftButton) {

        QPointF scenePoint = mapToScene(event->pos());
        emit mouseReleasedPaint(scenePoint.toPoint());
    }
}
