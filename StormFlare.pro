QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StormFlare
TEMPLATE = app
CONFIG += console c++17
# CONFIG -= app_bundle
# CONFIG -= qt
CONFIG += link_pkgconfig
PKGCONFIG += opencv4

INCLUDEPATH += usr/include/opencv4
INCLUDEPATH += /home/kamen/miniconda3/include/opencv4/opencv2

DESTDIR = $$PWD

#LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

# wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes --debug=no)
# wxLinkOptions = $$system(wx-config --debug=no --libs --unicode=yes)
# LIBS += $$wxLinkOptions
# QMAKE_CXXFLAGS_RELEASE += $$wxCXXFLAGS
# QMAKE_CXXFLAGS_DEBUG += $$wxCXXFLAGS


SOURCES += \
        elements/circle_element.cpp \
        elements/line_element.cpp \
        elements/poly_element.cpp \
        elements/rect_element.cpp \
        gui/dialog/newdialog.cpp \
        gui/qcanvas.cpp \
        gui/simplewindow.cpp \
        gui/widgets/canvaswidget.cpp \
        utils/brush/brush.cpp \
        utils/canvas/canvas.cpp \
        utils/tools/circletool.cpp \
        elements/p_element.cpp \
        utils/tools/freedrawing.cpp \
        utils/filters/imagefilter.cpp \
        utils/tools/linetool.cpp \
        main.cpp \
        utils/tools/rectdrawing.cpp \
        utils/tools/shapetool.cpp

HEADERS += \
    elements/circle_element.h \
    elements/line_element.h \
    elements/poly_element.h \
    elements/rect_element.h \
    gui/dialog/newdialog.h \
    gui/qcanvas.h \
    gui/simplewindow.h \
    gui/widgets/canvaswidget.h \
    utils/brush/brush.h \
    utils/canvas/canvas.h \
    utils/tools/circletool.h \
    elements/p_element.h \
    utils/tools/freedrawing.h \
    utils/filters/imagefilter.h \
    utils/tools/linetool.h \
    utils/tools/rectdrawing.h \
    utils/tools/shapetool.h

INCLUDEPATH += $$PWD/src $$PWD/include

SOURCES += \
    $$PWD/src/QtColorWidgets/abstract_widget_list.cpp \
    $$PWD/src/QtColorWidgets/bound_color_selector.cpp \
    $$PWD/src/QtColorWidgets/color_2d_slider.cpp \
    $$PWD/src/QtColorWidgets/color_delegate.cpp \
    $$PWD/src/QtColorWidgets/color_dialog.cpp \
    $$PWD/src/QtColorWidgets/color_line_edit.cpp \
    $$PWD/src/QtColorWidgets/color_list_widget.cpp \
    $$PWD/src/QtColorWidgets/color_names.cpp \
    $$PWD/src/QtColorWidgets/color_palette.cpp \
    $$PWD/src/QtColorWidgets/color_palette_model.cpp \
    $$PWD/src/QtColorWidgets/color_palette_widget.cpp \
    $$PWD/src/QtColorWidgets/color_preview.cpp \
    $$PWD/src/QtColorWidgets/color_selector.cpp \
    $$PWD/src/QtColorWidgets/color_utils.cpp \
    $$PWD/src/QtColorWidgets/color_wheel.cpp \
    $$PWD/src/QtColorWidgets/gradient_editor.cpp \
    $$PWD/src/QtColorWidgets/gradient_list_model.cpp \
    $$PWD/src/QtColorWidgets/gradient_slider.cpp \
    $$PWD/src/QtColorWidgets/harmony_color_wheel.cpp \
    $$PWD/src/QtColorWidgets/hue_slider.cpp \
    $$PWD/src/QtColorWidgets/swatch.cpp

HEADERS += \
    $$PWD/include/QtColorWidgets/abstract_widget_list.hpp \
    $$PWD/include/QtColorWidgets/bound_color_selector.hpp \
    $$PWD/include/QtColorWidgets/color_2d_slider.hpp \
    $$PWD/include/QtColorWidgets/color_delegate.hpp \
    $$PWD/include/QtColorWidgets/color_dialog.hpp \
    $$PWD/include/QtColorWidgets/color_line_edit.hpp \
    $$PWD/include/QtColorWidgets/color_list_widget.hpp \
    $$PWD/include/QtColorWidgets/color_names.hpp \
    $$PWD/include/QtColorWidgets/color_palette.hpp \
    $$PWD/include/QtColorWidgets/color_palette_model.hpp \
    $$PWD/include/QtColorWidgets/color_palette_widget.hpp \
    $$PWD/include/QtColorWidgets/color_preview.hpp \
    $$PWD/include/QtColorWidgets/color_selector.hpp \
    $$PWD/include/QtColorWidgets/color_utils.hpp \
    $$PWD/include/QtColorWidgets/color_wheel.hpp \
    $$PWD/include/QtColorWidgets/color_wheel_private.hpp \
    $$PWD/include/QtColorWidgets/colorwidgets_global.hpp \
    $$PWD/include/QtColorWidgets/gradient_delegate.hpp \
    $$PWD/include/QtColorWidgets/gradient_editor.hpp \
    $$PWD/include/QtColorWidgets/gradient_helper.hpp \
    $$PWD/include/QtColorWidgets/gradient_list_model.hpp \
    $$PWD/include/QtColorWidgets/gradient_slider.hpp \
    $$PWD/include/QtColorWidgets/harmony_color_wheel.hpp \
    $$PWD/include/QtColorWidgets/hue_slider.hpp \
    $$PWD/include/QtColorWidgets/swatch.hpp

FORMS += \
    $$PWD/src/QtColorWidgets/color_dialog.ui \
    $$PWD/src/QtColorWidgets/color_palette_widget.ui

RESOURCES += \
    $$PWD/resources/QtColorWidgets/color_widgets.qrc \
    $$PWD/resources/Logo/logo.qrc \
    $$PWD/resources/SlashScreen/screens.qrc \
    $$PWD/resources/icons/icons.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
