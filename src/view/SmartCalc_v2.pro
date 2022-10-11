QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../controller/s21_controller.cc \
    ../model/s21_model.cc \
    credite_calc_v2/s21_credit_calc.cpp \
    deposite_calc_v2/s21_depos_calc.cpp \
    main.cpp \
    qcustomplot.cpp \
    s21_view.cpp

HEADERS += \
    ../controller/s21_controller.h \
    ../model/s21_model.h \
    credite_calc_v2/s21_credit_calc.h \
    deposite_calc_v2/s21_depos_calc.h \
    qcustomplot.h \
    s21_view.h

FORMS += \
    credite_calc_v2/s21_credit_calc.ui \
    deposite_calc_v2/depos_calc.ui \
    s21_view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
