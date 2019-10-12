#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <xmlitem.h>
#include <xmltreemodel.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QUrl>

#define WINDOW_TITLE "Xml редактор"
#define FILE_FILTER  "Xml files(*.xml)"

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

private:
    Ui::MainWindow *ui;
    QUrl _fileUrl;
    void setupTreeView();
};

#endif // MAINWINDOW_H
