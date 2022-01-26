#ifndef CFG_H
#define CFG_H

using namespace std;

pair<string, string> delimSplice(string delim, string text);

unordered_map<string, pair<string, string>> readCFG(string fileName)
{
    unordered_map<string, pair<string, string>> cfgContents;
    fstream file = fstream(fileName);

    string thisLine="";
    char current;

    if(file.is_open())
    {
        while(file.peek()!=EOF)
        {
            current = file.get();
            
            if(current!=',' && current!='\n' && current!=NULL)
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

                cfgContents[name] = pair<string, string>(type, data);

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

    return pair<string, string>(left, right);
}

#endif