#include <cmath>
#include <iostream>
#include <cstdlib>
#include "stars.h"

using namespace std;

//-----------------------------------------------------------------------//
// Модель светимости пульсара, при которой энергия в конусе излучения
// распределена равномерно, как это было сделано у Faucher ...
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
// Декларация функций
double S_min (double, double, float, double, double, double, float, TMap *);
double norm_distr();
//-----------------------------------------------------------------------//

double NeutronStar::is_pulsar_visible (double t, SpecialStar * sun, TMap * T_copy, parametrs_lum * param) {
    float l, b, sm, DM;
    double L, eps_P = -1.5, eps_dot_P = 0.5, L_corr=0.8, L_0 = 0.18e-3;
    double dist_to_sun, lum_min, w50;
    double first[3], second[2];

    sun->move_to(t);
    dist_to_sun = sqrt(pow(sun->get_position_x() - x, 2) + pow(sun->get_position_y() - y, 2) + pow(sun->get_position_z() - z, 2));
    //sun->move_to(-t);

    //                w50 = 0.05*get_P(t);

    w50 = 6.81e-3*sqrt(get_P(t))/sin(i_incl);

    //		DM = get_DM (t, sun, &l, &b, &sm);
    //cout<<"Same pulsar"<<endl;
    //cout<<l<<"\t"<<b<<endl;


    first [0] = x - sun->get_position_x();
    first [1] = y - sun->get_position_y();
    first [2] = z - sun->get_position_z();


    // Вектор от Солнца к центру Галактики
    second[0] = - sun->get_position_x();
    second[1] = - sun->get_position_y();

    b = first[2] / dist_to_sun;
    b = asin (b)/2./pi*360.;

    l = atan2(second[0]*first[1]-second[1]*first[0], first[0]*second[0]+first[1]*second[1])*180./3.1415926;

    if (l<0) {
        l=360.+l;
    }

    L=log10(L_0*pow(get_P(t), eps_P)*pow(get_dot_P(t)/1e-15, eps_dot_P))+L_corr*norm_distr()+0.5;
    L = pow(10., L)/pow(dist_to_sun, 2.);


    if (dist_to_sun < 20 && abs(b) < 15 && (l<=50 || l>=230) && L>=5.e-6)  	{
        //	DM=15*dist_to_sun;
        DM = get_DM (t, sun, &l, &b, &sm);
        lum_min = S_min (l, b, sm, dist_to_sun, w50, get_P(t), DM, T_copy);
    } else {
        lum_min = 1e9;
    }




    //cout<<l/180.*3.1415926<<"\t"<<b/180.*3.1415926<<endl;
    //cout<<"Finita la comedia"<<endl;

    //cout<<L<<"\t"<<lum_min<<endl;
    if (L > lum_min /*&& abs(b)<15. && (l>=230. || l<=50.)*/) {
        return L;
    } else {
        return 0;
    }
}

//--------------------------------------------//
// Do not use this function twice!!!
// For singular use only!
//--------------------------------------------//
bool parametrs_lum::is_beam_on(double P) {
    double f, chance_1;
    f=0.09*pow(log10(P/10.0),2.)+0.03;
    chance_1 = rand () / rand_high_board;

    if (f >= chance_1) {
        return 1;
    } else {
        return 0;
    }

}


parametrs_lum::parametrs_lum (ifstream * in) {
}

void parametrs_lum::print_description (ostream * out) {
    *out<<"// Используется модель светимости C, а именно модель, где   //"<<endl;
    *out<<"// распределение энергии внутри конуса излучения принято    //"<<endl;
    *out<<"// плоским как это было у Faucher                           //"<<endl;
    *out<<"//----------------------------------------------------------//"<<endl;
}

void parametrs_lum::print_parametrs   (ostream * out) {
    *out<<"//                  Параметров у модели нет                 //"<<endl;
    *out<<"//----------------------------------------------------------//"<<endl;
}

void parametrs_lum::print_short      (ostream * out) {
}
