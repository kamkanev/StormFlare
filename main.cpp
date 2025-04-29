#include "gui/simplewindow.h"

#include <QtWidgets>


using namespace cv;
using namespace std;

// void CallbackMouse (int event, int x, int y, int flags, void* userdata);

int main(int argc, char *argv[])
{

    // Check if the image is created
    // successfully or not
    // if (!canvas.getImage().data) {
    //     cout << "Could not open or "
    //               << "find the image\n";

    //     return 0;
    // }

    //Q Apllication
    QApplication app(argc, argv);

    // Load the splash screen image
    QPixmap loadImage(":/SlashScreen/phoe_load4.png");  // Use a resource file or absolute path
    // QPixmap splashImage = loadImage.scaled(600, 600);
    QSplashScreen splash(loadImage.scaled(512, 512));
    splash.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // splash.resize(QSize(600, 600));

    // Create a progress bar
    QProgressBar progressBar(&splash);
    progressBar.setGeometry(10, splash.height() - 30, splash.width() - 20, 20);
    progressBar.setRange(0, 100);
    progressBar.show();

    // splash.move(splash.screen()->availableGeometry().center());
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - splash.width()) / 2;
    int y = (screenGeometry.height() - splash.height()) / 2;
    splash.move(x, y);
    // splash.move(screen()->availableGeometry().center());
    splash.show();

    SimpleWindow window;//(canvas.getImage());

    // Simulate loading steps
    for (int i = 0; i <= 100; i += 10) {
        progressBar.setValue(i);
        app.processEvents();  // Keep UI responsive
        QThread::msleep(200);
    }


    QTimer::singleShot(1000, [&]() { // 1-second delay
        splash.close();
        window.showMaximized();
    });


    return app.exec();
}
