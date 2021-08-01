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
#ifndef PUBLISHFUNC_QT_H
#define PUBLISHFUNC_QT_H

#include <QDir>
#include <QJsonDocument>
#include <QMessageBox>
#include <QMetaEnum>

/*************************************
公共函数 该公共空间内存放与项目无关的qt通用函数

**************************************/

namespace publisherFunc {
//显示窗口
void showTextBox(QString str);
void showTextBox(int str);
void moveToWindowCenter(QWidget *widget);
//暂停
void pause(const unsigned int &msec);
//文件相关
bool createDir(const QString &dir);
bool createFile(const QString &filePath, const QString &text,
                QIODevice::OpenMode model = QIODevice::WriteOnly);
QJsonDocument readJson(const QString &filePath);
QString readFile(const QString &filePath);
QStringList getDirFileNames(const QString &dirPath, const QDir::Filters &flag = QDir::Files,
                            const QStringList &suffix = QStringList());
//文件icon
QIcon fileIcon(const QString &extension);
QString fileType(const QString &extension);
QString getImagePixel(const QString &imagePath);


//调用外部命令
QPair<int, QString> startPopen(const QString &cmd, const QString &model = QString("r"));
//线程调用
QPair<int, QString> threadCall(const QString &cmd, bool isQProcess = true, int msec = -1);


//转换相关
QString humanReadableSize(const qint64 &size, int precision = 2);

template<class T>
QString Qt_enumToString(T type); //仅限Qt 枚举转换使用

template<class T>
T Qt_stringToEnum(T enumType, QString enumStr); //仅限Qt 字符串转枚举
}

template<class T>
QString publisherFunc::Qt_enumToString(T type)
{
    QMetaEnum meta = QMetaEnum::fromType<T>();
    if (meta.isValid())
        return QString(meta.valueToKey(static_cast<int>(type)));
    return QString();
}

template<class T>
T publisherFunc::Qt_stringToEnum(T enumType, QString enumStr)
{
    QMetaEnum meta = QMetaEnum::fromType<T>();
    if (meta.isValid()) {
        bool ok = false;
        int i = meta.keyToValue(enumStr.toStdString().c_str(), &ok);
        if (ok) {
            return static_cast<T>(i);
        }
    }
    return enumType;
}

#endif // PUBLISHFUNC_QT_H
