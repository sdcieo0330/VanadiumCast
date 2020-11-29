//
// Created by silas on 11/29/20.
//

#include "Networking/CachedStreamTest.h"
#include "MediaProcessing/CachedLocalStreamTest.h"

// Surround every main impl of QtTest with a pair of brackets, because every main impl contains the same var names
#define TEST(TestObject) { \
    TestObject tc; \
    QTest::qExec(&tc, argc, argv); \
}

int main(int argc, char *argv[]) {
    size_t testCount = 1;
    if (argc > 1) {
        testCount = strtoul(argv[1], nullptr, 10);
    }

    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi);
    QTest::setMainSourcePath("_file_name_", "/home/silas/CLionProjects/VanadiumCast/cmake-build-debug");

    for (size_t i = 0; i < testCount; ++i) {
        TEST(CachedStreamTest)
        TEST(CachedLocalStreamTest)
    }
}