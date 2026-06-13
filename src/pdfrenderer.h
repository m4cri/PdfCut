#ifndef PDFRENDERER_H
#define PDFRENDERER_H

#include<QPdfDocument>
#include<QListWidget>
#include<QSize>
#include<vector>
#include<QFileInfo>
#include<QFile>
#include<memory>
#include"listwidgethelper.h"

#include<qpdf/QPDF.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>
#include <qpdf/Buffer.hh>

class PdfContainer
{
private:
    ListWidgetHelper* listWidgetHelper;
    std::vector<QPDFPageObjectHelper> pdfPages;
    std::vector<std::unique_ptr<QPDF>> sourcePdfs;

public:
    PdfContainer(ListWidgetHelper *listWidgetHelper);
    ~PdfContainer();

    void addToList(QString fileName);

    int getPagesCount();
    int getFilesCount();
    std::string getFileName(); //Only used when exactly one file is loaded, returns the name of the firts file in sourcePdfs

    void saveToPdf(std::string fileName);

    void handleOrderChanged(int from, int to);// Works as a slot
    void deletePage(int index);
    void clearPages();


};

#endif // PDFRENDERER_H
