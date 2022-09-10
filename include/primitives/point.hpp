/*
Copyright 2022 Dagan Poulin, Justice Guillory
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef POINT_H
#define POINT_H

#include <utility>
#include <cmath>

class Point
{
    private:
        double x;
        double y;

        bool equal(Point other)
        {
            return (distance(other)==0);
        } 

    public:
        //Constructors
        //----------------------------------
        Point()
        {
            this->x=0;
            this->y=0;
        }
        Point(std::pair<double, double> p)
        {
            this->x=p.first;
            this->y=p.second;
        }

        //Mutators
        //----------------------------------
        void setX(double x2)
        {
            this->x=x2;
        }
        void setY(double y2)
        {
            this->y=y2;
        }

        //Accessors
        //----------------------------------
        double getX()
        {
            return this->x;
        }
        double getY()
        {
            return this->y;
        }

        double distance(Point other)
        {
            double dX = other.getX() - this->getX();
            double dY = other.getY() - this->getY();
            dX*=dX;
            dY*=dY;

            return sqrt(dX+dY);
        }

        bool operator==(Point rhs)
        {
            return (*this).equal(rhs);
        }

        bool operator!=(Point rhs)
        {
            return !((*this)==rhs);
        }

        Point operator+(Point rhs)
        {
            return Point(std::pair<double, double>(getX()+rhs.getX(), getY()+rhs.getY()));
        }
        void operator+=(Point rhs)
        {
            Point p2 = ((*this)+rhs);
            setX(p2.getX());
            setY(p2.getY());
        }
        Point operator-(Point rhs)
        {
            return Point(std::pair<double, double>(getX()-rhs.getX(), getY()-rhs.getY()));
        }
        void operator-=(Point rhs)
        {
            Point p2 = ((*this)-rhs);
            setX(p2.getX());
            setY(p2.getY());
        }
        Point operator*(double rhs)
        {
            Point ex=(*this);
            ex*=rhs;
            return ex;
        }
        Point operator/(double rhs)
        {
            Point ex=(*this);
            ex/=rhs;
            return ex;
        }
        void operator*=(double rhs)
        {
            setX(getX()*rhs);
            setY(getY()*rhs);
        }
        void operator/=(double rhs)
        {
            setX(getX()/rhs);
            setY(getY()/rhs);
        }
};

#endif