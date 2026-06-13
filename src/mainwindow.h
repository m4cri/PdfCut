#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "pdfrenderer.h"
#include "listwidgethelper.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    PdfContainer* pdfRend;
    ListWidgetHelper* listWHelpr;

public slots:
    void quitActionHandler();
    void aboutProjectHandler();
    void documentationHandler();

    void addFileHandler();
    void openFileHandler();

    void deletePageHandler();
    void clearAllHandler();
    void saveHandler();
    void saveAsHandler();
    void pageSelectedHandler();
};
#endif // MAINWINDOW_H
