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

#ifndef PREFSDIALOG_HH
#define PREFSDIALOG_HH


#include <QDialog>
#include <QMap>

class QButtonGroup;
class QCloseEvent;

namespace GDW
{
  namespace RPG
  {
    class Workspace;

    class PrefsDialog : public QDialog
    {
        Q_OBJECT

      public:
        PrefsDialog(bool, const QString&, Workspace* parent);
        ~PrefsDialog();

        bool LoadOnStart() const;
        QString Ruleset() const;
        void Ruleset(const QString&);

      private slots:
        void LoadOnStart(bool);
        void Ruleset(uint);

      private:
        bool mLoadOnStart;
        QString mRuleset;

        QButtonGroup* mRuleGroup;
        QMap<uint,QString> mRuleMap;
    };
  };
};

#endif // PREFSDIALOG_HH
