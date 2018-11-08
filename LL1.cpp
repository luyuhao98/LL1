#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <map>

#include <algorithm>
#include <typeinfo>
#include <iomanip>

using namespace std;

/*-------全局变量--------*/

/*文件读入流*/
ifstream fin("input.txt");

/*文件输出流*/
ofstream fout("output.txt");

/*非终结符对应项目*/
class item
{
        set<string> first;  //first set
        set<string> follow; //follow set
        /*产生式集合,每个产生式是string的集合*/
        vector<vector<string>> prods;
};

/*非终结符集合*/

map<string, item> N;

/*终结符集合*/
set<stirng> T;

/*-------函数区----------*/

/*读入文件，整理格式存入产生式*/
/*按行读取，跳过空行，跳过空格，词法分析，拆分多句*/
void read()
{
        string input;    //每行输入
        int i_input = 0; //input下标
        string str;      //待存符号
        char in;         //带判断字符

        int row = 0;
        while (getline(fin, input))
        {
                row++;
                if (!input.compare("")) //相等返回0
                        continue;       //遇到空行跳过

                /*---词法分析部---*/

                //1.获取产生式左部，限制必须为非终结符，即必须为大写字母。设置标记
                state = 0; //未获取产生式左部状态
                string left;
                item right;
                vector<input> prod; //产生式

                for (; i_input < input.size(); i_input++)
                {
                        //        str.clear();
                        in = input[i_input];
                        str.push_back(in);
                        if (in == ' ')
                        {
                                str.pop_back();
                                if (state == 1)
                                {
                                        cout << "ERROR: "
                                             << "row <" << row << "> column <" << i_input << ">"
                                             << "\"->\" is needed." << endl
                                             << "\"" << input << "\"" << endl;
                                        exit(1);
                                }
                                if (state == 2 && str.size())
                                {
                                        prod.push_back(str);
                                        str.clear();
                                }
                                continue;
                        }

                        if (state == 0)
                        {
                                //非终结符
                                if (in >= 'A' && in <= 'Z')
                                {
                                        left.push_back(in);
                                        str.clear();
                                        state = 1; //未获取
                                }
                                else
                                {
                                        cout << "ERROR: "
                                             << "row <" << row << "> column <" << i_input << ">"
                                             << "Illegal left!" << endl
                                             << "\"" << input << "\"" << endl;
                                        exit(1);
                                }
                        }
                        //读入 ->
                        else if (state == 1)
                        {
                                if (str.size() == 2 && !str.compare("->"))
                                {

                                        str.clear();
                                        state == 2;
                                }
                                else if (str.size() == 1 && !str.compare("-"))
                                {
                                        continue;
                                }
                                else
                                {
                                        cout << "ERROR: "
                                             << "row <" << row << "> column <" << i_input << ">"
                                             << "\"->\" Error!" << endl
                                             << "\"" << input << "\"" << endl;
                                        exit(1);
                                }
                        }
                        //词法分析右式
                        else if (state == 2)
                        {
                                //小写符号串，允许由a-z,0-9或下划线组成
                                if ((in >= 'a' && in <= 'z') || (in >= '0' && in <= '9') || in == '_')
                                {
                                        continue;
                                }
                                //单符号，如‘A’终结符 ，如‘*’ 非终结符，以及‘|’分界符`
                                else
                                {
                                        //如果只有单字符，str长度为1
                                        //此情况针对 小写符号串
                                        if (str.size() > 1)
                                        {
                                                str.pop_back(); //弹出单符号

                                                prod.push_back(str);
                                                str.clear();

                                                str.push_back(in); //收回单符号
                                        }
                                        //判断是否为‘|’
                                        if (in == '|')
                                        {
                                                //将产生式压入众产生式集合
                                                right.prods.push_back(prod);
                                                prod.clear();
                                                str.clear();
                                        }
                                        else
                                        {
                                                prod.push_back(str);
                                                str.clear();
                                        }
                                }
                        }
                }

                //句尾处理
                if (state == 2)
                {
                        //如果符号串不为空，将str结尾存入产生式prod
                        if (str.size())
                        {
                                prod.push_back(str);
                                str.clear();
                        }

                        //如果产生式不为空，则将产生式压入产生式集合
                        if (prod.size())
                        {
                                right.prods.push_back(prod);
                                prod.clear();
                        }
                        
                        //将左右部分压入非终结符map
                        N.insert(left, right);
                        left.clear();
                        right.prods.clear();
                }
                else if (state != 2)
                {
                        cout << "ILLEGAL LINE,WE JUST CAN'T MOVE ON";
                        exit(1);
                }
        }
}

int main(){
        
}