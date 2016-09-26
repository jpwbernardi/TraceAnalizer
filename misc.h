/*
 * misc.h
 *
 *  Created on: 04/02/2010
 *      Author: elmano
 */

#ifndef MISC_H_
#define MISC_H_
#endif /* MISC_H_ */

#define PI 3.1415926


//-----------------------------------------------------------------//
//Modular function
int my_mod ( int i, int j) {	return int(float(i) / (float) j); }

//-----------------------------------------------------------------//
//--------Calculate the distance between two nodes-----------------//
float dist(float a0, float b0, float a1, float b1) {
	float distx, disty;
	distx = a0-a1;
	disty = b0-b1;
	return sqrt(distx * distx + disty * disty);
}

//-----------------------------------------------------------------//
//--------Calculate the angle between two nodes--------------------//
double get_angle(double a0,double b0,double a1,double b1)
{
	double distx,disty;
	double angle = 0;
	distx = a1-a0;
	disty = b1-b0;

	if(disty==0 && distx==0)
		angle = 0 ;
	else if(distx==0 && disty>0)
		angle = PI/2;
	else if(distx==0 && disty<0)
		angle =-PI/2;
	else if (disty>=0 && distx>0)   //1
		angle=atan((disty)/(distx));
	else if (disty<=0 && distx>0)  //4
		angle=atan((disty)/(distx));
	else if (disty>=0 && distx<0)   //2
		angle=atan((disty)/(distx))+PI;
	else if (disty<=0 && distx<0)    //3
		angle=atan((disty)/(distx))-PI;
	angle = floor(angle * 100 +  0.5) / 100;
	return angle; //to avoid returning values like 1.72E-12
}


//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//return the min number
double max(double a, double b)
{
	if(a>=b)
		return a;
	else
		return b;
}


//-----------------------------------------------------------------//
//return the max number
double min(double a, double b)
{
	if(a<=b)
		return a;
	else
		return b;
}


//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//----------- Calculate the Mobility metric ------------------------//
double relative_speed(double speed1,double angle1,double speed2, double angle2)
{
	double rv;

	rv=sqrt(speed1*speed1+speed2*speed2-2*speed1*speed2*cos(angle1-angle2));

	return rv;
}


//-----------------------------------------------------------------//
//----------- Calculate the correlation metric ---------------------//
double DSDijt(double speed1, double angle1, double speed2, double angle2)
{
	double x_speed1 = speed1*cos(angle1);
	double y_speed1 = speed1*sin(angle1);

	double x_speed2 = speed2*cos(angle2);
	double y_speed2 = speed2*sin(angle2);

	double cor = 0.0;

	if( (speed1*speed2!=0.0) && (max(speed1,speed2)!=0) ){
		cor = (x_speed1*x_speed2 + y_speed1*y_speed2)/(speed1*speed2)*(min(speed1,speed2)/max(speed1,speed2));
		return cor;
	}
	else
		return 0.0;
}

double basic_correlation_positive(double speed1, double angle1, double speed2, double angle2)
{
	double x_speed1 = speed1*cos(angle1);
	double y_speed1 = speed1*sin(angle1);

	double x_speed2 = speed2*cos(angle2);
	double y_speed2 = speed2*sin(angle2);

	double cor = 0.0;

	if( (speed1*speed2!=0.0) && (max(speed1,speed2)!=0) ){
		cor = (x_speed1*x_speed2 + y_speed1*y_speed2)/(speed1*speed2)*(min(speed1,speed2)/max(speed1,speed2));
		return cor <= 0.0 ? 0.0 : cor; //negative cor does not make sense for temporal dependence
	}
	else
		return 0.0;
}


//-----------------------------------------------------------------//
//----------- Calculate the angle correlation
double angle_correlation(double speed1, double angle1, double speed2, double angle2)
{
	double x_speed1 = speed1*cos(angle1);
	double y_speed1 = speed1*sin(angle1);
	double x_speed2 = speed2*cos(angle2);
	double y_speed2 = speed2*sin(angle2);

	if(speed1*speed2!=0.0)
		return (x_speed1*x_speed2 + y_speed1*y_speed2)/(speed1*speed2);
	else
		return 0.0;
}


//-----------------------------------------------------------------//
//------------ Calculate the speed correlation
double speed_correlation(double speed1, double angle1,double speed2,double angle2)
{
	if(max(speed1,speed2)!=0.0)
		return (min(speed1,speed2)/max(speed1,speed2));
	else
		return 0.0;
}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//

float getStd(float array[], float mean, int size){

	float desvio = 0;
	float sum = 0;

	for (int k = 0; k < size; ++k) {
		desvio = array[k] - mean;
		desvio *= desvio;
		sum += desvio;
	}

	return sqrt(sum/size);
}

float getAverage(float array[], int size){

	float sum = 0;
	for (int k = 0; k < size; ++k) {
		sum += array[k];
	}

	return sum/size;
}

float getAverageNotZero(float array[], int size){

	//discard null values (0).
	float sum = 0;
	int count = 0;

	for (int k = 0; k < size && array[k] > 0; ++k) {
		sum += array[k];
		count++;
	}

	return sum/count;
}

float getStdNotZero(float array[], float mean, int size){

	float desvio = 0;
	float sum = 0;
	int count = 0;

	for (int k = 0; k < size && array[k] > 0; ++k) {
		desvio = array[k] - mean;
		desvio *= desvio;
		sum += desvio;
		count++;
	}

	return sqrt(sum/count);
}
double coefficient_of_variation(float mean, float std)
{
	return std/mean;
}
