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
#ifndef ANGLE_H
#define ANGLE_H

#include <utility>
#include <cmath>

class Degrees
{
    private:
        double theta;
    public:
        //Constructors
        //----------------------------------
        Degrees()
        {
            theta=0;
        }
        Degrees(double theta2)
        {
            theta=theta2;
        }
        Degrees(Radians deg2)
        {
            Degrees(deg2.getRadians() * (180.0/M_PI));
        }

        //Accessors
        //----------------------------------

        //Gets degrees as a double;
        double getDegrees()
        {
            return theta;
        }

        //Gets degrees as the Radians class;
        Radians getAsRadians()
        {
            return Radians(*this);
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
                theta=360+theta;
            }
        }

        //Sets degrees from the Radians class;
        void setDegrees(Radians deg2)
        {
            this->setDegrees(deg2.getAsDegrees().getDegrees());
        }
        
        //Operators
        //----------------------------------

        //Adding
        Degrees operator+(Radians rhs)
        {
            return Degrees(this->getAsRadians().getRadians()+rhs.getRadians());
        }
        Degrees operator+(Degrees rhs)
        {
            return Degrees(this->getDegrees()+rhs.getDegrees());
        }
        Degrees operator+(double rhs)
        {
            return Degrees(this->getDegrees()+rhs);
        }

        //Subtracting
        Degrees operator-(Radians rhs)
        {
            return Degrees(this->getAsRadians().getRadians()-rhs.getRadians());
        }
        Degrees operator-(Degrees rhs)
        {
            return Degrees(this->getDegrees()-rhs.getDegrees());
        }
        Degrees operator-(double rhs)
        {
            return Degrees(this->getDegrees()-rhs);
        }

        //Multiplying
        Degrees operator*(Radians rhs)
        {
            return Degrees(this->getAsRadians().getRadians()*rhs.getRadians());
        }
        Degrees operator*(Degrees rhs)
        {
            return Degrees(this->getDegrees()*rhs.getDegrees());
        }
        Degrees operator*(double rhs)
        {
            return Degrees(this->getDegrees()*rhs);
        }

        //Dividing
        Degrees operator/(Radians rhs)
        {
            return Degrees(this->getAsRadians().getRadians()/rhs.getRadians());
        }
        Degrees operator/(Degrees rhs)
        {
            return Degrees(this->getDegrees()/rhs.getDegrees());
        }
        Degrees operator/(double rhs)
        {
            return Degrees(this->getDegrees()/rhs);
        }

        //Equal
        bool operator==(Radians rhs)
        {
            return (this->getAsRadians().getRadians()==rhs.getRadians());
        }
        bool operator==(Degrees rhs)
        {
            return (this->getDegrees()==rhs.getDegrees());
        }
        bool operator==(double rhs)
        {
            return (this->getDegrees()==rhs);
        }

        //Not Equal
        bool operator!=(Radians rhs)
        {
            return !((*this)==rhs);
        }
        bool operator!=(Degrees rhs)
        {
            return !((*this)==rhs);
        }
        bool operator!=(double rhs)
        {
            return !((*this)==rhs);
        }

        //Assignment
        void operator=(Radians rhs)
        {
            this->setDegrees(rhs.getAsDegrees().getDegrees());
        }
        void operator=(double rhs)
        {
            this->setDegrees(rhs);
        }

        //Direct modification

        //+= operators
        void operator+=(Radians rhs)
        {
            this->setDegrees( ((*this)+rhs).getDegrees() );
        }
        void operator+=(Degrees rhs)
        {
            this->setDegrees( ((*this)+rhs).getDegrees() );
        }
        void operator+=(double rhs)
        {
            this->setDegrees( ((*this)+rhs).getDegrees() );
        }

        //-= operators
        void operator-=(Radians rhs)
        {
            this->setDegrees( ((*this)-rhs).getDegrees() );
        }
        void operator-=(Degrees rhs)
        {
            this->setDegrees( ((*this)-rhs).getDegrees() );
        }
        void operator-=(double rhs)
        {
            this->setDegrees( ((*this)-rhs).getDegrees() );
        }

        //*= operators
        void operator*=(Radians rhs)
        {
            this->setDegrees( ((*this)*rhs).getDegrees() );
        }
        void operator*=(Degrees rhs)
        {
            this->setDegrees( ((*this)*rhs).getDegrees() );
        }
        void operator*=(double rhs)
        {
            this->setDegrees( ((*this)*rhs).getDegrees() );
        }

        // /= operators
        void operator/=(Radians rhs)
        {
            this->setDegrees( ((*this)/rhs).getDegrees() );
        }
        void operator/=(Degrees rhs)
        {
            this->setDegrees( ((*this)/rhs).getDegrees() );
        }
        void operator/=(double rhs)
        {
            this->setDegrees( ((*this)/rhs).getDegrees() );
        }
};

