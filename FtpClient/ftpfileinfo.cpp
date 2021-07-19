#include "ftpfileinfo.h"

FtpFileInfo::FtpFileInfo(const QString &tmp)
{
    if (!tmp.isEmpty()) {
        QStringList list;
        foreach (QString tmp2, tmp.split(" ")) {
            if (!tmp2.isEmpty()) {
                list << tmp2;
            }
        }

        if (list[0].count() < 10) {
            return;
        }
        // drwxrwxrwx  对应十位
        m_statusArr[0] = (list[0][0] == 'd');
        m_statusArr[1] = (list[0][1] != '-');
        m_statusArr[2] = (list[0][2] != '-');
        m_statusArr[3] = (list[0][3] != '-');
        m_statusArr[4] = (list[0][4] != '-');
        m_statusArr[5] = (list[0][5] != '-');
        m_statusArr[6] = (list[0][6] != '-');
        m_statusArr[7] = (list[0][7] != '-');
        m_statusArr[8] = (list[0][8] != '-');
        m_statusArr[9] = (list[0][9] != '-');

        m_name = *list.rbegin();
        m_size = list[4].toInt();
        m_date = (list[5] + " " + list[6] + " " + list[7]);
        if (!m_statusArr[0]) {
            auto list = m_name.split(".");
            if (list.count()) {
                m_suffix = *list.rbegin();
            }
        }
    }
}
