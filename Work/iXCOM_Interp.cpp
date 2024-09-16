#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
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

//find coeff of quaratic func
void ComputeQuadrCoeff(double* Temp, double* Val, double* QuadrCoeff) //3d vectors
{
	double TempMatrix[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	double TempInverseMatrix[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	for (int i=0; i<3; i++)
	{
		TempMatrix[0+3*i]=Temp[i]*Temp[i];  // [T1^2, T1, 1;
		TempMatrix[1+3*i]=Temp[i];			//  T2^2, T2, 1;
		TempMatrix[2+3*i]=1;				//  T3^2, T3, 1]	
	}
	MakeInverseMatrix(TempMatrix, TempInverseMatrix);
	MatrixVectorProduct(TempInverseMatrix, Val, QuadrCoeff);
	return;
}

//record coeff for interpolation 
void RecordQuadrCoefAll(Par* sPar)
{
	RecordQuadrCoefAccBias(sPar);		// acc bias
	RecordQuadrCoefGyroBias(sPar);		// gyro bias
	RecordQuadrCoefAccScales(sPar);		// acc scale coeff
	RecordQuadrCoefGyroScales(sPar);	// gyro scale coeff
	return;
}

// record coeff for interpolation acc bias
void RecordQuadrCoefAccBias(Par* sPar)
{
	double Delta_F[3] = {0.0, 0.0, 0.0}; 
	double Temp[3] = {0.0, 0.0, 0.0};
	double QuadrCoeff[3] = {0.0, 0.0, 0.0};
	for (int i = 0; i < 3; i++)
	{
		for (int j=0; j < 3; j++)
		{
			Delta_F[j]=sPar->Clb_Database.Accels.DeltaF_T[6*j+i]; 
			Temp[j]=sPar->Clb_Database.Accels.DeltaF_T[6*j+3+i];
		}
		ComputeQuadrCoeff(Temp, Delta_F, QuadrCoeff); 
		for (int j=0; j < 3; j++)
			sPar->Clb_Database.InterpAcc.QuadrCoefBias[j+3*i]=QuadrCoeff[j];
	}

}

// record coeff for interpolation gyro bias
void RecordQuadrCoefGyroBias(Par* sPar)
{
	double Nu[3] = {0.0, 0.0, 0.0}; 
	double Temp[3] = {0.0, 0.0, 0.0};
	double QuadrCoeff[3] = {0.0, 0.0, 0.0};

	for (int i = 0; i < 3; i++)
	{
		for (int j=0; j < 3; j++)
		{
			Nu[j]=sPar->Clb_Database.Gyros.Nu_T[6*j+i]; 
			Temp[j]=sPar->Clb_Database.Gyros.Nu_T[6*j+3+i];
		}
		ComputeQuadrCoeff(Temp, Nu, QuadrCoeff); 
		for (int j=0; j < 3; j++)
			sPar->Clb_Database.InterpGyro.QuadrCoefBias[j+3*i]=QuadrCoeff[j];
	}
}

// record coeff for interpolation acc scale coeff
void RecordQuadrCoefAccScales(Par* sPar)
{
	double Kf[3] = {0.0, 0.0, 0.0}; 
	double Temp[3] = {0.0, 0.0, 0.0};
	double QuadrCoeff[9] = {0.0, 0.0, 0.0};

	for (int i = 0; i < 9; i++)
	{
		for (int j=0; j < 3; j++)
		{
			Kf[j]=sPar->Clb_Database.Accels.K_f_T[18*j+i]; 
			Temp[j]=sPar->Clb_Database.Accels.K_f_T[18*j+9+i];
		}
		ComputeQuadrCoeff(Temp, Kf, QuadrCoeff); 
		for (int j=0; j < 3; j++)
			sPar->Clb_Database.InterpAcc.QuadrCoefScales[j+3*i]=QuadrCoeff[j];
	}
}

// record coeff for interpolation gyro scale coeff
void RecordQuadrCoefGyroScales(Par* sPar)
{
	double Kw[3] = {0.0, 0.0, 0.0}; 
	double Temp[3] = {0.0, 0.0, 0.0};
	double QuadrCoeff[3] = {0.0, 0.0, 0.0};

	for (int i = 0; i < 9; i++)
	{
		for (int j=0; j < 3; j++)
		{
			Kw[j]=sPar->Clb_Database.Gyros.K_nu_T[18*j+i]; 
			Temp[j]=sPar->Clb_Database.Gyros.K_nu_T[18*j+9+i];
		}
		ComputeQuadrCoeff(Temp, Kw, QuadrCoeff); 
		for (int j=0; j < 3; j++)
			sPar->Clb_Database.InterpGyro.QuadrCoefScales[j+3*i]=QuadrCoeff[j];
	}
}

//interpolation
double ComputeCurValue (double* Temp, double* QuardCoeff, double CurTemp)
{
	double CurValue;
	if (CurTemp > Temp[0] && CurTemp < Temp[2])
		CurValue = QuardCoeff[0] * CurTemp* CurTemp+QuardCoeff[1]*CurTemp+QuardCoeff[2];
	else if (CurTemp <= Temp[0])
		CurValue = QuardCoeff[0]*Temp[0]*Temp[0]+QuardCoeff[1]*Temp[0]+QuardCoeff[2]+(2*QuardCoeff[0]*Temp[0]+QuardCoeff[1])*(CurTemp-Temp[0]);
	else 
		CurValue = QuardCoeff[0]*Temp[2]*Temp[2]+QuardCoeff[1]*Temp[2]+QuardCoeff[2]+(2*QuardCoeff[0]*Temp[2]+QuardCoeff[1])*(CurTemp-Temp[2]);	
	return CurValue;
	
}

// compute all estimates
void ComputeCurAll (Par* sPar) 
{
	ComputeCurAccBias(sPar);		
	ComputeCurGyroBias(sPar);		
	ComputeCurAccScales(sPar);		
	ComputeCurGyroScales(sPar);		
	return;
}

// compute  estimates acc bias
void ComputeCurAccBias (Par* sPar)  
{
	double DeltaF_Cur = 0.0;
	double QuardCoeff[3]={0.0, 0.0, 0.0};
	double Temp[3] = {0.0, 0.0, 0.0};
	double CurTemp=0;

	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			Temp[j]=sPar->Clb_Database.Accels.DeltaF_T[6*j+3+i];
			QuardCoeff[j]=sPar->Clb_Database.InterpAcc.QuadrCoefBias[j+3*i];
		}
		CurTemp=sPar->Clb_Database.InterpAcc.CurTemp[i];
		DeltaF_Cur=ComputeCurValue(Temp, QuardCoeff, CurTemp);
		sPar->Clb_Database.InterpAcc.Bias[i]=DeltaF_Cur;
	}
	return;
}

