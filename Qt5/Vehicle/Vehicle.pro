# ------------------------------------------------------------------------------
#
# GDW RPG Vehicles, a vehicle database for Traveller and other GDW derived RPGs.
#
# Copyright (C) 2018-2019 Michael N. Henry <mike.henry@mikehenry.se>
#
# This file is part of GDW RPG Vehicles.
#
# GDW RPG Vehicles is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 2 of the License, or (at your option) any
# later version.
#
# GDW RPG Vehicles is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.
#
# See the GNU General Public License for more details. You should have received
# a copy of the GNU General Public License along with GDW RPG Vehicles. If not,
# see <http://www.gnu.org/licenses/>.
#
# ------------------------------------------------------------------------------

QT       += core widgets svg
QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TEMPLATE = lib

TARGET = Vehicle
VERSION = 0.3.0
#CONFIG += staticlib

DEFINES += VEHICLE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    vehicle.cc \
    vehicleform.cc \
    vehicleitem.cc \
    vehiclemodel.cc

HEADERS += \
    vehicle_global.hh \
    vehicle.hh \
    vehicleform.hh \
    vehicleitem.hh \
    vehiclemodel.hh

FORMS += \
    vehicleform.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Object/release/ -lObject
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Object/debug/ -lObject
else:unix: LIBS += -L$$OUT_PWD/../Object/ -lObject

INCLUDEPATH += $$PWD/../Object
DEPENDPATH += $$PWD/../Object

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Weapon/release/ -lWeapon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Weapon/debug/ -lWeapon
else:unix: LIBS += -L$$OUT_PWD/../Weapon/ -lWeapon

INCLUDEPATH += $$PWD/../Weapon
DEPENDPATH += $$PWD/../Weapon

RESOURCES += \
    vehicle.qrc
