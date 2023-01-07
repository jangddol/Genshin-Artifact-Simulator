#include "../../header/Character/Ningguang.hh"


double Ningguang::GetDamage(Stat stat)
{   
    double CR = stat.GetCriticalRate();
    if (CR > 100) CR = 100.;
    if (CR < 0) CR = 0.;
    double CB = stat.GetCriticalBonus();
    double geoBonus = stat.GetGeoBonus();
    double normalAttackBonus = stat.GetNormalAttackBonus();
    double strongAttackBonus = stat.GetStrongAttackBonus();
    double eBonus = stat.GetEBonus();
    double qBonus = stat.GetQBonus();
    double totalATK = stat.GetTotalAttack();
    double resistCoef = stat.GetResistCoef();
    double defenseCoef = stat.GetDefenseCoef();
    double levelCoef = stat.GetLevelCoef();

    int constellation = 6;   //////////////////////  별자리 기능 추가
    
    int ningguangStarPlus = 0;
    if (constellation > 5) ningguangStarPlus = 6;

    double damBonus = (100. + geoBonus) / 100.;

    double temp1 = 0.533 * 2. * 4. * (damBonus + normalAttackBonus / 100.);
    double temp2 = 4.38 * 2. * (damBonus + eBonus / 100.);
    double temp3 = 3.31 * 3. * (damBonus + strongAttackBonus / 100.);
    double temp4 = 0.944 * (3. + (double)ningguangStarPlus) * (damBonus + strongAttackBonus / 100.);
    double temp5 = 1.65 * 12. * (damBonus + qBonus / 100.);

    double result = totalATK * (1 + CR * CB / 10000.) * resistCoef * defenseCoef * levelCoef * (temp1 + temp2 + temp3 + temp4 + temp5);
    return result;
}