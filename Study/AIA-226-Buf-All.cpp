//Акимов Иван Андреевич, 226 группа
//в программе использован класс string и некоторые его методы 
//общий вид буфера такой: это односвязный список, представленный в виде структуры String с 2 параметрами 
#include <iostream>
#include <string>
using namespace std;
struct String
{
	string str;//само значение (строка)
	String* next;//указатель на следующий элемент
};
class SBuffer
{
	private:
		String* head;
	public:
	SBuffer(){head=NULL;}
	SBuffer (string s);	//2 конструктора (пустой и со строкой)
	~SBuffer()	{delete head;}//деструктор
	void addString(string s);//добавлние строки в конец
	void addString (string s, int n);//добавление строки с заданной позиции
	void print();//вывод содержимого буфера на экран
	void clear(){head=NULL;}//очистка буфера
	void clear(int n);// очистка буфера с заданной позиции
	int count ();// кол-во элементов в буфере
	void copyStr (char* a);//копирование содержимого в массив
	void copySubStr (char* a, int b, int e);//копирование подстроки в массив
	char getChar (int n); //возвращает н-тый элемент 
	void changeChar (int n, char c); //меняет н-тый элемент
	void changeStr (int n, string s); //меняет подстроку, начиная с н-той позиции
	int searchChar (char c); // ищет заданный элемент
	int searchStr (string s); // ищет заданную подстроку
};

