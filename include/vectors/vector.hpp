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
#ifndef VECTOR_H
#define VECTOR_H

#include "sb-engine.hpp"

using namespace std;
using namespace chrono;

namespace SBE
{
    class Vector
    {
        private:
            Point origin;
            double x, y, z;

        public:
            Event<double, milliseconds> *govX, *govY, *govZ;

            //  Constructor
            //----------------------------------

            Vector(double x, double y, double z, auto nGovX, auto nGovY, auto nGovZ, Point origin)
            {
                this->x=x;
                this->y=y;
                this->z=z;

                this->govX=nGovX;
                this->govY=nGovY;
                this->govZ=nGovZ;

                this->origin=origin;
            }

            //  Updaters
            //----------------------------------

            void updateX(milliseconds timeDiff)
            {
                this->x = govX->operator()(timeDiff);   
            }
            void updateY(milliseconds timeDiff)
            {
                this->y = govY->operator()(timeDiff);   
            }
            void updateZ(milliseconds timeDiff)
            {
                this->z = govZ->operator()(timeDiff);   
            }

            void updateSelect(bool x, bool y, bool z, milliseconds timeDiff)
            {
                x ? updateX(timeDiff) : void();
                y ? updateY(timeDiff) : void();
                z ? updateZ(timeDiff) : void();
            }

            //  Accessors
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


            Point getOrigin()
            {
                return origin;
            }

            //  Mutators
            //----------------------------------

            void setX(double nX)
            {
                this->x=nX;
            }
            void setY(double nY)
            {
                this->y=nY;
            }
            void setZ(double nZ)
            {
                this->z=nZ;
            }

            //  Operators
            //----------------------------------

            Vector operator*(Vector &rhs)
            {
                return Vector(
                    (this->x+origin.getX())*(rhs.getX()+rhs.getOrigin().getX()),
                    (this->y+origin.getY())*(rhs.getY()+rhs.getOrigin().getY()),
                    (this->z+origin.getZ())*(rhs.getZ()+rhs.getOrigin().getZ()),
                    new Event<double, milliseconds>(
                        [this, &rhs](milliseconds dT)
                        { 
                            return (this->govX->operator()(dT))*(rhs.govX->operator()(dT));
                        }
                    ),
                    new Event<double, milliseconds>(
                        [this, &rhs](milliseconds dT)
                        { 
                            return (this->govY->operator()(dT))*(rhs.govY->operator()(dT));
                        }
                    ),
                    new Event<double, milliseconds>(
                        [this, &rhs](milliseconds dT)
                        { 
                            return (this->govZ->operator()(dT))*(rhs.govY->operator()(dT));
                        }
                    ),
                    Point(0,0,0)
                );
            }
            Vector operator*(double rhs)
            {
                return Vector(
                    this->getX()*rhs,
                    this->getY()*rhs,
                    this->getZ()*rhs,
                    new Event<double, milliseconds>(
                        [this, &rhs](milliseconds dT)
                        { 
                            return (this->govX->operator()(dT))*rhs;
                        }
                    ),
                    new Event<double, milliseconds>(
                        [this, &rhs](milliseconds dT)
                        { 
                            return (this->govY->operator()(dT))*rhs;
                        }
                    ),
                    new Event<double, milliseconds>(
                        [this, &rhs](milliseconds dT)
                        { 
                            return (this->govZ->operator()(dT))*rhs;
                        }
                    ),
                    origin*rhs
                );
            }

            void operator*=(Vector rhs)
            {
                (*this) = (*this) * rhs;
            }
            void operator*=(double rhs)
            {
                (*this) = (*this) * rhs;
            }

            Vector operator+(Vector &rhs)
            {
                auto dX = getOrigin().getX();
                auto dY = getOrigin().getY();
                auto dZ = getOrigin().getZ();
                return Vector(
                    x+dX+rhs.getX()+rhs.getOrigin().getX(),
                    y+dY+rhs.getY()+rhs.getOrigin().getY(),
                    z+dZ+rhs.getZ()+rhs.getOrigin().getZ(),
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return (this->govX->operator()(a) + rhs.govX->operator()(a));
                    }),
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return (this->govY->operator()(a) + rhs.govY->operator()(a));
                    }),
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return (this->govZ->operator()(a) + rhs.govZ->operator()(a));
                    }),
                    Point(0,0,0)
                );
            }
            Vector operator+(double rhs)
            {
                return Vector(
                    x+rhs,
                    y+rhs,
                    z+rhs,
                    govX,
                    govY,
                    govZ,
                    origin
                );
            }

            void operator+=(Vector &rhs)
            {
                (*this) = ((*this) + rhs);
            }
            void operator+=(double rhs)
            {
                (*this) = ((*this) + rhs);
            }

            Vector operator/(Vector &rhs)
            {
                return Vector(
                    (x+getOrigin().getX())*(1.0/(rhs.getX()+rhs.getOrigin().getX())),
                    (y+getOrigin().getY())*(1.0/(rhs.getY()+rhs.getOrigin().getY())),
                    (z+getOrigin().getZ())*(1.0/(rhs.getZ()+rhs.getOrigin().getZ())),
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return this->govX->operator()(a) * (1.0/rhs.getX());
                    }),
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return this->govY->operator()(a) * (1.0/rhs.getY());
                    }),
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return this->govZ->operator()(a) * (1.0/rhs.getZ());
                    }),
                    Point(0,0,0)
                );
            }
            Vector operator/(double rhs)
            {
                return Vector(
                    (x+getOrigin().getX())/rhs,
                    (y+getOrigin().getY())/rhs,
                    (z+getOrigin().getZ())/rhs,
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return this->govX->operator()(a)/rhs;
                    }),
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return this->govY->operator()(a)/rhs;
                    }),
                    new Event<double, milliseconds>([this, &rhs](milliseconds a)
                    {
                        return this->govZ->operator()(a)/rhs;
                    }),
                    origin
                );
            }

            void operator/=(Vector &rhs)
            {
                (*this) = ((*this) / rhs);
            }
            void operator/=(double rhs)
            {
                (*this) = ((*this) / rhs);
            }

    };
}
#endif