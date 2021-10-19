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


#include "GxPluginGeneratorAbstractExtension.h"

#include "GxPluginGeneratorCorePlugin_p.h"

#include <QVariant>

class CGxPluginGeneratorAbstractExtensionPrivate
{
public:

    CGxPluginGeneratorAbstractExtensionPrivate()
        : valid(true)
    {}

    bool valid;
    QString errorMessage;
    QHash<QString, QVariant> parameters;
};

CGxPluginGeneratorAbstractExtension::CGxPluginGeneratorAbstractExtension()
    : d_ptr(new CGxPluginGeneratorAbstractExtensionPrivate())
{
}

CGxPluginGeneratorAbstractExtension::~CGxPluginGeneratorAbstractExtension()
{
}

void CGxPluginGeneratorAbstractExtension::setParameter(const QHash<QString, QVariant>& params)
{
    Q_D(CGxPluginGeneratorAbstractExtension);
    d->parameters = params;
}

void CGxPluginGeneratorAbstractExtension::setParameter(const QString& name, const QVariant& value)
{
    Q_D(CGxPluginGeneratorAbstractExtension);
    d->parameters[name] = value;
}

QHash<QString, QVariant> CGxPluginGeneratorAbstractExtension::getParameter() const
{
    Q_D(const CGxPluginGeneratorAbstractExtension);
    return d->parameters;
}

bool CGxPluginGeneratorAbstractExtension::isValid() const
{
    Q_D(const CGxPluginGeneratorAbstractExtension);
    return d->valid;
}

void CGxPluginGeneratorAbstractExtension::validate()
{
    Q_D(CGxPluginGeneratorAbstractExtension);
    d->valid = verifyParameters(d->parameters);
}

void CGxPluginGeneratorAbstractExtension::updateCodeModel()
{
    Q_D(CGxPluginGeneratorAbstractExtension);
    this->updateCodeModel(d->parameters);
}

QString CGxPluginGeneratorAbstractExtension::getErrorMessage() const
{
    Q_D(const CGxPluginGeneratorAbstractExtension);
    return d->errorMessage;
}

CGxPluginGeneratorCodeModel* CGxPluginGeneratorAbstractExtension::getCodeModel() const
{
    return CGxPluginGeneratorCorePlugin::getInstance()->getCodeModel();
}

void CGxPluginGeneratorAbstractExtension::setErrorMessage(const QString& errorMsg)
{
    Q_D(CGxPluginGeneratorAbstractExtension);
    if (d->errorMessage != errorMsg)
    {
        d->errorMessage = errorMsg;
        emit errorMessageChanged(errorMsg);
    }
}
