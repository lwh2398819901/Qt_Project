#include "ftpclientwindow.h"
#include "ui_ftpclientwindow.h"
#include <QDebug>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include <QFileIconProvider>
#include "publisherfunc.h"
using namespace publisherFunc;

FtpClientWindow::FtpClientWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::FtpClientWindow)
{
    ui->setupUi(this);
    readConfig();
    QFileSystemModel *model = new QFileSystemModel();
    model->setRootPath(QDir::currentPath()); //设置根目录
    QModelIndex index = model->index(QDir::currentPath());

    ui->treeView->setModel(model);
    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->treeView->expand(index); //当前项展开
    ui->treeView->scrollTo(index); //定位到当前项
    ui->treeView->setColumnWidth(0, 300);
    ui->treeView->header()->setStretchLastSection(true);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this,
            &FtpClientWindow::showHostTreeMenu);

    ui->treeWidget_2->setColumnWidth(0, 300);
    ui->treeWidget_2->setColumnCount(4);
    ui->treeWidget_2->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->treeWidget_2->header()->setSectionsClickable(true);
    ui->treeWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget_2, &QTreeWidget::customContextMenuRequested, this,
            &FtpClientWindow::showFtpTreeMenu);

    updateFtpDir();

    connect(ui->putBtn, &QPushButton::clicked, this, &FtpClientWindow::put);
    connect(ui->updateBtn, &QPushButton::clicked, this, &FtpClientWindow::updateFtpDir);
    connect(ui->treeWidget_2->header(), &QHeaderView::sectionClicked, this,
            &FtpClientWindow::ftpSort);
    connect(ui->treeWidget_2, &QTreeWidget::itemDoubleClicked,
            [=](QTreeWidgetItem *item, int column) {

            });
    connect(ui->getBtn, &QPushButton::clicked, this, &FtpClientWindow::get);
    connect(ui->getPathBtn, &QPushButton::clicked, [=]() {
        QString str = QFileDialog::getExistingDirectory();
        if (!str.isEmpty()) {
            ui->hostSavePath->setText(str);
        }
    });

    ftpMenu = new QMenu(this);
    ftpDownAct = new QAction("下载", this);
    ftpRenameAct = new QAction("重命名", this);
    ftpCreateAct = new QAction("创建文件夹", this);
    ftpDeleteAct = new QAction("删除", this);

    ftpMenu->addAction(ftpDownAct);
    ftpMenu->addAction(ftpRenameAct);
    ftpMenu->addAction(ftpCreateAct);
    ftpMenu->addAction(ftpDeleteAct);

    connect(ftpDownAct, &QAction::triggered, this, &FtpClientWindow::get);
    connect(ftpRenameAct, &QAction::triggered, this, &FtpClientWindow::renameFtp);
    connect(ftpCreateAct, &QAction::triggered, this, &FtpClientWindow::createFtpDir);
    connect(ftpDeleteAct, &QAction::triggered, this, &FtpClientWindow::deleteFtp);

    hostMenu = new QMenu(this);
    hostPutAct = new QAction("上传", this);
    hostRename = new QAction("重命名", this);
    hostDeleteAct = new QAction("删除", this);

    hostMenu->addAction(hostPutAct);
    hostMenu->addAction(hostRename);
    hostMenu->addAction(hostDeleteAct);

    connect(hostPutAct, &QAction::triggered, this, &FtpClientWindow::put);
    connect(hostRename, &QAction::triggered, this, &FtpClientWindow::renameFtp);
    connect(hostDeleteAct, &QAction::triggered, this, &FtpClientWindow::createFtpDir);
}

FtpClientWindow::~FtpClientWindow()
{
    writeConfig();
    delete ui;
}

void FtpClientWindow::updateFtpDir()
{
    ftp.ftpIp = this->ui->ftpLine->text();
    ftp.userName = this->ui->userLine->text();
    ftp.passwd = this->ui->passwdLine->text();
    static bool flag = true;
    if (!flag)
        return;
    flag = false;
    ui->treeWidget_2->clear();
    const TreeItem *root = ftp.updateFtpFiles();

    getFtpFiles(root);

    flag = true;
}

void FtpClientWindow::renameFtp()
{
    QTreeWidgetItem *item = ui->treeWidget_2->currentItem();
    if (item != nullptr) {
        const TreeItem *treeItem = getTreeData(item);
        QString oldName = treeItem->fileInfo().name();
        QString newName =
                QInputDialog::getText(nullptr, "重命名", QString("%1  请输入新名称").arg(oldName));
        if (newName.isEmpty())
            return;

        if (treeItem->parent() != nullptr) {
            ftp.ftpRename(newName, oldName, treeItem->parent()->path());
        } else {
            ftp.ftpRename(newName, oldName);
        }

        updateFtpDir();
    }
}

void FtpClientWindow::renameHost() {}

void FtpClientWindow::createFtpDir()
{
    QTreeWidgetItem *item = ui->treeWidget_2->currentItem();

    if (item != nullptr) {

        const TreeItem *treeItem = getTreeData(item);
        QString new_DirName = QInputDialog::getText(nullptr, "新建文件夹", "请输入文件夹名称");
        if (new_DirName.isEmpty())
            return;

        if (treeItem->parent() != nullptr && !treeItem->fileInfo().isDir()) {
            ftp.createFtpDir(new_DirName, treeItem->parent()->path());
        } else {
            ftp.createFtpDir(new_DirName, treeItem->path());
        }

        updateFtpDir();
    }
}

