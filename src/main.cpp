/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    main.cpp
 * @brief
 *******************************************************************************
 */
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qApp->setStyleSheet("QScrollBar{background:transparent; height:10px;}\
                        QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}\
                        QScrollBar::handle:hover{background:gray;}\
                        QScrollBar::sub-line{background:transparent;}\
                        QScrollBar::add-line{background:transparent;}\
                       QScrollBar{background:transparent; width: 10px;}\
                        QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}\
                        QScrollBar::handle:hover{background:gray;}\
                        QScrollBar::sub-line{background:transparent;}\
                        QScrollBar::add-line{background:transparent;}");
    return a.exec();
}
