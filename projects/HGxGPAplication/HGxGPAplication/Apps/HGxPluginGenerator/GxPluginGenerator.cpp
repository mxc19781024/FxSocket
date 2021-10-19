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

#include "GxPluginGenerator_p.h"
#include "ui_GxPluginGeneratorMainWindow.h"

#include <ctkPluginFramework.h>
#include <ctkPluginContext.h>
#include <ctkServiceReference.h>
#include <ctkPluginConstants.h>
#include <GxPluginGeneratorCodeModel.h>
#include <GxPluginGeneratorConstants.h>
#include <GxPluginGeneratorOptionsDialog_p.h>
#include <ctkUtils.h>

#include <QDebug>
#include <QListWidgetItem>
#include <QDir>
#include <QFileSystemModel>
#include <QTime>
#include <QMessageBox>
#include <QSettings>

#include <stdexcept>


#include "GxPluginGeneratorMainExtension.h"
#include "GxPluginGeneratorCodeModel.h"
#include "GxPluginGeneratorCorePlugin_p.h"
#include "GxGPAppConfig.h"

class CGxTemporaryDir
{
public:

    static QString create(const QString& path = QString())
    {
        QString tmpPath = path;
        if (tmpPath.isEmpty())
        {
            tmpPath = "CGxPluginGenerator";
        }
        tmpPath += "." + QTime::currentTime().toString("hhmmsszzz");

        QDir tmp = QDir::temp();
        if (!tmp.mkdir(tmpPath))
        {
            QString msg = QString("Creating temporary directory ") + tmpPath + " in "
                + QDir::temp().canonicalPath() + " failed.";
            throw std::runtime_error(msg.toStdString());
        }

        tmp.cd(tmpPath);

        return tmp.canonicalPath();
    }
};

