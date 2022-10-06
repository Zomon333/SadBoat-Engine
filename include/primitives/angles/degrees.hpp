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
#ifndef DEGREES_H
#define DEGREES_H

namespace SBE
{
    //Degrees: Angles, Primitive
    //Helper class to contain degrees. Designed to allow orientability.
    //Bounded between [0,360)
    class Degrees : public Angle
    {
        private:
            double theta;
        public:
            //Constructors
            //----------------------------------
            Degrees()
            {
                setDegrees(0);
            }
            Degrees(double theta2)
            {
                setDegrees(theta2);
            }

            //Accessors
            //----------------------------------
            //Gets degrees as a double;
            double get()
            {
                return theta;
            }

            //Mutators
            //----------------------------------

            //Sets degrees from a double;
            //Bounded by 360, resets when above;
            void setDegrees(double theta2)
            {
                theta=fmod(theta2, 360);
                if(theta<0)
                {   
                    theta+=360;
                }
            }
            
            //Operators
            //----------------------------------

            //Adding
            Degrees operator+(Degrees rhs)
            {
                return Degrees(this->get()+rhs.get());
            }
            Degrees operator+(double rhs)
            {
                return Degrees(this->get()+rhs);
            }

            //Subtracting
            Degrees operator-(Degrees rhs)
            {
                return Degrees(this->get()-rhs.get());
            }
            Degrees operator-(double rhs)
            {
                return Degrees(this->get()-rhs);
            }

            //Multiplying
            Degrees operator*(Degrees rhs)
            {
                return Degrees(this->get()*rhs.get());
            }
            Degrees operator*(double rhs)
            {
                return Degrees(this->get()*rhs);
            }

            //Dividing
            Degrees operator/(Degrees rhs)
            {
                return Degrees(this->get()/rhs.get());
            }
            Degrees operator/(double rhs)
            {
                return Degrees(this->get()/rhs);
            }

            //Equal
            bool operator==(Degrees rhs)
            {
                return (this->get()==rhs.get());
            }
            bool operator==(double rhs)
            {
                return (this->get()==rhs);
            }

            //Not Equal
            bool operator!=(Degrees rhs)
            {
                return !((*this)==rhs);
            }
            bool operator!=(double rhs)
            {
                return !((*this)==rhs);
            }

            //Assignment
            void operator=(double rhs)
            {
                this->setDegrees(rhs);
            }

            //Direct modification

            //+= operators
            void operator+=(Degrees rhs)
            {
                this->setDegrees( ((*this)+rhs).get() );
            }
            void operator+=(double rhs)
            {
                this->setDegrees( ((*this)+rhs).get() );
            }

            //-= operators
            void operator-=(Degrees rhs)
            {
                this->setDegrees( ((*this)-rhs).get() );
            }
            void operator-=(double rhs)
            {
                this->setDegrees( ((*this)-rhs).get() );
            }

            //*= operators
            void operator*=(Degrees rhs)
            {
                this->setDegrees( ((*this)*rhs).get() );
            }
            void operator*=(double rhs)
            {
                this->setDegrees( ((*this)*rhs).get() );
            }

            // /= operators
            void operator/=(Degrees rhs)
            {
                this->setDegrees( ((*this)/rhs).get() );
            }
            void operator/=(double rhs)
            {
                this->setDegrees( ((*this)/rhs).get() );
            }
    };
};
#endif