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

#ifndef GXPLUGINGENERATOR_H
#define GXPLUGINGENERATOR_H

#include <QMainWindow>
#include <QMultiMap>

#include <GxPluginGeneratorAbstractUiExtension.h>

class ctkPluginFramework;

class QListWidgetItem;
class QFileSystemModel;

#include <QModelIndex>

namespace Ui {
    class CGxPluginGeneratorMainWindow;
}

class CGxPluginGeneratorAbstractUiExtension;
class CGxPluginGeneratorCodeModel;

class CGxPluginGenerator : public QMainWindow
{
    Q_OBJECT

public:
    explicit CGxPluginGenerator(ctkPluginFramework* framework, QWidget *parent = 0);
    ~CGxPluginGenerator();

    protected Q_SLOTS:

    void previewClicked();
    void generateClicked();
    void previewIndexChanged(const QModelIndex& index);
    void extensionItemClicked(QListWidgetItem* item);
    void errorMessageChanged(const QString& errMsg);

    void menuOptionsTriggered();

private:

    void extensionClicked(CGxPluginGeneratorAbstractUiExtension* extension);

    QString createPlugin(const QString& path);

    bool createPreview();

    enum GeneratorMode {
        EDIT,
        PREVIEW
    };

    ctkPluginFramework *const framework;
    Ui::CGxPluginGeneratorMainWindow *ui;
    GeneratorMode mode;

    QFileSystemModel* previewModel;

    QString previewDir;

    QMultiMap<int, CGxPluginGeneratorAbstractUiExtension*> uiExtensionMap;
    QHash<int, CGxPluginGeneratorAbstractUiExtension*> idToExtensionMap;


    CGxPluginGeneratorAbstractUiExtension			*m_pMainExtension;

    CGxPluginGeneratorCodeModel						*m_pCodeModel;
};

#endif // GXPLUGINGENERATOR_H
