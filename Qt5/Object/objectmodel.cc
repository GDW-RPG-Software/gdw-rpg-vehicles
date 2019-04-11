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

#include "objectmodel.hh"
#include "objectitem.hh"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

using namespace GDW::RPG;

ObjectModel::ObjectModel(QObject* parent)
  : QAbstractItemModel(parent), mRootItem(nullptr)
{}

ObjectModel::~ObjectModel()
{
  if(mRootItem != nullptr)
    delete mRootItem;
}

//
// Public API
//
void
ObjectModel::Export(QJsonArray& jarr) const
{
  mRootItem->Export(jarr);
}

void
ObjectModel::Reset()
{
  beginResetModel();

  delete mRootItem;
  mRootItem = nullptr;

  endResetModel();
}

void
ObjectModel::Print(QModelIndex index, QPrinter& printer) const
{
  ItemFor(index)->RenderPage(printer);
  //ui->textEdit->print(&printDev);
}

//
// Extension points
//
ObjectTreeItem*
ObjectModel::RootItem() const
{
  if(mRootItem == nullptr) {
    mRootItem = new ObjectTreeItem(RootData());
  }

  return mRootItem;
}

ObjectTreeItem*
ObjectModel::ItemFor(const QModelIndex& index) const
{
  if (index.isValid()) {
    ObjectTreeItem* item =
        static_cast<ObjectTreeItem*>(index.internalPointer());
    if (item)
      return item;
  }
  return RootItem();
}

//
// Mandatory overrides
//
QModelIndex
ObjectModel::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  ObjectTreeItem* parentItem = ItemFor(parent);
  ObjectTreeItem* childItem = parentItem->Child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex
ObjectModel::parent(const QModelIndex& index) const
{
  if (!index.isValid())
    return QModelIndex();

  ObjectTreeItem* childItem = ItemFor(index);
  ObjectTreeItem* parentItem = childItem->ParentItem();

  if (parentItem == RootItem())
    return QModelIndex();

  return createIndex(parentItem->Row(), 0, parentItem);
}

int
ObjectModel::rowCount(const QModelIndex& index) const
{
  if(index.column() > 0)
    return 0;

  return ItemFor(index)->ChildCount();
}

int
ObjectModel::columnCount(const QModelIndex& index) const
{
  return ItemFor(index)->ColumnCount();
}

QVariant
ObjectModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole && role != Qt::EditRole)
    return QVariant();

  return ItemFor(index)->Data(index.column());
}

//
// Optional overrides
//
Qt::ItemFlags
ObjectModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
  // return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant
ObjectModel::headerData(int section,
                        Qt::Orientation orientation,
                        int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return RootItem()->Data(section);

  return QVariant();
}

bool
ObjectModel::insertRows(int position, int rows, const QModelIndex& parent)
{
  ObjectTreeItem* parentItem = ItemFor(parent);

  beginInsertRows(parent, position, position + rows - 1);
  bool success =
      parentItem->InsertChildren(position, rows, Create(parentItem));
  endInsertRows();

  return success;
}

bool
ObjectModel::removeRows(int position, int rows, const QModelIndex& parent)
{
  ObjectTreeItem* parentItem = ItemFor(parent);

  beginRemoveRows(parent, position, position + rows - 1);
  bool success = parentItem->RemoveChildren(position, rows);
  endRemoveRows();

  return success;
}

bool
ObjectModel::setData(const QModelIndex& index,
                     const QVariant& value, int role)
{
  if (!index.isValid() || role != Qt::EditRole)
    return false;

  ObjectTreeItem* item = ItemFor(index);
  bool result = item->SetData(index.column(), value);

  if (result)
    emit dataChanged(index, index, {role});

  return result;
}

ObjectTreeItem*
ObjectModel::Unpack(const QJsonObject&, ObjectTreeItem*)
{
  return nullptr;
}

//
// Output
//
QTextStream&
GDW::RPG::operator<<(QTextStream& out, const ObjectModel& model)
{
  return out << *model.RootItem();
}