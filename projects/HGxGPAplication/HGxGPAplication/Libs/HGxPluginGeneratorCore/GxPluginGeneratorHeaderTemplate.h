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


#ifndef GXPLUGINGENERATORHEADERTEMPLATE_H
#define GXPLUGINGENERATORHEADERTEMPLATE_H

#include "GxPluginGeneratorAbstractTemplate.h"

class HGX_PLUGINGENERATOR_CORE_EXPORT CGxPluginGeneratorHeaderTemplate : public CGxPluginGeneratorAbstractTemplate
{
public:

  static const QString H_CLASSNAME_MARKER;
  static const QString H_INCLUDES_MARKER;
  static const QString H_FORWARD_DECL_MARKER;
  static const QString H_FORWARD_DECL_NAMESPACE_MARKER;
  static const QString H_SUPERCLASSES_MARKER;
  static const QString H_DEFAULT_ACCESS_MARKER;
  static const QString H_PUBLIC_MARKER;
  static const QString H_PROTECTED_MARKER;
  static const QString H_PRIVATE_MARKER;

  CGxPluginGeneratorHeaderTemplate(const QString& name, CGxPluginGeneratorAbstractTemplate* parent = 0);

  QStringList getMarkers() const;

  QString generateContent();
};

#endif // GXPLUGINGENERATORHEADERTEMPLATE_H
