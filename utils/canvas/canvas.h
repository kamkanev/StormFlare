#ifndef CANVAS_H
#define CANVAS_H

#include<opencv2/core.hpp>
#include<stack>
#include<elements/p_element.h>

class Canvas
{
public:
    Canvas(int weight, int height, cv::Scalar color);
    Canvas(cv::Mat image);

    cv::Mat getImage();
    cv::Mat getDrawImage();
    void update();
    void addToHistory(P_Element* element);
    void undo();
    void redo();
    void clear();

private:
    cv::Mat image;
    cv::Mat drawimage;
    cv::Mat baseimage;
    std::stack<P_Element*> history;
    std::stack<P_Element*> future;

};

#endif // CANVAS_H
