//
//  trackInitializer.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef Initializer_hpp
#define Initializer_hpp

#include "Geometry.hpp"
#include "Angle.hpp"
#include <vector>

using namespace std;

typedef enum enInitStatue
{
    TI_NO_ERROR,
    TI_ERROR_TRACK_NOT_CLOSE,
    TI_ERROR_CAR_INIT_INFO,
    TI_ERROR_DESTINATION_INFO,
    TI_ERROR_TRACK_EMPTY
}initStatus;

class Initializer
{
public:
    Initializer(string fileFullPath);
    
    initStatus initCheck();
    
    Coordinate getCarInitCoordinate();
    Degree getCarInitAngle();
    
    vector<Point> getTrackInfo();
    vector<Point> getEndZoneInfo();

protected:
    Initializer();
    
private:
    initStatus m_status;
    Coordinate m_carCoordinate;
    Degree m_carAngle;
    vector<Point> m_vTrackInfo;
    vector<Point> m_vEndZoneInfo;
    
    bool loadData(string fileFullPath);
    bool parserCarCoordinate(string str);
    bool parserEndZone(string strStart, string strEnd);
    Point stringToPoint(string strPoint, string strGap);
};

#endif /* Initializer_hpp */
