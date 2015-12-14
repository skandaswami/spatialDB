/******************************************************************************
* File: Point2DRegion2D.cpp
*******************************************************************************
* Purpose: Implementation of exploration functions and evaluation functions for
*   the topological relationships between a Point2D and a Region2D object.This also contains
 *  clustered predicate verification functions
 *
* Description: Implementation of Exploration and Evaluation algorithms and functions
 * for Topological Predicate Verification and Determination
 *
* Class: Spatial and Moving Objects Databases (CIS 4930/CIS 6930)
*
* Authors:Group 4 [Michael Kemerer,Tjindra Djundi,Natasha Mandal,Aswini Ramesh,Kyuseo Park]
*
* Date: Fall Semester 2015
******************************************************************************/
#include "Point2DRegion2D.h"

Point2DRegion2D::Point2DRegion2D(const Point2D &F, const Region2D &G) {
    // set obj1 and obj2
    objF=F;
    objG=G;

    // initialize vF and vG with false
    for (int i = 0; i<vF_size; i++) {
        vF[i] = false;
    }

};

Point2DRegion2D::~Point2DRegion2D() { };


bool *Point2DRegion2D::getVF() {
    return vF;
}

void Point2DRegion2D::exploreTopoPred() {
    //TODO Object should be derived by each of the types
    PlaneSweep S(objF,objG);
    S.newSweep();


    while((S.getStatus() == ParallelObjectTraversal::end_of_none) && (!(vF[poi_inside]))&&(vF[poi_on_bound])&&(vF[poi_outside]))
    {
        ParallelObjectTraversal::object object_value = S.getObject();
        if(object_value==ParallelObjectTraversal::first)
        {
            Poi2D p = S.getPoiEvent(ParallelObjectTraversal::first);

            if(S.poiInSeg(p))
            {
                vF[poi_on_bound]= true;
            }
            else
            {

                Seg2D s= S.predOfP(p);
                if(s!= NULL)
                {
                    //check whether interior of G is above the segment
                    bool ia = S.getInsideAbove(s);

                    if(ia)
                    {
                        vF[poi_inside]= true;
                    }
                    else
                    {
                        vF[poi_outside]= true;
                    }
                }
                else
                {
                    vF[poi_outside]= true;
                }

            }
        }

        else
        {   //object not equal to first

            ParallelObjectTraversal::object object_value = S.getObject();


            AttrHalfSeg2D h = S.getAttrHalfSegEvent(ParallelObjectTraversal::second);
            bool ia = h.insideAbove;

            if(h.hseg.isLeft) {
                S.addLeft(h.hseg.seg);
                S.setInsideAbove(h.hseg.seg,ia);
            }
            else
            {
                S.delRight(h.hseg.seg);
            }
            if(object_value==ParallelObjectTraversal::both)
            {
                vF[poi_on_bound]= true;
            }

        }
        S.selectNext();
    }
    if(S.getStatus()==ParallelObjectTraversal::end_of_second)
    {
        vF[poi_outside]=true;
    }
    return;

}
void Point2DRegion2D::evaluateTopoPred() {
    int IMC[3][3];


    //populating 9IM
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            IMC[i][j]=0;
        }
    }
    if(vF[poi_inside])
    {
        IMC[0][0]=1;
    }
    if(vF[poi_on_bound])
    {
        IMC[0][1]=1;
    }
    if(vF[poi_outside])
    {
        IMC[0][2]=1;
    }
    IMC[2][0]=1;

    IMC[2][1]=1;

    IMC[2][2]=1;


//Evaluation phase
if(IMC[0][0])
{

    if(IMC[0][1])
    {

        if(IMC[0][2])
        {
        //overlap
            topPredNumberPoint2DRegion2D = TopPredNumberPoint2DRegion2D::pr_overlap_m5;
            isPredSet=true;
        }
        else
        {
         //inside
            topPredNumberPoint2DRegion2D = TopPredNumberPoint2DRegion2D::pr_inside_m6;
            isPredSet=true;

        }


    }
    else
    {
        if(IMC[0][2])
        {
        //overlap
            topPredNumberPoint2DRegion2D = TopPredNumberPoint2DRegion2D::pr_overlap_m7;
            isPredSet=true;
        }
        else
        {
          //inside
            topPredNumberPoint2DRegion2D = TopPredNumberPoint2DRegion2D::pr_inside_m4;
            isPredSet=true;

        }
    }
}
else
{
    if(IMC[0][1])
    {
        if(IMC[0][2])
        {
            topPredNumberPoint2DRegion2D = TopPredNumberPoint2DRegion2D::pr_meet_m2;
            isPredSet=true;
        }
        else
        {
            topPredNumberPoint2DRegion2D = TopPredNumberPoint2DRegion2D::pr_meet_m3;
            isPredSet=true;
        }
    }
    else
    {
        if(IMC[0][2])
        {
   //disjoint
            topPredNumberPoint2DRegion2D = TopPredNumberPoint2DRegion2D::pr_disjoint_m1;
            isPredSet=true;
        }
        else
        {
    //disjoint
            topPredNumberPoint2DRegion2D = TopPredNumberPoint2DRegion2D::pr_disjoint_m1;
            isPredSet=true;
        }
    }


}



}

TopPredNumberPoint2DRegion2D  Point2DRegion2D::getTopologicalRelationship()
{
    if(!isPredSet)
    {
        exploreTopoPred();
        evaluateTopoPred();
    }
    return topPredNumberPoint2DRegion2D;
}

bool Point2DRegion2D::isTopologicalRelationship(TopPredNumberPoint2DRegion2D  predicate)
{
    if(!isPredSet)
    {
        exploreTopoPred();
        evaluateTopoPred();
    }
    if(topPredNumberPoint2DRegion2D==predicate)
    {
        return true;
    }
    return false;
}
bool Point2DRegion2D::overlap()
{
    if (!isPredSet)
    {
        exploreTopoPred();
        evaluateTopoPred();
    }
    if( topPredNumberPoint2DRegion2D ==TopPredNumberPoint2DRegion2D::pr_overlap_m5||topPredNumberPoint2DRegion2D ==TopPredNumberPoint2DRegion2D::pr_overlap_m7)
    {
        return true;
    }
    return false;
}

bool Point2DRegion2D::meet()
{
    if (!isPredSet)
    {
        exploreTopoPred();
        evaluateTopoPred();
    }
    if(topPredNumberPoint2DRegion2D ==TopPredNumberPoint2DRegion2D::pr_meet_m2||topPredNumberPoint2DRegion2D ==TopPredNumberPoint2DRegion2D::pr_meet_m3)
    {
        return true;
    }

    return false;
}

bool Point2DRegion2D::inside()
{
    if (!isPredSet)
    {
        exploreTopoPred();
        evaluateTopoPred();
    }
    if(topPredNumberPoint2DRegion2D ==TopPredNumberPoint2DRegion2D:: pr_inside_m4||topPredNumberPoint2DRegion2D ==TopPredNumberPoint2DRegion2D:: pr_inside_m6)
    {
        return true;
    }
    return false;
}

bool Point2DRegion2D::disjoint()
{
    if (!isPredSet)
    {
        exploreTopoPred();
        evaluateTopoPred();
    }
    if(topPredNumberPoint2DRegion2D ==TopPredNumberPoint2DRegion2D:: pr_disjoint_m1)
    {
        return true;
    }
    return false;
}

bool Point2DLine2D::contains()
{
    return false;
}

bool Point2DLine2D::coveredBy()
{
    return false;
}

bool Point2DLine2D::covers()
{
    return false;
}

bool Point2DLine2D::equal()
{
    return false;
}