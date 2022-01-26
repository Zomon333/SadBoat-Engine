#ifndef CFG_H
#define CFG_H

using namespace std;

pair<string, string> delimSplice(string delim, string text);

/*
name: readCFG
purpose: allows easy reading of a *.cfg file to import game data
description:
    Opens fileName parameter as a file stream and reads contents.
    Contents must follow the following format: TYPE:NAME=DATA,
    readCFG will then break the data into TYPE, NAME, and DATA based on delimiters.
    These variables will be stored in an unordered map as (TYPE, DATA) at the key NAME.

    This readCFG function follows standard SadBoat CFG formatting.
    The # symbol will be used to denote a single line comment
    The % symbol will be used to denote a multi line comment that will continue until it is terminated with another %
*/
unordered_map<string, pair<string, string>> readCFG(string fileName)
{
    unordered_map<string, pair<string, string>> cfgContents;
    fstream file = fstream(fileName);

    string thisLine="";
    bool lineComment = false;
    bool blockComment = false;
    char current;

    if(file.is_open())
    {
        while(file.peek()!=EOF)
        {
            current = file.get();
            


            /*
            Config Commenting code
            */
            if(lineComment && current=='\n')
            {
                current = file.get();
                lineComment = !lineComment;
            }

            if(current=='#' && blockComment == false)
            {
                lineComment = !lineComment;
            }

            if(current=='%' && lineComment == false)
            {
                current = file.get();
                blockComment = !blockComment;
            }




            if(current!=',' && current!='\n' && current!=NULL && !lineComment && !blockComment)
            {
                thisLine+=current;
            }
            if(current==',')
            {
                string type = ""; //TYPE:NAME=DATA,
                string name = "";
                string data = "";

                type = delimSplice(":", thisLine).first;
                name = delimSplice(":",delimSplice("=", thisLine).first).second;
                data = delimSplice("=", thisLine).second;

                if(type.length()>0 && name.length()>0)
                {
                    if(data.length()==0)
                    {
                        data="NULL";
                    }
                    
                    cout<<"Writing "<<type<<", "<<data<<" to "<<name<<" in cfgContents\n";
                    cfgContents[name] = pair<string, string>(type, data);
                }

                thisLine = "";

            }
        }
    }
    else
    {
        cout<<"Failed to read cfg file. Using program defaults."<<endl;
       //Initialize default cfgContents 
    }

    return cfgContents;
}

/*
Bug: returned pair still contains delim
*/
pair<string, string> delimSplice(string delim, string text)
{
    string left = "";
    string right = "";

    if(delim.length()<2)
    {
        bool leftFinished = false;
        for(int i = 0; i<text.length(); i++)
        {
            if(text[i]==delim[0])
            {
                leftFinished = true;
            }

            if(!leftFinished)
            {
                left+=text[i];
            }

            else
            {
                right+=text[i];
            }
        }
    }
    else
    {
        bool leftFinished = false;

        int j = 0; //text position

        for(;j<text.length();j++)
        {
            int i = j; //delimBuilder position relative to text
            int bounds = i + delim.length(); //end of delimBuilder bounds

            if(bounds<text.length())
            {
                string builtDelim = ""; //delimBuilder return
                for(;i<delim.length();i++)
                {
                    builtDelim+=text[i];
                }
                if(builtDelim==delim)
                {
                    leftFinished=true;
                }
            }

            if(!leftFinished)
            {
                left+=text[j];
            }
            else
            {
                right+=text[j];
            }
        }

    }

    if(left[0]==delim[0])
    {
        string tmp = "";
        for(int i = 1; i<left.length(); i++)
        {
            tmp+=left[i];
        }
        left = tmp;
    }
    if(right[0]==delim[0])
    {
        string tmp = "";
        for( int i = 1; i<right.length(); i++)
        {
            tmp+=right[i];
        }
        right = tmp;
    }


    return pair<string, string>(left, right);
}

#endif