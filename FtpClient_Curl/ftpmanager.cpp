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
#include "ftpmanager.h"
using namespace publisherFunc;
QString CurlQuery::queryFiles(FtpManager *ftpManager, QString ftpFilePath, bool isDir)
{
    if (!isDir) {
        if (ftpFilePath.isEmpty()) {
            return QString("curl ftp://%1 -u %2:%3 ")
                    .arg(ftpManager->ftpIp)
                    .arg(ftpManager->userName)
                    .arg(ftpManager->passwd);
        }
        return QString("curl ftp://%1/%2 -u %3:%4 ")
                .arg(ftpManager->ftpIp)
                .arg(ftpFilePath)
                .arg(ftpManager->userName)
                .arg(ftpManager->passwd);
    }

    if (ftpFilePath.isEmpty()) {
        return QString("curl ftp://%1/ -u %2:%3 ")
                .arg(ftpManager->ftpIp)
                .arg(ftpManager->userName)
                .arg(ftpManager->passwd);
    }
    return QString("curl ftp://%1/%2/ -u %3:%4 ")
            .arg(ftpManager->ftpIp)
            .arg(ftpFilePath)
            .arg(ftpManager->userName)
            .arg(ftpManager->passwd);
}

QString CurlQuery::createDir(FtpManager *ftpManager, QString dirName, QString ftpPath)
{
    return QString("%1 -X 'MKD %2'").arg(queryFiles(ftpManager, ftpPath)).arg(dirName);
}

QString CurlQuery::fileRename(FtpManager *ftpManager, QString newName, QString oldName,
                              QString ftpFilePath)
{
    return QString("%1 -Q '-RNFR %2' -Q '-RNTO %3'")
            .arg(queryFiles(ftpManager, ftpFilePath))
            .arg(oldName)
            .arg(newName);
}

QString CurlQuery::get(FtpManager *ftpManager, QString filePath, QString fileName, QString savePath)
{

    return QString("%1 -o %2/%3 -O")
            .arg(queryFiles(ftpManager, filePath, false))
            .arg(savePath)
            .arg(fileName);
}

QString CurlQuery::put(FtpManager *ftpManager, QString hostFilePath, QString hostFileName,
                       QString ftpFilePath)
{
    return QString("%1 -T %2")
            .arg(queryFiles(ftpManager, ftpFilePath + hostFileName, false))
            .arg(hostFilePath + "/" + hostFileName);
}

QString CurlQuery::rmFtpFiles(FtpManager *ftpManager,QString ftpFileName, bool isDir)
{
    if(isDir){

    }
}

QString CurlQuery::getFtpFileText(FtpManager *ftpManager, QString ftpFileName)
{
    return queryFiles(ftpManager, ftpFileName, false);
}




FtpManager::FtpManager(QObject *parent) : QObject(parent) {}

FtpManager::FtpManager(QString userName, QString passwd, QString ftpIp, TreeItem *root,
                       QObject *parent)
{
    this->userName = userName;
    this->passwd = passwd;
    this->ftpIp = ftpIp;
    this->root = root;
}

FtpManager::~FtpManager()
{
    if (root)
        delete root;
    root = nullptr;
}

const TreeItem *FtpManager::updateFtpFiles()
{
    if (root == nullptr) {
        root = new TreeItem();
    }
    root->clearChild();
    updateRootItem("", root);
    root->setSort(root->getSort());
    return root;
}

bool FtpManager::get(QString filePath, QString fileName, QString savePath)
{
    return threadCall(CurlQuery::get(this, filePath, fileName, savePath), false, -1).first == 0;
}

bool FtpManager::get(const TreeItem *item, QString savePath)
{
    if (item->FileInfo().isDir()) {
        QDir dir(savePath);
        dir.mkdir(item->FileInfo().Name());
        foreach (auto it, item->ChildList()) {
            get(it, savePath + "/" + item->FileInfo().Name());
        }
    }

    QString str = item->FileInfo().Name();
    QString path = "";
    const TreeItem *item2 = item;
    while (item2->Parent() != nullptr) {
        item2 = item2->Parent();
        if (item2 == nullptr || item2->FileInfo().Name().isEmpty())
            break;
        path = (item2->FileInfo().Name() + "/" + path);
    }
    return get(path + str, str, savePath);
}

bool FtpManager::put(QString hostFilePath, QString hostFileName, QString ftpFilePath)
{
    return threadCall(CurlQuery::put(this, hostFilePath, hostFileName, ftpFilePath), false, -1)
                   .first
            == 0;
}

bool FtpManager::ftpRename(QString newName, QString oldName, QString ftpFilePath)
{

    return threadCall(CurlQuery::fileRename(this, newName, oldName, ftpFilePath), false, -1).first
            == 0;
}

bool FtpManager::rmFtpFiles(QString dir)
{
    QString cmd = QString("curl ftp://%1/ -u %2:%3 ").arg(ftpIp).arg(userName).arg(passwd);
    auto it = threadCall(cmd, true, -1);
    return it.first == 0;
}

bool FtpManager::createFtpDir(QString dirName, QString ftpPath)
{
    return threadCall(CurlQuery::createDir(this, dirName, ftpPath), false, -1).first == 0;
}

bool FtpManager::moveFtpFiles(QString filePath)
{
    QString cmd = QString("curl ftp://%1/ -u %2:%3 ").arg(ftpIp).arg(userName).arg(passwd);
    QString str = threadCall(cmd, true, -1).second;
}

void FtpManager::setFileSort(SortOrder sortOrder)
{
    root->setSort(sortOrder);
}

QString FtpManager::getFtpFileText(QString ftpFileName)
{
    return threadCall(CurlQuery::getFtpFileText(this, ftpFileName), false, -1).second;
}

void FtpManager::updateRootItem(QString ftpFilePath, TreeItem *root)
{
    QString str = publisherFunc::startPopen(CurlQuery::queryFiles(this, ftpFilePath)).second;
    QStringList list = str.split("\n");
    foreach (QString tmp, list) {
        if (!tmp.isEmpty()) {
            FtpFileInfo info(tmp);
            if (info.isDir()) {
                TreeItem *item = new TreeItem(info);
                root->addChild(item);
                updateRootItem(ftpFilePath + "/" + info.Name(), item);
            } else {
                root->addChild(info);
            }
        }
    }
}

