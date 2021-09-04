#include <iostream>
#include"KeywordsTable.h"
#include "IdentifierTable.h"
#include "ConstTable.h"
#include "Store.h"

#include<vector>
#include <string> 
#include <fstream> 
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int index;
int linesum;//������ 
int sum;//�������� 
int state;//��ǰ״̬ 
string nowstring;//��ǰ�洢��token 
FILE* instream;//�ļ����� 
bool isletter(char c){
	return ((c>='a'&&c<='z')||(c<='Z'&&c>='A'));
}
bool isdigit(char c){
	return (c>='0'&&c<='9');
}
void error(char* ch,int line){//���������ַ����������ֱ�����ַ���ʼ 
	int flag=0;
	cout<<endl<<"��"<<line+1<<"�г���"<<endl;
	while(*ch != ' ' && *ch != '\n' && *ch != '\t'){
		if(*ch=='\n')linesum++;
		*ch=fgetc(instream);
		if(isletter(*ch)||isdigit(*ch)||*ch=='_'){
			flag=1;
		}
		else break;
	}
	if(flag)fseek(instream, -1, SEEK_CUR);//��ʱ�����һ���ַ�����Ҫ�˻� 
}
void printkey(int key){
	cout<<"("<<key<<","<<"-"<<")";
}

void printidentifier(int no){
	cout<<"("<<21<<","<<no<<")";
}

