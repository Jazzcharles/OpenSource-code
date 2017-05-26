#include<bits/stdc++.h>
#define PB push_back
using namespace std;
char opera[10]={'=','+','-','*','/','_'};
int n;
int cnt;
map<string,int> mp;
//�ñ�����������Ӧ�Ľ���� 
map<int,int> numbermp;
//�����ֶ�Ӧ�Ľ���� 

/*
Solved:
1.�������Ӧͬһ����
2.���ݱ�����ֵ������һ��������ֵ 

*/

struct Tac{
	string str;		//��Ԫʽ������ʽ�� 
	int id;			//��� 
	int operation;	//���� 
	string op1;		//������1 
	int num1;		//��������1Ϊ���� 
	string op2;		//������2 
	int num2;		//��������2Ϊ���� 
	string res;		//��� 
	int ans;		//�����Ϊ���� 
}t[100];


struct node{
	int id;			//������ 
	int operation;	//���� 
					
	int type;		//type=0:����num,type=1:����var 
	int num;		//����������� 
	vector<string> var;	
					//����������� 
	int lchild;		//���Һ������ 
	int rchild;
	
	bool operator < (const node& y) const{
		return id<y.id;
	}
	
	node(){
		id = 0;
		operation = 0;
		type = 0;
		num = 0;
		var.clear();
		lchild = 0;
		rchild = 0;
	}
};
vector<node> Dag;


//vector<node> nd;

void debug(){
	for(int i = 1;i < n; i++){
		cout<<t[i].str<<' '<<t[i].id<<' '<<t[i].op1<<' '<<opera[t[i].operation]<<' '<<t[i].op2<<' '<<t[i].res<<endl;
	}
} 
void superdebug(){
	for(int i = 0 ; i < Dag.size();i++){
		cout<<"id = "<<Dag[i].id<<" type = "<<Dag[i].type<<" num = "<<Dag[i].num;
		cout<<" var_num = "<<Dag[i].var.size()<<" first_var = "<<Dag[i].var[0];
		cout<<" lc = "<<Dag[i].lchild<<" rc = "<<Dag[i].rchild;
		cout<<" op "<<opera[Dag[i].operation]<<' '<<mp["T0"]<<endl;
	}
	
}

void process(Tac &stm){
	string s = stm.str;
	char op = s[1];
	for(int i=0;i<5;i++)
		if(op==opera[i]) stm.operation = i;
	int ptr = 3;
	while(s[ptr]==' ') ptr++;
	while(s[ptr]!=',') stm.op1+=s[ptr++];
	ptr++;
	while(s[ptr]==' ') ptr++;
	while(s[ptr]!=',') stm.op2+=s[ptr++];
	ptr++;
	while(s[ptr]==' ') ptr++;
	while(s[ptr]!=')') stm.res+=s[ptr++];
	
	if(stm.operation == 1 && stm.op2==" ") stm.operation = 5;
}

void read(){
	freopen("exp6_2.in","r",stdin);
	char s[100];
	n = 1;
	while(gets(s)){
		for(int i=0;i<strlen(s);i++){
			t[n].str += s[i];
		}			
		t[n].id = n;	
		process(t[n]);
		n++;
	}
	debug();
}


node & fiDag_node(int id){
	for(int i=0;i<Dag.size();i++){
		if(Dag[i].id == id) return Dag[i];
	}
} 


//������������s�Ƿ��Ѿ����ڣ�flag = 1���� 
node new_node_var(string s,int &flag){
	if(mp[s]) {
		flag = 1;
		int id = mp[s];
		return fiDag_node(id);
	}	
	else{
		node tmp;	
		//cout<<"var = "<<s<<' '<<"cnt = "<<cnt<<endl; 
		tmp.type = 1;
		tmp.var.push_back(s);
		mp[s] = cnt;
		tmp.id = cnt;
		flag = 0;
		cnt++;
		Dag.push_back(tmp);
		return tmp;
	}	
}


