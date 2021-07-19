#ifndef FTPFILEINFO_H
#define FTPFILEINFO_H

#include <QObject>
#include <QTime>

class FtpFileInfo
{
public:
    FtpFileInfo() {}
    FtpFileInfo(const QString &);

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
    int size() const { return m_size; }
    QString name() const { return m_name; }
    QString date() const { return m_date; }
    QString suffix() const { return m_suffix; }

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
        return this->name() < item.name();
    }

    bool operator>(const FtpFileInfo &item) const
    {

        if (*this == item)
            return false;
        return !(*this < item);
    }

    bool operator!=(const FtpFileInfo &item) const { return !(*this == item); }

private:
    bool m_statusArr[10] { 0 };
    QString m_name;
    QString m_date;
    QString m_suffix;
    int m_size { 0 };
};

#endif // FTPFILEINFO_H
