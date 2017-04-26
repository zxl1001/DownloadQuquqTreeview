#ifndef PROCESSDELEGATE_H
#define PROCESSDELEGATE_H
#include <QStyledItemDelegate>

class ProcessDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ProcessDelegate(uint column,QObject *parent = Q_NULLPTR);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &code) const;
private:
    uint m_column;
};

#endif // PROCESSDELEGATE_H