void FtpClientWindow::get()
{
    QString savePath = ui->hostSavePath->text();
    if (savePath.isEmpty()) {
        publisherFunc::showTextBox("保存路径不可为空");
        return;
    }

    ui->treeWidget_2->header()->setEnabled(false);
    ui->updateBtn->setEnabled(false);

    foreach (auto item, ui->treeWidget_2->selectedItems()) {
        item->setText(3, "准备下载");
    }
    bool cover = false;
    foreach (auto item, ui->treeWidget_2->selectedItems()) {
        const TreeItem *treeItem = getTreeData(item);
        bool flag = true;

        if (QFile::exists(savePath + "/" + treeItem->fileInfo().name()) && !cover) {

            QMessageBox msgBox;
            msgBox.setText(QString("%1 文件已经存在，是否覆盖？").arg(treeItem->fileInfo().name()));
            msgBox.setStandardButtons(QMessageBox::YesAll | QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::YesAll);
            int ret = msgBox.exec();

            if (ret == QMessageBox::No)
                flag = false;
            if (ret == QMessageBox::YesAll) {
                cover = true;
            }
        }
        if (flag) {
            item->setText(3, "开始下载");
            qDebug() << ftp.get(treeItem, savePath);
            item->setText(3, "完成");
        }
    }
    ui->treeWidget_2->header()->setEnabled(true);
    ui->updateBtn->setEnabled(true);
}

void FtpClientWindow::put()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (index.isValid()) {
        QFileSystemModel *model = (QFileSystemModel *)ui->treeView->model();
        hostPutFiles(model->fileInfo(index));
    }
}

void FtpClientWindow::deleteFtp() {


}

void FtpClientWindow::deleteHost() {}

void FtpClientWindow::ftpSort(int index)
{
    if (index == 0) {
        ftp.getFileSort() == SortOrder::AscendingOrder ? ftp.setFileSort(SortOrder::DescendingOrder)
                                                       : ftp.setFileSort(SortOrder::AscendingOrder);
    } else if (index == 1) {
        ftp.getFileSort() == SortOrder::MinSize ? ftp.setFileSort(SortOrder::MaxSize)
                                                : ftp.setFileSort(SortOrder::MinSize);
    } else if (index == 2) {
        ftp.getFileSort() == SortOrder::MinDate ? ftp.setFileSort(SortOrder::MaxDate)
                                                : ftp.setFileSort(SortOrder::MinDate);
    }
    ui->treeWidget_2->clear();
    getFtpFiles(ftp.root);
}

void FtpClientWindow::showFtpTreeMenu(QPoint point)
{
    bool flag = ui->treeWidget_2->selectedItems().count() <= 1;
    ftpRenameAct->setEnabled(flag);
    ftpCreateAct->setEnabled(flag);

    ftpMenu->exec(QCursor::pos());
}

void FtpClientWindow::showHostTreeMenu(QPoint point)
{

    hostMenu->exec(QCursor::pos());
}

void FtpClientWindow::getFtpFiles(const TreeItem *root, QTreeWidgetItem *itemRoot)
{
    foreach (const TreeItem *it, root->childList()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, it->fileInfo().name());
        if (!it->fileInfo().isDir()) {
            item->setText(1, publisherFunc::humanReadableSize(it->fileInfo().size(), 2));
        }
        item->setText(2, it->fileInfo().date());
        QVariant var = QVariant::fromValue((void *)it);
        item->setData(0, Qt::UserRole + 1, var);
        QFileIconProvider icon_provider;
        if (it->fileInfo().isDir()) {
            item->setIcon(0, icon_provider.icon(QFileIconProvider::Folder));
            getFtpFiles(it, item);
        } else {
            if (it->fileInfo().suffix().isEmpty()) {
                item->setIcon(0, icon_provider.icon(QFileIconProvider::File));
            } else {
                item->setIcon(0, publisherFunc::fileIcon(it->fileInfo().suffix()));
            }
        }
        if (itemRoot == nullptr) {
            ui->treeWidget_2->addTopLevelItem(item);
        } else {
            itemRoot->addChild(item);
        }
    }
}

void FtpClientWindow::hostPutFiles(QFileInfo info)
{
    if (info.isDir()) {
        ftp.createFtpDir(info.fileName());
    } else {
        ftp.put(info.path(), info.fileName());
    }
}

const TreeItem *FtpClientWindow::getTreeData(QTreeWidgetItem *item)
{
    QVariant var = item->data(0, Qt::UserRole + 1);
    const TreeItem *treeItem = (const TreeItem *)var.value<void *>();
    return treeItem;
}

void FtpClientWindow::readConfig()
{
    QSettings configIni("config.ini", QSettings::IniFormat);
    ui->ftpLine->setText(configIni.value("mainWindwos/ftpLine").toString());
    ui->userLine->setText(configIni.value("mainWindwos/userLine").toString());
    ui->passwdLine->setText(configIni.value("mainWindwos/passwdLine").toString());
    ui->hostUploadPath->setText(configIni.value("mainWindwos/hostUploadPath").toString());
    ui->hostSavePath->setText(configIni.value("mainWindwos/hostSavePath").toString());
}

void FtpClientWindow::writeConfig()
{
    QSettings configIni("config.ini", QSettings::IniFormat);
    configIni.setValue("mainWindwos/ftpLine", ui->ftpLine->text());
    configIni.setValue("mainWindwos/userLine", ui->userLine->text());
    configIni.setValue("mainWindwos/passwdLine", ui->passwdLine->text());
    configIni.setValue("mainWindwos/hostUploadPath", ui->hostUploadPath->text());
    configIni.setValue("mainWindwos/hostSavePath", ui->hostSavePath->text());
}
