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

