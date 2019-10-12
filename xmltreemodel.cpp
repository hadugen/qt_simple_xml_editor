#include "xmltreemodel.h"
#include <QDebug>
XmlTreeModel::XmlTreeModel(const QDomDocument& doc, QObject *parent)
    : QAbstractItemModel (parent) {
    _root = new XmlItem(doc);
    _doc = doc;
    showTree(doc.lastChild(), _root);

}

void XmlTreeModel::showTree(QDomNode root, XmlItem *rootItem) {
    while(!root.isNull()) {
        XmlItem *newRootItem = showNode(root, rootItem);
        showTree(root.firstChild(), newRootItem);
        root = root.nextSibling();
    }
}

XmlItem *XmlTreeModel::showNode(QDomNode node, XmlItem *root) {
    if(node.isNull() || node.isComment()) return root;
    XmlItem* newRow = new XmlItem(node);
    if (node.hasAttributes()) {
        auto attrs = node.attributes();
        for(int i = 0; i < attrs.size(); i++) {
            newRow->appendChild(new XmlItem(attrs.item(i)));
        }
    }
    root->appendChild(newRow);
    return newRow;
}

XmlTreeModel::~XmlTreeModel() {
    delete _root;
}

QModelIndex XmlTreeModel::index(int row, 
                                int column, const QModelIndex &parent) const {
    if(!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    XmlItem *parentItem = parent.isValid() ? 
                static_cast<XmlItem*>(parent.internalPointer()) :
                _root;
    XmlItem *child = parentItem->child(row);
    if(child) return createIndex(row, column, child);
    else return QModelIndex();
}

QModelIndex XmlTreeModel::parent(const QModelIndex &child) const {
    if(!child.isValid()) return QModelIndex();
    XmlItem *childItem = static_cast<XmlItem*>(child.internalPointer());
    XmlItem *parent = childItem->parentItem();
    if(parent == _root) return QModelIndex();
    return createIndex(parent->row(), 0, parent);
}

int XmlTreeModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0)
        return 0;
    XmlItem *parentItem = parent.isValid() ? 
                static_cast<XmlItem*>(parent.internalPointer()) :
                _root;
    return parentItem->childCount();
}

int XmlTreeModel::columnCount(const QModelIndex &parent) const {
    if(parent.isValid()) {
        return static_cast<XmlItem*>(parent.internalPointer())->columnCount();
    }
    return _root->columnCount();
}

QVariant XmlTreeModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) return QVariant();
    switch (role) {
    case Qt::EditRole: [[fallthrough]];
    case Qt::DisplayRole: return static_cast<XmlItem*>(index.internalPointer())->data(index.column());
    default: return QVariant();
    }
}

Qt::ItemFlags XmlTreeModel::flags(const QModelIndex &index) const {
    if(!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

bool XmlTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(!index.isValid()) return false;
    XmlItem *item = static_cast<XmlItem*>(index.internalPointer());
    return item->setData(index.column(), value.toString());
}


bool XmlTreeModel::removeRows(int row, int count, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row + count - 1);
    if(parent.isValid()) {
        static_cast<XmlItem*>(parent.internalPointer())->removeChilds(row, count);
    } else {
        _root->removeChilds(row, count);
    }
    endRemoveRows();
    return true;
}

void XmlTreeModel::addAttribute(const QModelIndex &parent) {
    beginInsertRows(parent, 0, 0);
    if(!parent.isValid()) return;
    auto currItem = static_cast<XmlItem*>(parent.internalPointer());
    currItem->addAttribute(_doc.createAttribute("newAttr" + QString::number(currItem->childCount())));
    endInsertRows();
}

void XmlTreeModel::addElement(const QModelIndex &parent) {
    if(!parent.isValid()) return;
    beginInsertRows(parent, 0, 0);
    static_cast<XmlItem*>(parent.internalPointer())->addElement(_doc.createElement("element"));
    endInsertRows();
}

void XmlTreeModel::addText(const QModelIndex &parent) {
    if(!parent.isValid()) return;
    beginInsertRows(parent, 0, 0);
    static_cast<XmlItem*>(parent.internalPointer())->addElement(_doc.createTextNode("text"));
    endInsertRows();
}

QDomDocument XmlTreeModel::getDocument() {
    return _doc;
}
