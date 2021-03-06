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

#include "munitiondialog.hh"

#include "munitionmodel.hh"

#include <QtWidgets>

using namespace GDW::RPG;

MunitionDialog::MunitionDialog(WeaponItem* wti, QWidget* parent)
  : QDialog(parent)
{
  QVBoxLayout* dialogLayout = new QVBoxLayout(this);

  QTreeView* treeview = new QTreeView(this);
  treeview->setModel(MunitionModel::Model(wti));
  connect(treeview, &QTreeView::clicked, this, &MunitionDialog::ItemSelected);
  connect(treeview, &QTreeView::doubleClicked, this, &QDialog::accept);
  dialogLayout->addWidget(treeview);

  QDialogButtonBox* buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  QPushButton* createButton =
      new QPushButton(QIcon("://icons/16x16/list-add.png"), tr("Create munition"));
  connect(createButton, &QPushButton::clicked, this,
          [=]() {
    this->finished(2);
    emit createMunition();
  });
  buttonBox->addButton(createButton, QDialogButtonBox::ResetRole);
  dialogLayout->addWidget(buttonBox);

  setWindowTitle(tr("Select a munition"));
  setLayout(dialogLayout);
}

QModelIndex
MunitionDialog::Selected() const
{
  return mSelectedIndex;
}

void
MunitionDialog::ItemSelected(const QModelIndex& index)
{
  mSelectedIndex = index;
}
