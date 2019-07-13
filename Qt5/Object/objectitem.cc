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

#include "ruleset.hh"
#include "objectitem.hh"
#include "object.hh"
#include "objectform.hh"
#include "objectmodel.hh"

#include "mustache.hh"

#include <QDebug>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPdfWriter>
#include <QPainter>
#include <QSettings>
#include <QStackedWidget>
#include <QSvgRenderer>
#include <QVBoxLayout>

void
GDW_RPG_Object_Initialize()
{
  Q_INIT_RESOURCE(object);
}

using namespace GDW::RPG;

ObjectTreeItem::Initialize ObjectTreeItem::Initializer;

ObjectTreeItem::ObjectTreeItem()
{}

ObjectTreeItem::ObjectTreeItem(Object* object, ObjectTreeItem* parent)
  : QObject(parent),
    mObject(object), mItemData(object->ItemData())
{}

ObjectTreeItem::ObjectTreeItem(const QList<QVariant>& data, ObjectTreeItem* parent)
  : QObject(parent), mObject(nullptr), mItemData(data)
{}

ObjectTreeItem::ObjectTreeItem(const ObjectTreeItem& item)
  : QObject(item.parent()),
    mObject(item.mObject->Copy()), mItemData(item.mItemData)
{}

ObjectTreeItem::~ObjectTreeItem()
{
  //  if(mObject != nullptr)
  //    delete mObject;

  // qDeleteAll(mChildItems);
}

ObjectTreeItem*
ObjectTreeItem::Copy() const
{
  return nullptr;
}

ObjectTreeItem*
ObjectTreeItem::Child(int row)
{
  return mChildItems.value(row);
}

int
ObjectTreeItem::ChildCount() const
{
  return mChildItems.count();
}

int
ObjectTreeItem::ColumnCount() const
{
  return mItemData.count();
}

QVariant
ObjectTreeItem::Data(int column) const
{
  return mItemData.value(column);
}

bool
ObjectTreeItem::SetData(int column, const QVariant& variant)
{
  if (column < 0 || column > mItemData.size())
    return false;

  if(variant.canConvert<ObjectTreeItem*>()) {
    ObjectTreeItem* oti = variant.value<ObjectTreeItem*>();
    mObject = oti->GetObject();
    mItemData = mObject->ItemData();
    return true;
  }

  return false;
}

bool
ObjectTreeItem::InsertChild(ObjectTreeItem* item, int position)
{
  if(item)
    item->setParent(this);

  mChildItems.insert(position, item);

  // Not necessary since the basic objects come with their
  // children already:
  if(mObject)
    mObject->InsertChild(item->GetObject(), position);

  return true;
}

ObjectTreeItem*
ObjectTreeItem::RemoveChild(int position)
{
  ObjectTreeItem* item = nullptr;

  if(position >= 0 && position < mChildItems.size()) {
    item = mChildItems.takeAt(position);
    item->setParent(nullptr);

    if(mObject)
      mObject->RemoveChild(position);
  }

  return item;
}

ObjectTreeItem*
ObjectTreeItem::ParentItem() const
{
  return static_cast<ObjectTreeItem*>(parent());
  //return qobject_cast<ObjectTreeItem*>(parent());
}

int
ObjectTreeItem::Row() const
{
  ObjectTreeItem* parent = ParentItem();

  if (parent)
    return parent->mChildItems.indexOf(const_cast<ObjectTreeItem*>(this));

  return 0;
}

QModelIndex
ObjectTreeItem::Index() const
{
  if(parent() == nullptr) {
    return QModelIndex();
  }

  return Model()->index(Row(), 0, ParentItem()->Index());
}

void
ObjectTreeItem::Export(QJsonArray& jarr) const
{
  for (int i = 0; i < mChildItems.size(); ++i) {
    Object* obj = mChildItems.at(i)->GetObject();
    QJsonValue jv(*obj);
    jarr.append(jv);
  }
}

QTextStream&
GDW::RPG::operator<<(QTextStream& ots, const ObjectTreeItem& item)
{
  QJsonArray jarr;

  item.Export(jarr);
  return ots << QJsonDocument(jarr).toJson(QJsonDocument::Compact);
}

ObjectForm*
ObjectTreeItem::GetForm(QUndoStack*)
{
  return nullptr;
}

void
ObjectTreeItem::RefreshItemData()
{
  if(mObject)
    mItemData = mObject->ItemData();
}

Object*
ObjectTreeItem::GetObject()
{
  return mObject;
}

const Object*
ObjectTreeItem::GetObject() const
{
  return mObject;
}

ObjectModel*
ObjectTreeItem::Model() const
{
  return nullptr;
}

QByteArray
ObjectTreeItem::Template() const
{
  return "";
}

QString
ObjectTreeItem::RenderSvg() const
{
  QVariantHash hash;

  Ruleset::ToVariantHash(hash);
  GetObject()->ToVariantHash(hash);

  Mustache::Renderer renderer;
  Mustache::QtVariantContext* context = GetObject()->Context(hash);

  return renderer.render(Template(), context);
}

void
ObjectTreeItem::RenderPage(QPaintDevice& device) const
{
  QXmlStreamReader reader(RenderSvg().toUtf8());
  QSvgRenderer svg(&reader);
  QPainter painter(&device);

  svg.render(&painter);
}

void
ObjectTreeItem::WriteSvg(QFile& file) const
{
  file.write(RenderSvg().toUtf8());
}
