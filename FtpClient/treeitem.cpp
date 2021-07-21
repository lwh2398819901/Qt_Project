#include "treeitem.h"

TreeItem::TreeItem() {}

TreeItem::TreeItem(const FtpFileInfo &info)
{
    this->m_info = info;
}

TreeItem::TreeItem(TreeItem *parent, const FtpFileInfo &info)
{
    this->m_parent = parent;
    parent->addChild(this);
    this->m_info = info;
}

TreeItem::~TreeItem()
{
    clear();
}

void TreeItem::clear()
{
    for (auto it = m_child.begin(); it != m_child.end(); ++it) {
        auto iter = *it;
        delete iter;
    }
    m_child.clear();
    m_parent = nullptr;
}

void TreeItem::setParent(TreeItem *parent)
{
    m_parent = parent;
}

void TreeItem::setFileInfo(const FtpFileInfo &info)
{
    m_info = info;
}

const FtpFileInfo &TreeItem::fileInfo() const
{
    return m_info;
}

const list<TreeItem *> TreeItem::childList() const
{
    return m_child;
}

const TreeItem *TreeItem::parent() const
{
    return m_parent;
}

bool TreeItem::addChild(TreeItem *child)
{
    child->setParent(this);

    foreach (auto it, m_child) {
        if (it->fileInfo() == child->fileInfo()) {
            return false;
        }
    }
    m_child.push_back(child);
    return true;
}

bool TreeItem::addChild(const FtpFileInfo &info)
{
    TreeItem *item = new TreeItem(info);
    return addChild(item);
}

bool TreeItem::addChildList(list<TreeItem *> &childList)
{
    this->clear();
    this->m_child = childList;
}

void TreeItem::deleteChild(TreeItem *child)
{
    for (auto it = m_child.begin(); it != m_child.end(); ++it) {
        if ((*it) == child) {
            auto iter = *it;
            m_child.erase(it);
            delete iter;
        }
    }
}

void TreeItem::deleteChild(const FtpFileInfo &info)
{
    for (auto it = m_child.begin(); it != m_child.end(); ++it) {
        if ((*it)->fileInfo() == info) {
            auto iter = *it;
            m_child.erase(it);
            delete iter;
        }
    }
}

TreeItem *TreeItem::findChild(QString fileName)
{
    foreach (auto it, m_child) {
        if (it->fileInfo().name() == fileName) {
            return it;
        }
    }
    return nullptr;
}

list<TreeItem *> TreeItem::findChildList(QString fileName)
{
    list<TreeItem *> _list;
    foreach (auto it, m_child) {
        if (it->fileInfo().name() == fileName) {
            _list.push_back(it);
        }
    }
    return _list;
}

TreeItem *TreeItem::findChild(FtpFileInfo &info)
{
    foreach (auto it, m_child) {
        if (it->fileInfo() == info) {
            return it;
        }
    }
    return nullptr;
}

void TreeItem::setSort(SortOrder order)
{
    auto func = [=](const TreeItem *t1, const TreeItem *t2) -> bool {
        switch (order) {
        case SortOrder::AscendingOrder:
            return t1->fileInfo() < t2->fileInfo();
        case SortOrder::DescendingOrder:
            return t1->fileInfo() > t2->fileInfo();
        case SortOrder::MinDate:
            return t1->fileInfo().date() < t2->fileInfo().date();
        case SortOrder::MaxDate:
            return t1->fileInfo().date() > t2->fileInfo().date();
        case SortOrder::MinSize:
            return t1->fileInfo().size() < t2->fileInfo().size();
        case SortOrder::MaxSize:
            return t1->fileInfo().size() > t2->fileInfo().size();
        }
    };

    m_sort = order;

    foreach (TreeItem *item, m_child) {
        if (item->fileInfo().isDir()) {
            item->setSort(m_sort);
        }
    }
    m_child.sort(func);
}

QString TreeItem::path()const
{
    QString path;
    const TreeItem*item = this;
    while (item->parent() != nullptr) {
        path=((item->fileInfo().name())+"/"+path);
        item = item->parent();
    }
    if(!fileInfo().isDir()){
        path=path.mid(0,path.size()-1);
    }
    return path;
}

