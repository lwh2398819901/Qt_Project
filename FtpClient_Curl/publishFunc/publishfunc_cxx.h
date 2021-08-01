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
#ifndef PUBLISHFUNC_CXX_H
#define PUBLISHFUNC_CXX_H
/*************************************
公共函数 该公共空间内存放与项目无关的c++通用函数

**************************************/


namespace publisherFunc {
//枚举转换
#define enum2Int(_enum) static_cast<int>(_enum)
#define int2Eunm(_intNum,Type) static_cast<decltype(Type)>(_intNum)
#define enumAddInt(_enum,_intNum) int2Eunm(enum2Int(_enum)+_intNum,_enum)

//成员函数
#define getSetFunc(funcName, memVar)\
    decltype (memVar) funcName(){return memVar;}\
    decltype (memVar) funcName()const{return memVar;}\
    void set##funcName(const decltype(memVar)& data) { memVar = data; }

}


#endif // PUBLISHFUNC_CXX_H
