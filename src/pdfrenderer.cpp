#include "pdfrenderer.h"

PdfContainer::PdfContainer(ListWidgetHelper *listWidgetHelper)
{
    this->listWidgetHelper = listWidgetHelper;
}

PdfContainer::~PdfContainer(){}

void PdfContainer::addToList(QString filePath){

    QPdfDocument pdfDoc;
    auto qPdfObject = std::make_unique<QPDF>();

     if(pdfDoc.load(filePath) != QPdfDocument::Error::None){
         throw std::runtime_error("Failed to open pdf");
     }

     QByteArray qb = filePath.toLocal8Bit();
     qPdfObject->processFile(qb.data());

    QSize imageSize(200,160);
    QFileInfo fileInfo(filePath);

    this->listWidgetHelper->addToListWidget(&pdfDoc,fileInfo,imageSize);

    std::vector<QPDFPageObjectHelper> currPdfPages = QPDFPageDocumentHelper(*qPdfObject).getAllPages();
    pdfPages.insert(std::end(pdfPages), std::begin(currPdfPages), std::end(currPdfPages));

    sourcePdfs.push_back(std::move(qPdfObject));
}

int PdfContainer::getPagesCount()
{
    return this->pdfPages.size();
}

int PdfContainer::getFilesCount()
{
    return this->sourcePdfs.size();
}

std::string PdfContainer::getFileName()
{
    return sourcePdfs.at(0)->getFilename();
}

void PdfContainer::saveToPdf(std::string fileName)
{
    QPDF outFile;
    outFile.emptyPDF();

    for (auto& page: pdfPages) {
        QPDFPageDocumentHelper(outFile).addPage(page,false);
    }

    QPDFWriter outpdfw(outFile);
    outpdfw.setOutputMemory();
    outpdfw.write();

    //Needed for windows compatibility
    std::shared_ptr<Buffer> buffer = outpdfw.getBufferSharedPointer();

    QFile file(QString::fromStdString(fileName));

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        file.write(reinterpret_cast<const char*>(buffer->getBuffer()), buffer->getSize());
        file.close();
    }

    pdfPages.clear();
    sourcePdfs.clear();

    auto newPdfObject = std::make_unique<QPDF>();
    newPdfObject->processFile(fileName.c_str());

    std::vector<QPDFPageObjectHelper> newPages = QPDFPageDocumentHelper(*newPdfObject).getAllPages();
    pdfPages.insert(std::end(pdfPages), std::begin(newPages), std::end(newPages));

    sourcePdfs.push_back(std::move(newPdfObject));
}

void PdfContainer::handleOrderChanged(int from, int to)
{
    QPDFPageObjectHelper toPage = pdfPages.at(to);

    pdfPages.at(to) = pdfPages.at(from);
    pdfPages.at(from) = toPage;
}

void PdfContainer::deletePage(int index)
{
    pdfPages.erase(pdfPages.begin()+index);
}

void PdfContainer::clearPages()
{
    pdfPages.clear();
    sourcePdfs.clear();
}
