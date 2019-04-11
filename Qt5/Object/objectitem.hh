/**
 * GDW RPG Vehicles, a vehicle database for Traveller and other GDW derived RPGs.
 *
 * Copyright (C) 2018-2019 Michael N. Henry
 *
 * This file is part of GDW RPG Vehicles.
 *
 * GDW RPG Vehicles is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * GDW RPG Vehicles is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with GDW RPG Vehicles. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OBJECTTREEITEM_HH
#define OBJECTTREEITEM_HH

#include "object_global.hh"

#include <QList>
#include <QVariant>

class QJsonArray;
class QStackedWidget;
class QTextStream;

namespace Ui {
  class Workspace;
}

namespace GDW
{
  namespace RPG
  {
    class Object;
    class ObjectForm;

    class OBJECTSHARED_EXPORT ObjectTreeItem : public QObject
    {
      public:
        ObjectTreeItem();
        ObjectTreeItem(const ObjectTreeItem&);
        ObjectTreeItem(Object*, ObjectTreeItem* parent = nullptr);
        ObjectTreeItem(const QList<QVariant>&,
                       ObjectTreeItem* parent = nullptr);
        virtual ~ObjectTreeItem();

        void Export(QJsonArray&) const;
        void RenderPage(QPaintDevice&) const;
        virtual QByteArray Template() const;

        ObjectTreeItem* AppendChild(ObjectTreeItem* child);
        bool InsertChildren(int, int, /*int type,*/ ObjectTreeItem* = nullptr);
        bool RemoveChildren(int, int);

        ObjectTreeItem* Child(int row);
        int ChildCount() const;
        int ColumnCount() const;
        virtual QVariant Data(int column) const;
        virtual bool SetData(int column, const QVariant&);
        int Row() const;
        ObjectTreeItem* ParentItem();

        void RefreshItemData();

        virtual ObjectForm* GetForm();

        virtual QDebug& Debug(QDebug&) const;

      signals:

      public slots:

      protected:
        virtual Object* GetObject();
        virtual const Object* GetObject() const;

      private:
        friend QTextStream& operator<<(QTextStream&, const ObjectTreeItem&);

        Object* mObject;
        QList<ObjectTreeItem*> mChildItems;
        QList<QVariant> mItemData;
        ObjectTreeItem* mParentItem;
    };
  };
};

Q_DECLARE_METATYPE(GDW::RPG::ObjectTreeItem)

#endif // OBJECTTREEITEM_HH