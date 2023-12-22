QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    ./src/AbsValueSC.cpp \
    ./src/BoothFunction.cpp \
    ./src/DeterministicOptimizer.cpp \
    ./src/Function.cpp \
    ./src/McCormickFunction.cpp \
    ./src/NumIterSC.cpp \
    ./src/Optimizer.cpp \
    ./src/RectArea.cpp \
    ./src/RosenbrockFunction.cpp \
    ./src/StochasticOptimizer.cpp \
    ./src/StopCriterion.cpp \
    mainwindow.cpp \
    mygraphicsscene.cpp \
    settings.cpp

HEADERS += \
    ./include/AbsValueSC.h \
    ./include/BoothFunction.h \
    ./include/DeterministicOptimizer.h \
    ./include/Function.h \
    ./include/McCormickFunction.h \
    ./include/NumIterSC.h \
    ./include/Optimizer.h \
    ./include/RectArea.h \
    ./include/RosenbrockFunction.h \
    ./include/StochasticOptimizer.h \
    ./include/StopCriterion.h \
    mainwindow.h \
    mygraphicsscene.h \
    settings.h

FORMS += \
    mainwindow.ui \
    settings.ui

INCLUDEPATH += \
    ./eigen \
    ./include \
    ./src

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
