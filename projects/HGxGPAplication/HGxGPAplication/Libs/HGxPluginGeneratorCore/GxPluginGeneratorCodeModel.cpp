/*=============================================================================

  Library: CTK

  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/


#include "GxPluginGeneratorCodeModel.h"

#include <QDir>
#include <QHash>

#include <ctkException.h>


class CGxPluginGeneratorCodeModelPrivate
{
public:

    QHash<QString, CGxPluginGeneratorAbstractTemplate*> rootTemplates;
    QHash<QString, QString> contentMap;

    QString symbolicNameWithPeriods;
    QString symbolicNameWithUnderscores;
    QString exportMacro;
    QString exportMacroInclude;
    QString license;
};

class CGxPluginGeneratorFolderTemplate : public CGxPluginGeneratorAbstractTemplate
{
public:

    CGxPluginGeneratorFolderTemplate(const QString& name, CGxPluginGeneratorAbstractTemplate* parent = 0)
        : CGxPluginGeneratorAbstractTemplate(name, parent)
    {}

    QString generateContent()
    {
        return "";
    }

    void create(const QString& location)
    {
        QDir dir(location);
        if (dir.mkdir(this->objectName()))
        {
            QString currLocation = location + QDir::separator() + this->objectName();
            QListIterator<QObject*> it(this->children());
            while (it.hasNext())
            {
                qobject_cast<CGxPluginGeneratorAbstractTemplate*>(it.next())->create(currLocation);
            }
        }
        else
        {
            throw ctkRuntimeException("The directory " + location + this->objectName() + " could not be created");
        }
    }
};

CGxPluginGeneratorCodeModel::CGxPluginGeneratorCodeModel()
    : d_ptr(new CGxPluginGeneratorCodeModelPrivate)
{
}

CGxPluginGeneratorCodeModel::~CGxPluginGeneratorCodeModel()
{
}

void CGxPluginGeneratorCodeModel::addContent(const QString& marker, const QString& content)
{
    Q_D(CGxPluginGeneratorCodeModel);
    d->contentMap[marker] = content;
}

QString CGxPluginGeneratorCodeModel::getContent(const QString& marker) const
{
    Q_D(const CGxPluginGeneratorCodeModel);
    if (d->contentMap.contains(marker))
    {
        return d->contentMap[marker];
    }
    return QString();
}

void CGxPluginGeneratorCodeModel::setExportMacro(const QString& exportMacro)
{
    Q_D(CGxPluginGeneratorCodeModel);
    d->exportMacro = exportMacro;
}

QString CGxPluginGeneratorCodeModel::getExportMacro() const
{
    Q_D(const CGxPluginGeneratorCodeModel);
    return d->exportMacro;
}

void CGxPluginGeneratorCodeModel::setExportMacroInclude(const QString& exportMacroInclude)
{
    Q_D(CGxPluginGeneratorCodeModel);
    d->exportMacroInclude = exportMacroInclude;
}

QString CGxPluginGeneratorCodeModel::getExportMacroInclude() const
{
    Q_D(const CGxPluginGeneratorCodeModel);
    return d->exportMacroInclude;
}

void CGxPluginGeneratorCodeModel::setSymbolicName(const QString& symbolicName)
{
    Q_D(CGxPluginGeneratorCodeModel);
    d->symbolicNameWithPeriods = QString(symbolicName).replace("_", ".");
    d->symbolicNameWithUnderscores = QString(symbolicName).replace(".", "_");
}

QString CGxPluginGeneratorCodeModel::getSymbolicName(bool withPeriods) const
{
    Q_D(const CGxPluginGeneratorCodeModel);
    if (withPeriods)
    {
        return d->symbolicNameWithPeriods;
    }
    return d->symbolicNameWithUnderscores;
}

void CGxPluginGeneratorCodeModel::setLicense(const QString& license)
{
    Q_D(CGxPluginGeneratorCodeModel);
    d->license = license;
}

QString CGxPluginGeneratorCodeModel::getLicense() const
{
    Q_D(const CGxPluginGeneratorCodeModel);
    return d->license;
}

void CGxPluginGeneratorCodeModel::addTemplate(CGxPluginGeneratorAbstractTemplate *templ,
    const QString& path)
{
    Q_D(CGxPluginGeneratorCodeModel);

    templ->setCodeModel(this);

    if (path.isEmpty())
    {
        d->rootTemplates.insert(templ->objectName(), templ);
        templ->setParent(this);
    }
    else
    {
        CGxPluginGeneratorAbstractTemplate* parentTemplate = 0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        QStringList paths = path.split("/", Qt::SkipEmptyParts);
#else
        QStringList paths = path.split("/", QString::SkipEmptyParts);
#endif
        QStringListIterator it(paths);
        if (it.hasNext())
        {
            QString rootEntry = it.next();
            // search the root templates
            if (d->rootTemplates.contains(rootEntry))
            {
                if (!dynamic_cast<CGxPluginGeneratorFolderTemplate*>(d->rootTemplates[rootEntry]))
                {
                    throw ctkRuntimeException("The segment \"" + rootEntry + "\" in \"" + path + "\" is not a folder");
                }
                parentTemplate = d->rootTemplates[rootEntry];
            }
            else
            {
                parentTemplate = new CGxPluginGeneratorFolderTemplate(rootEntry);
                d->rootTemplates.insert(rootEntry, parentTemplate);
            }
            while (it.hasNext())
            {
                QString currEntry = it.next();
                QListIterator<QObject*> children(parentTemplate->children());
                bool childFound = false;
                while (children.hasNext())
                {
                    QObject* child = children.next();
                    if (child->objectName() == currEntry)
                    {
                        childFound = true;
                        parentTemplate = qobject_cast<CGxPluginGeneratorAbstractTemplate*>(child);
                        if (parentTemplate == 0)
                        {
                            throw ctkRuntimeException("The segment \"" + currEntry + "\" in \"" + path + "\" is not a folder");
                        }
                        break;
                    }
                }

                if (!childFound)
                {
                    parentTemplate = new CGxPluginGeneratorFolderTemplate(currEntry, parentTemplate);
                }
            }
        }

        templ->setParent(parentTemplate);
    }
}

CGxPluginGeneratorAbstractTemplate* CGxPluginGeneratorCodeModel::getTemplate(const QString& path) const
{
    Q_D(const CGxPluginGeneratorCodeModel);

    if (!path.contains("/"))
    {
        foreach(CGxPluginGeneratorAbstractTemplate* t, d->rootTemplates)
        {
            if (t->objectName() == path) return t;
            CGxPluginGeneratorAbstractTemplate* child =
                t->findChild<CGxPluginGeneratorAbstractTemplate*>(path);
            if (child) return child;
        }
        return 0;
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QStringList paths = path.split("/", Qt::SkipEmptyParts);
#else
    QStringList paths = path.split("/", QString::SkipEmptyParts);
#endif
    if (paths.empty()) return 0;

    QObject* currChild = d->rootTemplates[paths.front()];
    paths.pop_front();

    int depth = 0;
    foreach(QString curr, paths)
    {
        foreach(QObject* child, currChild->children())
        {
            if (child->objectName() == curr)
            {
                currChild = child;
                ++depth;
                break;
            }
        }
    }

    if (paths.size() == depth)
    {
        return qobject_cast<CGxPluginGeneratorAbstractTemplate*>(currChild);
    }

    return 0;
}

void CGxPluginGeneratorCodeModel::create(const QString& location)
{
    Q_D(CGxPluginGeneratorCodeModel);

    QListIterator<CGxPluginGeneratorAbstractTemplate*> it(d->rootTemplates.values());
    while (it.hasNext())
    {
        CGxPluginGeneratorAbstractTemplate* templ = it.next();
        templ->create(location);
    }
}

void CGxPluginGeneratorCodeModel::reset()
{
    Q_D(CGxPluginGeneratorCodeModel);
    qDeleteAll(d->rootTemplates.values());
    d->rootTemplates.clear();
}
