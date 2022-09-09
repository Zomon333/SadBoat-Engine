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
#ifndef RADIANS_H
#define RADIANS_H

#include <utility>
#include <cmath>
#include "angle.hpp"

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

#endif