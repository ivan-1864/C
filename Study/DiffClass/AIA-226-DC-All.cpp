//в программе сожержатся 2 класса: Frac (дроби) и CVector (векторы). В main тесты только только для CVector. Проверка работы Frac происодит автоматически при проверки работы CVector
#include <iostream>
#include <cassert>
using namespace std;
int min(const int, const int);
class Frac 
{
		friend ostream & operator << (ostream& os, const Frac & a);
		friend istream & operator >> (istream& is, Frac & a);
	private:
		int n;
		unsigned int m;
	public:
		Frac();
		Frac(int n, unsigned int m);
		~Frac() {};
		Frac&  operator= (const Frac& rhs);
		const Frac operator+ (const Frac& rhs);
		const Frac operator- (const Frac& rhs);
		const Frac operator* (const Frac& rhs);
		bool operator == (const Frac& rhs);
		bool operator!= (const Frac& rhs);
		const Frac operator+= (const Frac& rhs);
		const Frac operator-= (const Frac& rhs);
		Frac easy ();//упрощет дробь
};
class CVector
{
		friend ostream & operator << (ostream& os, const CVector & a); 
		friend istream & operator >> (istream& is, CVector & a);
	private:
		int size;
		Frac* data;
	public:
		CVector(int size);
		CVector (int size, Frac*);
		~CVector ();
		Frac & operator [ ] (int i);
		CVector&  operator= (const CVector& rhs);
		const CVector operator+ (const CVector& rhs);
		const CVector operator- (const CVector& rhs);
		Frac operator* (const CVector& rhs);//скалярое произведение
		const CVector operator* (Frac x);//умножение на скаляр
		bool operator == (const CVector& rhs);
		bool operator!= (const CVector& rhs);
		const CVector& operator+= (const CVector& rhs);
		const CVector& operator-= (const CVector& rhs);
};
ostream& operator << (ostream& os, const Frac& a)
{
	os <<a.n<<"/"<<a.m;
	return os;
}
istream& operator >> (istream& is,Frac& a)
{
	is >> a.n >> a.m;
	assert(a.m!=0);
	return is;
}
Frac:: Frac()
{
	n=0;
	m=1;
}
Frac:: Frac(int n, unsigned int m)
{
	this->n=n;
   	this->m=m; 
	assert(m!=0);
	easy();	
}
Frac& Frac::  operator= (const Frac& rhs)
{
	n=rhs.n;
	m=rhs.m;
	return *this;
}
const Frac Frac:: operator+ (const Frac& rhs)
{
	Frac r=Frac();
	r.n=int(n*rhs.m+m*rhs.n);
	r.m=m*rhs.m;
	r.easy();
	return r;
}
const Frac Frac:: operator- (const Frac& rhs)
{
	Frac r=Frac();
	r.n=int(n*rhs.m-m*rhs.n);
	r.m=m*rhs.m;
	r.easy();
	return r;
}
const Frac Frac:: operator* (const Frac& rhs)
{
	Frac r=Frac();
	r.n=n*rhs.n;
	r.m=m*rhs.m;
	return r;
}
bool Frac:: operator == (const Frac& rhs)
{
	if (n==rhs.n&&m==rhs.m) return true;
   	return false;	
}	
bool Frac:: operator!= (const Frac& rhs)
{
	if (n!=rhs.n||m!=rhs.m) return true;
   	return false;	
}
const Frac Frac:: operator+= (const Frac& rhs)
{
	n=int(n*rhs.m+m*rhs.n);
	m*=rhs.m;
	easy();
	return *this;
}
const Frac Frac:: operator-= (const Frac& rhs)
{
	n=int(n*rhs.m-m*rhs.n);
	m*=rhs.m;
	easy();
	return *this;
}
Frac Frac:: easy()
{
	for (int i=min(n, m); i>1; i--)
		if (n%i==0&&m%i==0) 
		{
			n/=i;
			m/=i;
		}
	return *this;
}

