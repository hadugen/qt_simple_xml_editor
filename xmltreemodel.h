#ifndef XMLTREEMODEL_H
#define XMLTREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QDomDocument>
#include "xmlitem.h"
class XmlTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    XmlTreeModel(const QDomDocument& _doc, QObject* parent = nullptr);
    ~XmlTreeModel() override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    void addAttribute(const QModelIndex &parent = QModelIndex());
    void addElement(const QModelIndex &parent = QModelIndex());
    void addText(const QModelIndex &parent = QModelIndex());
    QDomDocument getDocument();
private:
    XmlItem *_root = nullptr;
    QDomDocument _doc;
    XmlItem *showNode(QDomNode node, XmlItem *root);
    void showTree(QDomNode root, XmlItem *rootItem);

};

#endif // XMLTREEMODEL_H
