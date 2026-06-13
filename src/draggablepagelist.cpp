#include "draggablepagelist.h"

DraggablePageList::DraggablePageList(QWidget *parent)
    : QListWidget(parent)
{
    setViewMode(QListView::IconMode);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setResizeMode(QListView::Adjust);
    setWrapping(true);
    setSpacing(10);
}

void DraggablePageList::dragMoveEvent(QDragMoveEvent *event)
{

    event->setDropAction(Qt::MoveAction);
    event->accept();

}

void DraggablePageList::dropEvent(QDropEvent *event)
{
    QListWidgetItem *draggedItem = currentItem();
    //QModelIndex fromPositionModel = QListWidget::indexFromItem(draggedItem);
    if (!draggedItem) {
        event->ignore();
        return;
    }


    QListWidgetItem *targetItem = itemAt(event->position().toPoint());

    int fromRow = row(draggedItem);
    int toRow = targetItem ? row(targetItem) : count();


    if (fromRow == toRow) {
        event->ignore();
        return;
    }


    QListWidgetItem *taken = takeItem(fromRow);

    if (!targetItem) {

        addItem(taken);

    }
    else
    {
        insertItem(toRow, taken);
    }


    setCurrentItem(taken);

    event->setDropAction(Qt::IgnoreAction);
    event->accept();

    if (model()) {
        emit model()->layoutChanged();
        emit this->orderChanged(fromRow,toRow); //Sends the starting index and the destination index of an item that was reordered
    }
}