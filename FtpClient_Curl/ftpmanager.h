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
#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QObject>
#include "treeitem.h"
class FtpManager;
struct CurlQuery
{
    static QString queryFiles(FtpManager *ftpManager, QString ftpFilePath = "", bool isDir = true);
    static QString createDir(FtpManager *ftpManager, QString dirName, QString ftpPath = "");
    static QString fileRename(FtpManager *ftpManager, QString newName, QString oldName,
                              QString ftpFilePath = "/");
    static QString get(FtpManager *ftpManager, QString filePath, QString fileName,
                       QString savePath);
    static QString put(FtpManager *ftpManager, QString hostFilePath, QString hostFileName,
                       QString ftpFilePath = "/");
    static QString rmFtpFiles(FtpManager *ftpManager,QString ftpFileName, bool isDir = false);
    static QString getFtpFileText(FtpManager *ftpManager,QString ftpFileName);
};

class FtpManager : public QObject
{
    Q_OBJECT
public:
    explicit FtpManager(QObject *parent = nullptr);
    explicit FtpManager(QString userName, QString passwd, QString ftpIp, TreeItem *root,
                        QObject *parent = nullptr);
    ~FtpManager();
signals:

public slots:
    const TreeItem *updateFtpFiles();
    bool get(QString filePath, QString fileName, QString savePath);
    bool get(const TreeItem *item, QString savePath);
    bool put(QString hostFilePath, QString hostFileName, QString ftpFilePath = "/");
    bool ftpRename(QString newName, QString oldName, QString ftpFilePath = "/");
    bool rmFtpFiles(QString dir);
    bool createFtpDir(QString dirName, QString ftpPath = "");
    bool moveFtpFiles(QString filePath);
    void setFileSort(SortOrder sortOrder);
    SortOrder getFileSort() { return root->getSort(); }
    QString getFtpFileText(QString ftpFileName);

private:
    void updateRootItem(QString ftpFilePath, TreeItem *root);

public:
    QString userName;
    QString passwd;
    QString ftpIp;
    TreeItem *root { nullptr };
};

#endif // FTPMANAGER_H
