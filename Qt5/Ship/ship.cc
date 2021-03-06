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

#include "ship.hh"

using namespace GDW::RPG;

const QString
Ship::JSON_TYPE = "__GDW_RPG_Ship__";

Ship::Ship(const QJsonObject& json)
  : Object (json)
{}

Ship*
Ship::New()
{
  static const QJsonObject object
  {
    {"__GDW_RPG_Type__", JSON_TYPE},
    {PROP_NAME, "[Name]"}
  };

  return new Ship(object);
}


const QString Ship::PROP_NAME = "name";

QVariant
Ship::Name() const
{
  return GetVariantFor(PROP_NAME);
}

void
Ship::Name(const QVariant& value)
{
  SetVariantFor(PROP_NAME, value);
}
