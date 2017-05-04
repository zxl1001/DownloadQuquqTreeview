/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    NavgatFrame.cpp
 * @brief
 *******************************************************************************
 */
#include "NavgatFrame.h"
#include "ui_NavgatFrame.h"
#include <QMessageBox>

#define DOWNBTN_START "Start"
#define DOWNBTN_STOP  "Stop"
#define DOWNBTN_FINEISER "OVER"
#define DOWNBTN_RM    "Remove"
NavgatFrame::NavgatFrame(BDSDownloadQueueWidget *downWidget, const QString &title, uint hash, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::NavgatFrame),
    m_itemHash(hash),
    m_queueWiget(Q_NULLPTR)
{
    ui->setupUi(this);
    ui->label->setText(title);
    m_queueWiget = downWidget;
    connect(m_queueWiget, SIGNAL(downloadElementFinished(uint,BDSDownloadQueue::DownloadStatus,QString)), this, SLOT(downloadElementFinished(uint,BDSDownloadQueue::DownloadStatus,QString)));
}

NavgatFrame::~NavgatFrame()
{
    qDebug()<<"~NavgatFrame()"<<m_itemHash;
//    if(m_queueWiget)
//    {   qDebug()<<"delete m_queueWidget:"<<m_itemHash;
//        if(ui->startDownBtn->text().trimmed() != DOWNBTN_FINEISER)
//        {
//            m_queueWiget->stopDownload();
//        }
//        delete m_queueWiget;
//    }
    delete ui;
}

BDSDownloadQueueWidget *NavgatFrame::queueWiget() const
{
    return m_queueWiget;
}

void NavgatFrame::downloadElementFinished(uint id, const BDSDownloadQueue::DownloadStatus &status, const QString &msg)
{
    qDebug()<<"下载完成.当前的状态:"<<id<<(uint)status<<msg;
//    if(id == 0)
//    {
//        ui->startDownBtn->setText(DOWNBTN_FINEISER);
//    }
    if(status == BDSDownloadQueue::DownloadStatus::DOWNFINISHED)
    {
//        ui->startDownBtn->setText(DOWNBTN_FINEISER);
        ui->startDownBtn->setText(DOWNBTN_STOP);
        ui->startDownBtn->setDisabled(true);
    }
    else if(status == BDSDownloadQueue::DownloadStatus::DOWNLOADABORT
            || status == BDSDownloadQueue::DownloadStatus::DOWNLOADERR)
    {
        ui->startDownBtn->setText(DOWNBTN_START);
    }
    else
    {
        ui->startDownBtn->setText(DOWNBTN_STOP);
    }
    ui->processLabel->setText(msg);
}

void NavgatFrame::on_startDownBtn_clicked()
{
    if(ui->startDownBtn->text().trimmed() == DOWNBTN_FINEISER)
    {
        QMessageBox::warning(Q_NULLPTR,tr("Warning"), tr("All file was be downloaded!"));
        return;
    }
    if(ui->startDownBtn->text().trimmed() == DOWNBTN_START)
    {
        ui->startDownBtn->setText(DOWNBTN_STOP);
        ui->processLabel->setText(m_queueWiget->getCurrentProcess());
        m_queueWiget->startDownload();
    }
    else
    {
        ui->startDownBtn->setText(DOWNBTN_START);
        m_queueWiget->stopDownload();
    }
}

void NavgatFrame::on_rmDownBtn_clicked()
{
    int re = QMessageBox::warning(Q_NULLPTR, tr("Warngin"),tr("Do you want to remove is item?"),QMessageBox::Cancel, QMessageBox::Yes);
    if(re != QMessageBox::Yes)
    {
        return;
    }
    emit removItem(m_itemHash);
}

uint NavgatFrame::itemHash() const
{
    return m_itemHash;
}


void NavgatFrame::on_popdownBtn_toggled(bool checked)
{
    emit popDown(m_itemHash, checked);
    if(checked)
    {
        ui->popdownBtn->setStyleSheet("QPushButton{border:none; background:none; border-image:url(://pics/arrow_expan.png)}");
    }
    else
    {
        ui->popdownBtn->setStyleSheet("QPushButton{border:none; background:none; border-image:url(://pics/arrow_right.png)}");
    }
}
