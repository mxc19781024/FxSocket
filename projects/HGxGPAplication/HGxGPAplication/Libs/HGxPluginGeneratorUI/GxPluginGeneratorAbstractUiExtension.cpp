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

#include "GxPluginGeneratorAbstractUiExtension.h"

class CGxPluginGeneratorAbstractUiExtensionPrivate
{
public:

    CGxPluginGeneratorAbstractUiExtensionPrivate()
        : extensionWidget(0)
    {

    }

    QWidget* extensionWidget;

    QString description;
    QString title;
    QString message;
    QIcon icon;
};

CGxPluginGeneratorAbstractUiExtension::CGxPluginGeneratorAbstractUiExtension()
    : d_ptr(new CGxPluginGeneratorAbstractUiExtensionPrivate())
{

}

CGxPluginGeneratorAbstractUiExtension::~CGxPluginGeneratorAbstractUiExtension()
{

}

QWidget* CGxPluginGeneratorAbstractUiExtension::getWidget()
{
    Q_D(CGxPluginGeneratorAbstractUiExtension);
    if (d->extensionWidget == 0)
    {
        d->extensionWidget = this->createWidget();
    }

    return d->extensionWidget;
}

QString CGxPluginGeneratorAbstractUiExtension::getDescription() const
{
    Q_D(const CGxPluginGeneratorAbstractUiExtension);
    return d->description;
}

QString CGxPluginGeneratorAbstractUiExtension::getTitle() const
{
    Q_D(const CGxPluginGeneratorAbstractUiExtension);
    return d->title;
}

QString CGxPluginGeneratorAbstractUiExtension::getMessage() const
{
    Q_D(const CGxPluginGeneratorAbstractUiExtension);
    return d->message;
}

QIcon CGxPluginGeneratorAbstractUiExtension::getIcon() const
{
    Q_D(const CGxPluginGeneratorAbstractUiExtension);
    return d->icon;
}

void CGxPluginGeneratorAbstractUiExtension::setDescription(const QString& description)
{
    Q_D(CGxPluginGeneratorAbstractUiExtension);
    if (d->description != description)
    {
        d->description = description;
        emit descriptionChanged(description);
    }
}

void CGxPluginGeneratorAbstractUiExtension::setTitle(const QString& title)
{
    Q_D(CGxPluginGeneratorAbstractUiExtension);
    if (d->title != title)
    {
        d->title = title;
        emit titleChanged(title);
    }
}

void CGxPluginGeneratorAbstractUiExtension::setMessage(const QString& msg)
{
    Q_D(CGxPluginGeneratorAbstractUiExtension);
    if (d->message != msg)
    {
        d->message = msg;
        emit messageChanged(msg);
    }
}

void CGxPluginGeneratorAbstractUiExtension::setIcon(const QIcon& icon)
{
    Q_D(CGxPluginGeneratorAbstractUiExtension);
    d->icon = icon;
    emit iconChanged(icon);
}

