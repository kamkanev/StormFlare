#ifndef QCANVAS_H
#define QCANVAS_H

#include <QLabel>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <opencv2/core.hpp>

class QCanvas : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QCanvas(QWidget *parent = nullptr);
    void setImage(const cv::Mat &image);

signals:
    void mousePressedPaint(QPoint event);
    void mouseMovedPaint(QPoint event);
    void mouseReleasedPaint(QPoint event);

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

#endif // QCANVAS_H
