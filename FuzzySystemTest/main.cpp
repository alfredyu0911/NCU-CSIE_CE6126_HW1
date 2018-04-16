//
//  main.cpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#include "AllLibrary.hpp"

using namespace std;

int main(int argc, const char * argv[])
{
    Track *track = new Track();
    Initializer *init = new Initializer();
    CarObject *car = NULL;
    
    if ( init->initCheck() == TI_NO_ERROR )
    {
        track->createTrack(init->getTrackInfo());
        
        car = new CarObject(init->getCarInitAngle().getDegree(), init->getCarInitCoordinate());
        track->putCarIntoTrack(car);
        
        track->setupComplete_gettingStart();
    }
    
    delete track;
    delete init;
    
    return 0;
}
