#ifndef DRAGGABLEPAGELIST_H
#define DRAGGABLEPAGELIST_H

#include <QListWidget>
#include <QDropEvent>
#include <QDragMoveEvent>

class DraggablePageList : public QListWidget
{
    Q_OBJECT
public:
    explicit DraggablePageList(QWidget *parent = nullptr);

protected:
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;

signals:
    void orderChanged(int from, int to);
};

#endif // DRAGGABLEPAGELIST_H
