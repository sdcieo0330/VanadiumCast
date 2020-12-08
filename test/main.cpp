//
// Created by silas on 11/29/20.
//

#include "Networking/CachedStreamTest.h"
#include "MediaProcessing/CachedLocalStreamTest.h"
#include "GUI/VideoGui.h"

// Surround every main impl of QtTest with a pair of brackets, because every main impl contains the same var names
#define TEST(TestObject) { \
    TestObject tc; \
    result = QTest::qExec(&tc, argc, argv); \
}

int main(int argc, char *argv[]) {
    size_t testCount = 1;

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi);
    QTest::setMainSourcePath("_file_name_", "/home/silas/CLionProjects/VanadiumCast/cmake-build-debug");

    int result = 0;

    for (size_t i = 0; i < testCount; ++i) {
        TEST(CachedStreamTest)
        if (result != 0) {
            break;
        }
        TEST(CachedLocalStreamTest)
        if (result != 0) {
            break;
        }
    }
//    QFile inputFile("/home/silas/test.mp4");
//    inputFile.open(QIODevice::ReadWrite);
//    QtAV::VideoRenderer *renderer = QtAV::VideoRenderer::create(QtAV::VideoRendererId_OpenGLWindow);
//    QtAV::AVPlayer avPlayer;
//    avPlayer.setRenderer(renderer);
////    avPlayer.setFile("/home/silas/test.mp4");
//    avPlayer.setIODevice(&inputFile);
//    avPlayer.setBufferMode(QtAV::BufferBytes);
//    avPlayer.setBufferValue(1000000);
//    avPlayer.setSpeed(1);
//    renderer->qwindow()->showMaximized();
//    avPlayer.play();
//    return app.exec();
}