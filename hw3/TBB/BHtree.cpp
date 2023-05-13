#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include "BHtree.hpp"
using namespace std;

long double threshold = 0.5;
long double G = 6.67*pow(10,-11);

//----------------------------------------------------------------------------------------------
//                                          Point
//----------------------------------------------------------------------------------------------
Point::Point(long double _x, long double _y)
{
    x = _x;
    y = _y;
}
Point::Point()
{
    x = 0;
    y = 0;
}

string Point::toString()
{
    string str = "Point(";
    str += to_string(x);
    str += ", ";
    str += to_string(y);
    str += ")";
    return str;
}

//----------------------------------------------------------------------------------------------
//                                          Boundary
//----------------------------------------------------------------------------------------------

Boundary::Boundary(Point minPoint, Point maxPoint) : minPoint(minPoint), maxPoint(maxPoint) {}

Boundary::Boundary()
{
    cerr << "Warning creating empty boundary" << endl;
    minPoint = Point();
    maxPoint = Point();
}

long double Boundary::getMiddleRow()
{
    return (maxPoint.x + minPoint.x) / 2;
}

long double Boundary::getMiddleColumn()
{
    return (maxPoint.y + minPoint.y) / 2;
}

SplitBoundary *Boundary::split()
{
    Point midPoint = Point(getMiddleRow(), getMiddleColumn());
    SplitBoundary *split = new SplitBoundary(minPoint, midPoint, maxPoint);
    return split;
}

bool Boundary::pointIsInBoundary(Point p)
{
    return (p.x >= minPoint.x && p.x < maxPoint.x && p.y >= minPoint.y && p.y < maxPoint.y);
}

long double Boundary::getMinSize(){
    long double size_x = maxPoint.x - minPoint.x; 
    long double size_y = maxPoint.y - minPoint.y; 
    return size_x > size_y ? size_y : size_x;
}

string Boundary::toString()
{
    string str = "Boundary: Top Left Point: ";
    str += minPoint.toString();
    str += " || Bottom Right Point: ";
    str += maxPoint.toString();
    return str;
}

//----------------------------------------------------------------------------------------------
//                                          SplitBoundary
//----------------------------------------------------------------------------------------------
SplitBoundary::SplitBoundary(Point minPoint, Point midPoint, Point maxPoint)
{
    top_left = (minPoint.x == maxPoint.x && minPoint.y == maxPoint.y) ? NULL : new Boundary(Point(minPoint.x, minPoint.y), Point(midPoint.x, midPoint.y));
    top_right = (maxPoint.x == minPoint.x) ? NULL : new Boundary(Point(midPoint.x, minPoint.y), Point(maxPoint.x, midPoint.y));
    bottom_left = (maxPoint.y == minPoint.y) ? NULL : new Boundary(Point(minPoint.x, midPoint.y), Point(midPoint.x, maxPoint.y));
    bottom_right = (maxPoint.x == minPoint.x || maxPoint.y == minPoint.y) ? NULL : new Boundary(Point(midPoint.x, midPoint.y), Point(maxPoint.x, maxPoint.y));
}
string SplitBoundary::toString()
{
    string str = "Split Boundry: \n";
    str += "\tTop-Left: " + (top_left ? top_left->toString() : string("NULL")) + "\n";
    str += "\tTop-Right: " + (top_right ? top_right->toString() : string("NULL")) + "\n";
    str += "\tBottom-Left: " + (bottom_left ? bottom_left->toString() : string("NULL")) + "\n";
    str += "\tBottom-Right: " + (bottom_right ? bottom_right->toString() : string("NULL")) + "\n";
    return str;
}

SplitBoundary::~SplitBoundary()
{
    delete top_left;
    delete top_right;
    delete bottom_left;
    delete bottom_right;
}

//----------------------------------------------------------------------------------------------
//                                          Planet
//----------------------------------------------------------------------------------------------
Planet::Planet(string name, Point position, long double vx, long double vy, long double mass)
    : name(name), position(position), vx(vx), vy(vy), mass(mass) {}

//----------------------------------------------------------------------------------------------
//                                          Forces
//----------------------------------------------------------------------------------------------
Forces::Forces(): fx(0), fy(0){}

void Forces::reset(){fx=0; fy=0;}
//----------------------------------------------------------------------------------------------
//                                          Quad
//----------------------------------------------------------------------------------------------
void Quad::calculate_mass_center()
{
    long double mass_sum = 0.0;
    long double x_sum = 0.0;
    long double y_sum = 0.0;
    for (int i = 0; i < 4; i++)
    {
        if (children[i]->devided == true || children[i]->planet != NULL)
        { // subtree contains at least one planet leaf
            mass_sum += children[i]->cluster_mass;
            x_sum += (children[i]->cluster_mass * children[i]->center_of_mass.x);
            y_sum += (children[i]->cluster_mass * children[i]->center_of_mass.y);
        }
    }
    cluster_mass = mass_sum;
    center_of_mass = Point(x_sum / mass_sum, y_sum / mass_sum);
}
Quad::Quad() : boundary(NULL), planet(NULL), devided(false)
{
    children[0] = NULL;
    children[1] = NULL;
    children[2] = NULL;
    children[3] = NULL;
}
Quad::Quad(Boundary *b) : planet(NULL), devided(false)
{
    boundary = new Boundary(*b);
    children[0] = NULL;
    children[1] = NULL;
    children[2] = NULL;
    children[3] = NULL;
}
Quad::~Quad(){
    if(boundary) delete boundary;
    if(children[TOP_LEFT]) delete children[TOP_LEFT];
    if(children[TOP_RIGHT]) delete children[TOP_RIGHT];
    if(children[BOTTOM_LEFT]) delete children[BOTTOM_LEFT];
    if(children[BOTTOM_RIGHT]) delete children[BOTTOM_RIGHT];
}

