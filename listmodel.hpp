/*  Copyright (c) 2014, Luiz Romário Santana Rios <luizromario@gmail.com>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
          names of its contributors may be used to endorse or promote products
          derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef LISTMODEL_HPP
#define LISTMODEL_HPP

#include <QAbstractListModel>

#include <initializer_list>
#include <functional>
#include <unordered_map>

namespace EasyModels
{

template<class T>
class List : public QAbstractListModel
{
public:
    List(std::initializer_list<T> l,
         std::function<QVariant(T &)> displayFunction,
         QObject *parent = nullptr) :
        List { l,
               { { Qt::DisplayRole, displayFunction } },
               {},
               parent }
    {}

    List(std::initializer_list<T> l,
         std::function<QVariant(T &)> displayFunction,
         std::function<bool(T &, const QVariant)> editFunction,
         QObject *parent = nullptr) :
        List { l,
               { { Qt::DisplayRole, displayFunction } },
               { { Qt::EditRole, editFunction } },
               parent }
    {}

    List(std::initializer_list<T> l,
         std::unordered_map<int, std::function<QVariant(T &)>> roles,
         QObject *parent = nullptr):
        List { l, roles, {}, parent }
    {}

    List(std::initializer_list<T> l,
         std::unordered_map<int, std::function<QVariant(T &)>> roles,
         std::unordered_map<int, std::function<bool(T &, const QVariant)>> editRoles,
         QObject *parent = nullptr):
        QAbstractListModel { parent },
        _items { l },
        _roles { roles },
        _editRoles { editRoles }
    {}

    int rowCount(const QModelIndex &) const
    {
        return _items.size();
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() ||
            index.row() >= _items.size())
            return QVariant {};

        auto item = _items.at(index.row());
        auto roleFunction = _roles.find(role);

        if (roleFunction == _roles.end())
            return QVariant {};

        return roleFunction->second(item);
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        return QAbstractListModel::flags(index) | (_editRoles.empty()? Qt::NoItemFlags : Qt::ItemIsEditable);
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (!index.isValid() ||
            index.row() >= _items.size())
            return false;

        auto &item = _items[index.row()];
        auto editRoleFunction = _editRoles.find(role);

        if (editRoleFunction == _editRoles.end())
            return false;

        return editRoleFunction->second(item, value);
    }

private:
    QList<T> _items;
    std::unordered_map<int, std::function<QVariant(T &)>> _roles;
    std::unordered_map<int, std::function<bool(T &, const QVariant)>> _editRoles;
};

}

#endif // LISTMODEL_HPP
