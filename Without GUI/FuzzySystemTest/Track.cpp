//
//  Track.cpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/30.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#include "AllLibrary.hpp"

inline float getDistance(Point p1, Point p2) { return sqrt(pow(p2.x-p1.x, 2) + pow(p2.y-p1.y, 2)); }

Track::Track()
{
}

Track::~Track()
{
}

void Track::createTrack(vector<Point> vPt)
{
    m_vTrackInfo = vPt;
}

void Track::putCarIntoTrack(CarObject *car)
{
    m_car = car;
    m_carDegree = m_car->m_directionDegree;
}

void Track::setupComplete_gettingStart()
{
    ofstream ofs("car.txt", ios::out);
    
    for ( int i=0 ; i < 60 ; i++ )
    {
        m_car->m_directionDegree = m_carDegree;
        Point pt_L = nextIntersectionPoint(m_car->getCarLeftSensorDegree(m_carDegree));
        Point pt_R = nextIntersectionPoint(m_car->getCarRightSensorDegree(m_carDegree));
        Point pt_F = nextIntersectionPoint(m_carDegree.getDegree());
        m_car->updateSensorLocation(m_carCoordinate);
        m_car->sensorLeft_receiveIntersectionPointSignal(pt_L);
        m_car->sensorRight_receiveIntersectionPointSignal(pt_R);
        m_car->sensorFront_receiveIntersectionPointSignal(pt_F);
        
        cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-" << endl;
        cout << setfill('0') << setw(2) << "Round " << i << endl;
        cout << "目前位置：" << fixed << setprecision(5) << m_carCoordinate.x << "  " << m_carCoordinate.y << endl;
        float dis = getDistance(pt_L, m_carCoordinate) - getDistance(pt_R, m_carCoordinate);
        cout << "目前偏" << (dis<0.0?"左":"右") << "：" << fixed << setprecision(5) << fabs(dis) << endl;
        cout << "前方剩餘" << "：" << fixed << setprecision(5) << fabs(getDistance(pt_F, m_carCoordinate)) << endl;
        
        if ( i == 33 )
            cout << "check" << endl;
        
        m_car->autopilotSystemTackControl();
        
        m_car->move(m_carDegree, m_carCoordinate);
        ofs << m_carCoordinate.x << " " << m_carCoordinate.y << endl;
        
        
        cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-" << endl;
    }
}

Point Track::nextIntersectionPoint(float dDegreeToCheck)
{
    Point pt_nearest;
    float distance_nearest = 10E23;
    
    Degree *degreeToCheck = new Degree(dDegreeToCheck);
    
    Quadrant aq = degreeToCheck->getQuadrant();
    
    vector<Point>::iterator it=m_vTrackInfo.begin();
    vector<Point>::iterator it2=it++;
    while ( it != m_vTrackInfo.end() )
    {
        bool flag = false;
        
        if ( it2 == m_vTrackInfo.end() )
        {
            it2 = m_vTrackInfo.begin();
            flag = true;
        }
        
        Point border_pt1=(*it), border_pt2=(*it2), pt_intersection;
        if ( checkLineIntersection(border_pt1, border_pt2, m_carCoordinate, *degreeToCheck, pt_intersection) )
        {
            switch (aq)
            {
                case Quadrant_First:
                {
                    if ( pt_intersection.x > m_carCoordinate.x && pt_intersection.y > m_carCoordinate.y )
                    {
                        float dis = getDistance(m_carCoordinate, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }
                        
                    }
                    break;
                }
                case Quadrant_Second:
                {
                    if ( pt_intersection.x < m_carCoordinate.x && pt_intersection.y > m_carCoordinate.y )
                    {
                        float dis = getDistance(m_carCoordinate, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }
                        
                    }
                    break;
                }
                case Quadrant_Third:
                {
                    if ( pt_intersection.x < m_carCoordinate.x && pt_intersection.y < m_carCoordinate.y )
                    {
                        float dis = getDistance(m_carCoordinate, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }
                        
                    }
                    break;
                }
                case Quadrant_Fourth:
                {
                    if ( pt_intersection.x > m_carCoordinate.x && pt_intersection.y < m_carCoordinate.y )
                    {
                        float dis = getDistance(m_carCoordinate, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }
                        
                    }
                    break;
                }
                case X_Axis_Positive:
                {
                    if ( pt_intersection.x > m_carCoordinate.x )
                    {
                        float dis = getDistance(m_carCoordinate, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }
                        
                    }
                    break;
                }
                case X_Axis_Negative:
                {
                    if ( pt_intersection.x < m_carCoordinate.x )
                    {
                        float dis = getDistance(m_carCoordinate, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }
                        
                    }
                    break;
                }
                case Y_Axis_Positive:
                {
                    if ( pt_intersection.y > m_carCoordinate.y )
                    {
                        float dis = getDistance(m_carCoordinate, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }
                        
                    }
                    break;
                }
                case Y_Axis_Negative:
                {
                    if ( pt_intersection.y < m_carCoordinate.y )
                    {
                        float dis = getDistance(m_carCoordinate, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }
                        
                    }
                    break;
                }
                    
                default:
                    break;
            }
        }
        it++;
        it2++;
        
        if ( flag )
            break;
    }
    
    delete degreeToCheck;
    return pt_nearest;
}

