/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    MainWindow.h
 * @brief
 *******************************************************************************
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "BDSDownladModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void appendItem(uint index, const QString &title, uint hash);


public slots:
    void removeDownloadItem(uint hash);
    void popDownItem(uint hash, bool down);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    BDSDownladModel *m_model;
};

#endif // MAINWINDOW_H
