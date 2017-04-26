/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    BDSDownlaodQueueToolBox.h
 * @brief
 *******************************************************************************
 */
#ifndef BDSDOWNLAODQUEUETOOLBOX_H
#define BDSDOWNLAODQUEUETOOLBOX_H

#include <QToolBox>

namespace Ui {
class BDSDownlaodQueueToolBox;
}

class BDSDownlaodQueueToolBox : public QToolBox
{
    Q_OBJECT

public:
    explicit BDSDownlaodQueueToolBox(QWidget *parent = 0);
    ~BDSDownlaodQueueToolBox();

private:
    Ui::BDSDownlaodQueueToolBox *ui;
};

#endif // BDSDOWNLAODQUEUETOOLBOX_H
