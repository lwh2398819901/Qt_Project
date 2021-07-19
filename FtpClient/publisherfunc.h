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

#ifndef PUBLISHERFUNC_H
#define PUBLISHERFUNC_H

#include <QDir>
#include <QJsonDocument>
#include <QMessageBox>
#include <QMetaEnum>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <bits/stl_map.h>

/*************************************
公共函数 该公共空间内存放与项目无关的通用函数

**************************************/
#define enum2Int(_enum) static_cast<int>(_enum)
#define int2Eunm(_intNum, Type) static_cast<decltype(Type)>(_intNum)
#define enumAddInt(_enum, _intNum) int2Eunm(enum2Int(_enum) + _intNum, _enum)

namespace publisherFunc {
void showTextBox(QString str);
void showTextBox(int str);
void pause(const unsigned int &msec);
void moveToWindowCenter(QWidget *widget);
bool createDir(const QString &dir);
bool createFile(const QString &filePath, const QString &text,
                QIODevice::OpenMode model = QIODevice::WriteOnly);
QJsonDocument readJson(const QString &filePath);
QString readFile(const QString &filePath);
QString getImagePixel(const QString &imagePath);
QStringList getDirFileNames(const QString &dirPath, const QDir::Filters &flag = QDir::Files,
                            const QStringList &suffix = QStringList());
QPair<int, QString> startPopen(const QString &cmd, const QString &model = QString("r"));

QString humanReadableSize(const qint64 &size, int precision = 2);

template<class T>
QString Qt_enumToString(T type); //仅限Qt 枚举转换使用

template<class T>
T Qt_stringToEnum(T enumType, QString enumStr); //仅限Qt 字符串转枚举

QPair<int, QString> threadCall(const QString &cmd, bool isQProcess = true, int msec = -1);

QIcon fileIcon(const QString &extension);
QString fileType(const QString &extension);
};

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

#endif // PUBLISHERFUNC_H
