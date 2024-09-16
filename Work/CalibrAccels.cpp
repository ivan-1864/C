#include <stddef.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#include <direct.h>
#include <windows.h>

#include "iXCOM_c.h"
#include "iXCOM_d.h"
#include "iXCOM_Epoch_st.h"
#include "iXCOM_Database_st.h"
#include "iXCOM_expired.h"
#include "iXCOM_Calibr_main.h"

#include "iXCOM_st.h"
#include "iXCOM_f.h"



//----------------------------------
// компенсация полной модели
void CompensateAccelsErrors(Par* sPar, double* omg, double* Acc_raw, double* Acc)
{
	CompensateAccelsBias(sPar, Acc_raw,	Acc);							// компенсация систематических смещений нулевых сигналов акселерометров
	CompensateAccelsScales(sPar, Acc, Acc);								// компенсация систематических погрешностей масштабов и перекосов акселерометров
	CompensateAccelsElasic(sPar, Acc, Acc);								// компенсация систематических упругих перекосов акселерометров
	CompensateAccelTimingSkewAdjustment(sPar, omg, Acc);				// компенсация рассинхронизации показаний инерциальных датчиков
}
//----------------------------------


//компенсация смещений нулей АКС
void CompensateAccelsBias(Par* sPar, double* Acc_raw, double* Acc)  
{
	double DeltaAcc[3] = { 0.0, 0.0, 0.0 };
	
	for (int i = 0; i < 3; i++)
		DeltaAcc[i] = sPar->Clb_Database.InterpAcc.Bias[i];
	
	for (int i = 0; i < 3; i++ ) 
		Acc[i] = Acc_raw[i] - DeltaAcc[i];

}

//компенсация масшибов и перекосов АКС
void CompensateAccelsScales(Par* sPar, double* Acc_raw, double* Acc)  //компенсация масштабов и перекосов
{
	double DeltaAcc[3] = { 0.0, 0.0, 0.0 };
	double Kf[9] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; 

	for (int i = 0; i < 9; i++)
		Kf[i] = sPar->Clb_Database.InterpAcc.Scales[i];

	MatrixVectorProduct( Kf, Acc_raw, DeltaAcc );

	for (int i = 0; i < 3; i++ )
		Acc[i] = Acc_raw[i] - DeltaAcc[i];
}

// компенсация систематических упругих перекосов акселерометров
void CompensateAccelsElasic(Par* sPar, double* Acc_raw, double* Acc)// из кода АВ
{
	int i1, i2;
	int m=5;
	double ff[5]={0.0, 0.0, 0.0, 0.0, 0.0}; 
	double K[15]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 
	for (int i=0; i < 15; i++)
		K[i]=sPar->Clb_Database.Accels.Kf2[i];
	for (int i = 0; i < 3; i++) 
	{
		i1 = (i+1)%3;
		i2 = (i+2)%3;
		if (m == 5)
		{
			ff[0] = Acc_raw[i ]*Acc_raw[i1];
			ff[1] = Acc_raw[i ]*Acc_raw[i2];
			ff[2] = Acc_raw[i1]*Acc_raw[i1];
			ff[3] = Acc_raw[i1]*Acc_raw[i2];
			ff[4] = Acc_raw[i2]*Acc_raw[i2];
		}
		Acc[i] = Acc_raw[i]-DotProduct(K + i*m,ff, m);

	}
}

// компенсация рассинхронизации показаний инерциальных датчиков
void CompensateAccelTimingSkewAdjustment(Par* sPar, double* omg, double* Acc)
{
	double Tau[9]			= { 0.0, 0.0, 0.0 };
	double AccTau[3]        = { 0.0, 0.0, 0.0 };
	double DeltaAcc[3]	    = { 0.0, 0.0, 0.0 };
	double hat_omega[9]	    = { 0.0, omg[_3], -omg[_2], -omg[_3], 0.0, omg[_1], omg[_2], -omg[_1], 0.0 };

	Tau[_1] = sPar->Clb_Database.Accels.Tau[_1];
	Tau[_2] = sPar->Clb_Database.Accels.Tau[_2];
	Tau[_3] = sPar->Clb_Database.Accels.Tau[_3];
	
	for (int i = 0; i < 3; i++)
		AccTau[i]=Acc[i]*Tau[i];
	
	MatrixVectorProduct(hat_omega, AccTau, DeltaAcc);

	for (int i = 0; i < 3; i++)
		Acc[i] += DeltaAcc[i]; }



