/*
 * Copyright (C) 2017 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

class UbuntuSettingsPrintersPrivate {
private:
  Q_DISABLE_COPY(UbuntuSettingsPrintersPrivate)
  Q_DECLARE_PUBLIC(UbuntuSettingsPrinters)
  UbuntuSettingsPrintersPrivate(UbuntuSettingsPrinters*);
  UbuntuSettingsPrinters * const q_ptr;
  QFormLayout layout;
  QDoubleSpinBox x, y, z;
  QDialogButtonBox buttons;
  QVector3D coordinates;
  void onAccepted();
};

