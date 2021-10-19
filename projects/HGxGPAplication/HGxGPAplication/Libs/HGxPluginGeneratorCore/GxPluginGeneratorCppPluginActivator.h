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


#ifndef GXPLUGINGENERATORCPPPLUGINACTIVATOR_H
#define GXPLUGINGENERATORCPPPLUGINACTIVATOR_H

#include "GxPluginGeneratorCppTemplate.h"

class HGX_PLUGINGENERATOR_CORE_EXPORT CGxPluginGeneratorCppPluginActivator
  : public CGxPluginGeneratorCppTemplate
{

public:

  static const QString PLUGINACTIVATOR_START_MARKER;
  static const QString PLUGINACTIVATOR_STOP_MARKER;

  CGxPluginGeneratorCppPluginActivator(CGxPluginGeneratorAbstractTemplate* parent = 0);

  QString generateContent();

  QStringList getMarkers() const;

};

#endif // GXPLUGINGENERATORCPPPLUGINACTIVATOR_H
