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

#include "commands.hh"

#include "vehiclemodel.hh"
#include "objectitem.hh"


#include <QDebug>

namespace GDW
{
  namespace RPG
  {
    //
    // Insert Item Command
    //
    InsertItemCommand::InsertItemCommand(const QModelIndex& index,
                                         VehicleModel& model,
                                         QUndoCommand* parent)
      : QUndoCommand(parent),
        mRow(index.row()+1),
        mInserted(false),
        mParent(index.parent()),
        mVehicleModel(model)
    {
      setText(QObject::tr("insert"));
    }

    void
    InsertItemCommand::undo()
    {
      qDebug() << "InsertItemCommand::undo()";
      if(mInserted)
        mVehicleModel.removeRow(mRow, mParent);
    }

    void
    InsertItemCommand::redo()
    {
      qDebug() << "InsertItemCommand::redo()";

      mInserted = mVehicleModel.insertRow(mRow, mParent);

//      bool isSet = true;
//      for (int col = 0; col < mModel.columnCount(); ++col) {
//        QModelIndex index = mModel.index(mRow, col, mParent);
//        ObjectTreeItem* item = mModel.GetItem(index);
//        isSet = isSet && mModel.setData(index, item->Data(col));
//      }
//      mInserted = isSet;
    }

    //
    // Remove Item Command
    //
    RemoveItemCommand::RemoveItemCommand(const QModelIndex& index,
                                         VehicleModel& model,
                                         QUndoCommand* parent)
      : QUndoCommand(parent),
        mRow(index.row()),
        mRemoved(false),
        mParent(index.parent()),
        mVehicleModel(model)
    {
      setText(QObject::tr("remove"));
    }

    void
    RemoveItemCommand::undo()
    {
      qDebug() << "RemoveItemCommand::undo()";
      if(mRemoved)
        mVehicleModel.insertRow(mRow, mParent);
    }

    void
    RemoveItemCommand::redo()
    {
      qDebug() << "RemoveItemCommand::redo()";
      mRemoved = mVehicleModel.removeRow(mRow, mParent);
    }
  };
};
