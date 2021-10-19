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


#include "GxPluginGeneratorMainExtension.h"

#include <GxPluginGeneratorCodeModel.h>
#include <GxPluginGeneratorConstants.h>
#include <GxPluginGeneratorCMakeLists.h>
#include <GxPluginGeneratorHeaderTemplate.h>
#include <GxPluginGeneratorCppPluginActivator.h>
#include <GxPluginGeneratorTargetLibraries.h>

CGxPluginGeneratorMainExtension::CGxPluginGeneratorMainExtension()
    : ui(0)
{
    this->setTitle(tr("Main"));
    this->setDescription("The main parameters for a new plugin");
}

QWidget* CGxPluginGeneratorMainExtension::createWidget()
{
    ui = new Ui::CGxPluginGeneratorMainExtension();
    QWidget* container = new QWidget();
    ui->setupUi(container);

    connectSignals();

    setTitle(tr("Main"));

    return container;
}

void CGxPluginGeneratorMainExtension::connectSignals()
{
    connect(ui->symbolicNameEdit, SIGNAL(textChanged(QString)), this, SLOT(symbolicNameChanged()));
    connect(ui->activatorClassEdit, SIGNAL(textChanged(QString)), this, SLOT(activatorClassChanged()));
    connect(ui->symbolicNameEdit, SIGNAL(textChanged(QString)), this, SLOT(updateParameters()));
    connect(ui->exportDirectiveEdit, SIGNAL(textChanged(QString)), this, SLOT(updateParameters()));
    connect(ui->activatorClassEdit, SIGNAL(textChanged(QString)), this, SLOT(updateParameters()));
    connect(ui->activatorHeaderEdit, SIGNAL(textChanged(QString)), this, SLOT(updateParameters()));
    connect(ui->activatorSourceEdit, SIGNAL(textChanged(QString)), this, SLOT(updateParameters()));
}

void CGxPluginGeneratorMainExtension::symbolicNameChanged()
{
    QString symbolicName = ui->symbolicNameEdit->text().replace(".", "_");
    ui->exportDirectiveEdit->setText(symbolicName + "_EXPORT");

    QString activatorClassName;
    QStringList tokens = symbolicName.split('_');

    if (tokens.size() > 1)
    {
        tokens.pop_front();
        activatorClassName += tokens.takeFirst();
        foreach(QString token, tokens)
        {
            activatorClassName += token.left(1).toUpper() + token.mid(1);
        }
    }

    activatorClassName += "Plugin";

    ui->activatorClassEdit->setText(activatorClassName);
    ui->activatorHeaderEdit->setText(activatorClassName + "_p.h");
    ui->activatorSourceEdit->setText(activatorClassName + ".cpp");
}

void CGxPluginGeneratorMainExtension::activatorClassChanged()
{
    QString activatorClassName = ui->activatorClassEdit->text();
    ui->activatorHeaderEdit->setText(activatorClassName + "_p.h");
    ui->activatorSourceEdit->setText(activatorClassName + ".cpp");

    ui->advancedButton->setText(tr("Advanced (activator class: %1)").arg(activatorClassName));
}

bool CGxPluginGeneratorMainExtension::verifyParameters(
    const QHash<QString, QVariant>& params)
{
    if (params["symbolic-name"].toString().isEmpty())
    {
        this->setErrorMessage(tr("The symbolic name cannot be empty, Commonality guy must be set the plugin symbolic name like \"com_henggetec_XX\""));
        return false;
    }

    if (params["export-directive"].toString().isEmpty())
    {
        this->setErrorMessage(tr("The export directive cannot be empty"));
        return false;
    }

    if (params["activator-classname"].toString().isEmpty())
    {
        this->setErrorMessage(tr("The activator class name cannot be empty"));
        return false;
    }

    if (params["activator-headerfile"].toString().isEmpty())
    {
        this->setErrorMessage(tr("The activator header filename cannot be empty"));
        return false;
    }

    if (params["activator-sourcefile"].toString().isEmpty())
    {
        this->setErrorMessage(tr("The activator source filename cannot be empty"));
        return false;
    }

    this->setErrorMessage("");
    return true;
}

