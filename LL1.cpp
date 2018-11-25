#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <map>
#include <iomanip>
#include <stack>
#include <algorithm>
//
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

                                                T.insert(str);
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
        cout << "---------" << endl;
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
                                        if (prod.size() == 1 && !prod[0].compare("?")) //如果右部产生式为空则清空?: 例如E->Ea|? => (1) E->E' (2) E'->aE'|?
                                                prod.clear();
                                        prod.push_back(left2);
                                        right1.prods.push_back(prod);
                                }
                        }
                        vector<string> prod;
                        prod.push_back("?");
                        right2.prods.push_back(prod);
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
//重载函数:串first 的声明
set<string> findfirst(vector<string>::iterator its, vector<string>::iterator ite);

//寻找符号的first集合
set<string> findfirst(string left)
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
                        //调用函数：返回串的first集合
                        set<string> get = findfirst(it1->begin(), it1->end());
                        myset.insert(get.begin(), get.end());

                        it1++;
                }
        }
        return myset;
}

//返回串的first集合
set<string> findfirst(vector<string>::iterator its, vector<string>::iterator ite)
{
        vector<string>::iterator it = its;
        set<string> myset;

        while (it != ite)
        {
                set<string> get = findfirst(*it);

                //优化：上一步已经找到*it的first集合，如果*i2为非终结符,那么就清算*it的first集合，并将*it的rfirst位标志为true
                if (!T.count(*it) && (*it).compare("?"))
                {
                        Nmodified[*it].first.insert(get.begin(), get.end());
                        Nmodified[*it].rfirst = true;
                }
                myset.insert(get.begin(), get.end());

                //如果含有空（？）并且不为最后一个，那么？需要被排除，否则结束循环。
                if (!(get.count("?") && it != ite - 1))
                        break;
                myset.erase("?");
                it++;
        }

        return myset;
}

void first()
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

