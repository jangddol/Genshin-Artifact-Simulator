#include "../../header/Character/Raiden.hh"


double Raiden::GetDamageWithStat(Stat stat) const
{
    double CR = stat.GetCriticalRate();
    if (CR > 100) CR = 100.;
    if (CR < 0) CR = 0.;
    double CB = stat.GetCriticalBonus();
    double elecBonus = stat.GetElectroBonus();
    double qBonus = stat.GetQBonus();
    double totalATK = stat.GetTotalAttack();
    double resistCoef = stat.GetResistCoef();
    double defenseCoef = stat.GetDefenseCoef();
    double levelCoef = stat.GetLevelCoef();

    double damBonus = (100. + elecBonus + qBonus) / 100.;
    double result = totalATK * (1 + CR * CB / 10000.) * damBonus * resistCoef * defenseCoef * levelCoef;
    return result;
}
