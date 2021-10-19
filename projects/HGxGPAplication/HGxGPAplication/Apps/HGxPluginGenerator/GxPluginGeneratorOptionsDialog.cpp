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


#include "GxPluginGeneratorOptionsDialog_p.h"
#include "ui_GxPluginGeneratorOptionsDialog.h"

#include <GxPluginGeneratorConstants.h>

#include <QSettings>

CGxPluginGeneratorOptionsDialog::CGxPluginGeneratorOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGxPluginGeneratorOptionsDialog)
{
    ui->setupUi(this);

    QSettings settings;
    ui->licenseEdit->setPlainText(settings.value(
        CGxPluginGeneratorConstants::PLUGIN_LICENSE_MARKER).toString());
}

CGxPluginGeneratorOptionsDialog::~CGxPluginGeneratorOptionsDialog()
{
    delete ui;
}

void CGxPluginGeneratorOptionsDialog::accept()
{
    QSettings settings;
    settings.setValue(CGxPluginGeneratorConstants::PLUGIN_LICENSE_MARKER,
        ui->licenseEdit->toPlainText());

    QDialog::accept();
}
