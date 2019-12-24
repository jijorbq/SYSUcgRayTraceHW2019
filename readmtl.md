### 这是用来读取对应的mtl文件的，mtl主要是一些材料的性质，如漫反射、镜面反射等的系数

```c++

#include<fstream>
#include<istream>
#include <string>
#include <vector>

void readmlt(std::string path){
    std::ifstream filein(path);
    std::string line;

    while(std::getline(filein,line)){
        std::istringstream in(sline);
        std::string prefix;
        in>>prefix;
        if(prefix=="newmtl"){
            std::string temp;
            in>>temp;
            name.push_back(temp);
            continue;
        }
        if(prefix=="Ns"){
            GLfloat temp;
            in >> temp;
            Ns.push_back(temp);
            continue;
        }
        if(prefix=="Ka"){
            std::vector<GLfloat> temp;
            GLfloat num;
            in>>num;
            temp.push_back(num);
            in>>num;
            temp.push_back(num);
            in>>num;
            temp.push_back(num);
            Ka.push_back(temp);
            continue;
        }
        if(prefix=="Kd"){
            std::vector<GLfloat> temp;
            GLfloat num;
            in>>num;
            temp.push_back(num);
            in>>num;
            temp.push_back(num);
            in>>num;
            temp.push_back(num);
            Kd.push_back(temp);
            continue;
        }
        if(prefix=="Ks"){
            std::vector<GLfloat> temp;
            GLfloat num;
            in>>num;
            temp.push_back(num);
            in>>num;
            temp.push_back(num);
            in>>num;
            temp.push_back(num);
            Ks.push_back(temp);
            continue;
        }
        if(prefix=="Ke"){
            std::vector<GLfloat> temp;
            GLfloat num;
            in>>num;
            temp.push_back(num);
            in>>num;
            temp.push_back(num);
            in>>num;
            temp.push_back(num);
            Ke.push_back(temp);
            continue;
        }
        if(prefix=="Ni"){
            GLfloat temp;
            in >> temp;
            Ni.push_back(temp);
            continue;
        }
        if(prefix=="d"){
            GLfloat temp;
            in >> temp;
            d.push_back(temp);
            continue;
        }
    }
}
```