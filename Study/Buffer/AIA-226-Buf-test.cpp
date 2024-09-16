#include <iostream>
#include <string>
#include "AIA-226-Buf-Sbuffer.h"
using namespace std;
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