// compute  estimates gyro bias
void ComputeCurGyroBias (Par* sPar)  
{
	double Nu_Cur = 0.0;
	double QuardCoeff[3]={0.0, 0.0, 0.0};
	double Temp[3] = {0.0, 0.0, 0.0};
	double CurTemp=0;

	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			Temp[j]=sPar->Clb_Database.Gyros.Nu_T[6*j+3+i];
			QuardCoeff[j]=sPar->Clb_Database.InterpGyro.QuadrCoefBias[j+3*i];
		}
		CurTemp=sPar->Clb_Database.InterpGyro.CurTemp[i];
		Nu_Cur = ComputeCurValue(Temp, QuardCoeff, CurTemp);
		sPar->Clb_Database.InterpGyro.Bias[i]=Nu_Cur;
	}
	return;
}

// compute  estimates acc scales
void ComputeCurAccScales (Par* sPar) 
{
	double Kf_Cur = 0.0;
	double QuardCoeff[3] = {0.0, 0.0, 0.0};
	double Temp[3] = {0.0, 0.0, 0.0};
	double CurTemp = 0;
	int CurIndex = 0;

	for (int i=0; i<9; i++)
	{
		for (int j=0; j<3; j++)
		{
			Temp[j]=sPar->Clb_Database.Accels.K_f_T[18*j+9+i];
			QuardCoeff[j]=sPar->Clb_Database.InterpAcc.QuadrCoefScales[j+3*i];
		}
		CurIndex = int(i/3);
		CurTemp=sPar->Clb_Database.InterpAcc.CurTemp[CurIndex]; 
		Kf_Cur= ComputeCurValue(Temp, QuardCoeff, CurTemp);
		sPar->Clb_Database.InterpAcc.Scales[i]=Kf_Cur;
	}
	return;
}

