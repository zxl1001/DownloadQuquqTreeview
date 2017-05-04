/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    MainWindow.cpp
 * @brief
 *******************************************************************************
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "NavgatFrame.h"
#include "BDSDownloadQueueWidget.h"

#include <QPushButton>
#include <QDebug>
#include <QComboBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_model(new BDSDownladModel(ui->treeView))
{
    ui->setupUi(this);

    m_model->setColumnCount(1);
    ui->treeView->setIndentation(0);
    ui->treeView->setModel(m_model);
    ui->treeView->setColumnWidth(0,200);
    ui->treeView->setColumnWidth(1,400);
    ui->treeView->setColumnWidth(2,150);
    for(int i = 0; i<10; ++i)
    {
        QString title = QString("BDSR-000%1_1.2").arg(i);
        uint hash = qHash(title);
        appendItem(i,title, hash);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendItem(uint index, const QString &title, uint hash)
{
    QStandardItem *section = new QStandardItem(QIcon("://down_ing.png"),QString());
    section->setData(hash);
    QStandardItem *item    = new QStandardItem(tr("Item %1").arg(index));
    section->appendRow(item);
    m_model->appendRow(section);

    uint rowCount = 1 + qrand() % 10;
    BDSDownloadQueueWidget *downWidget = new BDSDownloadQueueWidget(rowCount,hash);
    NavgatFrame *sectionFrame = new NavgatFrame(downWidget,title,hash);

//    ui->treeView->setIndexWidget(m_model->index(index,0),sectionFrame);
    downWidget->setMaximumHeight(rowCount < 5 ? rowCount * 41 : 5 * 41);
    ui->treeView->setIndexWidget(m_model->indexFromItem(section),sectionFrame);
    ui->treeView->setIndexWidget(m_model->indexFromItem(item), downWidget);
    connect(sectionFrame, SIGNAL(removItem(uint)), this, SLOT(removeDownloadItem(uint)));
    connect(sectionFrame, SIGNAL(popDown(uint,bool)), this, SLOT(popDownItem(uint,bool)));
}

void MainWindow::removeDownloadItem(uint hash)
{
    qDebug()<<__FILE__<<__LINE__<<"removeDownloadItem hash:"<<hash;
    for(int i=0; i<m_model->rowCount(); ++i)
    {
        QStandardItem *item = m_model->item(i,0);
        if(hash ==  item->data(Qt::UserRole+1).toUInt())
        {
            if(item->rowCount()>0)
            {
                item->removeRows(0,item->rowCount());
            }
            ui->treeView->setIndexWidget(item->index(), Q_NULLPTR);
            m_model->removeRowFow(item);
            return;
        }
    }
    m_model->revert();
}

void MainWindow::popDownItem(uint hash, bool down)
{
    for(int i=0; i<m_model->rowCount(); ++i)
    {
        QStandardItem *item = m_model->item(i,0);
        if(hash == item->data(Qt::UserRole+1).toUInt())
        {
            if(down)
            {
                ui->treeView->expand(item->index());
            }
            else
            {
                ui->treeView->collapse(item->index());
            }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    for(int i=100; i<105; ++i)
    {
        QString title = QString("BDSR-000%1_1.2").arg(i);
        uint hash = qHash(title);
        appendItem(i,title, hash);
    }
    for(int i=0; i < m_model->rowCount(); ++i)
    {
        qDebug()<<"QTreeView visualRect:"<<ui->treeView->visualRect(m_model->index(i,0));
    }
}
