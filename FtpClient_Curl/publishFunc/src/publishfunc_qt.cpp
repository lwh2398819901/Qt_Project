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
#include "../include/publishfunc_qt.h"

#include <QDir>
#include <QFile>
#include <QTime>
#include <QImage>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QDebug>
#include <QFileIconProvider>
#include <QTemporaryFile>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <bits/stl_map.h>

namespace publisherFunc {

bool createFile(const QString &filePath, const QString &text, QIODevice::OpenMode model)
{
    QFile file(filePath);
    if (file.open(model)) {
        file.write(text.toLocal8Bit());
        file.close();
        return true;
    }
    return false;
}

QString readFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QString str = file.readAll();
        file.close();
        return str;
    }
    return QString();
}

QStringList getDirFileNames(const QString &dirPath, const QDir::Filters &flag,
                            const QStringList &suffix)
{
    QDir dir(dirPath); //获取文件夹内所有文件名
    if (dir.exists()) {
        dir.setFilter(flag); //设置文件类型
        dir.setNameFilters(suffix); //设置文件后缀
        return dir.entryList();
    }
    return QStringList();
}

QString getImagePixel(const QString &imagePath)
{
    QImage image(imagePath);
    return QString("%1x%2").arg(image.width()).arg(image.height());
}

void pause(const unsigned int &msec)
{
    if (msec != 0) {
        QTime dieTime = QTime::currentTime().addMSecs(static_cast<int>(msec));
        while (QTime::currentTime() < dieTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
}

QPair<int, QString> startPopen(const QString &cmd, const QString &model)
{
    QString res {};
    char buf[1024] {};
    FILE *p_file = popen(cmd.toLocal8Bit(), model.toLocal8Bit());
    if (p_file) {
        while (fgets(buf, 1024, p_file) != nullptr) {
            res += QString(buf);
            memset(buf, 0, 1024);
        }
    }
    int i = WEXITSTATUS(pclose(p_file));
    return QPair<int, QString>(i, res);
}

void moveToWindowCenter(QWidget *widget)
{
    //获取主屏幕分辨率
    QRect screenRect = QApplication::desktop()->screenGeometry();
    widget->move(screenRect.width() / 2 - widget->width() / 2,
                 screenRect.height() / 2 - widget->height() / 2 - 100);
}

QJsonDocument readJson(const QString &filePath)
{
    //创建json对象，读取json文件
    QByteArray allData = publisherFunc::readFile(filePath).toLocal8Bit();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
    if (json_error.error != QJsonParseError::NoError) {
        return QJsonDocument();
    }
    return jsonDoc;
}

bool createDir(const QString &dir)
{
    QDir _dir(dir);
    if (_dir.exists(dir)) {
        return true;
    }
    return _dir.mkpath(dir);
}

void showTextBox(QString str)
{
    QMessageBox::warning(0, "", str);
}

void showTextBox(int str)
{
    QMessageBox::warning(0, "", QString::number(str));
}

QPair<int, QString> threadCall(const QString &cmd, bool isQProcess, int msec)
{

    QPair<int, QString> res { -1, "ERROR" };
    bool threadEixt = false;
    std::thread t([&]() {
        if (isQProcess) { //使用QProcess
            QStringList list = cmd.split(" ");
            if (list.count()) {
                QProcess p(nullptr);
                QString _cmd = list[0];
                list.removeAt(0);
                p.start(_cmd, list);
                p.waitForFinished(msec);
                res.first = p.exitCode();
                res.second = p.readAllStandardOutput();
                res.second = p.readAllStandardError();
            }
        } else {
            res = publisherFunc::startPopen(cmd);
        }
        threadEixt = true;
    });
    t.detach();
    while (!threadEixt) {
        publisherFunc::pause(200);
    }

    return res;
}

QIcon fileIcon(const QString &extension)
{
    QFileIconProvider provider;
    if(extension.isEmpty())
        return provider.icon(QFileIconProvider::File);

    QIcon icon;
    QString strTemplateName = QDir::tempPath() + QDir::separator()
            + QCoreApplication::applicationName() + "_XXXXXX." + extension;
    QTemporaryFile tmpFile(strTemplateName);
    tmpFile.setAutoRemove(false);

    if (tmpFile.open()) {
        tmpFile.close();
        icon = provider.icon(QFileInfo(tmpFile.fileName()));
        // tmpFile.remove();
    } else {
        qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
    }

    return icon;
}

QString fileType(const QString &extension)
{
    QFileIconProvider provider;
    QString strType;
    QString strFileName = QDir::tempPath() + QDir::separator() + QCoreApplication::applicationName()
            + "_XXXXXX." + extension;
    QTemporaryFile tmpFile(strFileName);
    tmpFile.setAutoRemove(false);

    if (tmpFile.open()) {
        tmpFile.close();
        strType = provider.type(QFileInfo(tmpFile.fileName()));
        // tmpFile.remove();
    } else {
        qCritical() << QString("failed to write temporary file %1").arg(tmpFile.fileName());
    }

    return strType;
}

QString humanReadableSize(const qint64 &size, int precision)
{
    double sizeAsDouble = size;
    static QStringList measures;
    if (measures.isEmpty())
        measures << QCoreApplication::translate("QInstaller", "bytes")
                 << QCoreApplication::translate("QInstaller", "KiB")
                 << QCoreApplication::translate("QInstaller", "MiB")
                 << QCoreApplication::translate("QInstaller", "GiB")
                 << QCoreApplication::translate("QInstaller", "TiB")
                 << QCoreApplication::translate("QInstaller", "PiB")
                 << QCoreApplication::translate("QInstaller", "EiB")
                 << QCoreApplication::translate("QInstaller", "ZiB")
                 << QCoreApplication::translate("QInstaller", "YiB");
    QStringListIterator it(measures);
    QString measure(it.next());
    while (sizeAsDouble >= 1024.0 && it.hasNext()) {
        measure = it.next();
        sizeAsDouble /= 1024.0;
    }
    return QString::fromLatin1("%1 %2").arg(sizeAsDouble, 0, 'f', precision).arg(measure);
}

}