CGxPluginGenerator::CGxPluginGenerator(ctkPluginFramework* framework, QWidget *parent) :
    QMainWindow(parent),
    framework(framework), ui(new Ui::CGxPluginGeneratorMainWindow),
    mode(EDIT), previewModel(0)
    , m_pCodeModel(nullptr)
    , m_pMainExtension(nullptr)
{
    ui->setupUi(this);

    previewModel = new QFileSystemModel(this);
    ui->previewTreeView->setModel(previewModel);
    ui->previewTreeView->hideColumn(1);
    ui->previewTreeView->hideColumn(2);
    ui->previewTreeView->hideColumn(3);

    this->setStatusBar(0);

    connect(ui->actionOptions, SIGNAL(triggered(bool)), this, SLOT(menuOptionsTriggered()));
    connect(ui->generateButton, SIGNAL(clicked()), this, SLOT(generateClicked()));
    connect(ui->previewButton, SIGNAL(clicked()), this, SLOT(previewClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(ui->uiExtensionList, SIGNAL(itemClicked(QListWidgetItem*)),
        this, SLOT(extensionItemClicked(QListWidgetItem*)));
    connect(ui->previewTreeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(previewIndexChanged(QModelIndex)));

    if (!m_pMainExtension)
    {
        m_pMainExtension = new CGxPluginGeneratorMainExtension;

        ui->extensionStack->addWidget(m_pMainExtension->getWidget());
        connect(m_pMainExtension, SIGNAL(errorMessageChanged(QString)), this, SLOT(errorMessageChanged(QString)));

        m_pMainExtension->validate();

        ui->uiExtensionList->setCurrentRow(0);
        extensionClicked(m_pMainExtension);
    }


    ui->outputDirButton->setDirectory(HGX_PLUGINS_SRC_DIR);

}

CGxPluginGenerator::~CGxPluginGenerator()
{
    delete ui;
    if (!previewDir.isEmpty())
    {
        ctk::removeDirRecursively(previewDir);
    }
}

void CGxPluginGenerator::menuOptionsTriggered()
{
    CGxPluginGeneratorOptionsDialog optionsDialog;
    int result = optionsDialog.exec();
    if (result == QDialog::Accepted && mode == PREVIEW)
    {
        QString selPath;
        QString oldPreviewDir = previewDir;
        if (!ui->previewTreeView->selectionModel()->selection().isEmpty())
        {
            QModelIndex index = ui->previewTreeView->selectionModel()->selectedIndexes().front();
            selPath = previewModel->data(index, QFileSystemModel::FilePathRole).toString();
        }
        if (createPreview())
        {
            ui->modeStack->setCurrentWidget(ui->previewPage);
            ui->previewButton->setText(tr("<< Back"));
            ui->previewTreeView->expandAll();
            if (!selPath.isEmpty())
            {
                selPath.replace(oldPreviewDir, previewDir);
                QModelIndex index = previewModel->index(selPath);
                ui->previewTreeView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
                previewIndexChanged(index);
            }
        }
    }
}

void CGxPluginGenerator::previewClicked()
{
    if (mode == EDIT)
    {
        if (createPreview())
        {
            ui->modeStack->setCurrentWidget(ui->previewPage);
            ui->previewButton->setText(tr("<< Back"));
            ui->previewTreeView->expandAll();
            if (!ui->previewTreeView->selectionModel()->selection().isEmpty())
            {
                previewIndexChanged(ui->previewTreeView->selectionModel()->selectedIndexes().front());
            }
            mode = PREVIEW;
        }
    }
    else
    {
        ui->modeStack->setCurrentWidget(ui->editPage);
        ui->previewButton->setText(tr("Preview >>"));
        mode = EDIT;

        ctk::removeDirRecursively(previewDir);
        previewDir.clear();
    }
}

void CGxPluginGenerator::generateClicked()
{
    try
    {
        createPlugin(ui->outputDirButton->directory());

        QMessageBox msgBox;
        msgBox.setText(tr("Successfully create plugin"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
    catch (const std::runtime_error& error)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Creating the plugin failed."));
        msgBox.setInformativeText(QString::fromLatin1(error.what()));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

QString CGxPluginGenerator::createPlugin(const QString& path)
{

    /*  ctkServiceReference codeModelRef = framework->getPluginContext()->
                                          getServiceReference("CGxPluginGeneratorCodeModel");

      CGxPluginGeneratorCodeModel* codeModel = qobject_cast<CGxPluginGeneratorCodeModel*>(framework->getPluginContext()->getService(codeModelRef));

      if (!codeModel)
      {
          return "";
      }
      codeModel->reset();



      // set global code model info from QSettings object
      QSettings settings;
      codeModel->setLicense(settings.value(CGxPluginGeneratorConstants::PLUGIN_LICENSE_MARKER).toString());

      foreach(CGxPluginGeneratorAbstractUiExtension* extension, idToExtensionMap)
      {
        extension->updateCodeModel();
      }

      QString pluginDir = path + "/" + codeModel->getSymbolicName(true);
      if (!QDir(path).mkdir(codeModel->getSymbolicName(true)))
      {
        QString msg(tr("Creating directory \"%1\" failed.").arg(pluginDir));
        throw std::runtime_error(msg.toStdString());
      }

      codeModel->create(pluginDir);
      return pluginDir;*/




    if (!m_pMainExtension)
    {
        return "";
    }
    m_pMainExtension->updateCodeModel();

    CGxPluginGeneratorCodeModel *codeModel = CGxPluginGeneratorCorePlugin::getInstance()->getCodeModel();
    QString pluginDir = path + "/" + codeModel->getSymbolicName(true);
    if (!QDir(path).mkdir(codeModel->getSymbolicName(true)))
    {
        QString msg(tr("Creating directory \"%1\" failed.").arg(pluginDir));
        throw std::runtime_error(msg.toStdString());
    }

    codeModel->create(pluginDir);
    return pluginDir;

}

void CGxPluginGenerator::previewIndexChanged(const QModelIndex& index)
{
    QString filePath = previewModel->data(index, QFileSystemModel::FilePathRole).toString();
    ui->previewTextLabel->setText(QDir(QString(filePath).replace(previewDir, ui->outputDirButton->directory())).absolutePath());

    QFile file(filePath);
    file.open(QFile::ReadOnly);
    QTextStream textStream(&file);
    ui->previewTextEdit->setText(textStream.readAll());
}

bool CGxPluginGenerator::createPreview()
{
    try
    {
        previewDir = CGxTemporaryDir::create();

        QString tmpPluginDir = createPlugin(previewDir);

        previewModel->setRootPath(tmpPluginDir);
        ui->previewTreeView->setRootIndex(previewModel->index(previewDir));
    }
    catch (const std::runtime_error& error)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Creating the preview failed."));
        msgBox.setInformativeText(QString::fromLatin1(error.what()));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    return true;
}

void CGxPluginGenerator::extensionItemClicked(QListWidgetItem* item)
{
    CGxPluginGeneratorAbstractUiExtension* extension = idToExtensionMap[item->data(Qt::UserRole).toInt()];
    extensionClicked(extension);
}

void CGxPluginGenerator::extensionClicked(CGxPluginGeneratorAbstractUiExtension* extension)
{
    if (!extension)
    {
        return;
    }
    ui->extensionStack->setCurrentWidget(extension->getWidget());
    ui->extensionMsgLabel->setText(extension->getTitle());
    this->errorMessageChanged(extension->getErrorMessage());
}

void CGxPluginGenerator::errorMessageChanged(const QString& errMsg)
{
    ui->extensionErrMsgLabel->setText(errMsg);

    bool enableButtons = false;
    if (errMsg.isEmpty())
    {
        enableButtons = true;
    }

    ui->previewButton->setEnabled(enableButtons);
    ui->generateButton->setEnabled(enableButtons);
}
