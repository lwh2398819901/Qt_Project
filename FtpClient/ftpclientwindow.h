#ifndef FTPCLIENTWINDOW_H
#define FTPCLIENTWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QFileInfo>
#include <QMenu>
#include <QAction>
#include "ftpmanager.h"
namespace Ui {
class FtpClientWindow;
}

class FtpClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FtpClientWindow(QWidget *parent = nullptr);
    ~FtpClientWindow();
    void updateFtpDir();
    void renameFtp();
    void renameHost();
    void createFtpDir();
    void get();
    void put();
    void deleteFtp();
    void deleteHost();
    void setUIEnable(bool flag);
    bool checkGetCover();
    bool checkPutCover();
    bool message(QString mes);
    void ftpSort(int index);
    void showFtpTreeMenu(QPoint point);
    void showHostTreeMenu(QPoint point);

private:
    void getFtpFiles(const TreeItem *root, QTreeWidgetItem *item = nullptr);
    void hostPutFiles(QFileInfo info);

    const TreeItem *getTreeData(QTreeWidgetItem *item);
    //读写配置文件
    void readConfig();
    void writeConfig();

private:
    Ui::FtpClientWindow *ui;
    FtpManager ftp;

    QMenu *ftpMenu;
    QAction *ftpDownAct;
    QAction *ftpRenameAct;
    QAction *ftpDeleteAct;
    QAction *ftpCreateAct;
    QAction *ftpCopyAct;
    QAction *ftpPasteAct;
    QAction *ftpShearAct;

    QMenu *hostMenu;
    QAction *hostPutAct;
    QAction *hostRename;
    QAction *hostDeleteAct;
};

#endif // FTPCLIENTWINDOW_H
