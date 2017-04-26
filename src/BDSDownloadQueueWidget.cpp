/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    BDSDownloadQueueWidget.cpp
 * @brief
 *******************************************************************************
 */
#include "BDSDownloadQueueWidget.h"
#include "ui_BDSDownloadQueueWidget.h"

#define DOWN_ID Qt::UserRole + 1

BDSDownloadQueueWidget::BDSDownloadQueueWidget(uint hash, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BDSDownloadQueueWidget),
    m_downloadModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    m_hashValue = hash;
    ui->downloadTableView->setModel(m_downloadModel);
    ui->downloadTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->downloadTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->downloadTableView->verticalHeader()->hide();
    ui->downloadTableView->horizontalHeader()->setStretchLastSection(true);
    ui->downloadTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->downloadTableView->horizontalHeader()->hide();
    ui->downloadTableView->setItemDelegateForColumn(1,new ProcessDelegate(1, this));
    ui->downloadTableView->setColumnWidth(0,300);
    ui->downloadTableView->setColumnWidth(1,400);
    ui->downloadTableView->setColumnWidth(2,200);
    ui->downloadTableView->verticalHeader()->setDefaultSectionSize(40);

    setDownloadList();

    connect(&m_downloadQueue, SIGNAL(apiDownloadProgress(uint,qint64,qint64)), this,SLOT(downloadProgress(uint,qint64,qint64)));
    connect(&m_downloadQueue, SIGNAL(signalFileFileFinished(uint,QString)), this,SLOT(downloadFinished(uint,QString)));
}

BDSDownloadQueueWidget::~BDSDownloadQueueWidget()
{
    qDebug()<<"zxl ~BDSDownloadQueueWidget"<<m_hashValue;
    delete ui;
}

uint BDSDownloadQueueWidget::hashValue() const
{
    return m_hashValue;
}

void BDSDownloadQueueWidget::setHashValue(const uint &hashValue)
{
    m_hashValue = hashValue;
}

void BDSDownloadQueueWidget::setDownloadList()
{
    m_downloadModel->removeRows(0,m_downloadModel->rowCount());
    for(int i=0; i< 3; ++i)
    {
        BDSDownloadItem item;
        item.setId(101+i);
        item.setUrl(/*"http://10.69.143.65:9000/download/"*/"http://10.69.143.65/m.mp4");
        item.setSavefilename(QString("/home/zxl/tmp/tm1_%1.mp4").arg(item.getId()));
        item.setIsValid(false);
        m_downloadQueue.append(item);

        QStandardItem *itemFirst = new QStandardItem(QString("download file_").arg(i));
        itemFirst->setData(item.getId(),DOWN_ID);
        m_downloadModel->setItem(i,0, itemFirst);
        m_downloadModel->setItem(i,1, new QStandardItem("0"));
        m_downloadModel->setItem(i,2, new QStandardItem("Waiting"));
    }
    m_downloadModel->setRowCount(3);
}
void BDSDownloadQueueWidget::cleanProcess()
{
    for(int i=0; i<m_downloadModel->rowCount(); ++i)
    {
        m_downloadModel->setData(m_downloadModel->index(i,1),"0",Qt::DisplayRole);
        m_downloadModel->setData(m_downloadModel->index(i,2), "Waiting", Qt::DisplayRole);
    }
}

void BDSDownloadQueueWidget::startDownload()
{
    m_downloadQueue.startDownload();
}

void BDSDownloadQueueWidget::stopDownload()
{
    m_downloadQueue.stopDownload();
}

QString BDSDownloadQueueWidget::getCurrentProcess()
{
    return m_downloadQueue.getProcess();
}

void BDSDownloadQueueWidget::downloadProgress(uint id, qint64 bytesReceived, qint64 bytesTotal)
{
    for(int i=0; i<m_downloadModel->rowCount(); ++i)
    {
        uint itemId = m_downloadModel->index(i,0).data(DOWN_ID).toUInt();
        if(itemId == id)
        {
            int value = bytesReceived * 100 / bytesTotal;
            m_downloadModel->setData(m_downloadModel->index(i,1),value, Qt::DisplayRole);
            m_downloadModel->setData(m_downloadModel->index(i,2),"Downloading...", Qt::DisplayRole);
            break;
        }
    }
}

void BDSDownloadQueueWidget::downloadFinished(uint id, QString msg)
{
    qDebug()<<"downloadFinished..."<<id<<msg;
    if(id == 0)
    {
        emit downloadElementFinish(id, m_downloadQueue.getProcess());
        return;
    }
    for(int i=0; i<m_downloadModel->rowCount(); ++i)
    {
        uint itemId = m_downloadModel->index(i,0).data(DOWN_ID).toUInt();
        if(itemId == id)
        {
            m_downloadModel->item(i,2)->setData(msg, Qt::DisplayRole);
            break;
        }
    }
}

//void BDSDownloadQueueWidget::on_startDownBtn_clicked(bool checked)
//{
//    if(checked)
//    {
//        if(ui->startDownBtn->text() == "Over")
//        {
//            ui->startDownBtn->setChecked(false);
//            return;
//        }
//        cleanDispay();
//        m_downloadQueue.startDownload();
//        ui->startDownBtn->setText("Stop");
//    }
//    else
//    {
//        m_downloadQueue.stopDownload();
//        ui->startDownBtn->setText("Retry");
//    }
//}
