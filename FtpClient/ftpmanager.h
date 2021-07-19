#ifndef FTPMAMAGER_H
#define FTPMAMAGER_H

#include <QObject>
#include "treeitem.h"

class CurlQuery;

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

private:
    void updateRootItem(QString ftpFilePath, TreeItem *root);

public:
    QString userName;
    QString passwd;
    QString ftpIp;
    TreeItem *root { nullptr };
};

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
};

#endif // FTPMAMAGER_H
