#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <xmlitem.h>
#include <xmltreemodel.h>
#include <pdf.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QUrl>
#include <fstream>
#include <string>

#define WINDOW_TITLE "Xml редактор"
#define XML_FILE_FILTER  "xml files(*.xml)"
#define PDF_FILE_FILTER "pdf files(*.pdf)"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_action_openFile_triggered();

    void on_action_savefile_triggered();

    void on_action_saveAs_triggered();

    void on_action_pdfExport_triggered();

private:
    Ui::MainWindow *ui;
    QUrl _fileUrl;
    void setupTreeView();
};

#endif // MAINWINDOW_H
