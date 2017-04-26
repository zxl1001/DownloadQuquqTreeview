/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    BDSDownladModel.h
 * @brief
 *******************************************************************************
 */
#ifndef BDSDOWNLADMODEL_H
#define BDSDOWNLADMODEL_H

#include <QStandardItemModel>


class BDSDownladModel : public QStandardItemModel
{
public:
    explicit BDSDownladModel(QObject *parent = Q_NULLPTR);
    void removeRowFow(QStandardItem *item);
};

#endif // BDSDOWNLADMODEL_H
