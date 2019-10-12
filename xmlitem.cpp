#include "xmlitem.h"
#include <QDebug>
XmlItem::XmlItem(const QDomNode &node) {
    this->node = node;
}

XmlItem::~XmlItem() {
    qDeleteAll(_childItems);
}

void XmlItem::appendChild(XmlItem *child) {
    child->setParentItem(this);
    _childItems.append(child); 
}

void XmlItem::prependChild(XmlItem *child) {
    child->setParentItem(this);
    _childItems.prepend(child);
}

bool XmlItem::isElement() {
    return node.isElement();
}

void XmlItem::addElement(QDomNode node) {
    XmlItem *item = new XmlItem(node);
    this->node.appendChild(node);
    appendChild(item);
}

void XmlItem::addAttribute(QDomAttr attr) {
    XmlItem *item = new XmlItem(attr);
    this->node.toElement().setAttributeNode(attr);
    prependChild(item);
}

bool XmlItem::removeChilds(int row, int amount) {
    if(row < 0 || row > _childItems.size()) return false;
    if(amount < 0 || amount > _childItems.size()) return false;
    for(int i = row; i < row + amount; i++) {
        if(_childItems[i]->node.isAttr()) {
            auto attr = _childItems[i]->node.toAttr();
            node.toElement().removeAttributeNode(attr);
        } else {
            node.removeChild(_childItems[i]->node);
        }
        delete _childItems[i];
    }
    _childItems.remove(row, amount);
    return true;
}

XmlItem *XmlItem::child(int row) {
    if(row < 0 || row >= _childItems.size()) return nullptr;
    return _childItems.at(row);
}

int XmlItem::childCount() const {
    return _childItems.size();
}

int XmlItem::columnCount() const {
    return 2;
}

QVariant XmlItem::data(int column) const {
    switch (column) {
    case 0: return node.nodeName();
    case 1: return node.nodeValue();
    default: return "NULL";
    }
}

int XmlItem::row() const{
    if(_parentItem) {
        return _parentItem->_childItems.indexOf(const_cast<XmlItem*>(this));
    }
    return 0;
}

XmlItem *XmlItem::parentItem() {
    return _parentItem;
}

void XmlItem::setParentItem(XmlItem *parent) {
    _parentItem = parent;
}

bool XmlItem::setData(int column, QString value) {
    if(column == 0) {
        if(value[0].isDigit() || value.indexOf(' ') != 0) return false;
        if(node.isElement()) {
            node.toElement().setTagName(value);
        } else if(node.isAttr()) {
            auto parent = node.toAttr().ownerElement();
            if(parent.hasAttribute(value)) return false;
            auto doc = node.ownerDocument();
            auto newAttr = doc.createAttribute(value);
            newAttr.setValue(node.toAttr().value());
            parent.removeAttributeNode(node.toAttr());
            parent.setAttributeNode(newAttr);
            node = newAttr;
            /*parent.removeAttributeNode(node.toAttr());
            _parentItem->addAttribute(newAttr);*/
        }
    } else if(column == 1) {
        if(!node.isElement()) {
            node.setNodeValue(value);
        }
    }
    return true;
}
