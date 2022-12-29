#include "Ningguang.hh"


double Ningguang::GetDamage(Stat stat)
{
    double CR = stat[0];
    double CB = stat[1];
    double geoBonus = stat[15];
    double normalAttackBonus = stat[19];
    double strongAttackBonus = stat[20];
    double eBonus = stat[22];
    double qBonus = stat[23];
    double totalATK = stat[24];
    double resistCoef = stat[30];
    double defenseCoef = stat[31];
    double levelCoef = stat[34];

    int constellation = 6;   //////////////////////  별자리 기능 추가
    
    int ningguangStarPlus;
    if (constellation > 5)
    {
        ningguangStarPlus = 6;
    }
    else
    {
        ningguangStarPlus = 0;
    }

    double damBonus = (100 + geoBonus) / 100;

    double temp1 = 0.533 * 2 * 4 * (damBonus + normalAttackBonus / 100);
    double temp2 = 4.38 * 2 * (damBonus + eBonus / 100);
    double temp3 = 3.31 * 3 * (damBonus + strongAttackBonus / 100);
    double temp4 = 0.944 * (3. + (double)ningguangStarPlus) * (damBonus + strongAttackBonus / 100);
    double temp5 = 1.65 * 12 * (damBonus + qBonus / 100);

    double result = totalATK * (1 + CR * CB / 10000) * resistCoef * defenseCoef * levelCoef * (temp1 + temp2 + temp3 + temp4 + temp5);
    return result;
}