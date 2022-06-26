
#pragma once

#include <QMainWindow>

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
        void processMegatuneKeyValue(const QString key, const QString value);
};