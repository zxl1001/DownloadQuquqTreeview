/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    BDSDownladModel.cpp
 * @brief
 *******************************************************************************
 */
#include "BDSDownladModel.h"
#include <QEventLoop>
#include <QTimer>

BDSDownladModel::BDSDownladModel(QObject *parent)
    :QStandardItemModel(parent)
{

}

void BDSDownladModel::removeRowFow(QStandardItem *item)
{
    if(item != Q_NULLPTR)
    {
//        removeRows(item->row(),1);
        removeRow(item->row());
    }
    resetInternalData();
}