//������������s�Ƿ��Ѿ����ڣ�flag = 1���� 
node new_node_num(string str,int &flag){
	int s = 0;
	for(int i=0;i<str.length();i++) s = s*10+str[i]-'0'; 
	if(numbermp[s]) {
		flag = 1;
		int id = numbermp[s];
		return fiDag_node(id);
	}	
	else{
		node tmp;
		tmp.num = s;
		tmp.type = 0;
		//cout<<"num = "<<tmp.num<<' '<<"cnt = "<<cnt<<endl;
		numbermp[s] = cnt;
		tmp.id = cnt;
		flag = 0;
		cnt++;
		Dag.push_back(tmp);
		return tmp;
	}	
}




void pop_node(node &n1){
	Dag.pop_back();
	cnt--;
	if(n1.type) 
		for(int i = 0;i<n1.var.size();i++) mp[n1.var[i]] = 0;
	else
		numbermp[n1.num] = 0;
}

void erase_node(string str){
	queue<node> q;
	for(int i = 0 ; i < Dag.size(); i++){
		if(!Dag[i].var.size() || Dag[i].var[0] != str) q.push(Dag[i]);
	}
	Dag.clear();
	while(!q.empty()){
		node temp = q.front();
		q.pop();
		Dag.PB(temp); 		
	}	
}

void relocate_node(string str){	
	int id = mp[str];
	if(!id) return ;
	
	mp[str] = 0;
	int szflag = 0;//�ý��ֻ�����Լ�����һ�������� 
	int nodeloc = -1;
	int strloc = -1;
	for(int i = 0 ; i < Dag.size(); i++){
		int sz = Dag[i].var.size();
		for(int j = 0; j < sz; j++){
			if(Dag[i].var[j] == str){
				if(sz == 1){
					szflag = 1;
				}
				nodeloc = i;
				strloc = j;
				break;
			}
		}
		if(nodeloc != -1) break;
	}
	if(nodeloc == -1) return ;
	//����ǰ���ֻ����һ�������� 
	if(szflag){
		for(int i = 0 ; i < Dag.size(); i++){
			//����ǰ��ĳ�����ӽڵ㲻��ɾ�� 
			if(Dag[i].lchild == id || Dag[i].rchild == id){
			//	cout<<"��ĳ���ڵ���ӽڵ�"<<' '<<id<<' '<<str<<endl;
				return ;
			}
		}
		//cout<<"���ӽڵ�"<<' '<<str<<endl;
		erase_node(str); 
	}
	//�ӵ�ǰ���Ķ��������ɾ������str 
	else{
		queue<string> qq;
		for(int j = 0 ; j < strloc ; j++){
			string stemp = Dag[nodeloc].var[j];
			qq.push(stemp);
		}
		for(int j = strloc+1; j < Dag[nodeloc].var.size(); j++){
			string stemp = Dag[nodeloc].var[j];
			qq.push(stemp);
		}
		Dag[nodeloc].var.clear();
		while(!qq.empty()){
			string stemp = qq.front();
			qq.pop();
			Dag[nodeloc].var.PB(stemp);
		}
	}
}




int cal(int a,int b,int op){
	char opr = opera[op];
	if(opr == '+') return a+b;
	if(opr == '-') return a-b;
	if(opr == '*') return a*b;
	if(opr == '/') return a/b;
}

node new_node(string str,int &flag){
	int f = 0; 
	node tmp;
	if(isdigit(str[0])) tmp = new_node_num(str,f);
	else tmp = new_node_var(str,f);
	flag = f;	
	return tmp;
}


