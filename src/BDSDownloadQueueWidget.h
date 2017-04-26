/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    BDSDownloadQueueWidget.h
 * @brief
 *******************************************************************************
 */
#ifndef BDSDOWNLOADQUEUEWIDGET_H
#define BDSDOWNLOADQUEUEWIDGET_H

#include "BDSDownloadQueue.h"
#include "processdelegate.h"
#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class BDSDownloadQueueWidget;
}

class BDSDownloadQueueWidget : public QDialog
{
    Q_OBJECT

public:
    explicit BDSDownloadQueueWidget(uint hash,QWidget *parent = 0);
    ~BDSDownloadQueueWidget();

    uint hashValue() const;
    void setHashValue(const uint &hashValue);
    void setDownloadList();
    void cleanProcess();
    void startDownload();
    void stopDownload();
    QString getCurrentProcess();

signals:
    void downloadElementFinish(uint id, const QString &msg);

public slots:
    void downloadProgress(uint id, qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(uint id, QString msg);

private:
    Ui::BDSDownloadQueueWidget *ui;
    uint m_hashValue;
    QStandardItemModel *m_downloadModel;
    BDSDownloadQueue    m_downloadQueue;
};

#endif // BDSDOWNLOADQUEUEWIDGET_H
