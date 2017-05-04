/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    BDSDownloadQueue.h
 * @brief
 *******************************************************************************
 */
#ifndef BDSDOWNLOADQUEUE_H
#define BDSDOWNLOADQUEUE_H

#include <QObject>
#include <QQueue>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class BDSDownloadItem
{
public:
    uint getId() const;
    void setId(const uint &value);

    QString getUrl() const;
    void setUrl(const QString &value);

    QString getSavefilename() const;
    void setSavefilename(const QString &value);

    qint64 getBytesReceived() const;
    void setBytesReceived(const qint64 &value);

    qint64 getBytesTotal() const;
    void setBytesTotal(const qint64 &value);

    bool getIsValid() const;
    void setIsValid(bool isValid);

private:
    uint id;
    QString url;
    QString savefilename;
    qint64  bytesReceived;
    qint64  bytesTotal;
    bool    m_isValid;
};


class BDSDownloadQueue : public QObject
{
    Q_OBJECT
public:
    explicit BDSDownloadQueue(QObject *parent = 0);
    const QList<BDSDownloadItem> &downloadFiles() const;
    void setDownloadFiles(const QList<BDSDownloadItem> &downloadFiles);
    void stopDownload();
    void startDownload();
    QString getProcess();
    bool isDownloadOver();
    void append(const BDSDownloadItem &item);
    void closeSaveFile();

    uint setFileValid(uint id);

    enum  class DownloadStatus: uint
    {
        DOWNWAITING, DOWNLOADING, DOWNLOADABORT, DOWNLOADERR, DOWNFINISHED
    };

    DownloadStatus getStatus() const;
    void setStatus(const DownloadStatus &status);

signals:
    void apiAllDownlFinished();
    void signalDownloadFileFinished(uint id, const BDSDownloadQueue::DownloadStatus &status,const QString &msg);
    void apiDownloadProgress(uint id, qint64, qint64);
private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadError(QNetworkReply::NetworkError err);
    void downloadFinished();
    void downloadReadyRead();
private:
    QNetworkAccessManager m_networkManager;
    QList<BDSDownloadItem> m_downloadFiles;
    QQueue<BDSDownloadItem> m_queue;
    QFile          m_saveFile;
    QNetworkReply *m_reply;
    DownloadStatus m_status;
};

#endif // BDSDOWNLOADQUEUE_H
