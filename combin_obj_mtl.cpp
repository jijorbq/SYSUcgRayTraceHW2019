#include <iostream>
#include<vector>
#include<map>
#include <fstream>
#include<sstream>

std::vector<std::vector<float>>tomap;//需要映射的二维数组

struct material
{
    std::vector<float>Ka;//环境反射
    std::vector<float>Kd;//环境反射
    std::vector<float>Ks;//环境反射
    //不同的mtl不同，有些mtl文件没有下面所说的三种变量，所以给了初始值。
    float Ns=0;//反射指数
    float Ni=0;//折射率
    float d=0;//衰退率
    int illum=0;
};
std::ostream& operator<<(std::ostream& out, const material& s)
{
    out<<"Ns: "<<s.Ns<<"  Ni: "<<s.Ni<<"  d:  "<<s.d<<"  illum:"<<s.illum<<std::endl;
    return out;
}
std::ostream& operator<<(std::ostream& out,std::vector<float>&mat)
{
    for(int i=0;i<mat.size();++i)
    {
        out<<mat[i]<<"  ";
    }
    out<<std::endl;
    return out;
}
std::ostream& operator<<(std::ostream& out,std::vector<int>&mat)
{
    for(int i=0;i<mat.size();++i)
    {
        int index=mat[i]-1;
        for(int j=0;j<tomap[index].size();++j)
        {
            out<<tomap[index][j]<<"  ";
        }
        out<<std::endl;
    }
    out<<std::endl;
    return out;
}
void combine(std::string objfile,std::string mtlfile)
{

    std::map<std::string,material>Materials;
    std::ifstream infile(mtlfile);
    std::string sline;//每一行
    if (! infile.is_open())
    {
        std::cout << "Error   opening file";
    }
    std::string nowmaterial;
    float t1,t2,t3;
    int t4;
    while(getline(infile,sline))
    {
        std::istringstream in(sline);
        std::string prefix;
        in>>prefix;
        if(prefix=="newmtl")
        {
            in>>nowmaterial;
        }
        else if(prefix=="Ns")
        {
            in>>t1;
            Materials[nowmaterial].Ns=t1;
        }
        else if(prefix=="Ni")
        {
            in>>t1;
            Materials[nowmaterial].Ni=t1;
        }
        else if(prefix=="d")
        {
            in>>t1;
            Materials[nowmaterial].d=t1;
        }
        else if(prefix=="illum")
        {
            in>>t4;
            Materials[nowmaterial].illum=t4;
        }
        else if(prefix=="Ka")
        {
            in>>t1>>t2>>t3;
            Materials[nowmaterial].Ka={t1,t2,t3};
        }
        else if(prefix=="Kd")
        {
            in>>t1>>t2>>t3;
            Materials[nowmaterial].Kd={t1,t2,t3};
        }
        else if(prefix=="Ks"||prefix=="Tf")
        {
            in>>t1>>t2>>t3;
            Materials[nowmaterial].Ks={t1,t2,t3};
        }
    }
//    for(auto i:Materials)
//    {
//        std::cout<<i.second;
//        std::cout<<"Ka: "<<i.second.Ka;
//        std::cout<<"Kd: "<<i.second.Kd;
//        std::cout<<"Ks: "<<i.second.Ks;
//    }
    //obj-loader
    std::vector<std::vector<float>>vArr;//存放点的二维数组
    std::vector<std::vector<float>>vnArr;//存放法线的二维数组
    std::vector<std::vector<float>>vtArr;//存放纹理的二维数组
    std::vector<std::vector<int>>fvArr;//存放面顶点的二维数组
    std::vector<std::vector<int>>ftArr;//存放面纹理的二维数组
    std::vector<std::vector<int>>fnArr;//存放面法线的二维数组
    
    
    std::ifstream file(objfile);
    if (! infile.is_open())
    {
        std::cout << "Error   opening file";
    }
    while(getline(file,sline))
    {
        //std::cout<<sline<<std::endl;
        std::istringstream in(sline);
        std::string prefix;
        in>>prefix;
        if(prefix=="v")
        {
            float f1,f2,f3;
            in>>f1>>f2>>f3;
            std::vector<float>temp={f1,f2,f3};
            vArr.push_back(temp);
        }
        else if(prefix=="vt")
        {
            float f1,f2;
            in>>f1>>f2;
            std::vector<float>temp={f1,f2};
            vtArr.push_back(temp);
        }
        else if(prefix=="vn")
        {
            float f1,f2,f3;
            in>>f1>>f2>>f3;
            std::vector<float>temp={f1,f2,f3};
            vnArr.push_back(temp);
        }
    }
    std::cout<<"!!!!!"<<vnArr.size();
    std::ifstream newfile(objfile);
    if (! infile.is_open())
    {
        std::cout << "Error   opening file";
    }
    while(getline(newfile,sline))
    {
        
        std::istringstream in(sline);
        std::string prefix;
        in>>prefix;
        if(prefix=="usemtl")
        {
            in>>nowmaterial;
        }
        else if(prefix=="f")
        {
            //std::cout<<sline<<std::endl;
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
            
            int index=(int)fvArr.size();
            std::cout<<"************Face"<<index<<"********"<<std::endl;
            tomap=vArr;
            std::cout<<"Vertex:"<<std::endl;
            std::cout<<fvArr[index-1];
            
            tomap=vnArr;
            std::cout<<"normal:"<<std::endl;
            std::cout<<fnArr[index-1];
            
            std::cout<<Materials[nowmaterial];
            std::cout<<"Ka: "<<Materials[nowmaterial].Ka;
            std::cout<<"Kd: "<<Materials[nowmaterial].Kd;
            std::cout<<"Ks: "<<Materials[nowmaterial].Ks;
            
        }
    }
}
        
int main()
{
    std::string objfile="/Users/liusiyu/Downloads/3_up/CG/objs/pig/pig.obj";
    std::string mtlfile="/Users/liusiyu/Downloads/3_up/CG/objs/pig/pig.mtl";
    combine(objfile, mtlfile);
}
