#ifndef ACCELGYROCHART_H
#define ACCELGYROCHART_H
#include <QtCharts>
/*!
 * \brief This class is used for Initialize charts and append data to it
 */
class AccelGyroChart
{ 
private:
    QLineSeries *Series[3];
    /*!
     * \brief Count of measurement in series.
     */
    int Count;
    /*!
     * \brief Limit of measurement in series.
     */
    int Limit;
    /*!
     * \brief when true enables resetting series when it reach to the limit,
     * otherwise disableds resetting series.
     */
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
    /*!
     * \brief Initalizes chart, units are not defined here.
     *
     * \param[in] limit Maximum measurement count.
     * \param[in] min Y axis minimum value.
     * \param[in] max X axis maximum value.
     * \return Pointer to initalized QChart to set in QWidget
     */
    QChart* Init(int limit, double min, double max);
    /*!
     * \brief Append new data to chart series and displays it, if count reach to limit, resets series.
     * This function will append ony 3 values of data array.
     *
     * \param[in] data  Array of data to append, minimum 3 values.
     * \param[in] start Index of first value data array.
     * \return
     */
    bool Append(double data[], int start);
    /*!
     * \brief Disableds resetting series when it reach to the limit.
     */
    void Stop();
    /*!
     * \brief Enables resetting series when it reach to the limit.
     */
    void Start();

};

#endif // ACCELGYROCHART_H
