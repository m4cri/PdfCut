#include "listwidgethelper.h"

ListWidgetHelper::ListWidgetHelper(QListWidget *listWidget)
{
    this->listWidget = listWidget;
}

ListWidgetHelper::~ListWidgetHelper()
{

}

void ListWidgetHelper::addToListWidget(QPdfDocument *pdfDoc, QFileInfo fileInfo, QSize imageSize)
{
    for (int i = 0; i < pdfDoc->pageCount(); ++i) {
        QImage pdfImage = pdfDoc->render(i,imageSize);

        QImage solidImage(pdfImage.size(), QImage::Format_ARGB32);
        solidImage.fill(Qt::white);

        QPainter painter(&solidImage);
        painter.drawImage(0, 0, pdfImage);
        painter.end();


        QPixmap pixmap = QPixmap::fromImage(solidImage);

        QString realName = fileInfo.fileName();
        if(realName.size()>10) realName = realName.left(7) + "...";

        QListWidgetItem *item = new QListWidgetItem(QIcon(pixmap), QString("Page %1 (%2)").arg(i + 1).arg(realName));

        this->listWidget->addItem(item);
    }
}
