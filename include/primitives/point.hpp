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

namespace SBE
{
    //Point: Primitive
    //A point in 3D space. 
    class Point
    {
        private:
            double x;
            double y;
            double z;

            bool equal(Point other)
            {
                Tolerance error = Tolerance(0,uTol);
                
                return error[distance(other)];
            } 

        public:
            //Constructors
            //----------------------------------
            Point(double x=0, double y=0, double z=0) : Point(std::tuple<double, double, double>(x, y, z))
            {
                return;
            }
            
            Point(std::tuple<double, double, double> p)
            {
                this->x=std::get<0>(p);
                this->y=std::get<1>(p);
                this->z=std::get<2>(p);
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
            void setZ(double z2)
            {
                this->z=z2;
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
            double getZ()
            {
                return this->z;
            }

            double distance(Point other)
            {
                double dX = other.getX() - this->getX();
                double dY = other.getY() - this->getY();
                dX*=dX;
                dY*=dY;

                double diag = dX+dY;
                double dZ = other.getZ() - this->getZ();
                
                dZ*=dZ;

                return sqrt(diag+dZ);
            }
            
            //  Operators
            //----------------------------------

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
                return Point(std::tuple<double, double, double>(
                    getX()+rhs.getX(),
                    getY()+rhs.getY(),
                    getZ()+rhs.getZ()));
            }
            void operator+=(Point rhs)
            {
                Point p2 = ((*this)+rhs);
                setX(p2.getX());
                setY(p2.getY());
                setZ(p2.getZ());
            }
            Point operator-(Point rhs)
            {
                return Point(std::tuple<double, double, double>(
                    getX()-rhs.getX(),
                    getY()-rhs.getY(),
                    getZ()-rhs.getZ()));
            }
            void operator-=(Point rhs)
            {
                Point p2 = ((*this)-rhs);
                setX(p2.getX());
                setY(p2.getY());
                setZ(p2.getZ());
            }
            Point operator*(Point rhs)
            {
                return Point(
                    this->getX() * rhs.getX(),
                    this->getY() * rhs.getY(),
                    this->getZ() * rhs.getZ()
                );
            }
            Point operator*(double rhs)
            {
                Point ex=(*this);
                ex*=rhs;
                return ex;
            }
            Point operator/(Point rhs)
            {
                return Point(
                    this->getX() / rhs.getX(),
                    this->getY() / rhs.getY(),
                    this->getZ() / rhs.getZ()
                );
            }
            Point operator/(double rhs)
            {
                Point ex=(*this);
                ex/=rhs;
                return ex;
            }
            void operator*=(Point rhs)
            {
                (*this)=(*this)*rhs;
            }
            void operator*=(double rhs)
            {
                setX(getX()*rhs);
                setY(getY()*rhs);
                setZ(getZ()*rhs);
            }
            void operator/=(Point rhs)
            {
                (*this)=(*this)/rhs;
            }
            void operator/=(double rhs)
            {
                setX(getX()/rhs);
                setY(getY()/rhs);
                setZ(getZ()/rhs);
            }
    };
};
#endif