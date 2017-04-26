/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    BDSDownloadQueue.cpp
 * @brief
 *******************************************************************************
 */
#include "BDSDownloadQueue.h"

#include <QDir>
#include <QFileInfo>

#define DOWN_ITEM_ID "ID"
#define DOWN_ITEM_SAVE_NAME "SAVE_NAME"

BDSDownloadQueue::BDSDownloadQueue(QObject *parent) : QObject(parent),
    m_reply(Q_NULLPTR),
    m_status(DownloadStatus::DOWNWAITING)
{

}
void BDSDownloadQueue::startDownload()
{
    qDebug()<<"start download queue:";
    if(m_status == DownloadStatus::DOWNLOADING)
    {
        return;
    }
    else if(m_status == DownloadStatus::DOWNLOADABORT)
    {
        qDebug()<<"dowlnlad is finished";
        m_queue.clear();
        for(int i=0; i< m_downloadFiles.count(); ++i)
        {
            m_queue.append(m_downloadFiles.at(i));
        }
        startNextDownload();
    }
    else if(m_status == DownloadStatus::DOWNWAITING)
    {
        startNextDownload();
    }
}

void BDSDownloadQueue::startNextDownload()
{
    if(isDownloadOver())
    {
        emit signalFileFileFinished(0,"Abort!");
        return;
    }
    if(m_queue.isEmpty())
    {
        return;
    }
    m_status = DownloadStatus::DOWNLOADING;

    BDSDownloadItem curDownloadItem = m_queue.dequeue();
    if(curDownloadItem.getIsValid())
    {
        emit signalFileFileFinished(curDownloadItem.getId(),"Finished!");
        startNextDownload();
        return;
    }

    m_saveFile.setFileName(curDownloadItem.getSavefilename());
    QFileInfo fileInfo(m_saveFile);
    QDir dir;
    if(!dir.mkpath(fileInfo.absolutePath()))
    {
        qDebug()<<"file path is create fail! path:"<<fileInfo.absolutePath();
        return;
    }
    if(!m_saveFile.open(QIODevice::WriteOnly))
    {
        qDebug()<<"problem opening save file! path:"<<fileInfo.absolutePath();
        startNextDownload();
        return;
    }
    QNetworkRequest req(curDownloadItem.getUrl());
    m_reply = m_networkManager.get(req);
    m_reply->setProperty(DOWN_ITEM_ID,curDownloadItem.getId());
    m_reply->setProperty(DOWN_ITEM_SAVE_NAME, curDownloadItem.getSavefilename());
    connect(m_reply, SIGNAL(downloadProgress(qint64, qint64)),SLOT(downloadProgress(qint64,qint64)));
    connect(m_reply, SIGNAL(finished()),  SLOT(downloadFinished()));
    connect(m_reply, SIGNAL(readyRead()), SLOT(downloadReadyRead()));
}

void BDSDownloadQueue::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit apiDownloadProgress(m_reply->property(DOWN_ITEM_ID).toUInt(), bytesReceived, bytesTotal );
}

void BDSDownloadQueue::downloadFinished()
{
    closeSaveFile();
    uint id = m_reply->property(DOWN_ITEM_ID).toUInt();
    if(m_reply->error())
    {
        //download failed
        qDebug()<<"Failed"<<m_reply->errorString();
        emit signalFileFileFinished(id,m_reply->errorString());
    }
    else
    {
        setFileValid(id);
        emit signalFileFileFinished(id,"Finished!");
    }
    m_reply->deleteLater();
    if(m_status == DownloadStatus::DOWNLOADABORT)
    {
        return;
    }
    startNextDownload();
}

uint BDSDownloadQueue::setFileValid(uint id)
{
    for(int i=0; i< m_downloadFiles.count(); ++i)
    {
        auto &item = m_downloadFiles[i];
        if(item.getId() == id)
        {
            item.setIsValid(true);
            return id;
        }
    }
    return 0;
}

void BDSDownloadQueue::downloadReadyRead()
{
    while(!m_reply->atEnd())
    {
        m_saveFile.write(m_reply->read(512));
    }
}

BDSDownloadQueue::DownloadStatus BDSDownloadQueue::getStatus() const
{
    return m_status;
}

void BDSDownloadQueue::setStatus(const DownloadStatus &status)
{
    m_status = status;
}

const QList<BDSDownloadItem> &BDSDownloadQueue::downloadFiles() const
{
    return m_downloadFiles;
}

void BDSDownloadQueue::setDownloadFiles(const QList<BDSDownloadItem> &downloadFiles)
{
    m_downloadFiles = downloadFiles;
    foreach (auto &item, m_downloadFiles) {
        m_queue.append(item);
    }
}

void BDSDownloadQueue::stopDownload()
{
    qDebug()<<"stopDownload";

    closeSaveFile();
    if(m_status == DownloadStatus::DOWNLOADABORT)
    {
        return;
    }
    if(m_reply)
    {
        qDebug()<<"start.....is open:"<<m_reply->isOpen()<<"is running:"<<m_reply->isRunning()<<"is finished:"<<m_reply->isFinished();
        if(m_reply->isFinished())
        {
            return;
        }
        m_reply->abort();
        qDebug()<<"end.....is open:"<<m_reply->isOpen()<<"is running:"<<m_reply->isRunning()<<"is finished:"<<m_reply->isFinished();
        m_status = DownloadStatus::DOWNLOADABORT;
        m_reply->disconnect();
    }
}

QString BDSDownloadQueue::getProcess()
{
    //return QString::number(100 - (m_queue.count() * 100 / m_downloadFiles.count() / 100));
    if(m_downloadFiles.count()==0)
    {
        return QString();
    }
    return QString("%1 / %2").arg(m_downloadFiles.count() - m_queue.count()).arg(m_downloadFiles.count());
}

bool BDSDownloadQueue::isDownloadOver()
{
    foreach (auto &item, m_downloadFiles) {
        if(!item.getIsValid())
        {
            return false;
        }
    }
    return true;
}

void BDSDownloadQueue::append(const BDSDownloadItem &item)
{
    m_downloadFiles.append(item);
    m_queue.append(item);
}

void BDSDownloadQueue::closeSaveFile()
{
    if(m_saveFile.isOpen())
    {
        m_saveFile.flush();
        m_saveFile.close();
    }
}

uint BDSDownloadItem::getId() const
{
    return id;
}

void BDSDownloadItem::setId(const uint &value)
{
    id = value;
}

QString BDSDownloadItem::getUrl() const
{
    return url;
}

void BDSDownloadItem::setUrl(const QString &value)
{
    url = value;
}

QString BDSDownloadItem::getSavefilename() const
{
    return savefilename;
}

void BDSDownloadItem::setSavefilename(const QString &value)
{
    savefilename = value;
}

qint64 BDSDownloadItem::getBytesReceived() const
{
    return bytesReceived;
}

void BDSDownloadItem::setBytesReceived(const qint64 &value)
{
    bytesReceived = value;
}

qint64 BDSDownloadItem::getBytesTotal() const
{
    return bytesTotal;
}

void BDSDownloadItem::setBytesTotal(const qint64 &value)
{
    bytesTotal = value;
}

bool BDSDownloadItem::getIsValid() const
{
    return m_isValid;
}

void BDSDownloadItem::setIsValid(bool isValid)
{
    m_isValid = isValid;
}
