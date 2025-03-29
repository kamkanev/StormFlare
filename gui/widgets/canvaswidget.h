#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>

class CanvasWidget : public QGraphicsView {
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget *parent = nullptr);
    void setImage(const cv::Mat &image);
    cv::Mat getImage();

protected:
    void wheelEvent(QWheelEvent *event) override;        // Zooming
    void mousePressEvent(QMouseEvent *event) override;   // Start dragging
    void mouseMoveEvent(QMouseEvent *event) override;    // Dragging
    void mouseReleaseEvent(QMouseEvent *event) override; // Stop dragging

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;
    bool isPanning = false;
    QPoint lastMousePos;
};

#endif // CANVASWIDGET_H