SBuffer::SBuffer (string s)
{
	String* nd = new String;
	nd->str=s;
	nd->next=NULL;
	head=nd;
}
void SBuffer:: addString(string s)
{
	String* nd = new String;	
	nd->str=s;
	nd->next=NULL;
	if (head==NULL) head = nd;
	else 
	{
		String* cur=head;
		while (cur -> next!=NULL)
			cur=cur->next;
		cur->next=nd;
	}	
}
void SBuffer:: addString (string s, int n)
{
	String* cur = head;
	String* cur1 = head;
	int i=0;
	//далее будет много подобных алгоритмов, объясню на этом:
	// 2 указателя нужно, тк список односвязный - когда мы поймем, что зашли дальше нужного значения, нужно вернуться на одну операцию назад. для этого вводим еще один указатель, который отстает на одну инерацию 
	while (cur!=NULL)
	{
		string sc=cur1->str;
		i+=sc.size();
		cur1=cur1->next;
		if (i>=n) 
		{
			i-=sc.size();
			break;			
		}
		cur=cur->next;
	}//i - абсолютное (те в общей строке буфера) текущее положение
	int c=n-i;//c - локальное (те в данной подстроке str) текущее положение
	cur->str.insert(c, s);//ставляем подстоку
}
void SBuffer:: print()
{
	String * cur= head;
	while (cur!=NULL)
	{
		cout << cur->str;
		cur=cur->next;
	}	
}
void SBuffer:: clear(int n)
{
	String* cur = head;
	String* cur1 = head;
	int i=0;
	while (cur!=NULL)
	{
		string sc=cur1->str;
		i+=sc.size();
		cur1=cur1->next;
		if (i>=n) 
		{
			i-=sc.size();
			break;			
		}
		cur=cur->next;
	} 
	cur->str=cur->str.erase(n-i);
	cur->next=NULL;
}
int SBuffer:: count ()
{
	String* cur = head;
	int i=0;
	while (cur!=NULL)
	{
		string s=cur->str;
		i+=s.size();
		cur=cur->next;
	}
return i;
}
void SBuffer:: copyStr (char* a)
{
	int t=0;
	String* cur= head;
	while (cur!=NULL)
	{
		int n=cur->str.size();
		for (int i=0; i<n; i++)
		{
			a[i+t]=cur->str[i];
		}
		t+=n;
		cur=cur->next;
	}
}
void SBuffer:: copySubStr (char* a, int b, int e)
{
	int size=e-b+1;
	int i=0;
	String* cur=head;
	String* cur1=head;
	while (cur!=NULL)
	{
		string sc=cur1->str;
		i+=sc.size();
		cur1=cur1->next;
		if (i>=b)
		{
			i-=sc.size();
			break;			
		}
	}
	for (int j=0; j<size; j++)
	{
		if (cur->str[b-i+j]!='\0') 
			a[j]=cur->str[b-i+j];//если это не конец строки, списываем его 
   		else // если же конец, делаем шаг по списку,переобозначение, чтобы начало считываться с начала, а также j--, чтобы вернуться на ту же итерацию 
		{
			i=j;
			j--;
			b=0;
			cur=cur->next;
		}
	}	
}
char SBuffer:: getChar (int n)
{
	String* cur = head;
	String* cur1 = head;
	int i=0;
	while (cur!=NULL)
	{
		string sc=cur1->str;
		i+=sc.size();
		cur1=cur1->next;
		if (i>=n) 
		{
			i-=sc.size();
			break;			
		}
		cur=cur->next;
	} 
	return cur->str[n-i];		
}
void SBuffer:: changeChar (int n, char c)
{
	String* cur = head;
	String* cur1 = head;
	int i=0;
	while (cur!=NULL)
	{
		string sc=cur1->str;
		i+=sc.size();
		cur1=cur1->next;	
		if (i>n) 
		{
			i-=sc.size();
			break;			
		}
		cur=cur->next;
	} 		
	cur->str[n-i]=c;
}
void SBuffer:: changeStr (int n, string s)
{
	String* cur = head;
	String* cur1 = head;
	int i=0;	
	while (cur!=NULL)
	{
		string sc=cur1->str;
		i+=sc.size();
		cur1=cur1->next;
		if (i>=n) 
		{
			i-=sc.size();
			break;			
		}
		cur=cur->next;
	}
	for (int j=0; j<s.size(); j++)
	{
		if (cur->str[n-i+j]!='\0') 
			cur->str[n-i+j]=s[j]; 
  		else 	
		{
			i=j;
			j--;
			n=0;
			cur=cur->next;
		}
	}	
}
int SBuffer:: searchChar (char c)
{
	String* cur = head;
	int t=0;
	while (cur!=NULL)
	{
		string sc=cur->str;
		for (int i=0; i<sc.size(); i++)
			if (c==sc[i]) return t+i;
		t+=sc.size();		
		cur=cur->next;
	} 
	return -1;	
}
int SBuffer:: searchStr (string s)
{
	char st [s.size()];//ввел вспомогательный массив, буду действовать на подобие очереди
	String* cur = head;
	int t=0, c=0, f=0;
	for (int i=0; i<s.size(); i++)
	{
		if (cur->str.size()>i-t) 
			st[i]=cur->str[i-t];//наполнение очереди
		else	
		{
			t=i;
			i--;
			cur=cur->next;
		}	
	}
	for (int j=0; ; j++)// цикл выглядит бесконечным, но при исчерпании вариантов (в конце буфера) прерывается
	{
		for (int i=0; i<s.size(); i++)
			if (st[i]==s[i]) c++;//проверка равенства очереди и заданной строки
		if (c==s.size()) return j;
		c=0;
		if (f==0)//f=flag - если в конец очереди ничего не поступило, не происходит сдвига влево
		{
			for (int i=0; i<s.size()-1; i++)
				st[i]=st[i+1];//сдвиг всех элементов на 1 влево
		}
		if (j+s.size()-t<cur->str.size())//если помещаемся в подстроке, заполняемя посделний элемент очереди
		{
			st[s.size()-1]=cur->str[j+s.size()-t];
			f=0;
		}
		else if (cur->next!=NULL)
		{
			t=j+s.size();
			j--;
			cur=cur->next;
			f=1;
		}
		else break;
	}	
	return -1;
}
int main()
{
	string s="012", c="345";
	string ss="678", cc="9qwe";

	//проверка констурктора по умолчанию
	SBuffer B;
	cout << "пустой буфер:";
  	B.print();
	cout << " его длина:" << B.count();
	cout << endl;

	//проверка констуктора по строке, а также функций кол-ва элементов и выводящий буфер
	SBuffer A=SBuffer(s);
	cout << "буфер, порожденный строкой s:";
  	A.print();
	cout << " его длина:" << A.count();
	cout << endl;
	
	//проверка ф-ии добавления элементов
	cout << "s:";
	A.print();
	cout << " ";	
	A.addString(c);
	cout << "s+c:";
	A.print();
	cout << " ";	
	A.addString(ss);
	cout << "s+c+ss:";
	A.print();
	cout << " длина:" << A.count();
	cout << endl;
	
	//добавление подстроки с определенного места
	A.addString(cc, 2);
	cout << "добавам сс на 2 место:";
	A.print();
	cout << endl;
	
	//копирование буфера в массив
	const int size = A.count();
	char a[size];
	A.copyStr(a);
	cout << "буфер:";
	A.print();
	cout << " ";
	cout << "массив:";
	for (int i=0; i<size; i++)
		cout << a[i];
	cout << endl;

	//копирование подстроки
	char b[5];
	A.copySubStr(b, 3, 7);
	cout << "буфер:";
	A.print();
	cout << " подстрока 3-7:";
	for (int i=0; i<5; i++)
		cout << b[i];
	cout << endl;
	
	//очистка буфера
	A.clear();
	cout << "очищенный буфер: ";
	A.print();
	cout << "его длина: " << A.count();
	cout << endl;
		
	//восстановим буфер
	A.addString(s);
	A.addString(c);
	A.addString(ss);
	cout << "буфер s+c+ss: ";
	A.print();

	//вывод н-то (пятого элемента)
	cout << " пятый элемент буфера: " << A.getChar(5);
	cout << endl;

	//замена символа
	A.changeChar (3, ':');
	cout << "измененный буфер: ";
	A.print();
	cout << endl;

	//замена строки	
	string f="abcde";
	A.changeStr (2, f);
	cout << "измененный буфер: ";
	A.print();
	cout << endl;

	//неудачный поиск символа (выводит -1)
	cout << "номер символа !: "<< A.searchChar ('!');
	cout << endl;

	//удачный поиск символа (выводит номер символа)
	cout << "номер символа 7: "<< A.searchChar ('7');
	cout << endl;

	//неудачный поиск подстроки
	cout << "начало подстроки ?abc: " << A.searchStr("?acb");
	cout << endl;

	//удачный поиск подстроки
	cout << "начало подстроки abcde: " << A.searchStr("abcde");
	cout << endl;

	//очистка буфера с н-ного элемента
	A.clear(4);
	cout << "укороченный буфер: ";
	A.print();	
	cout << endl;
}

