篮球：
https://www.turbosquid.com/3d-models/basketball-ball-3d-model-1460117
机器猫:
https://www.turbosquid.com/3d-models/ready-3d-1214995


读取obj文件的代码:(多次测试没有问题)
```c++
std::vector<std::vector<GLfloat>>vArr;//存放点的二维数组
std::vector<std::vector<GLfloat>>vnArr;//存放法线的二维数组
std::vector<std::vector<GLfloat>>vtArr;//存放纹理的二维数组
std::vector<std::vector<int>>fvArr;//存放面顶点的二维数组
std::vector<std::vector<int>>ftArr;//存放面纹理的二维数组
std::vector<std::vector<int>>fnArr;//存放面法线的二维数组
void MyGLWidget::read(std::string addrstr)
{

    std::string s1;
    GLfloat f2,f3,f4;

    std::ifstream infile(addrstr);
    std::string sline;//每一行
    if (! infile.is_open())
    {
        std::cout << "Error   opening file";
    }
    while(getline(infile,sline))
    {
        //std::cout<<sline<<std::endl;
        std::istringstream in(sline);
        std::string prefix;
        in>>prefix;
        if(prefix=="v")
        {
            GLfloat f1,f2,f3;
            in>>f1>>f2>>f3;
            std::vector<GLfloat>temp={f1,f2,f3};
            vArr.push_back(temp);
        }
        else if(prefix=="vt")
        {
            GLfloat f1,f2;
            in>>f1>>f2;
            std::vector<GLfloat>temp={f1,f2};
            vtArr.push_back(temp);
        }
        else if(prefix=="vn")
        {
            GLfloat f1,f2,f3;
            in>>f1>>f2>>f3;
            std::vector<GLfloat>temp={f1,f2,f3};
            vnArr.push_back(temp);
        }
        else if(prefix=="f")
        {
            std::cout<<sline<<std::endl;
            std::vector<int>vt;
            std::vector<int>tt;
            std::vector<int>nt;
            while(1)
            {
                std::string m;
                in>>m;
                if(m=="")
                    break;
                int a=0;
                int k=0;
                while(m[k]!='/')
                {
                    a=a*10+m[k]-'0';
                    k++;
                }
                vt.push_back(a);
                a=0;
                ++k;
                while(m[k]!='/')
                {
                    a=a*10+m[k]-'0';
                    k++;
                }
                tt.push_back(a);
                a=0;
                ++k;
                while(m[k])
                {
                    a=a*10+m[k]-'0';
                    k++;
                }
                nt.push_back(a);
            }
            fvArr.push_back(vt);
            ftArr.push_back(tt);
            fnArr.push_back(nt);
        }

    }
}
```