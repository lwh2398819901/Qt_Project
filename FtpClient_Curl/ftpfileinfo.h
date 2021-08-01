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
#ifndef FTPFILEINFO_H
#define FTPFILEINFO_H

#include <QString>
#include <publishFunc/publishfunc.h>

class FtpFileInfo
{
private:
    QString m_name; //文件名称
    QString m_date; //时间
    QString m_suffix; //后缀
    int m_size { 0 }; //大小
    bool m_statusArr[10] { 0 };
public:
    FtpFileInfo();
    FtpFileInfo(const QString &);

    getSetFunc(Name, m_name)
    getSetFunc(Date, m_date)
    getSetFunc(Suffix, m_suffix)
    getSetFunc(Size, m_size)
    bool isDir() const { return m_statusArr[0]; }
    bool ownerRead() const { return m_statusArr[1]; }
    bool ownerWrite() const { return m_statusArr[2]; }
    bool ownerAuthority() const { return m_statusArr[3]; }
    bool ownerReadWrite() const { return m_statusArr[1] && m_statusArr[2]; }
    bool groupRead() const { return m_statusArr[4]; }
    bool groupWrite() const { return m_statusArr[5]; }
    bool groupAuthority() const { return m_statusArr[6]; }
    bool groupReadWrite() const { return m_statusArr[4] && m_statusArr[5]; }
    bool otherRead() const { return m_statusArr[7]; }
    bool otherWrite() const { return m_statusArr[8]; }
    bool otherAuthority() const { return m_statusArr[9]; }
    bool otherReadWrite() const { return m_statusArr[7] && m_statusArr[8]; }

    bool operator==(const FtpFileInfo &item) const
    {
        return this->m_name == item.m_name && this->m_size == item.m_size
                && this->m_date == item.m_date && this->m_suffix == item.m_suffix
                && this->m_statusArr == item.m_statusArr;
    }

    bool operator<(const FtpFileInfo &item) const
    {
        if (m_statusArr[0] != item.m_statusArr[0]) {
            return (m_statusArr[0] ? true : false);
        }
        return this->m_name < item.m_name;
    }

    bool operator>(const FtpFileInfo &item) const
    {
        if (*this == item) {
            return false;
        }
        return !(*this < item);
    }

    bool operator!=(const FtpFileInfo &item) const { return !(*this == item); }

};

#endif // FTPFILEINFO_H
