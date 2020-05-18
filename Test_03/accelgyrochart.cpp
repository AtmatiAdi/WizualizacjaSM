#include "accelgyrochart.h"

AccelGyroChart::AccelGyroChart()
{

}
QChart* AccelGyroChart::Init(int limit, double min, double max){
    Min = min;
    Max = max;
    Enabled = true;
    Count = 0;
    Limit = limit;
    Chart = new QChart();
    XAxis = new QValueAxis();
    XAxis->setRange(0, Limit);
    YAxis = new QValueAxis();
    YAxis->setRange(Min, Max);
    for (int a = 0; a < 3; a++){
        Series[a] = new QLineSeries();
        Chart->addSeries(Series[a]);
        Pen[a].setColor(Color[a]);
        Pen[a].setWidth(1);
        Series[a]->setPen(Pen[a]);
        Chart->setAxisX(XAxis, Series[a]);
        Chart->setAxisY(YAxis, Series[a]);
    }
    Chart->legend()->hide();
    Chart->createDefaultAxes();
    Chart->setAnimationOptions(QChart::NoAnimation);
    ChartView = new QChartView(Chart);
    ChartView->setRenderHint(QPainter::Antialiasing);
    return Chart;
}
bool AccelGyroChart::Append(double data[], int start){
    bool result = true;
    if (Count > Limit){
        if (Enabled == false) return true;
        for (int a = 0; a < 3; a++){
            Series[a]->clear();
        }
        Count = 0;
        result = false;
    }
    for (int a = 0; a < 3; a++){
        Series[a]->append(Count, data[a + start]);
    }
    Count++;
    return result;
}
void AccelGyroChart::Stop(){
    Enabled = false;
}
void AccelGyroChart::Start(){
    Enabled = true;
}
