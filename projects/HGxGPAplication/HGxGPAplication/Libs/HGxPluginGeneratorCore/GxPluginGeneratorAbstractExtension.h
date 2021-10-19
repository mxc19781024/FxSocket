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


#ifndef GXPLUGINGENERATORABSTRACTEXTENSION_H
#define GXPLUGINGENERATORABSTRACTEXTENSION_H

#include <QObject>
#include <QHash>

#include <GxPluginGeneratorCoreExport.h>

class CGxPluginGeneratorAbstractExtensionPrivate;
class CGxPluginGeneratorCodeModel;

class HGX_PLUGINGENERATOR_CORE_EXPORT CGxPluginGeneratorAbstractExtension : public QObject
{
  Q_OBJECT

public:
    CGxPluginGeneratorAbstractExtension();
    virtual ~CGxPluginGeneratorAbstractExtension();

    //virtual void getCommandLineArgs() const = 0;

    void setParameter(const QHash<QString, QVariant>& params);
    void setParameter(const QString& name, const QVariant& value);
    QHash<QString, QVariant> getParameter() const;

    bool isValid() const;
    void validate();
    void updateCodeModel();

    QString getErrorMessage() const;

    CGxPluginGeneratorCodeModel* getCodeModel() const;

Q_SIGNALS:

    void errorMessageChanged(const QString&);

protected:

    void setErrorMessage(const QString& errMsg);

    virtual bool verifyParameters(const QHash<QString, QVariant>& params) = 0;
    virtual void updateCodeModel(const QHash<QString, QVariant>& params) = 0;

private:

    Q_DECLARE_PRIVATE(CGxPluginGeneratorAbstractExtension)

    const QScopedPointer<CGxPluginGeneratorAbstractExtensionPrivate> d_ptr;

};

Q_DECLARE_INTERFACE(CGxPluginGeneratorAbstractExtension, "com.henggetec.generator.core.extension");

#endif // GXPLUGINGENERATORABSTRACTEXTENSION_H
