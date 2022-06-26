
#include "MainWindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include <QDebug>

namespace
{
    const QString cWindowTitle = "Opentune";

    const QString cSection_Megatune = "MegaTune";
    const QString cSection_TStudio = "TunerStudio";
    const QString cSection_SettingGroups = "SettingGroups";
    const QString cSection_PcVariables = "PcVariables";
    const QString cSection_Constants = "Constants";
    const QString cSection_EventTriggers = "EventTriggers";
    const QString cSection_ConstantsExtensions = "ConstantsExtensions";
    const QString cSection_Menu = "Menu";
    const QString cSection_SettingContextHelp = "SettingContextHelp";
    const QString cSection_UserDefined = "UserDefined";
    const QString cSection_ControllerCommands = "ControllerCommands";
    const QString cSection_CurveEditor = "CurveEditor";
    const QString cSection_TableEditor = "TableEditor";
    const QString cSection_GaugeConfigurations = "GaugeConfigurations";
    const QString cSection_FrontPage = "FrontPage";
    const QString cSection_OutputChannels = "OutputChannels";
    const QString cSection_Datalog = "Datalog";
    const QString cSection_LoggerDefinition = "LoggerDefinition";
    const QString cSection_ReferenceTables = "ReferenceTables";
    const QString cSection_Tools = "Tools";
    const QString cSection_VeAnalyze = "VeAnalyze";
    const QString cSection_WueAnalyze = "WueAnalyze";

    const QString cQuote('"');
} // namespace


MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent)
{
    QMenuBar *pMenuBar = new QMenuBar(this);

    QMenu *pMenuFile = new QMenu(pMenuBar);
    pMenuFile->setTitle(tr("File"));

    QMenu *pMenuHelp = new QMenu(pMenuBar);
    pMenuHelp->setTitle(tr("Help"));

    pMenuBar->addAction(pMenuFile->menuAction());
    pMenuBar->addAction(pMenuHelp->menuAction());

    QAction *pActionOpen = new QAction(this);
    pActionOpen->setText(tr("Open"));
    connect(pActionOpen, &QAction::triggered, this, &MainWindow::openIniFile);

    QAction *pActionExit = new QAction(this);
    pActionExit->setText(tr("Exit"));
    connect(pActionExit, &QAction::triggered, this, [this] () {
        close();
    });

    pMenuFile->addAction(pActionOpen);
    pMenuFile->addSeparator();
    pMenuFile->addAction(pActionExit);

    setMenuBar(pMenuBar);
}

void MainWindow::openIniFile()
{
    QString filePathAndName = QFileDialog::getOpenFileName(this, tr("Open INI File"),
                                                    "",
                                                    tr("INI files (*.ini)"));

    if (filePathAndName.isEmpty())
    {
        return;
    }

    QFile file(filePathAndName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "File is unreadable!", QMessageBox::Ok);
        return;
    }

    if (file.atEnd())
    {
        qDebug() << "ERROR: file '" + file.fileName() + "' is empty";
        return;
    }

    QString section;
    int lineNum = 1;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        line = line.trimmed(); // Remove start and end white spaces

        if (line.isEmpty() || line.startsWith(';') || line.startsWith('#'))
        {
            ++lineNum;
            continue;
        }

        if (line.startsWith('[') && line.endsWith(']'))
        {
            section = line;
            section.remove("[");
            section.remove("]");
            //qDebug() << "New section:" << section;

            ++lineNum;
            continue;
        }

        // Extract key from line
        QString key;
        bool keyExtracted = extractKey(key, line);

        if (!keyExtracted)
        {
            qDebug() << "ERROR: can't extract key from line " + QString::number(lineNum) + ": '" + line + "'";
            ++lineNum;
            continue;
        }

        // Extract value as string
        QString value;
        bool valueExtracted = extractValue(value, line);

        if (!valueExtracted)
        {
            qDebug() << "ERROR: can't extract value from line " + QString::number(lineNum) + ": '" + line + "'";
            ++lineNum;
            continue;
        }

        //qDebug() << "New key: '" + key + "', " + "value: '" + value + "'";

        if (section == cSection_Megatune)
        {
            processMegatuneKeyValue(key, value);
        }
        else if (section == cSection_TStudio)
        {
            processTStudioKeyValue(key, value);
        }
        else if (section == cSection_SettingGroups)
        {
            processSettingGroupsKeyValue(key, value);
        }
        else if (section == cSection_PcVariables) {}
        else if (section == cSection_Constants) {}
        else if (section == cSection_EventTriggers) {}
        else if (section == cSection_ConstantsExtensions) {}
        else if (section == cSection_Menu) {}
        else if (section == cSection_SettingContextHelp) {}
        else if (section == cSection_UserDefined) {}
        else if (section == cSection_ControllerCommands) {}
        else if (section == cSection_CurveEditor) {}
        else if (section == cSection_TableEditor) {}
        else if (section == cSection_GaugeConfigurations) {}
        else if (section == cSection_FrontPage) {}
        else if (section == cSection_OutputChannels) {}
        else if (section == cSection_Datalog) {}
        else if (section == cSection_LoggerDefinition) {}
        else if (section == cSection_ReferenceTables) {}
        else if (section == cSection_Tools) {}
        else if (section == cSection_VeAnalyze) {}
        else if (section == cSection_WueAnalyze) {}
        ++lineNum;
    }

    //QFileInfo fileInfo(file);
    //setWindowTitle(fileInfo.fileName() + " - " + cWindowTitle);
}

