#include "mainwindow.h"
#include <QApplication>
#include <QThread>
#include <QJsonDocument>

struct Chart
{
    QVector<double> x;
    QVector<double> y;
};

struct ChartSeries
{
    QMap<QString, Chart> charts;
};

struct ChartGroup
{
    QMap<QString, ChartSeries> series;
};

int readJson(QMap<QString, ChartGroup> &out, QString json)
{
    out = {};
    QMap<QString, ChartGroup> tmpOut;
    QJsonDocument d = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject sett2 = d.object();
    QJsonValue value = sett2.value("Groups");
    if(!value.isArray())
    {
        qWarning() << "[JSON/ERROR] No top level object \"Groups\" found, probably json text is corrupt";
        return 1;
    }
    QJsonArray groups = value.toArray();
    for (auto val : groups)
    {
        if(!val.isObject())
        {
            qWarning() << "[JSON/ERROR] While parsing a group";
            return 1;
        }
        auto group = val.toObject();
        auto nameVal = group["Name"];
        auto seriesVal = group["Series"];
        if(!nameVal.isString())
        {
            qWarning() << "[JSON/ERROR] While parsing a group name";
            return 1;
        }
        auto groupName = nameVal.toString();
        if(!seriesVal.isArray())
        {
            qWarning() << "[JSON/ERROR] While parsing the group's:" << groupName << " series array";
            return 1;
        }
        auto series = seriesVal.toArray();
        ChartGroup cg;
        for (auto serval : series)
        {
            if(!serval.isObject())
            {
                qWarning() << "[JSON/ERROR] While parsing the group's:" << groupName << " series";
                return 1;
            }
            auto seq = serval.toObject();
            auto nameVal = seq["Name"];
            auto chartsVal = seq["Charts"];
            if(!nameVal.isString())
            {
                qWarning() << "[JSON/ERROR] While parsing a series name";
                return 1;
            }
            auto seriesName = nameVal.toString();
            if(!chartsVal.isArray())
            {
                qWarning() << "[JSON/ERROR] While parsing the series':" << seriesName << " chart array";
                return 1;
            }
            auto charts = chartsVal.toArray();
            ChartSeries cs;
            for (auto chartVal : charts)
            {
                if(!chartVal.isObject())
                {
                    qWarning() << "[JSON/ERROR] While parsing a chart object at " << groupName << "/" << seriesName << "";
                    return 1;
                }
                auto chart = chartVal.toObject();
                auto nameVal = chart["Name"];
                auto xVal = chart["x"];
                auto yVal = chart["y"];
                if(!nameVal.isString())
                {
                    qWarning() << "[JSON/ERROR] While parsing a chart name at " << groupName << "/" << seriesName << "";
                    return 1;
                }
                auto chartName = nameVal.toString();
                if(!xVal.isArray())
                {
                    qWarning() << "[JSON/ERROR] While parsing a charts x array at " << groupName << "/" << seriesName << "" << "/" << chartName << "";
                    return 1;
                }
                if(!yVal.isArray())
                {
                    qWarning() << "[JSON/ERROR] While parsing a charts y array at " << groupName << "/" << seriesName << "" << "/" << chartName << "";
                    return 1;
                }
                QVector<double> xArr;
                QVector<double> yArr;
                auto xRawArr = xVal.toArray();
                auto yRawArr = yVal.toArray();
                for (auto elem : xRawArr)
                {
                    if(!elem.isDouble())
                    {
                        qWarning() << "[JSON/ERROR] While parsing an x element at " << groupName << "/" << seriesName << "" << "/" << chartName << "";
                        return 1;
                    }
                    xArr.push_back(elem.toDouble());
                }
                for (auto elem : yRawArr)
                {
                    if(!elem.isDouble())
                    {
                        qWarning() << "[JSON/ERROR] While parsing an y element at " << groupName << "/" << seriesName << "" << "/" << chartName << "";
                        return 1;
                    }
                    yArr.push_back(elem.toDouble());
                }
                if(yArr.size() != xArr.size())
                {
                    qWarning() << "[JSON/ERROR] x and y array sizes differ at " << groupName << "/" << seriesName << "" << "/" << chartName << "";
                    return 1;
                }
                cs.charts.insert(chartName, {xArr, yArr});
            }
            cg.series.insert(seriesName, cs);
        }
        tmpOut.insert(groupName, cg);
    }
    out = std::move(tmpOut);
    return 0;
}

void printGroups(QMap<QString, ChartGroup> const &groups)
{
    for (auto const &groupName : groups.keys())
    {
        qInfo() << groupName;
        for (auto const &seriesName : groups[groupName].series.keys())
        {
            qInfo() << "" << seriesName;
            for (auto const &chartName : groups[groupName].series[seriesName].charts.keys())
            {
                qInfo() << " " << chartName;
                for (auto const &x : groups[groupName].series[seriesName].charts[chartName].x)
                {
                    qInfo() << "  " << x;
                }
                qInfo() << "   #";
                for (auto const &y : groups[groupName].series[seriesName].charts[chartName].y)
                {
                    qInfo() << "  " << y;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QString val;
    QFile file;
    file.setFileName("tests/json/test_0.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QMap<QString, ChartGroup> groups;
    readJson(groups, val);
    printGroups(groups);
    return a.exec();
}