void CGxPluginGeneratorMainExtension::updateCodeModel(const QHash<QString, QVariant>& params)
{
    CGxPluginGeneratorCodeModel* codeModel = this->getCodeModel();

    codeModel->setSymbolicName(params["symbolic-name"].toString());
    codeModel->setExportMacroInclude(QString("#include <") + codeModel->getSymbolicName() + "_Export.h>");
    codeModel->setExportMacro(params["export-directive"].toString());

    // Add CMakeLists.txt template
    codeModel->addTemplate(new CGxPluginGeneratorCMakeLists());

    CGxPluginGeneratorAbstractTemplate* cmakelistsTemplate = codeModel->getTemplate(CGxPluginGeneratorConstants::TEMPLATE_CMAKELISTS_TXT);

    cmakelistsTemplate->addContent(CGxPluginGeneratorCMakeLists::PLUGIN_PROJECT_NAME_MARKER,
        codeModel->getSymbolicName(),
        CGxPluginGeneratorAbstractTemplate::REPLACE);
    cmakelistsTemplate->addContent(CGxPluginGeneratorConstants::PLUGIN_EXPORTMACRO_MARKER,
        codeModel->getExportMacro(),
        CGxPluginGeneratorAbstractTemplate::REPLACE);

    // Add <plugin-activator>.h template
    QString activatorClassName = params["activator-classname"].toString();
    CGxPluginGeneratorAbstractTemplate* activatorHeaderTemplate =
        new CGxPluginGeneratorHeaderTemplate(CGxPluginGeneratorConstants::TEMPLATE_PLUGINACTIVATOR_H);
    activatorHeaderTemplate->addContent(CGxPluginGeneratorConstants::PLUGIN_LICENSE_MARKER, codeModel->getLicense());
    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_INCLUDES_MARKER, "#include <ctkPluginActivator.h>");
    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_CLASSNAME_MARKER, activatorClassName);
    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_SUPERCLASSES_MARKER, "public QObject, public ctkPluginActivator");
    //activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_DEFAULT_ACCESS_MARKER, "Q_OBJECT\nQ_INTERFACES(ctkPluginActivator)");
    QString exportPlugin = QString("Q_PLUGIN_METADATA(IID \"%1\")").arg(codeModel->getSymbolicName());
    QString defaultAccessMark = QString("Q_OBJECT\nQ_INTERFACES(ctkPluginActivator)\n#if QT_VERSION >= 0x50000\n%1\n#endif").arg(exportPlugin);
    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_DEFAULT_ACCESS_MARKER, defaultAccessMark);

    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_PUBLIC_MARKER, activatorClassName + "();\n~" + activatorClassName + "();");
    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_PUBLIC_MARKER, "void start(ctkPluginContext* context);\nvoid stop(ctkPluginContext* context);");
    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_PUBLIC_MARKER, QString("static ") + activatorClassName + "* getInstance();");
    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_PUBLIC_MARKER, "ctkPluginContext* getPluginContext() const;");
    activatorHeaderTemplate->addContent(CGxPluginGeneratorHeaderTemplate::H_PRIVATE_MARKER, QString("static ") + activatorClassName + "* instance;\nctkPluginContext* context;");
    activatorHeaderTemplate->setFilename(params["activator-headerfile"].toString());

    codeModel->addTemplate(activatorHeaderTemplate);

    // Add <plugin-activator>.cpp template
    CGxPluginGeneratorAbstractTemplate* activatorCppTemplate =
        new CGxPluginGeneratorCppPluginActivator();
    activatorCppTemplate->addContent(CGxPluginGeneratorConstants::PLUGIN_LICENSE_MARKER, codeModel->getLicense());
    activatorCppTemplate->addContent(CGxPluginGeneratorCppTemplate::CPP_CLASSNAME_MARKER, activatorClassName);
    activatorCppTemplate->addContent(CGxPluginGeneratorCppTemplate::CPP_INCLUDES_MARKER, QString("#include \"") + activatorHeaderTemplate->getFilename() + "\"");
    activatorCppTemplate->addContent(CGxPluginGeneratorCppTemplate::CPP_GLOBAL_MARKER, activatorClassName + "* " + activatorClassName + "::instance = 0;");
    activatorCppTemplate->addContent(CGxPluginGeneratorCppTemplate::CPP_CONSTRUCTOR_INITLIST_MARKER, "context(0)");
    activatorCppTemplate->addContent(CGxPluginGeneratorCppTemplate::CPP_DESTRUCTOR_BODY_MARKER, "");
    activatorCppTemplate->addContent(CGxPluginGeneratorCppPluginActivator::PLUGINACTIVATOR_START_MARKER, "instance = this;\nthis->context = context;");
    activatorCppTemplate->addContent(CGxPluginGeneratorCppTemplate::CPP_METHODS_MARKER, activatorClassName + "* " + activatorClassName + "::getInstance()\n{\n  return instance;\n}");
    activatorCppTemplate->addContent(CGxPluginGeneratorCppTemplate::CPP_METHODS_MARKER, QString("ctkPluginContext* ") + activatorClassName + "::getPluginContext() const\n{\n  return context;\n}");
    activatorCppTemplate->setFilename(params["activator-sourcefile"].toString());
    codeModel->addTemplate(activatorCppTemplate);

    // Add target_libraries.cmake template
    // TODO only add the CTKPluginFramework library if there are no plugin dependencies
    CGxPluginGeneratorAbstractTemplate* targetLibrariesTemplate =
        new CGxPluginGeneratorTargetLibraries();
    targetLibrariesTemplate->addContent(CGxPluginGeneratorTargetLibraries::TARGETLIBRARIES_MARKER, "CTKPluginFramework");
    codeModel->addTemplate(targetLibrariesTemplate);

    // add project files to CMakeLists.txt
    cmakelistsTemplate->addContent(CGxPluginGeneratorCMakeLists::PLUGIN_SRCS_MARKER, activatorCppTemplate->getFilename());
    cmakelistsTemplate->addContent(CGxPluginGeneratorCMakeLists::PLUGIN_MOC_SRCS_MARKER, activatorHeaderTemplate->getFilename());
}

void CGxPluginGeneratorMainExtension::updateParameters()
{
    this->setParameter("symbolic-name", ui->symbolicNameEdit->text());
    this->setParameter("plugin-name", ui->nameEdit->text());
    this->setParameter("plugin-version", ui->versionEdit->text());
    this->setParameter("export-directive", ui->exportDirectiveEdit->text());
    this->setParameter("activator-classname", ui->activatorClassEdit->text());
    this->setParameter("activator-headerfile", ui->activatorHeaderEdit->text());
    this->setParameter("activator-sourcefile", ui->activatorSourceEdit->text());

    this->validate();
}

