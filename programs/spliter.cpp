#ifndef SPLITER_CPP
#define SPLITER_CPP

#include<exception>
#include<vector>
#include<string>
#include<stack>
#include<iostream>

#define shiftState \
if(is_useless(code[i])){\
}else if(code[i]=='/'&&i+1<code.size()&&code[i+1]=='/'){\
	st.push(COMMENT);\
}else if(is_operator(code[i])){\
	st.push(OPERATOR);\
	goto home;\
}else if(is_num(code[i])){\
	st.push(NUM);\
	goto home;\
}else if(code[i]=='('){\
	st.push(CIRP);\
	token+="(";\
}else if(code[i]=='{'){\
	st.push(FLOP);\
	token+="{";\
}\
else if((code[i]>='a'&&code[i]<='z')||\
	(code[i]>='A'&&code[i]<='Z')||code[i]=='_'||code[i]=='$'){\
	st.push(ID);\
	goto home;\
}\
else if(code[i]=='\"'||code[i]=='\''){\
	st.push(STRING);\
	token+=string(1,code[i]);\
}\
else{\
	throw SyntaxError(code[i]);\
}

#ifndef SyntaxError
class SyntaxError:public std::exception{
	char contextc;
public:
	SyntaxError(char context=0):contextc(context){}
	const char *what()const noexcept{
		if(contextc)
			return std::string("SyntaxError : Cann\'t parse character \'"+std::string(1,contextc)+"\'").c_str();
		
		return "SyntaxError : Spliter failed";
	}
};
#endif

enum state{
	NUM,//1.2
	OPERATOR,//=>
	STRING,//"abc"
	CIRP,//()
	FLOP,//{}
	ID,//for example `abc_123`
	WAIT,
	COMMENT,// '//...'
};

bool is_operator(char ch){
	return ch==';'||ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='='||ch=='>'||ch=='<'||ch=='^'
	||ch=='&'||ch=='%'||ch=='!'||ch=='$'||ch=='`'||ch=='~'||ch==','||ch=='.';
}

bool is_useless(char ch){
	return ch==' '||ch=='\t'||ch=='\n';
}

bool is_num(char ch){
	return ch>='0'&&ch<='9';
}

bool belongID(char ch){
	return (ch>='a'&&ch<='z')||
	(ch>='A'&&ch<='Z')||ch=='_'||ch=='$'||(ch>='0'&&ch<='9');
}

void split(std::string code,std::vector<std::string> &object){
	using namespace std;
	object.clear();
	stack<state> st;
	st.push(WAIT);
	
	string token;
	
	code=code+string(" ");
	
	for(int i=0;i<code.size();i++){
		home:;
		switch (st.top()){
		case WAIT:
			if(!token.empty()){
				//cout<<"?"<<token<<endl;
				object.emplace_back(token);
				token={};
			}
			shiftState
			break;
		case NUM:
			if(code[i]=='.'||(code[i]>='0'&&code[i]<='9')){
				token+=string(1,code[i]);
			}else{
				st.pop();//quit num mode
				goto home;
			}
			break;
		case OPERATOR:
			if(code[i]=='/'&&i+1<code.size()&&code[i+1]=='/'){
				st.pop();
				st.push(COMMENT);
				goto home;
			}
			if(is_operator(code[i])){
				token+=string(1,code[i]);
			}else{
				st.pop();//quit operator mode
				goto home;
			}
			break;
		case CIRP:
			//cout<<"!"<<string(1,code[i])<<"!\n";
			if(code[i]==')'){
				st.pop();//quit cirp mode
				token+=")";
			}else{
				shiftState
			}
			break;
		case FLOP:
			if(code[i]=='}'){
				st.pop();//quit cirp mode
				token+="}";
			}else{
				shiftState
			}
			break;
		case ID:
			if(belongID(code[i])){
				token+=string(1,code[i]);
			}else{
				st.pop();//quit ID mode
				goto home;
			}
			break;
		case COMMENT:
			if(code[i]=='\n'||i+1>=code.size()){
				st.pop();
				goto home;
			}
			break;
		case STRING:
			token+=string(1,code[i]);
			if(code[i]=='\\'){
				token+=string(1,code[++i]);
			}else if(code[i]=='\"'){
				st.pop();//quit string mode
			}
			break;
		default:
			
			break;
		}
	}
}

#endif
