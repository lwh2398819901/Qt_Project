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
    connect(ui->treeWidget_2->header(), &QHeaderView::sectionClicked, this,
            &FtpClientWindow::ftpSort);
    connect(ui->treeWidget_2, &QTreeWidget::itemDoubleClicked, this,
            &FtpClientWindow::showFtpFileText);

    connect(ui->getBtn, &QPushButton::clicked, this, &FtpClientWindow::get);
    connect(ui->putBtn, &QPushButton::clicked, this, &FtpClientWindow::put);
    connect(ui->updateBtn, &QPushButton::clicked, this, &FtpClientWindow::updateFtpDir);
    connect(ui->getPathBtn, &QPushButton::clicked, [=]() {
        QString str = QFileDialog::getExistingDirectory();
        if (!str.isEmpty()) {
            ui->hostSavePath->setText(str);
        }
    });

    ftpMenu = new QMenu(this);
    auto AddAct = [&](QMenu *menu, QAction *&act, QString str, void (FtpClientWindow::*func)()) {
        act = new QAction(str, this);
        menu->addAction(act);
        connect(act, &QAction::triggered, this, func);
    };

    AddAct(ftpMenu, ftpSeekAct, "预览（文件大小１m以内）", &FtpClientWindow::showFtpFileText);
    AddAct(ftpMenu, ftpDownAct, "下载", &FtpClientWindow::get);
    AddAct(ftpMenu, ftpRenameAct, "重命名", &FtpClientWindow::renameFtp);
    AddAct(ftpMenu, ftpCreateAct, "创建文件夹", &FtpClientWindow::createFtpDir);
    //    AddAct(ftpMenu, ftpDeleteAct, "删除", &FtpClientWindow::deleteFtp);
    //    AddAct(ftpMenu, ftpPasteAct, "粘贴", &FtpClientWindow::deleteFtp);
    //    AddAct(ftpMenu, ftpShearAct, "剪切", &FtpClientWindow::deleteFtp);
    //    AddAct(ftpMenu, ftpCopyAct, "复制", &FtpClientWindow::deleteFtp);

    hostMenu = new QMenu(this);
    AddAct(hostMenu, hostPutAct, "上传", &FtpClientWindow::put);
    AddAct(hostMenu, hostRename, "重命名", &FtpClientWindow::renameFtp);
    AddAct(hostMenu, hostDeleteAct, "删除", &FtpClientWindow::createFtpDir);
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
    getFtpFiles(ftp.updateFtpFiles());
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

        if (treeItem->parent() && (treeItem->parent()->findChild(newName) != nullptr)) {
            showTextBox("error:该名称已经存在");
            return;
        }

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

void FtpClientWindow::deleteFtp() {}

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

void FtpClientWindow::showFtpFileText()
{
    const TreeItem *treeItem = getTreeData(ui->treeWidget_2->currentItem());
    if (!treeItem->fileInfo().isDir() && treeItem->fileInfo().size() < 1024 * 1024 * 3) {
        ui->ftpFileText->setText(ftp.getFtpFileText(treeItem->path()));
    }
}

void FtpClientWindow::getFtpFiles(const TreeItem *root, QTreeWidgetItem *itemRoot)
{
    QFileIconProvider icon_provider;
    foreach (const TreeItem *it, root->childList()) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, it->fileInfo().name());
        QVariant var = QVariant::fromValue((void *)it);
        item->setData(0, Qt::UserRole + 1, var);
        item->setText(2, it->fileInfo().date());

        if (it->fileInfo().isDir()) {
            item->setIcon(0, icon_provider.icon(QFileIconProvider::Folder));
            getFtpFiles(it, item);
        } else {
            item->setIcon(0, publisherFunc::fileIcon(it->fileInfo().suffix()));
            item->setText(1, publisherFunc::humanReadableSize(it->fileInfo().size(), 2));
        }
        (itemRoot == nullptr) ? ui->treeWidget_2->addTopLevelItem(item) : itemRoot->addChild(item);
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
    if (var.isValid())
        return (const TreeItem *)var.value<void *>();
    return nullptr;
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
