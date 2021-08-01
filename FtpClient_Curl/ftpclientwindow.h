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
#ifndef FTPCLIENTWINDOW_H
#define FTPCLIENTWINDOW_H

#include <QMainWindow>

namespace Ui {
class FtpClientWindow;
}

class FtpClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FtpClientWindow(QWidget *parent = nullptr);
    ~FtpClientWindow();

private:
    Ui::FtpClientWindow *ui;
};

#endif // FTPCLIENTWINDOW_H