// compute  estimates gyro scales
void ComputeCurGyroScales (Par* sPar) 
{
	double Kw_Cur = 0.0;
	double QuardCoeff[3]={0.0, 0.0, 0.0};
	double Temp[3] = {0.0, 0.0, 0.0};
	double CurTemp=0;
	int CurIndex = 0;

	for (int i=0; i<9; i++)
	{
		for (int j=0; j<3; j++)
		{
			Temp[j]=sPar->Clb_Database.Gyros.K_nu_T[18*j+9+i];
			QuardCoeff[j]=sPar->Clb_Database.InterpGyro.QuadrCoefScales[j+3*i];
		}
		CurIndex = int(i/3);
		CurTemp=sPar->Clb_Database.InterpGyro.CurTemp[CurIndex]; 
		Kw_Cur = ComputeCurValue(Temp, QuardCoeff, CurTemp);
		sPar->Clb_Database.InterpGyro.Scales[i]=Kw_Cur;
	}
	return;
}

// print all estimates
void PrintCurValue (Par* sPar) 
{
	printf("; ------- Gyros Current Value -------\n");
	printf(";------- Nu(T)-------\n");	
	printf("Nu^1[d/s] = %11.8lf\n", sPar->Clb_Database.InterpGyro.Bias[0]);
	printf("Nu^2[d/s] = %11.8lf\n", sPar->Clb_Database.InterpGyro.Bias[1]);
	printf("Nu^2[d/s] = %11.8lf\n", sPar->Clb_Database.InterpGyro.Bias[2]);
	printf(";------- Kw(T)-------\n");
	printf("Kw_11 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[0]);
	printf("Kw_12 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[1]);
	printf("Kw_13 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[2]);
	printf("Kw_21 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[3]);
	printf("Kw_22 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[4]);
	printf("Kw_23 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[5]);
	printf("Kw_31 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[6]);
	printf("Kw_32 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[7]);
	printf("Kw_33 = %11.8lf\n", sPar->Clb_Database.InterpGyro.Scales[8]);


	printf("; ------- Acclels Current Value -------\n");
	printf(";------- Delta_F(T)-------\n");
	printf("DeltaF^1[m/s/s] = %11.8lf\n", sPar->Clb_Database.InterpAcc.Bias[0]);
	printf("DeltaF^2[m/s/s] = %11.8lf\n", sPar->Clb_Database.InterpAcc.Bias[1]);
	printf("DeltaF^3[m/s/s] = %11.8lf\n", sPar->Clb_Database.InterpAcc.Bias[2]);
	printf(";------- Kf(T)-------\n");
	printf("Kf_11 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[0]);
	printf("Kf_12 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[1]);
	printf("Kf_13 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[2]);
	printf("Kf_21 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[3]);
	printf("Kf_22 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[4]);
	printf("Kf_23 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[5]);
	printf("Kf_31 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[6]);
	printf("Kf_32 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[7]);
	printf("Kf_33 = %11.8lf\n", sPar->Clb_Database.InterpAcc.Scales[8]);

	_getch();

}

// record all estimates
void WriteInterpDB (Par* sPar)
{


	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Bias[0]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Bias[1]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Bias[2]);

	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Bias[0]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Bias[1]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Bias[2]);

	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[0]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[1]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[2]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[3]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[4]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[5]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[6]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[7]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpAcc.Scales[8]);

	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[0]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[1]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[2]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[3]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[4]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[5]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[6]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[7]);
	fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.InterpGyro.Scales[8]);

	fprintf(sPar->pfInterpTxt, "\n ");

}

// record all estimates
void WriteCalibrTable(Par* sPar)
{
	for (int i=0; i<3; i++)
	{
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.DeltaF_T[0+6*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.DeltaF_T[1+6*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.DeltaF_T[2+6*i]);

		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.Nu_T[0+6*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.Nu_T[1+6*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.Nu_T[2+6*i]);

		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[0+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[1+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[2+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[3+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[4+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[5+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[6+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[7+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Accels.K_f_T[8+18*i]);

		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[0+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[1+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[2+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[3+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[4+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[5+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[6+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[7+18*i]);
		fprintf(sPar->pfInterpTxt, "%13.10lf ",sPar->Clb_Database.Gyros.K_nu_T[8+18*i]);

		fprintf(sPar->pfInterpTxt, "\n ");


	}
}