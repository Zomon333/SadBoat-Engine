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

class Angle
{
    public:
        enum ATYPE
        {
            RADIANS,
            DEGREES
        };
        
        ATYPE getUnit() { return unit; }
        void setUnit(ATYPE unit2) { unit=unit2; }
        virtual double get() = 0;

    private:
        ATYPE unit;
};

class Radians : public Angle
{
    private:
        double c;
        double piTerms;
    public:
        //Constructors
        //----------------------------------

        // coefficient * piTerms < 2pi, 2pi = 6.28318
        Radians()
        {
            setUnit(RADIANS);
            setRadians(0, 1, true);
        }
        // coefficient * piTerms < 2pi, 2pi = 6.28318
        Radians(double coefficient, double piTerms=1)
        {
            setUnit(RADIANS);
            setRadians(coefficient, piTerms, true);
        }

        //Accessors
        //----------------------------------

        //Get radians as double in piTerms;
        double get()
        {
            return c*piTerms;
        }
        //Get c and piTerms so you can do math in terms of pi
        std::pair<double, double> getPair()
        {
            return std::pair<double, double>(c, piTerms);
        }


        //Mutators
        //----------------------------------

        //Set the amount of radians in terms of pi.
        //Will factor pi out of c2 to update piTerms dynamically if factor is true.
        // coefficient * piTerms < 2pi, 2pi = 6.28318
        void setRadians(double c2, double piTerms2, bool factor=true)
        {
            c=c2;
            piTerms=piTerms2;
            if(factor)
                factorPi();
        }

        //Set the amount of radians in terms of pi.
        //Will factor pi out of c2 to update piTerms dynamically if factor is true.
        // coefficient * piTerms < 2pi, 2pi = 6.28318
        void setRadians(double c2, bool factor=true)
        {
            c=c2;
            piTerms=1;
            if(factor)
                factorPi();
        }

        //Factors
        void factorPi()
        {
            //Get what the raw radians should be
            double old = get();

            //Set it so we're not in terms of pi anymore
            piTerms=1;
            //Break it down so we're within our unit circle
            c=fmod(old,(2*M_PI));

            //And while you can still factor out a pi 
            while(c>=M_PI)
            {
                //Remove it from C
                c/=M_PI;

                //and put it in piTerms
                piTerms*=M_PI;
            }
        }

        //Operators
        //----------------------------------

        //Adding
        Radians operator+(Radians rhs)
        {
            double sum = this->get() + rhs.get();
            double pi=1;

            return Radians(sum, pi);
        }
        Radians operator+(double rhs)
        {
            double sum = get()+rhs;
            double pi=1;

            return Radians(sum, pi);
        }

        //Subtracting
        Radians operator-(Radians rhs)
        {
            return Radians(this->get()-rhs.get());
        }
        Radians operator-(double rhs)
        {
            return Radians(this->get()-rhs);
        }

        //Multiplying
        Radians operator*(Radians rhs)
        {
            return Radians(this->get()*rhs.get());
        }
        Radians operator*(double rhs)
        {
            return Radians(this->get()*rhs);
        }

        //Dividing
        Radians operator/(Radians rhs)
        {
            return Radians(this->get()/rhs.get());
        }
        Radians operator/(double rhs)
        {
            return Radians(this->get()/rhs);
        }

        //Equal
        bool operator==(Radians rhs)
        {
            return (this->get()==rhs.get());
        }
        bool operator==(double rhs)
        {
            return (this->get()==rhs);
        }

        //Not Equal
        bool operator!=(Radians rhs)
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
            piTerms=1;
            c=rhs;
        }

        //Direct modification

        //+= operators
        void operator+=(Radians rhs)
        {
            this->setRadians( ((*this)+rhs).get() );
        }
        void operator+=(double rhs)
        {
            this->setRadians( ((*this)+rhs).get() );
        }

        //-= operators
        void operator-=(Radians rhs)
        {
            this->setRadians( ((*this)-rhs).get() );
        }

        void operator-=(double rhs)
        {
            this->setRadians( ((*this)-rhs).get() );
        }

        //*= operators
        void operator*=(Radians rhs)
        {
            this->setRadians( ((*this)*rhs).get() );
        }
        void operator*=(double rhs)
        {
            this->setRadians( ((*this)*rhs).get() );
        }

        // /= operators
        void operator/=(Radians rhs)
        {
            this->setRadians( ((*this)/rhs).get() );
        }
        void operator/=(double rhs)
        {
            this->setRadians( ((*this)/rhs).get() );
        }
};