int min (const int a,const int b)
{
	return a<b ? a : b;
}
CVector:: CVector (int ssize)
{
	size=ssize;
	data=new Frac[size];
	for (int i=0; i<size; i++)
		data[i]=Frac(0, 1);
}
CVector:: CVector (int ssize, Frac* AV)
{
	size=ssize;
	data= new Frac[size];
	for (int i=0; i<size; i++)
		data[i]=AV[i];	
}
ostream& operator << (ostream& os, const CVector& a)
{
		int k=a.size-1;
		os <<'(';
		for (int i=0; i<k; i++)
			os << a.data[i]<< ", ";
		os<<a.data[k]<< ")";
	return os;
}
istream& operator >> (istream& is,CVector& a)
{
	for (int i=0; i<a.size; i++)
		is >> a.data[i];
	return is;
}
CVector:: ~CVector()
{
	delete [] data;
}
Frac & CVector:: operator [] (int i)
{
	return data[i];
}
CVector& CVector:: operator= (const CVector& rhs)
{
	assert(size=rhs.size);
	for (int i=0; i<size; i++)
		data[i]=rhs.data[i];
	return *this;
}
const CVector CVector:: operator+ (const CVector& rhs)
{
	assert(size=rhs.size);
	CVector r=CVector(size);
	for (int i=0; i<size; i++)
		r.data[i]=data[i]+rhs.data[i];
	return r;	
}
const CVector CVector:: operator- (const CVector& rhs)
{
	assert(size=rhs.size);
	CVector r=CVector(size);
	for (int i=0; i<size; i++)
		r.data[i]=data[i]-rhs.data[i];
	return r;	
}
Frac CVector:: operator* (const CVector& rhs)
{
	assert(size=rhs.size);
	Frac sum=Frac();
	for (int i=0; i<size; i++)	
		sum+=rhs.data[i]*data[i];	
	return sum;		
}
const CVector CVector:: operator* (Frac x)
{
	CVector r=CVector(size);	
	for (int i=0; i<size; i++)	
		r.data[i]=data[i]*x;
	return r;
}
bool CVector:: operator == (const CVector& rhs)
{
	assert(size=rhs.size);
	for (int i=0; i<size; i++)
		if (data[i]!=rhs.data[i]) return false;
	return true;
}
bool CVector:: operator != (const CVector& rhs)
{
	assert(size=rhs.size);
	for (int i=0; i<size; i++)
		if (data[i]!=rhs.data[i]) return true;
	return false;
}
const CVector& CVector:: operator+= (const CVector& rhs)
{
	assert(size=rhs.size);
	for (int i=0; i<size; i++)
		data[i]+=rhs.data[i];
	return *this;
}
const CVector& CVector:: operator-= (const CVector& rhs)
{
	assert(size=rhs.size);
	for (int i=0; i<size; i++)
		data[i]-=rhs.data[i];
	return *this;
}
int main()
{
CVector Av=CVector(3);//констуктор по умолчанию
CVector Bv=CVector(3);
Frac AV[3]={Frac(4,1), Frac(4,1), Frac(4,1)};
CVector Cv=CVector(3, AV);// конструктор, принимающий константы
cin >> Av >> Bv;
cout << "Av=" << Av << endl;
cout << "Bv=" << Bv << endl;
cout << "Cv=" << Cv << endl;
cout << "Av+Bv="<< Av+Bv << endl;//сложение векторов
cout << "Av-Bv=" << Av-Bv << endl;//вычетание векторов
cout << "Av*(2/1)=" << Av*Frac(2, 1) << endl;//умножение на скаляр
cout << "Av*Bv=" << Av*Bv << endl;//скалярное произведение
Bv+=Av;
cout << "Bv=Bv+Av=" << Bv<< endl;
Bv-=Av;
cout << "Bv=Bv-Av" << Bv<< endl;
cout << "Bv[2]=" << Bv[2]<< endl;//вывести 2й индекс
Bv=Cv;
if (Bv==Cv) cout << "Bv=Cv"<< endl;
if (Av!=Cv) cout << "Av!=Cv"<< endl;
}