class Radians
{
    private:
        double c;
        double piTerms;
    public:
        //Constructors
        //----------------------------------
        Radians()
        {
            c=0;
            piTerms=1;
        }
        Radians(double rads, double piTerms=1)
        {
            c=rads;
            this->piTerms=piTerms;
        }
        Radians(Degrees deg2)
        {
            c = deg2.getDegrees() * ((M_PI)/180.0);
        }


        //Accessors
        //----------------------------------

        //Get radians as double in piTerms;
        double getRadians()
        {
            return c*piTerms;
        }

        //Get radians as Degrees class;
        Degrees getAsDegrees()
        {
            return Degrees(*this);
        }

        //Mutators
        //----------------------------------
        void setRadians(double c2, double piTerms2)
        {
            c=c2;
            piTerms=piTerms2;
        }
        void setRadians(Degrees deg2)
        {
            c = deg2.getAsRadians().getRadians();
        }

        //Operators
        //----------------------------------

        //Adding
        Radians operator+(Radians rhs)
        {
            return Radians(this->getRadians()+rhs.getRadians());
        }
        Radians operator+(Degrees rhs)
        {
            return Radians(this->getAsDegrees().getDegrees()+rhs.getDegrees());
        }
        Radians operator+(double rhs)
        {
            return Radians(this->getAsDegrees().getDegrees()+rhs);
        }

        //Subtracting
        Radians operator-(Radians rhs)
        {
            return Radians(this->getRadians()-rhs.getRadians());
        }
        Radians operator-(Degrees rhs)
        {
            return Radians(this->getRadians()-rhs.getAsRadians().getRadians());
        }
        Radians operator-(double rhs)
        {
            return Radians(this->getRadians()-rhs);
        }

        //Multiplying
        Radians operator*(Radians rhs)
        {
            return Radians(this->getRadians()*rhs.getRadians());
        }
        Radians operator*(Degrees rhs)
        {
            return Radians(this->getAsDegrees().getDegrees()*rhs.getDegrees());
        }
        Radians operator*(double rhs)
        {
            return Radians(this->getRadians()*rhs);
        }

        //Dividing
        Radians operator/(Radians rhs)
        {
            return Radians(this->getRadians()/rhs.getRadians());
        }
        Radians operator/(Degrees rhs)
        {
            return Radians(this->getRadians()/rhs.getAsRadians().getRadians());
        }
        Radians operator/(double rhs)
        {
            return Radians(this->getRadians()/rhs);
        }

        //Equal
        bool operator==(Radians rhs)
        {
            return (this->getRadians()==rhs.getRadians());
        }
        bool operator==(Degrees rhs)
        {
            return (this->getRadians()==rhs.getAsRadians().getRadians());
        }
        bool operator==(double rhs)
        {
            return (this->getRadians()==rhs);
        }

        //Not Equal
        bool operator!=(Radians rhs)
        {
            return !((*this)==rhs);
        }
        bool operator!=(Degrees rhs)
        {
            return !((*this)==rhs);
        }
        bool operator!=(double rhs)
        {
            return !((*this)==rhs);
        }

        //Assignment
        void operator=(Degrees rhs)
        {
            piTerms=1;
            c=rhs.getAsRadians().getRadians();
        }
        void operator=(double rhs)
        {
            piTerms=1;
            c=rhs;
        }

        //Direct modification

        //+= operators
        void operator+=(Radians rhs)
        {
            this->setRadians( ((*this)+rhs).getRadians() );
        }
        void operator+=(Degrees rhs)
        {
            this->setRadians( ((*this)+rhs).getRadians() );
        }
        void operator+=(double rhs)
        {
            this->setRadians( ((*this)+rhs).getRadians() );
        }

        //-= operators
        void operator-=(Radians rhs)
        {
            this->setRadians( ((*this)-rhs).getRadians() );
        }
        void operator-=(Degrees rhs)
        {
            this->setRadians( ((*this)-rhs).getRadians() );
        }
        void operator-=(double rhs)
        {
            this->setRadians( ((*this)-rhs).getRadians() );
        }

        //*= operators
        void operator*=(Radians rhs)
        {
            this->setRadians( ((*this)*rhs).getRadians() );
        }
        void operator*=(Degrees rhs)
        {
            this->setRadians( ((*this)*rhs).getRadians() );
        }
        void operator*=(double rhs)
        {
            this->setRadians( ((*this)*rhs).getRadians() );
        }

        // /= operators
        void operator/=(Radians rhs)
        {
            this->setRadians( ((*this)/rhs).getRadians() );
        }
        void operator/=(Degrees rhs)
        {
            this->setRadians( ((*this)/rhs).getRadians() );
        }
        void operator/=(double rhs)
        {
            this->setRadians( ((*this)/rhs).getRadians() );
        }
};

#endif