/*class Degrees : public Angle
{
    private:
        double theta;
    public:
        //Constructors
        //----------------------------------
        Degrees()
        {
            setUnit(DEGREES);
            setDegrees(0);
        }
        Degrees(double theta2)
        {
            setUnit(DEGREES);
            setDegrees(theta2);
        }
        Degrees(Radians deg2)
        {
            setUnit(DEGREES);
            setDegrees(deg2.get() * (180.0/M_PI));
        }

        //Accessors
        //----------------------------------

        //Gets degrees as a double;
        double get()
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
            this->setDegrees(deg2.getAsDegrees().get());
        }
        
        //Operators
        //----------------------------------

        //Adding
        Degrees operator+(Radians rhs)
        {
            return Degrees(this->getAsRadians().get()+rhs.get());
        }
        Degrees operator+(Degrees rhs)
        {
            return Degrees(this->get()+rhs.get());
        }
        Degrees operator+(double rhs)
        {
            return Degrees(this->get()+rhs);
        }

        //Subtracting
        Degrees operator-(Radians rhs)
        {
            return Degrees(this->getAsRadians().get()-rhs.get());
        }
        Degrees operator-(Degrees rhs)
        {
            return Degrees(this->get()-rhs.get());
        }
        Degrees operator-(double rhs)
        {
            return Degrees(this->get()-rhs);
        }

        //Multiplying
        Degrees operator*(Radians rhs)
        {
            return Degrees(this->getAsRadians().get()*rhs.get());
        }
        Degrees operator*(Degrees rhs)
        {
            return Degrees(this->get()*rhs.get());
        }
        Degrees operator*(double rhs)
        {
            return Degrees(this->get()*rhs);
        }

        //Dividing
        Degrees operator/(Radians rhs)
        {
            return Degrees(this->getAsRadians().get()/rhs.get());
        }
        Degrees operator/(Degrees rhs)
        {
            return Degrees(this->get()/rhs.get());
        }
        Degrees operator/(double rhs)
        {
            return Degrees(this->get()/rhs);
        }

        //Equal
        bool operator==(Radians rhs)
        {
            return (this->getAsRadians().get()==rhs.get());
        }
        bool operator==(Degrees rhs)
        {
            return (this->get()==rhs.get());
        }
        bool operator==(double rhs)
        {
            return (this->get()==rhs);
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
            this->setDegrees(rhs.getAsDegrees().get());
        }
        void operator=(double rhs)
        {
            this->setDegrees(rhs);
        }

        //Direct modification

        //+= operators
        void operator+=(Radians rhs)
        {
            this->setDegrees( ((*this)+rhs).get() );
        }
        void operator+=(Degrees rhs)
        {
            this->setDegrees( ((*this)+rhs).get() );
        }
        void operator+=(double rhs)
        {
            this->setDegrees( ((*this)+rhs).get() );
        }

        //-= operators
        void operator-=(Radians rhs)
        {
            this->setDegrees( ((*this)-rhs).get() );
        }
        void operator-=(Degrees rhs)
        {
            this->setDegrees( ((*this)-rhs).get() );
        }
        void operator-=(double rhs)
        {
            this->setDegrees( ((*this)-rhs).get() );
        }

        //*= operators
        void operator*=(Radians rhs)
        {
            this->setDegrees( ((*this)*rhs).get() );
        }
        void operator*=(Degrees rhs)
        {
            this->setDegrees( ((*this)*rhs).get() );
        }
        void operator*=(double rhs)
        {
            this->setDegrees( ((*this)*rhs).get() );
        }

        // /= operators
        void operator/=(Radians rhs)
        {
            this->setDegrees( ((*this)/rhs).get() );
        }
        void operator/=(Degrees rhs)
        {
            this->setDegrees( ((*this)/rhs).get() );
        }
        void operator/=(double rhs)
        {
            this->setDegrees( ((*this)/rhs).get() );
        }
};

class Radians : public Angle
{
    private:
        double c;
        double piTerms;
    public:
        //Constructors
        //----------------------------------
        Radians()
        {
            setUnit(RADIANS);
            c=0;
            piTerms=1;
        }
        Radians(double coefficient, double piTerms=1)
        {
            setUnit(RADIANS);
            c=coefficient;
            this->piTerms=piTerms;
        }
        Radians(Degrees deg2)
        {
            setUnit(RADIANS);
            c = deg2.get() * ((M_PI)/180.0);
        }


        //Accessors
        //----------------------------------

        //Get radians as double in piTerms;
        double get()
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
            c = deg2.getAsRadians().get();
        }

        //Operators
        //----------------------------------

        //Adding
        Radians operator+(Radians rhs)
        {
            return Radians(this->get()+rhs.get());
        }
        Radians operator+(Degrees rhs)
        {
            return Radians(this->getAsDegrees().get()+rhs.get());
        }
        Radians operator+(double rhs)
        {
            return Radians(this->getAsDegrees().get()+rhs);
        }

        //Subtracting
        Radians operator-(Radians rhs)
        {
            return Radians(this->get()-rhs.get());
        }
        Radians operator-(Degrees rhs)
        {
            return Radians(this->get()-rhs.getAsRadians().get());
        }
        Radians operator-(double rhs)
        {
            return Radians(this->get()-rhs);
        }

        //Multiplying
        Radians operator*(Radians rhs)
        {
            return Radians(this->get()*rhs.get());
        }
        Radians operator*(Degrees rhs)
        {
            return Radians(this->getAsDegrees().get()*rhs.get());
        }
        Radians operator*(double rhs)
        {
            return Radians(this->get()*rhs);
        }

        //Dividing
        Radians operator/(Radians rhs)
        {
            return Radians(this->get()/rhs.get());
        }
        Radians operator/(Degrees rhs)
        {
            return Radians(this->get()/rhs.getAsRadians().get());
        }
        Radians operator/(double rhs)
        {
            return Radians(this->get()/rhs);
        }

        //Equal
        bool operator==(Radians rhs)
        {
            return (this->get()==rhs.get());
        }
        bool operator==(Degrees rhs)
        {
            return (this->get()==rhs.getAsRadians().get());
        }
        bool operator==(double rhs)
        {
            return (this->get()==rhs);
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
            c=rhs.getAsRadians().get();
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
            this->setRadians( ((*this)+rhs).get() );
        }
        void operator+=(Degrees rhs)
        {
            this->setRadians( ((*this)+rhs).get() );
        }
        void operator+=(double rhs)
        {
            this->setRadians( ((*this)+rhs).get() );
        }

        //-= operators
        void operator-=(Radians rhs)
        {
            this->setRadians( ((*this)-rhs).get() );
        }
        void operator-=(Degrees rhs)
        {
            this->setRadians( ((*this)-rhs).get() );
        }
        void operator-=(double rhs)
        {
            this->setRadians( ((*this)-rhs).get() );
        }

        //*= operators
        void operator*=(Radians rhs)
        {
            this->setRadians( ((*this)*rhs).get() );
        }
        void operator*=(Degrees rhs)
        {
            this->setRadians( ((*this)*rhs).get() );
        }
        void operator*=(double rhs)
        {
            this->setRadians( ((*this)*rhs).get() );
        }

        // /= operators
        void operator/=(Radians rhs)
        {
            this->setRadians( ((*this)/rhs).get() );
        }
        void operator/=(Degrees rhs)
        {
            this->setRadians( ((*this)/rhs).get() );
        }
        void operator/=(double rhs)
        {
            this->setRadians( ((*this)/rhs).get() );
        }
};*/


#endif