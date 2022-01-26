#ifndef S_H
#define S_H

class shader
{
private:
    string shaderData;
    string vertexShader = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    unsigned int shaderID;
    unsigned int shaderProgramID;

public:
    shader()
    {
        if(shaderID!=0)
        {
            glDeleteProgram(shaderProgramID);
        }

        shaderData = "";
        shaderID = 0;
        shaderProgramID = 0;

        rgbShader(0, 0, 0, 0);
    }

    shader(int r, int g, int b, int a)
    {
        shader();
        rgbShader(r, g, b, a);
    }
    
    ~shader()
    {
        glDeleteProgram(shaderProgramID);
        cachedColors[rgbString()] = 0;
        colorExists[rgbString()] = false;
    }

    void compileShader()
    {
        shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        shaderProgramID = glCreateProgram();

        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        glShaderSource(shaderID, 1, shaderData, NULL);
        glCompileShader(shaderProgramID);
        
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, shaderProgram);

        glLinkProgram(shaderProgramID);
    }

    unsigned int getShader()
    {
        return shaderProgramID;
    }
    
    void rgbShader(int r, int g, int b, int a)
    {
        shaderData = "";
        shaderData += "#version 330 core\n";
        shaderData += "out vec4 FragColor;\n";
        shaderData += "void main()\n";
        shaderData += "{\n";
        shaderData += "     FragColor = vec4(";

        float rFloat = (float)(r) / 255.0f;
        float gFloat = (float)(g) / 255.0f;
        float bFloat = (float)(b) / 255.0f;
        float aFloat = (float)(a) / 255.0f;

        shaderData.append(rFloat);
        shaderData += "f, ";
        shaderData.append(gFloat);
        shaderData += "f, ";
        shaderData.append(bFloat);
        shaderData += "f, ";
        shaderData.append(aFloat);

        shaderData += "f);\n"
        shaderData += "}\n\0";

    }

    void overrideShader(string newShader)
    {
        shaderData = newShader;
    }

    string rgbString()
    {
        string result = "";
        result += r;
        result += ",";
        result += g;
        result += ",";
        result += b;
        result += ",";
        result += a;
        return result;
    }

    void cacheShader()
    {
        if(!colorExists[rgbString()])
        {
            cachedColors[rgbString()] = shaderProgramID;
            colorExists[rgbString()] = true;
        }
    }

};

#endif