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


#ifndef GXPLUGINGENERATORABSTRACTTEMPLATE_H
#define GXPLUGINGENERATORABSTRACTTEMPLATE_H

#include <QObject>
#include <QScopedPointer>
#include <QStringList>

#include <GxPluginGeneratorCoreExport.h>

class CGxPluginGeneratorCodeModel;
class CGxPluginGeneratorAbstractTemplatePrivate;

class HGX_PLUGINGENERATOR_CORE_EXPORT CGxPluginGeneratorAbstractTemplate : public QObject
{
  Q_OBJECT

public:

  enum Position {
    PREPEND,
    APPEND,
    REPLACE
  };

  CGxPluginGeneratorAbstractTemplate(const QString& name, CGxPluginGeneratorAbstractTemplate* parent = 0);

  virtual ~CGxPluginGeneratorAbstractTemplate();

  void setFilename(const QString& filename);
  QString getFilename() const;

  void addContent(const QString& marker, const QString& content, Position pos = APPEND);

  QStringList getContent(const QString& marker) const;

  virtual void create(const QString& location);

  void reset();

  virtual QStringList getMarkers() const;

  virtual QString generateContent() = 0;

protected:

  QString getSymbolicName(bool withPeriods = false) const;

private:

  friend class CGxPluginGeneratorCodeModel;

  Q_DECLARE_PRIVATE(CGxPluginGeneratorAbstractTemplate)

  void setCodeModel(CGxPluginGeneratorCodeModel* codeModel);

  const QScopedPointer<CGxPluginGeneratorAbstractTemplatePrivate> d_ptr;
};

#endif // GXPLUGINGENERATORABSTRACTTEMPLATE_H
