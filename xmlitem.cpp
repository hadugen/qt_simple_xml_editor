#include "xmlitem.h"
#include <QDebug>
XmlItem::XmlItem(const QDomNode &node) {
    this->_node = node;
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
    return _node.isElement();
}

void XmlItem::addElement(QDomNode node) {
    XmlItem *item = new XmlItem(node);
    this->_node.appendChild(node);
    appendChild(item);
}

void XmlItem::addAttribute(QDomAttr attr) {
    XmlItem *item = new XmlItem(attr);
    this->_node.toElement().setAttributeNode(attr);
    prependChild(item);
}

bool XmlItem::removeChilds(int row, int amount) {
    if(row < 0 || row > _childItems.size()) return false;
    if(amount < 0 || amount > _childItems.size()) return false;
    for(int i = row; i < row + amount; i++) {
        if(_childItems[i]->_node.isAttr()) {
            auto attr = _childItems[i]->_node.toAttr();
            _node.toElement().removeAttributeNode(attr);
        } else {
            _node.removeChild(_childItems[i]->_node);
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
    case 0: return _node.nodeName();
    case 1: return _node.nodeValue();
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
        if(_node.isElement()) {
            _node.toElement().setTagName(value);
        } else if(_node.isAttr()) {
            auto parent = _node.toAttr().ownerElement();
            if(parent.hasAttribute(value)) return false;
            auto doc = _node.ownerDocument();
            auto newAttr = doc.createAttribute(value);
            newAttr.setValue(_node.toAttr().value());
            parent.removeAttributeNode(_node.toAttr());
            parent.setAttributeNode(newAttr);
            _node = newAttr;
        }
    } else if(column == 1) {
        if(!_node.isElement()) {
            _node.setNodeValue(value);
        }
    }
    return true;
}
