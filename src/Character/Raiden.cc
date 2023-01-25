#include "../../header/Character/Raiden.hh"


double Raiden::GetDamageWithStat(Stat stat) const
{
    double CR = stat.GetCriticalRate();
    if (CR > 100) CR = 100.;
    if (CR < 0) CR = 0.;
    double CB = stat.GetCriticalBonus();
    double elecBonus = stat.GetElectroBonus();
    double qBonus = stat.GetQBonus();
    double eBonus = stat.GetEBonus();
    double totalATK = stat.GetTotalAttack();
    double resistCoef = stat.GetResistCoef();
    double defenseCoef = stat.GetDefenseCoef();
    double levelCoef = stat.GetLevelCoef();
    
    double wonryuk = mTotalPartyEnergy * 0.19 + 6.;
    
    double burstDamage = 6.41 + wonryuk * 0.0622; // busrt, Qlv = 8
    double qAttackDamage = 11.04 + wonryuk * 0.1508;
    // qAttackDamage = 70.6% * 3 + 69.4% * 3 + 85% * 3
                    // + (48.8% + 48.9%) * 2 + 116.8% * 2 (total 13 hits)
                    // + hitNum * wonryuk * 1.16%
                    // 70.6% * 3 + 69.4% * 3 + 85% * 3
                    // + (48.8% + 48.9%) * 2 + 116.8% * 2 = 11.04 (1104%)
                    // 13 * 0.0116 = 0.1508
    double eDamage = 15 * 0.672 + 1.88; // burst : 188%, DOT : 15hits * 67.2% (15hits is very small)

    double totalDamage = (burstDamage + qAttackDamage) * qBonus / 100. + eDamage * eBonus / 100.;
    return totalATK * (1 + CR * CB / 10000.) * defenseCoef * levelCoef * resistCoef * totalDamage;
}
