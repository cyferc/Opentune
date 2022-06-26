
#pragma once

#include <QMainWindow>

struct tMTuneSection
{
    float MTversion;
    QChar queryCommand;
    QString signature;
    QString versionInfo;
};

struct tTStudioSection
{
    float iniSpecVersion;
};

struct tSettingsGroupsSection
{
    struct tGroup
    {
        struct tOption
        {
            QString referenceName;
            QString displayName;
        };

        QString referenceName;
        QString displayName;
        std::vector<tOption> options;
    };

    std::vector<tGroup> groups;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *pParent = nullptr);

    private slots:
        void openIniFile();

    private:
        void parseIniFile(const QString filePathAndName);
        bool extractKey(QString &key, QByteArray line);
        bool extractValue(QString &value, QByteArray line);
        QString removeQuotes(QString input);
        
        void processMegatuneKeyValue(const QString key, const QString value);
        void processTStudioKeyValue(const QString key, const QString value);
        void processSettingGroupsKeyValue(const QString key, const QString value);

        tMTuneSection mTuneSection;
        tTStudioSection studioSection;
        tSettingsGroupsSection settingsGroupsSection;
};