bool MainWindow::extractKey(QString &key, QByteArray line)
{
    int equalsIndex = line.indexOf('=');
    if (equalsIndex == -1)
    {
        return false;
    }

    line.remove(equalsIndex, line.length() - equalsIndex);
    key = line.trimmed();
    return true;
}

bool MainWindow::extractValue(QString &value, QByteArray line)
{
    // Remove any comment at the end
    int commentIndex = line.indexOf(';');
    if (commentIndex != -1)
    {
        line.remove(commentIndex, line.length() - commentIndex);
    }

    int equalsIndex = line.indexOf('=');
    if (equalsIndex == -1)
    {
        return false;
    }

    line.remove(0, equalsIndex + 1);
    value = line.trimmed();
    return true;
}

QString MainWindow::removeQuotes(QString input)
{
    input.remove(cQuote);
    return input;
}

void MainWindow::processMegatuneKeyValue(const QString key, const QString value)
{
    if (key == "MTversion")
    {
        mTuneSection.MTversion = value.toFloat();
        //qDebug() << "MTversion = '" + QString::number(mTuneSection.MTversion) + "'"; 
    }
    else if (key == "queryCommand")
    {
        mTuneSection.queryCommand = removeQuotes(value).front();
        //qDebug() << "queryCommand = '" + QString(mTuneSection.queryCommand) + "'"; 
    }
    else if (key == "signature")
    {
        mTuneSection.signature = removeQuotes(value);
        //qDebug() << "signature = '" + mTuneSection.signature + "'"; 
        setWindowTitle(mTuneSection.signature + " - " + cWindowTitle);
    }
    else if (key == "versionInfo")
    {
        mTuneSection.versionInfo = removeQuotes(value);
        //qDebug() << "versionInfo = '" + mTuneSection.versionInfo + "'"; 
    }
}

void MainWindow::processTStudioKeyValue(const QString key, const QString value)
{
    if (key == "iniSpecVersion")
    {
        studioSection.iniSpecVersion = value.toFloat();
        //qDebug() << "iniSpecVersion = '" + QString::number(mTuneSection.iniSpecVersion) + "'"; 
    }
}

void MainWindow::processSettingGroupsKeyValue(const QString key, const QString value)
{
    static QString currentGroupRefName;
    const QString tempCurrentGroupRefName = currentGroupRefName;
    const int cIndexReferenceName = 0;
    const int cIndexDisplayName = 1; 

    auto splitValue = value.split(',');
    if (splitValue.size() != 2)
    {
        qDebug() << "ERROR processSettingGroupsKeyValue";
        return;
    }

    splitValue[cIndexReferenceName] = splitValue[cIndexReferenceName].trimmed();
    splitValue[cIndexDisplayName] = splitValue[cIndexDisplayName].trimmed();
    splitValue[cIndexDisplayName] = removeQuotes(splitValue[1]);

    const QString cReferenceName = splitValue[cIndexReferenceName];
    const QString cDisplayName = splitValue[cIndexDisplayName];

    if (key == "settingGroup")
    {
        currentGroupRefName = splitValue[cIndexReferenceName];

        std::vector<tSettingsGroupsSection::tGroup>::iterator iter;
        iter = std::find_if(settingsGroupsSection.groups.begin(), settingsGroupsSection.groups.end(), [tempCurrentGroupRefName] (tSettingsGroupsSection::tGroup group) {
            return tempCurrentGroupRefName == group.referenceName;
        });

        if (iter != settingsGroupsSection.groups.end())
        {
            // Update group
            iter->displayName = cDisplayName;
        }
        else
        {
            // Add group
            tSettingsGroupsSection::tGroup group;
            group.referenceName = cReferenceName;
            group.displayName = cDisplayName;
            settingsGroupsSection.groups.emplace_back(group);
        }
    }
    else if (key == "settingOption")
    {
        std::vector<tSettingsGroupsSection::tGroup>::iterator groupIter;
        groupIter = std::find_if(settingsGroupsSection.groups.begin(), settingsGroupsSection.groups.end(), [tempCurrentGroupRefName] (tSettingsGroupsSection::tGroup group) {
            return tempCurrentGroupRefName == group.referenceName;
        });

        if (groupIter != settingsGroupsSection.groups.end())
        {
            // Update group
            std::vector<tSettingsGroupsSection::tGroup::tOption>::iterator optionIter;
            optionIter = std::find_if(groupIter->options.begin(), groupIter->options.end(), [cReferenceName] (tSettingsGroupsSection::tGroup::tOption group) {
                return cReferenceName == group.referenceName;
            });
        }
    }
}