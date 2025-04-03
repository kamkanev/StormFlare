#include "canvas.h"

using namespace cv;

Canvas::Canvas(int weight, int height, Scalar color) {
    baseimage = Mat(weight, height, CV_8UC3,
                    color);
    baseimage.copyTo(image);
    baseimage.copyTo(drawimage);
}

Canvas::Canvas(Mat image){
    baseimage = image;
    baseimage.copyTo(image);
    baseimage.copyTo(drawimage);

}

void Canvas::addToHistory(P_Element* element){
    if(!future.empty()){
        future = std::stack<P_Element*>();
    }
    history.push(element);
}

void Canvas::undo(){
    if(history.empty()){
        return;
    }
    future.push(history.top());
    history.pop();

    baseimage.copyTo(image);
    baseimage.copyTo(drawimage);

    std::stack<P_Element*> stk_copy(history);
    while(!stk_copy.empty()) {
        stk_copy.top()->rebuild(drawimage, image);
        stk_copy.pop();
    }
}

void Canvas::redo(){
    if(future.empty()){
        return;
    }
    history.push(future.top());
    future.top()->rebuild(drawimage, image);
    future.pop();
}

void Canvas::update(){
    drawimage.copyTo(image);
}

Mat Canvas::getImage(){
    return image;
}

Mat Canvas::getDrawImage(){
    return drawimage;
}