bool Track::checkLineIntersection(Point ptStart, Point ptEnd, Point car, Degree &carAngle, Point &ptIntersection)
{
    // y - y' = m(x - x')  ->  y = m(x - x') + y'
    //
    // m1(x-x1')+y1' = m2(x-x2')+y2'  ->  m1x-m1x1'+y1' = m2x-m2x2'+y2'
    // ->  (m1-m2)x = m1x1' - m2x2' - y1' +y2'
    // ->  x = (m1x1' - m2x2' - y1' + y2') / (m1-m2)
    
    float m_line = (ptEnd.y - ptStart.y) / (ptEnd.x - ptStart.x);
    float m_car = tan(carAngle.getRadians());

    if ( ptEnd.x == ptStart.x && carAngle.getDegree() == 90.0 )
    {
        return false;
    }
    else if ( ptEnd.x == ptStart.x )
    {
        // border is vertical, car is not
        float X = ptStart.x;
        float Y = m_car*(X - car.x) + car.y;
        
        if ( Y > max(ptStart.y, ptEnd.y) || Y < min(ptStart.y, ptEnd.y) )
            return false;
        
        ptIntersection.x = X;
        ptIntersection.y = Y;
        return true;
    }
    else if ( carAngle.getDegree() == 90.0 )
    {
        // the car is vertical, border is not
        float X = car.x;
        float Y = m_line*(X - ptStart.x) + ptStart.y;
        
        if ( X > max(ptStart.x, ptEnd.x) || X < min(ptStart.x, ptEnd.x) )
            return false;
        
        ptIntersection.x = X;
        ptIntersection.y = Y;
        return true;
    }
    else if ( ptStart.y == ptEnd.y )
    {
        float X = (m_line*ptStart.x - m_car*car.x - ptStart.y + car.y) / (m_line - m_car);
        float Y = ptStart.y;
        
        if ( X <= max(ptStart.x, ptEnd.x) && X >= min(ptStart.x, ptEnd.x) )
        {
            ptIntersection.x = X;
            ptIntersection.y = Y;
            return true;
        }
    }
    else
    {
        float X = (m_line*ptStart.x - m_car*car.x - ptStart.y + car.y) / (m_line - m_car);
        float Y = m_car*(X - car.x) + car.y;
        
        if ( X <= max(ptStart.x, ptEnd.x) && X >= min(ptStart.x, ptEnd.x) &&
             Y <= max(ptStart.y, ptEnd.y) && Y >= min(ptStart.y, ptEnd.y)   )
        {
            ptIntersection.x = X;
            ptIntersection.y = Y;
            return true;
        }
    }
    
    return false;
}
/*
bool Track::checkLineIntersection(Point ptStart, Point ptEnd, Point car, Degree &carAngle, Point &ptIntersection)
{
    float Slope = tan(carAngle.getRadians());
    Point s2 = Point(car.x + 10000, car.x + 10000*Slope);
    
    Geometry geo;
    if ( geo.isIntersect(ptStart, ptEnd, car, s2) == false )
        return false;
    
    ptIntersection = geo.intersection(ptStart, ptEnd, car, s2);
    
    return true;
}*/