void printconst(int no){
	cout<<"("<<46<<","<<no<<")";
}
void printother(int no){
	cout<<"("<<no<<","<<"-"<<")";
}
int main() {
	KeywordsTable k; 
	IdentifierTable i;
	ConstTable ct;
	Store s;
    instream = fopen("in.txt", "r");
    char ch;
    cout<<"�Ǻ�����:"<<endl;
	do{
	switch(state){
		case 0:
			nowstring="";
			ch = fgetc(instream);
			if(ch == ' ' || ch == '\n' || ch == '\t'){
				while(ch == ' ' || ch == '\n' || ch == '\t'){
					if(ch=='\n')linesum++;
					ch=fgetc(instream);
				}
			}
			if(ch=='0')state=19;
			else if(isdigit(ch))state=2;
			else if(isletter(ch)||ch=='_')state=1;
			else switch(ch){
				case '<':state=8;break;
				case '>':state=9;break;
				case '=':state=10;break;
				case '+':state=11;break;
				case '-':state=12;break;
				case '*':state=0;cout<<"(33,-)";s.insert(33);sum++;break;
				case '/':state=13;break;
				case '(':state=0;cout<<"(24,-)";s.insert(24);sum++;break;
				case ')':state=0;cout<<"(25,-)";s.insert(25);sum++;break;
				case ':':state=16;break;
				case '[':state=0;cout<<"(26,-)";s.insert(26);sum++;break;
				case ']':state=0;cout<<"(27,-)";s.insert(27);sum++;break;
				case '{':state=0;cout<<"(36,-)";s.insert(36);sum++;break;
				case '}':state=0;cout<<"(37,-)";s.insert(37);sum++;break;
				case '.':state=22;break;
				case '"':state=17;break;
				case '\'':state=23;break;
				case '#':state=0;cout<<"(39,-)";s.insert(39);sum++;break;
				case ';':state=0;cout<<"(32,-)";s.insert(32);sum++;break; 
				case ',':state=0;cout<<"(52,-)";s.insert(52);sum++;break; 
			}
			break;
		case 1://��ʶ�� or �ؼ��� 
			nowstring+=ch;
			ch=fgetc(instream);
			if(isletter(ch)||isdigit(ch)||ch=='_')state=1;
			else{
				fseek(instream, -1, SEEK_CUR);
				state=0;
				int key=k.get(nowstring);
				if(key!=-1){// keyword
					printkey(key);
					s.insert(key);
					sum++;
				}
				else{//identifier 
					i.insert(nowstring);
					printidentifier(i.get(nowstring));
					s.insert(21);
					sum++;
				}
			}
			break;
		case 2://��ֵ���� 
			nowstring+=ch;
			ch=fgetc(instream);
			if(isdigit(ch)){
				state=2;
			}
			else if(isletter(ch)&&ch!='E'){
				error(&ch,linesum);
				state=0;
			}
			else switch(ch){
				case '.':state=3;break;
				case 'E':state=5;break;
				default:
				fseek(instream, -1, SEEK_CUR);
				state=0;
				ct.insert(nowstring);
				printconst(ct.get(nowstring));
				s.insert(46);
				sum++;
				break;
			}
			break;
		case 3://С���� 
			nowstring+=ch;
			ch=fgetc(instream);
			if(isdigit(ch))state=4;
			else {
				error(&ch,linesum);
				state=0;
			}
			break; 
		case 4://С��
			nowstring+=ch;
			ch=fgetc(instream);
			if(isdigit(ch))state=4;
			else switch(ch){
				case 'E':state=5;break;
				default:
				fseek(instream, -1, SEEK_CUR);
				state=0;
				ct.insert(nowstring);
				printconst(ct.get(nowstring));
				s.insert(46);
				sum++;
				break;
			}
			break; 
		case 5://ָ��״̬ :����E 
			nowstring+=ch;
			ch=fgetc(instream);
			if(isdigit(ch))state=7; 
			else switch(ch){
				case '+':state=6;break;
				case '-':state=6;break;
				default:
				error(&ch,linesum);
				state=0;
				break;
			}
			break;
		case 6://ָ��״̬��E�����������
			nowstring+=ch;
			ch=fgetc(instream);
			if(isdigit(ch))state=7; 
			else {
				error(&ch,linesum);
				state=0;
			}
			break;
		case 7://ָ��
			nowstring+=ch;
			ch=fgetc(instream);
			if(isdigit(ch))state=7; 
			else {
				fseek(instream, -1, SEEK_CUR);
				state=0;
				ct.insert(nowstring);
				printconst(ct.get(nowstring));
				s.insert(46);
				sum++;
			}
			break;
		case 8:// <״̬ 
			nowstring+=ch;
			ch=fgetc(instream);
			switch(ch){
				case '='://<=
					nowstring+=ch;
					state=0;
					printother(40);
					s.insert(40);
					sum++;
					break;
				case '>'://<>
					nowstring+=ch;
					state=0;
					printother(38);
					s.insert(38);
					sum++;
					break;
				default://<
					fseek(instream, -1, SEEK_CUR);
					state=0;
					printother(30);
					s.insert(30);
					sum++;
					break;
			}
			break;
		case 9:// >״̬ 
			nowstring+=ch;
			ch=fgetc(instream);
			switch(ch){
				case '='://>=
					nowstring+=ch;
					state=0;
					printother(42);
					s.insert(42);
					sum++;
					break;
				default://<
					fseek(instream, -1, SEEK_CUR);
					state=0;
					printother(41);
					s.insert(41);
					sum++;
					break;
			}
			break;
		case 10:// =״̬
			nowstring+=ch;
			ch=fgetc(instream);
			switch(ch){
				case '='://==
					nowstring+=ch;
					state=0;
					printother(29);
					s.insert(29);
					sum++;
					break;
				default://=
					fseek(instream, -1, SEEK_CUR);
					state=0;
					printother(28);
					s.insert(28);
					sum++;
					break;
			}
			break;
		case 11:// +״̬ 
			nowstring+=ch;
			ch=fgetc(instream);
			switch(ch){
				case '='://+=
					nowstring+=ch;
					state=0;
					printother(48);
					s.insert(48);
					sum++;
					break;
				case '+'://++
					nowstring+=ch;
					state=0;
					printother(23);
					s.insert(23);
					sum++;
					break;
				default://+
					fseek(instream, -1, SEEK_CUR);
					state=0;
					printother(43);
					s.insert(43);
					sum++;
					break;
			}
			break;
		case 12:// -״̬ 
			nowstring+=ch;
			ch=fgetc(instream);
			switch(ch){
				case '='://-=
					nowstring+=ch;
					state=0;
					printother(49);
					s.insert(49);
					sum++;
					break;
				case '-'://--
					nowstring+=ch;
					state=0;
					printother(45);
					s.insert(45);
					sum++;
					break;
				default://-
					fseek(instream, -1, SEEK_CUR);
					state=0;
					printother(44);
					s.insert(44);
					sum++;
					break;
			}
			break;
		case 13:// /״̬
			nowstring+=ch;
			ch=fgetc(instream);
			if(ch=='/')state=14;
			else if(ch=='*')state=15;
			else {
				fseek(instream, -1, SEEK_CUR);
				state=0;
				printother(34);
				s.insert(34);
				sum++;
			}
			break;
		case 14://  //״̬ 
		 	ch=fgetc(instream);
			while(ch!='\n')ch=fgetc(instream);
			if(ch=='\n'){
				linesum++;
				state=0;
			}
			break;
		case 15:// /*״̬
			ch=fgetc(instream);
			while(ch!='*')
			{
				if(ch=='\n')linesum++;
				ch=fgetc(instream);			
			}
			ch=fgetc(instream);
			if(ch=='/')state=0;
			else {
				if(ch=='\n')linesum++;
				state=15;
			}
			break;
		case 16://:״̬
			nowstring+=ch;
			ch=fgetc(instream);
			if(ch==':'){
				nowstring+=ch;
				state=0;
				printother(50);
				s.insert(50);
				sum++;
			}
			else {
				fseek(instream,-1,SEEK_CUR); 
				 state=0;
				 printother(31);
				 s.insert(31);
				 sum++;
			}
			break;
		case 17:// "״̬
			nowstring+=ch;
			ch=fgetc(instream);
			while(ch!='"'){
				if(ch=='\n'){
					error(&ch,linesum);
					linesum++;
					state=0;
					break;
				}
				nowstring+=ch;
				ch=fgetc(instream);
			}
			if(ch=='"')state=18;
			break;
		case 18:// ""״̬ 
			nowstring+=ch;
			ct.insert(nowstring);
			printconst(ct.get(nowstring));
			s.insert(46);
			sum++;
			state=0;
			break;
		case 19:// �˽��ƻ�ʮ������
			nowstring+=ch;
			ch=fgetc(instream);
			if(ch=='x'||ch=='X'){
				state=20;
			}
			else if (isdigit(ch)){
				if(ch>='0'&&ch<'8')
				state=21;
				else {
					error(&ch,linesum);
					state=0;
				}
			}
			else {
				if(isletter(ch)){
					error(&ch,linesum);
					state=0;
				}
				else{
					fseek(instream,-1,SEEK_CUR); 
					state=0;
					ct.insert(nowstring);
					printconst(ct.get(nowstring));
					s.insert(46);
					sum++;
				}
			} 
			break;
		case 20://ʮ������ 
			nowstring+=ch;
			ch=fgetc(instream);
			if(isdigit(ch)||(ch>='a'&&ch<='f')){
				state=20;
			}
			else if(isletter(ch)){
				error(&ch,linesum);
				state=0;
			}
			else {
				fseek(instream,-1,SEEK_CUR); 
				state=0;
				ct.insert(nowstring);
				printconst(ct.get(nowstring));
				s.insert(46);
				sum++;
			}
			break;
		case 21://�˽��� 
			nowstring+=ch;
			ch=fgetc(instream);
			if(ch>='0'&&ch<='7'){
				state=21;
			}
			else if(isdigit(ch)||isletter(ch)){
				error(&ch,linesum);
				state=0;
			}
			else {
				fseek(instream,-1,SEEK_CUR); 
				state=0;
				ct.insert(nowstring);
				printconst(ct.get(nowstring));
				s.insert(46);
				sum++;
			}
			break;
		case 22://.��ͷ�ĸ����� 
			nowstring+=ch;
			ch=fgetc(instream);
			if(isdigit(ch)){
				state=2;
				break;
			} 
			else{
				error(&ch,linesum);
				state=0;
			}
		case 23://�ַ�����
			nowstring+=ch;
			ch=fgetc(instream);
			if(ch=='\\'){
				while(ch!='\''){
					if(ch=='\n'){
						error(&ch,linesum);
						linesum++;
						state=0;
						break;
					}
					nowstring+=ch;
					ch=fgetc(instream);
				}
			}
			else {
				if(ch=='\n'){
					error(&ch,linesum);
					linesum++;
					state=0;
					break;
				}
				nowstring+=ch;
				ch=fgetc(instream);
			}
			if(ch=='\'')state=24;
			else {
				error(&ch,linesum);
				state=0;
				break;
			}
			break;
		case 24:
			nowstring+=ch;
			ct.insert(nowstring);
			printconst(ct.get(nowstring));
			s.insert(46);
			sum++;
			state=0;
			break;
		}
	}while(ch!=EOF);
	cout<<endl<<"�������:"<<linesum<<endl<<"��������:"<<sum<<endl;
	s.printhelp();
	ct.printhelp();
	i.printhelp();
    fclose(instream);
    int t=1;
    bool fla=t<(2>3);
    cout<<fla;
	return 0;
}
