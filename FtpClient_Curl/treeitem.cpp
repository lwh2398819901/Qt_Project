/*
 * Copyright (C) 2021 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     liuwenhao <liuwenhao@uniontech.com>
 *
 * Maintainer: liuwenhao <liuwenhao@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "treeitem.h"

TreeItem::TreeItem() {}

TreeItem::TreeItem(const FtpFileInfo &info)
{
    this->m_info = info;
}

TreeItem::TreeItem(TreeItem *parent, const FtpFileInfo &info)
{
    this->m_parent = parent;
    this->m_info = info;
    parent->addChild(this);
}

TreeItem::~TreeItem()
{
    clearChild();
}

void TreeItem::clearChild()
{
    for (auto it = m_child.begin(); it != m_child.end(); ++it) {
        auto iter = *it;
        delete iter;
    }
    m_child.clear();
    m_parent = nullptr;
}

bool TreeItem::addChild(TreeItem *child)
{
    foreach (auto it, m_child) {
        if (it->m_info == child->m_info) {
            return false;
        }
    }
    m_child.push_back(child);
    child->setParent(this);
    return true;
}

bool TreeItem::addChild(const FtpFileInfo &info)
{
    return addChild(new TreeItem(info));
}

bool TreeItem::addChildList(list<TreeItem *> &childList)
{
    this->clearChild();
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
        if ((*it)->m_info == info) {
            auto iter = *it;
            m_child.erase(it);
            delete iter;
        }
    }
}

TreeItem *TreeItem::findChild(const QString fileName)
{
    foreach (auto it, m_child) {
        if (it->m_info.Name() == fileName) {
            return it;
        }
    }
    return nullptr;
}

TreeItem *TreeItem::findChild(const QString fileName) const
{
    foreach (auto it, m_child) {
        if (it->m_info.Name() == fileName) {
            return it;
        }
    }
    return nullptr;
}

list<TreeItem *> TreeItem::findChildList(QString fileName)
{
    list<TreeItem *> _list;
    foreach (auto it, m_child) {
        if (it->m_info.Name() == fileName) {
            _list.push_back(it);
        }
    }
    return _list;
}

TreeItem *TreeItem::findChild(const FtpFileInfo &info)
{
    foreach (auto it, m_child) {
        if (it->m_info == info) {
            return it;
        }
    }
    return nullptr;
}

TreeItem *TreeItem::findChild(const FtpFileInfo &info) const
{
    foreach (auto it, m_child) {
        if (it->m_info == info) {
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
            return t1->m_info < t2->m_info;
        case SortOrder::DescendingOrder:
            return t1->m_info > t2->m_info;
        case SortOrder::MinDate:
            return t1->m_info.Date() < t2->m_info.Date();
        case SortOrder::MaxDate:
            return t1->m_info.Date() > t2->m_info.Date();
        case SortOrder::MinSize:
            return t1->m_info.Size() < t2->m_info.Size();
        case SortOrder::MaxSize:
            return t1->m_info.Size() > t2->m_info.Size();
        default:
            return t1->m_info < t2->m_info;
        }
    };

    m_sort = order;

    foreach (TreeItem *item, m_child) {
        if (item->m_info.isDir()) {
            item->setSort(m_sort);
        }
    }
    m_child.sort(func);
}

QString TreeItem::path() const
{
    QString path;
    const TreeItem *item = this;
    while (item->m_parent != nullptr) {
        path = ((item->m_info.Name()) + "/" + path);
        item = item->m_parent;
    }
    if (!m_info.isDir()) {
        path = path.mid(0, path.size() - 1);
    }
    return path;
}
