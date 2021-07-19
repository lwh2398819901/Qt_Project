#ifndef TREEITEM_H
#define TREEITEM_H
#include "ftpfileinfo.h"
#include <list>
#include <map>
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
public:
    TreeItem();
    TreeItem(const FtpFileInfo &info);
    TreeItem(TreeItem *parent, const FtpFileInfo &info);
    ~TreeItem();
    void clear();
    void setParent(TreeItem *parent);
    void setFileInfo(const FtpFileInfo &info);
    const FtpFileInfo &fileInfo() const;
    const list<TreeItem *> childList() const;
    const TreeItem *parent() const;

    bool addChild(TreeItem *child);
    bool addChild(const FtpFileInfo &info);
    bool addChildList(list<TreeItem *> &childList);
    void deleteChild(TreeItem *child);
    void deleteChild(const FtpFileInfo &info);
    TreeItem *findChild(QString fileName);
    list<TreeItem *> findChildList(QString fileName);
    TreeItem *findChild(FtpFileInfo &info);
    void setSort(SortOrder order = SortOrder::AscendingOrder);
    SortOrder getSort() { return m_sort; }
    QString path()const;


private:
    TreeItem *m_parent { nullptr };
    list<TreeItem *> m_child;
    FtpFileInfo m_info;
    SortOrder m_sort;
};

#endif // TREEITEM_H
