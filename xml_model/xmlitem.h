#ifndef XMLITEM_H
#define XMLITEM_H

#include <QVariant>
#include <QDomNode>

class XmlItem
{
public:
    XmlItem(const QDomNode &_node);
    ~XmlItem();
    void appendChild(XmlItem *child);

    XmlItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    XmlItem *parentItem();
    void setParentItem(XmlItem *parent);
    bool setData(int column, QString value);
    bool removeChilds(int row, int amount);

    void addElement(QDomNode node);
    void addAttribute(QDomAttr attr);
    void prependChild(XmlItem *child);

    bool isElement();
private:
    QVector<XmlItem*> _childItems;
    QVector<QVariant> _itemData;
    XmlItem *_parentItem = nullptr;
    QDomNode _node;
};

#endif // XMLITEM_H
