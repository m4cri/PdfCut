#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QPdfDocument>
#include <QDebug>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QtGlobal>
#include <qpdf/QPDF.hh>

#define UPDATE_PAGECOUNTER ui->totalPages->setText(QString("%1 pages total").arg(pdfRend->getPagesCount()));

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("PdfCut");

    ui->actionSave->setEnabled(false);
    ui->actionSave_As->setEnabled(false);
    ui->actionDelete_selected_page->setEnabled(false);
    ui->actionClear_all->setEnabled(false);
    ui->deleteToolButton->setEnabled(false);

    //Menubar actions
    QObject::connect(ui->actionQuit,&QAction::triggered,this,&MainWindow::quitActionHandler);
    QObject::connect(ui->actionAbout_PdfCut,&QAction::triggered,this,&MainWindow::aboutProjectHandler);
    QObject::connect(ui->actionAdd_File_s,&QAction::triggered,this,&MainWindow::addFileHandler);
    QObject::connect(ui->actionDelete_selected_page,&QAction::triggered,this,&MainWindow::deletePageHandler);
    QObject::connect(ui->actionClear_all,&QAction::triggered,this,&MainWindow::clearAllHandler);
    QObject::connect(ui->actionSave,&QAction::triggered,this,&MainWindow::saveHandler);
    QObject::connect(ui->actionSave_As,&QAction::triggered,this,&MainWindow::saveAsHandler);
    QObject::connect(ui->actionOpen_File,&QAction::triggered,this,&MainWindow::openFileHandler);
    QObject::connect(ui->actionDocumentation,&QAction::triggered,this,&MainWindow::documentationHandler);

    QObject::connect(ui->addFileButton,&QPushButton::clicked,this,&MainWindow::addFileHandler);

    QListWidget *pdfList = ui->pdfListWidget;
    listWHelpr = new ListWidgetHelper(pdfList);
    pdfRend = new PdfContainer(listWHelpr);

    //Makes the vector storing the pages change when the list is reordered
    QObject::connect(ui->pdfListWidget, &DraggablePageList::orderChanged, this, [this](int from, int to) {
        pdfRend->handleOrderChanged(from, to);
    });

    //Toolbar and workspace buttons
    QObject::connect(ui->addFileToolButton,&QPushButton::clicked,this,&MainWindow::addFileHandler);
    QObject::connect(ui->saveButton,&QPushButton::clicked,this,&MainWindow::saveHandler);
    QObject::connect(ui->deleteToolButton,&QPushButton::clicked,this,&MainWindow::deletePageHandler);
    QObject::connect(ui->saveAsButton,&QPushButton::clicked,this,&MainWindow::saveAsHandler);

    QObject::connect(ui->pdfListWidget,&QListWidget::itemClicked,this,&MainWindow::pageSelectedHandler);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pdfRend;
    delete listWHelpr;
}

void MainWindow::quitActionHandler()
{
    QApplication::quit();
}

void MainWindow::aboutProjectHandler()
{
    QMessageBox::about(this,"About PdfCut",
                       QString("<h2>PdfCut</h2>"
                       "<h3>Version 1.0</h3>"
                       "<h4>Qt version %1, qpdf version %2</h4>"
                       "<p style='min-width: 400px;'>A lightweight application for cutting, merging and extracting pages from PDF doucments.</p>"
                       "<p>Copyright (C) 2026 Somfai Marton</p>"
                       "<p>This program comes with ABSOLUTELY NO WARRANTY.<br>"
                       "This is free software, and you are welcome to redistribute it "
                       "under certain conditions. See the <a href='https://www.gnu.org/licenses/gpl-3.0.html'>GNU GPLv3 License</a> for details.</p>"
                       "<p>Check out the project at <a href='https://github.com/m4cri/PdfCut'>GitHub</a>.</p>").arg(QT_VERSION_STR,QPDF_VERSION));
}

void MainWindow::documentationHandler()
{
    QDesktopServices::openUrl(QUrl("https://github.com/m4cri/PdfCut"));
}

void MainWindow::addFileHandler()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open PDF File"),"",tr("PDF file (*.pdf)"));

    if(fileName.trimmed().isEmpty()) return;

    qInfo() << "File name: " + fileName;

    try {
        pdfRend->addToList(fileName);
    } catch (const std::exception& e) {
        QMessageBox::warning(this,"File loading error","<h3>Failed to load PDF</h3>"
                                                         "<p>Please try again</p>");
        return;
    }

    UPDATE_PAGECOUNTER


    ui->actionSave_As->setEnabled(true);
    ui->actionClear_all->setEnabled(true);
    ui->saveAsButton->setEnabled(true);

    if( pdfRend->getFilesCount() == 1)
    {
        ui->saveButton->setEnabled(true);
        ui->actionSave->setEnabled(true);
    }
    else
    {
        ui->saveButton->setEnabled(false);
        ui->actionSave->setEnabled(false);
    }

    if(ui->stackedWidget->currentIndex()!=1) ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::openFileHandler()
{
    ui->pdfListWidget->clear();
    pdfRend->clearPages();
    UPDATE_PAGECOUNTER

    MainWindow::addFileHandler();
}

void MainWindow::deletePageHandler()
{
    int deleteIndex = ui->pdfListWidget->currentRow();

    QListWidgetItem* deleteItem = ui->pdfListWidget->takeItem(deleteIndex);
    delete deleteItem;

    pdfRend->deletePage(deleteIndex);
    UPDATE_PAGECOUNTER

    if(pdfRend->getPagesCount()==0)
    {
        ui->actionDelete_selected_page->setEnabled(false);
        ui->deleteToolButton->setEnabled(false);
        ui->saveButton->setEnabled(false);
        ui->actionSave->setEnabled(false);
    }
}

void MainWindow::clearAllHandler()
{
    ui->pdfListWidget->clear();
    pdfRend->clearPages();
    UPDATE_PAGECOUNTER

    ui->actionSave->setEnabled(false);
    ui->actionSave_As->setEnabled(false);
    ui->actionDelete_selected_page->setEnabled(false);
    ui->actionClear_all->setEnabled(false);

    ui->saveButton->setEnabled(false);
    ui->saveAsButton->setEnabled(false);
    ui->deleteToolButton->setEnabled(false);
}

void MainWindow::saveHandler()
{
    if(pdfRend->getPagesCount()<=0){
        qInfo() << "No pages found";
        return;
    }

    pdfRend->saveToPdf(pdfRend->getFileName());
    qInfo() << "Pdf saved succesfully (" << pdfRend->getFileName() <<")";
}

void MainWindow::saveAsHandler()
{
    if(pdfRend->getPagesCount()<=0){
        qInfo() << "No pages found";
        return;
    }

    std::string fileName = QFileDialog::getSaveFileName(this,tr("Save PDF File"),"",tr("PDF file (*.pdf)")).toStdString();
    pdfRend->saveToPdf(fileName);
    qInfo() << "Pdf saved as succesfully (" << pdfRend->getFileName() <<")";
}

void MainWindow::pageSelectedHandler()
{
    ui->deleteToolButton->setEnabled(true);
    ui->actionDelete_selected_page->setEnabled(true);
}
