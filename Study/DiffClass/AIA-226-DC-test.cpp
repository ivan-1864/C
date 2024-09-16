#include <iostream>
#include <cassert>
#include "AIA-226-DC-CVector.h"
using namespace std;

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