void add_edge(Tac &stm){
	if(!stm.operation){//��ֵ 
		int flag = 0;
		node n1 = new_node(stm.op1,flag);
		//�����Ƿ���ֹ��������ֹ��Ƿ����� 
		relocate_node(stm.res);
		node &tmp = fiDag_node(n1.id);
		tmp.var.PB(stm.res);
		mp[stm.res] = tmp.id;	
	}	
	else{//���� 
		int flag1 = 0;
		int flag2 = 0;
		node n1 = new_node(stm.op1,flag1);
		node n2 = new_node(stm.op2,flag2);
		
		//�����������������ֻ�������ı��� 
		if(!n1.type && !n2.type){
			//cout<<"All op are numbers"<<endl;
			int p = cal(n1.num,n2.num,stm.operation);
			if(!flag2) pop_node(n2);
			if(!flag1) pop_node(n1);
			if(!numbermp[p]) {
				int flag = 0;
				node ret = new_node(stm.res,flag);
				numbermp[p] = ret.id;
				ret.num = p;
				ret.type = 0;
				Dag.pop_back();
				Dag.PB(ret);	
			}
			else{
				int id = numbermp[p];
				node &temp = fiDag_node(id);
				temp.var.PB(stm.res);
				mp[stm.res] = id;
			}
		}
		//������������ͬʱΪ���� 
		else{ 
			//cout<<"Not all numbers Debugging"<<endl;
			int flag = 0;
			node ret = new_node(stm.res,flag);
			ret.lchild = n1.id;
			ret.rchild = n2.id;	
			ret.operation = stm.operation;
			//��������Ѵ��ڽ�� 
			if(!flag) { 
				Dag.pop_back();
				int fouDag = -1;	
				//�����������Ƿ��Ѿ����й����� 
				for(int i = 0; i < Dag.size();i++){
					if(Dag[i].lchild && Dag[i].rchild 
					   && Dag[i].lchild == ret.lchild && Dag[i].rchild == ret.rchild 
					   && Dag[i].operation == ret.operation){
						fouDag = i;
						break;
					}
				}
				if(fouDag != -1){
					Dag[fouDag].var.PB(stm.res);
					mp[stm.res] = Dag[fouDag].id;	
				}
				else{
					Dag.push_back(ret);	
				}				
			}
			else{
				cout<<"TAT"<<endl;
				//����Ѿ����� ,ɾ�����еĽ�㣬�½���㲢�������Һ��� 
				relocate_node(stm.res);
				int flag = 0;
				node freshnode = new_node(stm.res,flag);
				freshnode.lchild = n1.id;
				freshnode.rchild = n2.id;
				freshnode.operation = stm.operation;
				Dag.pop_back();
				Dag.PB(freshnode);
			}
		}	
	}
	//superdebug();
	//cout<<endl<<endl;
}



void solve(){
	for(int i = 1; i < n; i++){
		add_edge(t[i]);
	}
	cout<<endl<<"---ready to debug---"<<endl;
	superdebug();
	
	
	
	
	
}


void display(){
	for(int i = 0 ; i < Dag.size(); i++){
		if(!Dag[i].operation && !Dag[i].type){
			int sz = Dag[i].var.size();
			int num = Dag[i].num;
			for(int j = 0; j < sz; j++){
				cout<<"(=,"<<num<<", ,"<<Dag[i].var[j]<<")"<<endl;
			}
		}
		if(Dag[i].operation){
			int sz = Dag[i].var.size();
			string first_var = Dag[i].var[0];
			int lchild = Dag[i].lchild;
			int rchild = Dag[i].rchild;
			string lc = fiDag_node(lchild).var[0];
			string rc = fiDag_node(rchild).var[0];
			cout<<"("<<opera[Dag[i].operation]<<","<<lc<<","<<rc<<","<<first_var<<")"<<endl;
			for(int j = 1; j < sz;j++){
				string tmp_var = Dag[i].var[j];
				cout<<"(=,"<<first_var<<", ,"<<tmp_var<<")"<<endl;
			} 
		}
	}		
}

int main(){
	cnt = 1;
	read();
	solve();
	display();
	
	
	
	
	
	
	
	return 0;
} 
/*
(=,3, ,T0)
(*,2,T0,T1)
(+,R,r,T2)
(*,T1,T2,A)
(=,A, ,B)
(*,2,T0,T3)
(+,R,r,T4)
(*,T3,T4,T5)
(-,R,r,T6)
(*,T5,T6,B)

(*,A,B,T1)
(/,6,2,T2)
(-,T1,T2,T3)
(=,T3, ,X)
(=,5, ,C)
(*,A,B,T4)
(=,2,  ,C)
(+,18,C,T5)

(-,A,C,D)
(*,A,C,E)
(*,D,E,F)
(=,2, ,S)
(-,A,C,T)
(*,A,C,Q)
(*,2,S,G)
(*,T,Q,J)
(*,G,5,K)
(+,K,J,L)
(=,L, ,M)
*/
