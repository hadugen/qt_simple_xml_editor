#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupTreeView();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_action_openFile_triggered() {
    _fileUrl = QFileDialog::getOpenFileUrl(this, "Открыть файл", QUrl(), FILE_FILTER);
    if(!_fileUrl.isValid()) return;
    QFile file(_fileUrl.toLocalFile());
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Ошибка открытия файла", "Не удалось открыть файл:\n" + file.errorString());
        return;
    }
    setWindowTitle(WINDOW_TITLE " - " + _fileUrl.fileName());
    QDomDocument doc;
    doc.setContent(&file);
    file.close();
    XmlTreeModel *model = new XmlTreeModel(doc);
    if(ui->treeView->model()) {
        delete ui->treeView->model();
    }
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->action_saveAs->setEnabled(true);
    ui->action_savefile->setEnabled(true);
}

void MainWindow::on_action_savefile_triggered() {
    auto model = static_cast<XmlTreeModel*>(ui->treeView->model());
    auto doc = model->getDocument();
    if(doc.isNull()) return;

    QFile file(_fileUrl.toLocalFile());
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Ошибка открытия файла", "Не удалось открыть файл: " + file.errorString());
        return;
    }
    QTextStream stream(&file);
    doc.save(stream, 4);
}

void MainWindow::on_action_saveAs_triggered() {
    QString path = QFileDialog::getSaveFileName(this, "Сохранить как", "untitled.xml", FILE_FILTER);
    if(path.isEmpty()) return;

    auto model = static_cast<XmlTreeModel*>(ui->treeView->model());
    auto doc = model->getDocument();
    if(doc.isNull()) return;

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Ошибка открытия файла", "Не удалось открыть файл: " + file.errorString());
        return;
    }
    QTextStream stream(&file);
    doc.save(stream, 4);
}

void MainWindow::setupTreeView() {
    ui->treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, [this](const QPoint &pos){
        auto index = ui->treeView->indexAt(pos);
        auto item = static_cast<XmlItem*>(index.internalPointer());
        if(!item) return;

        QMenu *menu = new QMenu;
        menu->setAttribute(Qt::WA_DeleteOnClose);
        if(item->isElement()) {
            menu->addAction("Добавить элемент", [=]{
                auto model = static_cast<XmlTreeModel*>(ui->treeView->model());
                model->addElement(index);
            });
            menu->addAction("Добавить текст", [=]{
                auto model = static_cast<XmlTreeModel*>(ui->treeView->model());
                model->addText(index);
            });
            menu->addAction("Добавить атрибут", [=]{
                auto model = static_cast<XmlTreeModel*>(ui->treeView->model());
                model->addAttribute(index);
            });
        }
        menu->addAction("Удалить", [=]{
            ui->treeView->model()->removeRows(index.row(), 1, index.parent());
        });
        menu->popup(ui->treeView->mapToGlobal(pos));
    });
}