void Quad::insertPlanet(Planet *p)
{
    if (!p)
    {
        cerr << "Error: Cannot insert NULL planet" << endl;
        return;
    }
    if (!(boundary->pointIsInBoundary(p->position)))
    {
        cerr << "Error: Cannot insert planet that is outside of the space that is examined" << endl;
        cerr << "\t" << p->position.toString() << endl;
        cerr << "\t" << boundary->toString() << endl;
        return;
    }

    if (devided == false && planet == NULL)
    {
        planet = p;
        cluster_mass = p->mass;
        center_of_mass = Point(p->position);

        return;
    }

    if (devided == false)
    {
        if (p->position.x == planet->position.x && p->position.y == planet->position.y)
        {
            cerr << "Cannot insert planets at the same point" << endl;
            return;
        }
        devided = true;
        SplitBoundary *split = boundary->split();
        children[TOP_LEFT] = new Quad(split->top_left);
        children[TOP_RIGHT] = new Quad(split->top_right);
        children[BOTTOM_LEFT] = new Quad(split->bottom_left);
        children[BOTTOM_RIGHT] = new Quad(split->bottom_right);
        delete split;
        for (int i = 0; i < 4; i++)
        {
            if (children[i]->boundary->pointIsInBoundary(planet->position))
            { // add old planet to the new leaf
                children[i]->insertPlanet(planet);
                planet = NULL;
                break;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            if (children[i]->boundary->pointIsInBoundary(p->position))
            { // use recurssion to add the new planet
                children[i]->insertPlanet(p);
                calculate_mass_center();
                return;
            }
        }
        cerr << "Enexpected error: line" << __LINE__;
    }

    if (devided == true)
    {
        for (int i = 0; i < 4; i++)
        {
            if (children[i]->boundary->pointIsInBoundary(p->position))
            { // use recurssion to add the new planet
                children[i]->insertPlanet(p);
                calculate_mass_center();
                return;
            }
        }
    }
}

bool Quad::hasChildren(){
    return (children[TOP_LEFT] || children[TOP_RIGHT] || children[BOTTOM_LEFT] || children[BOTTOM_RIGHT]);
}

void Quad::calculateForce(Planet *p){
    if(planet != NULL && planet==p){
        //cerr<<"Debug: reached planet"<<endl;
        return;
    }
    //if we reached a leaf directly use planet
    if(planet != NULL){
        if(planet==p) return ; //don't calculate force from self
        long double d = getDistance(p->position, planet->position);
        long double f = ( G* planet->mass * p->mass / pow(d,2) );
        p->forces.fx += f * (planet->position.x - p->position.x) / d;
        p->forces.fy += f * (planet->position.y - p->position.y) / d;
        return ;
    }
    //if it is far away enough, use cluster
    if(!boundary->pointIsInBoundary(p->position) && hasChildren() && getDistance(p->position, center_of_mass) >= boundary->getMinSize()){
        long double d = getDistance(p->position, center_of_mass);
        long double f = ( G* cluster_mass * p->mass / pow(d,2) );
        p->forces.fx += f * (center_of_mass.x - p->position.x) / d;
        p->forces.fy += f * (center_of_mass.y - p->position.y) / d;
        return ;
    }

    //else recursively call for all children (if they exist)
    if(children[TOP_LEFT]) children[TOP_LEFT]->calculateForce(p);
    if(children[TOP_RIGHT]) children[TOP_RIGHT]->calculateForce(p);
    if(children[BOTTOM_LEFT]) children[BOTTOM_LEFT]->calculateForce(p);
    if(children[BOTTOM_RIGHT]) children[BOTTOM_RIGHT]->calculateForce(p);
    return ;
}

//----------------------------------------------------------------------------------------------
//                                          BHtree
//----------------------------------------------------------------------------------------------
BHtree::BHtree(Boundary *initialSpace)
{
    root = NULL;
    maxDepth = 0;
    nodes = 0;
    planetNodes = 0;
    root = new Quad();
    root->boundary = initialSpace;
}

BHtree::BHtree(Boundary *initialSpace, vector<Planet *>& planets)
{
    root = NULL;
    maxDepth = 0;
    nodes = 0;
    planetNodes = 0;
    root = new Quad();
    root->boundary = initialSpace;
    for (int i = 0; i < planets.size(); i++)
    {
        root->insertPlanet(planets[i]);
    }
}

void BHtree::insertPlanet(Planet* planet){
    root->insertPlanet(planet);
}

void BHtree::calculateForce(Planet* planet){
    root->calculateForce(planet);
}

BHtree::~BHtree(){
    if(root) delete root;
}
//----------------------------------------------------------------------------------------------

long double getDistance(Point p1, Point p2){
    //get sides
    long double x_side = abs(p1.x-p2.x);
    long double y_side = abs(p1.y-p2.y);
    //use pythagorean theorem
    return sqrt(pow(x_side,2)+pow(y_side,2));
}