#include "../../header/Character/Kokomi.hh"
#include "../../header/Weapon/EverlastingMoonglow.hh"


double Kokomi::GetDamageWithStat(const Stat& stat) const
{   
    double totalHP = stat.GetTotalHP();
    double totalATK = stat.GetTotalAttack();
    double healBonus = stat.GetHealBonus();
    double CR = stat.GetCriticalRate();
    if (CR > 100) CR = 100.;
    if (CR < 0) CR = 0.;
    double CB = stat.GetCriticalBonus();
    double resCoef = stat.GetResistCoef();
    double defCoef = stat.GetDefenseCoef();
    double LvCoef = stat.GetLevelCoef();
    double hydroBonus = stat.GetHydroBonus();
    double normalAttackBonus = stat.GetNormalAttackBonus();
    double eBonus = stat.GetEBonus();

    double normalAttackPlusDMG = 0;
    if (this->GetWeaponName() == EVERLASTINGMOONGLOW)
    {
        normalAttackPlusDMG = totalHP * 0.01;
    }
    
    double damage = totalATK * (1.09 + 0.985 + 1.51) / 3.;
    double eDamage = totalATK * 1.75 + totalHP * 0.114;
    double QDamage = totalHP * 0.0774;
    
    double cure = ((totalHP * 0.0129 + 144.) * 15. + (totalHP * 0.0704 + 795.) * 5.) * (100. + healBonus) / 100.;
    
    double physicsPlusDMG = cure * 0.9; // 조개 4셋 가정
    
    double normalDamageBonus = (100. + hydroBonus + normalAttackBonus + healBonus * 0.15) / 100.;
    double eDamageBonus = (100. + hydroBonus + eBonus) / 100.;
    
    double returnValue = ((damage + QDamage + normalAttackPlusDMG) * 15. * normalDamageBonus + eDamage * 5. * eDamageBonus) * (1. + CR * CB / 10000.) * resCoef * defCoef * LvCoef + physicsPlusDMG;
    return returnValue;
}