//
// Created by Djundi on 11/3/15.
//

#include "Object2D.h"
#include "Topic2/Implementation/Point2D.h"
#include "Topic2/Implementation/Line2DImpl.h"
#include "Topic2/Implementation/Region2DImpl.h"


// Equal operator that checks if the Object2D object and the operand Object2D are the same
bool Object2D::operator == (Object2D& operand){
    if(this==&operand)
        return true;
    else
        return false;
}

bool Object2D::isPoint2D()
{
    if(dynamic_cast<Point2D*>(this))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Object2D::isLine2D()
{
    if(dynamic_cast<Line2D*>(this))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Object2D::isRegion2D()
{
    if(dynamic_cast<Region2D*>(this))
    {
        return true;
    }
    else
    {
        return false;
    }
}