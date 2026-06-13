#ifndef LISTWIDGETHELPER_H
#define LISTWIDGETHELPER_H

#include <QListWidget>
#include <QPdfDocument>
#include <QFileInfo>
#include <QPixmap>
#include <QPainter>
#include <QSize>
#include <QImage>

class ListWidgetHelper
{
private:
    QListWidget* listWidget;
public:
    ListWidgetHelper(QListWidget* listWidget);
    ~ListWidgetHelper();

    void addToListWidget(QPdfDocument* pdfDoc, QFileInfo fileInfo, QSize imageSize);
};

#endif // LISTWIDGETHELPER_H
