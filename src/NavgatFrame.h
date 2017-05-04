/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    NavgatFrame.h
 * @brief
 *******************************************************************************
 */
#ifndef NAVGATFRAME_H
#define NAVGATFRAME_H

#include <QFrame>
#include "BDSDownloadQueueWidget.h"

namespace Ui {
class NavgatFrame;
}

class NavgatFrame : public QFrame
{
    Q_OBJECT

public:
    explicit NavgatFrame(BDSDownloadQueueWidget *downWidget,const QString &title, uint hash,QWidget *parent = 0);
    ~NavgatFrame();

    BDSDownloadQueueWidget *queueWiget() const;
    uint itemHash() const;

signals:
    void removItem(uint hash);
    void popDown(uint hash, bool down);
public slots:
    void downloadElementFinished(uint id, const BDSDownloadQueue::DownloadStatus &status, const QString &msg);

private slots:
    void on_startDownBtn_clicked();

    void on_rmDownBtn_clicked();

    void on_popdownBtn_toggled(bool checked);

private:
    Ui::NavgatFrame *ui;
    BDSDownloadQueueWidget *m_queueWiget;
    uint m_itemHash;
};

#endif // NAVGATFRAME_H