set<string> findfollow(string n)
{
        //如果已经找到follow集合，则直接返回
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
                                //else：互斥关系：上面找到后直接跳到下一个，不判断isfind
                                else if (isfind)
                                {
                                        //取后面字符串的first集
                                        set<string> get = findfirst(it2, it1->end());
                                        myset.insert(get.begin(), get.end());
                                        //如果含空字符,则将空(?)排除后，将产生式左侧非终结符的follow集加入
                                        if (get.count("?"))
                                        {
                                                myset.erase("?");
                                                set<string> get = findfollow(iter->first);
                                                myset.insert(get.begin(), get.end());
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

//Forecast analysis table 预测分析表
map<string, map<string, vector<string>>> FAT;
//构造预测分析表
void makeFAT()
{
        auto iter = Nmodified.begin();
	vector<string> synch;
	synch.push_back("synch");
        while (iter != Nmodified.end())
        {
                auto it1 = iter->second.prods.begin();
                //it1代表每一个产生式
                while (it1 != iter->second.prods.end())
                {
                        //get为每个产生式的first集合
                        auto get = findfirst(it1->begin(), it1->end());
                        auto it2 = get.begin();
                        bool hasempty = false;
                        while (it2 != get.end())
                        {
                                //it2非空，即终结符号时
                                if ((*it2).compare("?"))
                                {
					if(!FAT[iter->first].count(*it2)){
				      		FAT[iter->first][*it2] = *it1;
					}
					else {
						cout<<"["<<iter->first<<","<<*it2<<"]"<<"处表项冲突!"<<endl;
						exit(0);
					}
                                }
                                else
                                {
                                        hasempty = true;
                                }
                                it2++;
                        }
                        if (hasempty)
                        {
                                get = findfollow(iter->first);
                                it2 = get.begin();
                                while (it2 != get.end())
                                {	
					if(!FAT[iter->first].count(*it2)){
				  		FAT[iter->first][*it2] = *it1;
					}
					else {
						cout<<"["<<iter->first<<","<<*it2<<"]"<<"处表项冲突!"<<endl;
						exit(0);
					}
                                        it2++;
                                }
                        }
                        it1++;
                }

		auto get = findfollow(iter->first);
		auto it2 = get.begin();
		while(it2!=get.end()){
			if(!FAT[iter->first].count(*it2)){
				FAT[iter->first][*it2] = synch;
			}
			it2++;
		}
                iter++;
        }
}
void outFAT()
{

        cout << endl
             << "Forecast analysis Table :" << endl;
        vector<string> y(T.begin(), T.end());
        y.push_back("$");
        auto ity = y.begin();
        cout << "| " << setiosflags(ios::left) << setw(5) << "";
        while (ity != y.end())
        {
                cout << "| " << setw(10) << *ity;
                ity++;
        }
        cout << "|" << endl;
        for (int i = 0; i < (T.size() + 1) * 12 + 8; i++)
        {
                cout << "-";
        }
        cout << endl;
        auto itx = Nmodified.begin();
        while (itx != Nmodified.end())
        {
                cout << "| " << setw(5) << itx->first;
                ity = y.begin();
                while (ity != y.end())
                {
                        if (FAT[itx->first].count(*ity))
                        {
                                string stemp(itx->first);
                                stemp += "->";
                                //cout<<itx->first<<"->";
                                auto it1 = FAT[itx->first][*ity];
                                auto it2 = it1.begin();
                                while (it2 != it1.end())
                                {
                                        stemp += *it2;
                                        it2++;
                                }
                                cout << "| " << setw(10) << stemp;
                        }
                        else
                        {
                                cout << "| " << setw(10) << "ERROR";
                        }
                        ity++;
                }
                itx++;
                cout << "|" << endl;
        }
}
//输入栈
vector<string> inputstack;

int getinputstack()
{
        //清空栈
        inputstack.clear();
        //输入字符串
        string input;
        cin >> input;
        //符号str
        string str;
        for (int i = 0; i < input.size(); i++)
        {
                char in = input[i];
                str.push_back(in);
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

                                //若str不存在则报错，return
                                if (!T.count(str))
                                {
                                        cout << str << "为无效符号,请重新输入" << endl;
                                        return 0;
                                }
                                inputstack.push_back(str);
                                str.clear();
                                str.push_back(in); //收回单符号
                        }
                        inputstack.push_back(str);
                        str.clear();
                }
        }
        if (str.size())
        {
                if (!T.count(str))
                {
                        cout << str << "为无效符号,请重新输入" << endl;
                        return 0;
                }
                inputstack.push_back(str);
                str.clear();
        }
        inputstack.push_back("$");
        //输入栈逆转

        reverse(inputstack.begin(), inputstack.end());
        return 1;
}
void analysis()
{
        vector<string> astack; //分析栈
        astack.push_back("$");
        astack.push_back(Nmodified.begin()->first);
        //满足分析栈或输入栈为空则结束
        while (!(astack.empty() || inputstack.empty()))
        {
                //带输出串
                string str;

                //打印分析栈
                auto it = astack.begin();
                while (it != astack.end())
                {
                        str += *it;
                        it++;
                }
                cout << setiosflags(ios::right) << setw(15) << str;
                str.clear();
                //反向打印输入栈
                auto it1 = inputstack.rbegin();
                while (it1 != inputstack.rend())
                {
                        str += *it1;
                        it1++;
                }
                cout << setiosflags(ios::right) << setw(25) << str;
                str.clear();

                //如果栈顶相等，此时肯定两个栈顶为两个终结符或$
                if (!(astack.end() - 1)->compare(*(inputstack.end() - 1)))
                {
                        astack.pop_back();
                        inputstack.pop_back();
                }
                //否则当左侧栈顶为非终结符,且列表中存在时
                else if (FAT.count(*astack.rbegin()) && FAT[*astack.rbegin()].count(*(inputstack.rbegin())))
                {
                        //如果为synch
                        if (!FAT[*astack.rbegin()][*inputstack.rbegin()].begin()->compare("synch"))
                        {
        			str+="Error,[";
				str+=*astack.rbegin();
				str+=",";
				str+=*inputstack.rbegin();
				str+="]=synch,弹出分析栈顶";
				str+=*astack.rbegin();
	      			cout << setiosflags(ios::right) << setw(15) << str;
				astack.pop_back();

                        }
                        //如果为表达式时
                        else
                        {
                                //打印输出，即分析过程
                                str += *(astack.end() - 1);
                                str += "—>";
                                it = FAT[*(astack.end() - 1)][*(inputstack.end() - 1)].begin();
                                while (it != FAT[*(astack.end() - 1)][*(inputstack.end() - 1)].end())
                                {
                                        str += *it;
                                        it++;
                                }
                                cout << setiosflags(ios::right) << setw(15) << str;
                                str.clear();

                                string needpop(*astack.rbegin());
                                astack.pop_back();
                                it1 = FAT[needpop][*inputstack.rbegin()].rbegin();

                                while (it1 != (FAT[needpop][*inputstack.rbegin()]).rend())
                                {
                                        if(it1->compare("?"))
                                                astack.push_back(*it1);
                                        it1++;
                                }
                        }
                }
                //如果左侧栈顶为非终结符，表中不存在，则按ERROR处理
                else if (FAT.count(*astack.rbegin()) && !FAT[*astack.rbegin()].count(*(inputstack.rbegin())))
                {
			str+="Error,[";
			str+=*astack.rbegin();
			str+=",";
			str+=*inputstack.rbegin();
			str+="]=空白，弹出输入栈顶";
			str+=*inputstack.rbegin();
                        cout << setiosflags(ios::right) << setw(15) << str;
			inputstack.pop_back();

                }
                //即，如果左侧栈顶为终结符或$，但是与右侧栈顶不相同,则将左侧栈顶弹出。
                else
                {
			str+="Error,分析栈顶";
			str+=*astack.rbegin();
			str+="与输入栈顶";
			str+=*inputstack.rbegin();
			str+="不匹配，故弹出分析栈顶";
                        cout << setiosflags(ios::right) << setw(15) << str;
			astack.pop_back();
                }
                cout<<endl;
        }
	if(inputstack.empty()&&astack.empty()){
		cout<<"分析成功!"<<endl;
	}
	else{
		cout<<"分析失败"<<endl;
	}
}
int main()
{
        read();
        elimimateleftrecursion();
        outProds(Nmodified);
        first();
        follow();
        outset();
        outT();
        makeFAT();
        outFAT();
        while (!getinputstack());
        analysis();
}
