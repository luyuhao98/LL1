#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <map>
#include <iomanip>

// #include <algorithm>
// #include <typeinfo>

using namespace std;

/*-------全局变量--------*/

/*文件读入流*/
ifstream fin("input.txt");

// /*文件输出流*/
// ofstream fout("output.txt");

/*非终结符对应项目*/
class item
{
      public:
        set<string> first;  //first set
        set<string> follow; //follow set
        /*产生式集合,每个产生式是string的集合*/
        vector<vector<string>> prods;
        bool rfirst;
        bool rfollow;
        item()
        {
                rfirst = false;
                rfollow = false;
        }
};

/*原非终结符集合*/

map<string, item> Nin;

/*修改后终结符集合*/
map<string, item> Nmodified;

/*终结符集合*/
set<string> T;

/*-------函数区----------*/

/*读入文件，整理格式存入产生式*/
/*按行读取，跳过空行，跳过空格，词法分析，拆分多句*/
void read()
{
        string input; //每行输入
        string str;   //待存符号
        int i_input;  //input下标
        char in;      //带判断字符

        int row = 0;
        while (getline(fin, input))
        {
                row++;
                if (!input.compare("")) //相等返回0
                        continue;       //遇到空行跳过

                /*---词法分析部---*/

                //1.获取产生式左部，限制必须为非终结符，即必须为大写字母。设置标记
                int state = 0; //未获取产生式左部状态
                string left;
                item right;
                vector<string> prod; //产生式

                for (i_input = 0; i_input < input.size(); i_input++)
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
                                        T.insert(str);
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
                                        state = 2;
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
                                //单符号，如‘A’终结符 ，如‘*’ 非终结符，如‘|’分界符`,以及空字符表示’？‘
                                else
                                {
                                        //如果只有单字符，str长度为1
                                        //此情况针对 小写符号串
                                        if (str.size() > 1)
                                        {
                                                str.pop_back(); //弹出单符号

                                                prod.push_back(str);
                                                str.clear();
                                                T.insert(str);

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
                                                if (!((in >= 'A' && in <= 'Z') || in == '?'))
                                                        T.insert(str);
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
                                T.insert(str);
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
                        Nin.insert(pair<string, item>(left, right));
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
//输出产生式
void outProds(map<string, item> &N)
{
        map<string, item>::iterator iter = N.begin();
        while (iter != N.end())
        {
                vector<vector<string>>::iterator it1 = iter->second.prods.begin();
                while (it1 != iter->second.prods.end())
                {
                        cout << iter->first << "->";
                        vector<string>::iterator it2 = (*it1).begin();
                        while (it2 != (*it1).end())
                        {
                                cout << *it2;
                                it2++;
                        }
                        cout << endl;
                        it1++;
                }
                iter++;
        }
}

void outset()
{
        map<string, item>::iterator iter = Nmodified.begin();
        while (iter != Nmodified.end())
        {
                /*输出First集*/
                cout << "First set of < " << iter->first << " > : { ";
                set<string>::iterator it = iter->second.first.begin();
                while (it != iter->second.first.end())
                {
                        cout << *it << " ";
                        it++;
                }
                cout << "}" << endl;

                /*输出FOLLOW集*/
                cout << "Follow set of < " << iter->first << " > : { ";
                it = iter->second.follow.begin();
                while (it != iter->second.follow.end())
                {
                        cout << *it << " ";
                        it++;
                }
                cout << "}" << endl;

                iter++;
        }
}
//输出终结符
void outT()
{
        set<string>::iterator it = T.begin();
        cout << "----T----" << endl;

        while (it != T.end())
        {
                cout << *it << endl;
                it++;
        }
}

//消除左递归
void elimimateleftrecursion()
{
        map<string, item>::iterator iter = Nin.begin();
        while (iter != Nin.end())
        {
                vector<vector<string>>::iterator it = iter->second.prods.begin();
                int needEliminate = 0;
                vector<int> recurse;

                //定义Nmodified的左右
                while (it != iter->second.prods.end())
                {
                        if (!iter->first.compare((*it)[0]))
                        {
                                needEliminate = 1;
                                recurse.push_back(1); //A->Aa
                        }
                        else
                        {
                                recurse.push_back(0); //A->b
                        }
                        it++;
                }

                /*---Nmodified---*/
                //如果需要修改
                if (needEliminate)
                {
                        string left1(iter->first);
                        string left2(iter->first);
                        left2.push_back('\'');
                        item right1, right2;

                        for (int i = 0; i < recurse.size(); i++)
                        {
                                if (recurse[i])
                                {
                                        vector<string> prod;
                                        prod.assign(iter->second.prods[i].begin() + 1, iter->second.prods[i].end());
                                        prod.push_back(left2);
                                        right2.prods.push_back(prod);
                                }
                                else
                                {

                                        vector<string> prod(iter->second.prods[i]);
                                        if (prod.size() == 1 && !prod[0].compare("?")) //如果右部产生式为空则清空？
                                                prod.clear();
                                        prod.push_back(left2);
                                        right1.prods.push_back(prod);
                                }
                        }
                        Nmodified.insert(pair<string, item>(left1, right1));
                        Nmodified.insert(pair<string, item>(left2, right2));
                }
                else
                {
                        Nmodified.insert(*iter);
                }

                needEliminate = 0;
                recurse.clear();
                iter++;
        }
}

set<string> findfirst(string left) //如果fast为真则直接返回已经求好的first集
{
        //return set
        set<string> myset;

        //终结符或者空
        if (T.count(left) || !left.compare("?"))
        {
                myset.insert(left);
        }
        else
        {
                if (Nmodified[left].rfirst)
                { //快速返回first集
                        return Nmodified[left].first;
                }
                //非终结符
                item right(Nmodified[left]);
                //it1 产生式指针
                vector<vector<string>>::iterator it1 = right.prods.begin();
                while (it1 != right.prods.end())
                {
                        //it2 产生式内元素指针
                        vector<string>::iterator it2 = it1->begin();

                        while (it2 != it1->end())
                        {
                                set<string> get = findfirst(*it2);
                                myset.insert(get.begin(), get.end());

                                //如果含有空（？）并且不为最后一个，那么？需要被排除，否则结束循环。
                                if (!(get.count("?") && it2 != it1->end() - 1))
                                        break;
                                myset.erase("?");
                                it2++;
                        }
                        it1++;
                }
        }
        return myset;
}
void fisrt()
{
        map<string, item>::iterator iter = Nmodified.begin();
        while (iter != Nmodified.end())
        {
                set<string> get = findfirst(iter->first);
                iter->second.first.insert(get.begin(), get.end());
                iter->second.rfirst = true;
                iter++;
        }
}

set<string> findfollow(string n) //fast为true则直接返回求号的follow集合
{

        if (Nmodified[n].rfollow)
        {
                return Nmodified[n].follow;
        }
        //return set
        set<string> myset;

        map<string, item>::iterator iter = Nmodified.begin();
        while (iter != Nmodified.end())
        {
                vector<vector<string>>::iterator it1 = iter->second.prods.begin();
                while (it1 != iter->second.prods.end())
                {
                        vector<string>::iterator it2 = it1->begin();
                        bool isfind = 0;
                        while (it2 != it1->end())
                        { //it2是产生式子中的每一个符号
                                if (!isfind && !n.compare(*it2))
                                { //第一次找到非终结符号n
                                        isfind = 1;
                                        if (it2 == it1->end() - 1 && n.compare(iter->first))
                                        { //如果n是最后一个字符，且不为left
                                                set<string> get = findfollow(iter->first);
                                                myset.insert(get.begin(), get.end());
                                        }
                                }
                                //else：互斥关系：上面找到后直接跳到下一个，不判端isfind
                                else if (isfind)
                                {
                                        set<string> get = findfirst(*it2);
                                        myset.insert(get.begin(), get.end());

                                        //如果不含空，则直接退出循环
                                        if (!(get.count("?")))
                                                break;
                                        //如果含空且为最后一个，且n不为left,即A->αBβ中，A!=B，?∈first(β),那么follow（A）插入follow（B）中
                                        else if (it2 == it1->end() - 1)
                                        {
                                                myset.erase("?");
                                                set<string> get = findfollow(iter->first);
                                                myset.insert(get.begin(), get.end());
                                        }
                                        //如果含有空且不为最后一个,或者含有空为最后一个但是n为left,那么去掉空后，继续循环。
                                        else
                                        {
                                                myset.erase("?");
                                        }
                                }
                                it2++;
                        }
                        it1++;
                }
                iter++;
        }

        return myset;
}
void follow()
{
        map<string, item>::iterator iter = Nmodified.begin();
        iter->second.follow.insert("$"); //第一个插$
        while (iter != Nmodified.end())
        {
                set<string> get = findfollow(iter->first);
                iter->second.follow.insert(get.begin(), get.end());
                iter->second.rfollow = true;
                iter++;
        }
}
int main()
{
        read();
        elimimateleftrecursion();
        outProds(Nmodified);
        fisrt();
        follow();
        outset();
        outT();
}