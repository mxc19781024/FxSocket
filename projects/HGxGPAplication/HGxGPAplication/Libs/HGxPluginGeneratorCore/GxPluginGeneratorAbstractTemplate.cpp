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


#include "GxPluginGeneratorAbstractTemplate.h"

#include "GxPluginGeneratorCodeModel.h"
#include "GxPluginGeneratorConstants.h"

#include <QHash>
#include <QFile>

class CGxPluginGeneratorAbstractTemplatePrivate
{
public:

    CGxPluginGeneratorAbstractTemplatePrivate()
        : codeModel(0)
    {}

    CGxPluginGeneratorCodeModel* codeModel;

    QString filename;
    QHash<QString, QStringList> contentMap;
};

CGxPluginGeneratorAbstractTemplate::CGxPluginGeneratorAbstractTemplate(
    const QString& name, CGxPluginGeneratorAbstractTemplate* parent)
    : QObject(parent), d_ptr(new CGxPluginGeneratorAbstractTemplatePrivate)
{
    this->setObjectName(name);
}

CGxPluginGeneratorAbstractTemplate::~CGxPluginGeneratorAbstractTemplate()
{

}

void CGxPluginGeneratorAbstractTemplate::setCodeModel(CGxPluginGeneratorCodeModel *codeModel)
{
    Q_D(CGxPluginGeneratorAbstractTemplate);
    d->codeModel = codeModel;
}

void CGxPluginGeneratorAbstractTemplate::setFilename(const QString& filename)
{
    Q_D(CGxPluginGeneratorAbstractTemplate);
    d->filename = filename;
}

QString CGxPluginGeneratorAbstractTemplate::getFilename() const
{
    Q_D(const CGxPluginGeneratorAbstractTemplate);

    QString filename = this->objectName();
    if (!d->filename.isEmpty())
    {
        filename = d->filename;
    }

    return filename;
}

void CGxPluginGeneratorAbstractTemplate::reset()
{
    Q_D(CGxPluginGeneratorAbstractTemplate);
    d->contentMap.clear();
}

void CGxPluginGeneratorAbstractTemplate::addContent(const QString &marker, const QString &content, Position pos)
{
    Q_D(CGxPluginGeneratorAbstractTemplate);
    switch (pos)
    {
    case PREPEND:
    {
        d->contentMap[marker].prepend(content);
        break;
    }
    case APPEND:
    {
        d->contentMap[marker].append(content);
        break;
    }
    case REPLACE:
    {
        QStringList& v = d->contentMap[marker];
        v.clear();
        v.append(content);
        break;
    }
    }
}

QStringList CGxPluginGeneratorAbstractTemplate::getContent(const QString &marker) const
{
    Q_D(const CGxPluginGeneratorAbstractTemplate);
    if (d->contentMap.contains(marker))
    {
        return d->contentMap[marker];
    }

    QString globalDefault = d->codeModel->getContent(marker);
    if (!globalDefault.isEmpty())
    {
        return QStringList(globalDefault);
    }

    return QStringList();
}

void CGxPluginGeneratorAbstractTemplate::create(const QString& location)
{
    QString filename = getFilename();

    const QString path = location + "/" + filename;
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    /* 由于Visual Studio2015会出现更改UTF-8编码的文件为GB2312编码，导致中文文本乱码，
    故在用插件生成器生成插件源文件时，指定源文件的编码格式为UTF-8加BOM
    */
    QByteArray array;
    array.resize(3);
    array[0] = 0xEF;
    array[1] = 0xBB;
    array[2] = 0xBF;
    file.write(array);
    file.write(this->generateContent().toLatin1());
    file.close();
}

QStringList CGxPluginGeneratorAbstractTemplate::getMarkers() const
{
    return CGxPluginGeneratorConstants::getGlobalMarkers();
}

QString CGxPluginGeneratorAbstractTemplate::getSymbolicName(bool withPeriods) const
{
    Q_D(const CGxPluginGeneratorAbstractTemplate);
    return d->codeModel->getSymbolicName(withPeriods);
}
