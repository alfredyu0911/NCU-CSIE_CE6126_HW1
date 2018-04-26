#ifndef SYSTEMDATACENTER_H
#define SYSTEMDATACENTER_H

#include <vector>
#include "Geometry.hpp"
#include "Angle.hpp"

typedef struct stRecord
{
    Point pt;
    float distanceL, distanceR, distanceF;
    float wheelAngle;
    stRecord(Point p, float L, float R, float F, float deg)
    {
        pt = p;
        distanceL = L;
        distanceR = R;
        distanceF = F;
        wheelAngle = deg;
    }

}Record;

class SystemDataCenter
{
public:
    SystemDataCenter();

    Point nextIntersectionPoint(float dDegreeToCheck);

    vector<Point> m_vTrackBorderInfo, m_vEndZoneBorderInfo;

    Point m_carPosition, m_intersectPt_L, m_intersectPt_R, m_intersectPt_F;
    Degree m_carAngle, m_carWheelAngle;

    void carTurnRight(Degree degree)
    {
        if ( degree.getDegree() > WHEEL_ANGLE_RIGHT_MAX )
            m_carWheelAngle = Degree(WHEEL_ANGLE_RIGHT_MAX);
        else
            m_carWheelAngle = Degree(degree.getDegree());
    }

    void carTurnLeft(Degree degree)
    {
        if ( degree.getDegree() < WHEEL_ANGLE_LEFT_MAX )
            m_carWheelAngle = Degree(WHEEL_ANGLE_LEFT_MAX);
        else
            m_carWheelAngle = Degree(degree.getDegree());
    }

    vector<Record> m_vRecord;

    void addCarNewRecord(Record Record) { m_vRecord.push_back(Record); }
    void clearCarRecord() { m_vRecord.clear(); }
};

#endif // SYSTEMDATACENTER_H
