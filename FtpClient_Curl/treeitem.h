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
#ifndef TREEITEM_H
#define TREEITEM_H
#include "ftpfileinfo.h"
#include <list>
using namespace std;
enum class SortOrder {
    AscendingOrder, // A-Z
    DescendingOrder, // Z-A
    MinDate, //时间排序
    MaxDate,
    MinSize, //大小排序
    MaxSize
};

class TreeItem
{
private:
    TreeItem *m_parent { nullptr };
    list<TreeItem *> m_child;
    FtpFileInfo m_info;
    SortOrder m_sort;

public:
    TreeItem();
    TreeItem(TreeItem *parent, const FtpFileInfo &info);
    TreeItem(const FtpFileInfo &info);
    ~TreeItem();
    getSetFunc(Parent, m_parent)
    getSetFunc(ChildList, m_child)
    getSetFunc(FileInfo, m_info)
    bool addChild(TreeItem *child);
    bool addChild(const FtpFileInfo &info);
    bool addChildList(list<TreeItem *> &childList);
    void deleteChild(TreeItem *child);
    void deleteChild(const FtpFileInfo &info);
    TreeItem *findChild(const QString fileName);
    TreeItem *findChild(const QString fileName) const;
    TreeItem *findChild(const FtpFileInfo &info);
    TreeItem *findChild(const FtpFileInfo &info) const;
    list<TreeItem *> findChildList(QString fileName);
    void setSort(SortOrder order = SortOrder::AscendingOrder);
    SortOrder getSort() { return m_sort; }
    void clearChild();
    QString path() const;
};

#endif // TREEITEM_H
