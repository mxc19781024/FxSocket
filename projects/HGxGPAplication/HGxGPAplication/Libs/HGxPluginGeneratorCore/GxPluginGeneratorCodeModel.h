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


#ifndef GXPLUGINGENERATORCODEMODEL_H
#define GXPLUGINGENERATORCODEMODEL_H

#include <QObject>
#include <QScopedPointer>

#include "GxPluginGeneratorAbstractTemplate.h"

class CGxPluginGeneratorCodeModelPrivate;

class HGX_PLUGINGENERATOR_CORE_EXPORT CGxPluginGeneratorCodeModel : public QObject
{
  Q_OBJECT

public:

  CGxPluginGeneratorCodeModel();

  virtual ~CGxPluginGeneratorCodeModel();

  void addTemplate(CGxPluginGeneratorAbstractTemplate* templ, const QString& path = "");

  CGxPluginGeneratorAbstractTemplate* getTemplate(const QString& path) const;

  void addContent(const QString& marker, const QString& content);
  QString getContent(const QString& marker) const;

  void setExportMacro(const QString& exportMacro);
  QString getExportMacro() const;

  void setExportMacroInclude(const QString& exportMacroInclude);
  QString getExportMacroInclude() const;

  void setSymbolicName(const QString& symbolicName);
  QString getSymbolicName(bool withPeriods = false) const;

  void setLicense(const QString& license);
  QString getLicense() const;

  void create(const QString& location);

  void reset();

private:

    Q_DECLARE_PRIVATE(CGxPluginGeneratorCodeModel)

    const QScopedPointer<CGxPluginGeneratorCodeModelPrivate> d_ptr;


};

Q_DECLARE_INTERFACE(CGxPluginGeneratorCodeModel, "com.henggetec.generator.core.codeModel");


#endif // GXPLUGINGENERATORCODEMODEL_H
