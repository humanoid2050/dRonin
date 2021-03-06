/**
 ******************************************************************************
 *
 * @file       iwizard.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 *             Parts by Nokia Corporation (qt-info@nokia.com) Copyright (C) 2009.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup CorePlugin Core Plugin
 * @{
 * @brief The Core GCS plugin
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>
 */

#ifndef IWIZARD_H
#define IWIZARD_H

#include <coreplugin/core_global.h>
#include <QtCore/QObject>

QT_BEGIN_NAMESPACE
class QIcon;
QT_END_NAMESPACE

namespace Core {

class CORE_EXPORT IWizard : public QObject
{
    Q_OBJECT
public:
    enum Kind { FileWizard, ClassWizard, ProjectWizard };

    IWizard(QObject *parent = nullptr)
        : QObject(parent)
    {
    }
    virtual ~IWizard() {}

    virtual Kind kind() const = 0;
    virtual QIcon icon() const = 0;
    virtual QString description() const = 0;
    virtual QString name() const = 0;

    virtual QString category() const = 0;
    virtual QString trCategory() const = 0;

    virtual QStringList runWizard(const QString &path, QWidget *parent) = 0;

    // Utility to find all registered wizards
    static QList<IWizard *> allWizards();
    // Utility to find all registered wizards of a certain kind
    static QList<IWizard *> wizardsOfKind(Kind kind);
};

} // namespace Core

#endif // IWIZARD_H
