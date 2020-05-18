#ifndef ACCELGYROCHART_H
#define ACCELGYROCHART_H
#include <QtCharts>

class AccelGyroChart
{ 
private:
    QLineSeries *Series[3];
    int Count;
    int Limit;
    bool Enabled;
    QChart *Chart;
    QPen Pen[3];
    QRgb Color[3] = {(0xFF3333), (0x33FF33), (0x3333FF)};
    QValueAxis *XAxis;
    QValueAxis *YAxis;
    QChartView *ChartView;
    double Min, Max;
public:
    AccelGyroChart();
    QChart* Init(int limit, double min, double max);
    // False gdy przepelnilismy wykres
    // True w pzreciwnym razie
    bool Append(double data[], int start);
    void Stop();
    void Start();

};

#endif // ACCELGYROCHART_H
