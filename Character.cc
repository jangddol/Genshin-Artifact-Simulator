#include "Character.hh"
#include <algorithm>


void Character::Initialization()
{
    // ĳ���� ���� ����, ����, �������� ȿ���� ��� �ջ��ؼ� mStat���� �ѱ�� �۾�
    
    Stat WeaponMainStat = mWeapon.GetMainStat();
    Stat WeaponSubStat = mWeapon.GetSubStat();
    Stat WeaponSubSubStat = mWeapon.GetSubSubStat();
    Stat FlowerMainStat = fArtFlower.GetMainStat();
    Stat FlowerSubStat = fArtFlower.GetSubStat();
    Stat FeatherMainStat = fArtFeather.GetMainStat();
    Stat FeatherSubStat = fArtFeather.GetSubStat();
    Stat ClockMainStat = fArtClock.GetMainStat();
    Stat ClockSubStat = fArtClock.GetSubStat();
    Stat CupMainStat = fArtCup.GetMainStat();
    Stat CupSubStat = fArtCup.GetSubStat();
    Stat CrownMainStat = fArtCrown.GetMainStat();
    Stat CrownSubStat = fArtCrown.GetSubStat();

    for (int i = 0; i < 35; i++)
    {
        mStat.SetOption(i, 0.);
        mStat.AddOption(i, mCharacterStat.GetOption(i));
        mStat.AddOption(i, WeaponMainStat.GetOption(i));
        mStat.AddOption(i, WeaponSubStat.GetOption(i));
        mStat.AddOption(i, WeaponSubSubStat.GetOption(i));
        mStat.AddOption(i, FlowerMainStat.GetOption(i));
        mStat.AddOption(i, FlowerSubStat.GetOption(i));
        mStat.AddOption(i, FeatherMainStat.GetOption(i));
        mStat.AddOption(i, FeatherSubStat.GetOption(i));
        mStat.AddOption(i, ClockMainStat.GetOption(i));
        mStat.AddOption(i, ClockSubStat.GetOption(i));
        mStat.AddOption(i, CupMainStat.GetOption(i));
        mStat.AddOption(i, CupSubStat.GetOption(i));
        mStat.AddOption(i, CrownMainStat.GetOption(i));
        mStat.AddOption(i, CrownSubStat.GetOption(i));
    }
}


double Character::GetDamage()
{
    return GetDamage(mStat);
}


double Character::GetDamage(Stat stat)
{
    double AP = stat.GetAttackPer();
    double ATK = stat.GetAttack();
    double BaseATK = stat.GetBaseAttack();
    double CR = stat.GetCriticalRate();
    double CB = stat.GetCriticalBonus();

    return (BaseATK * (1 + AP / 100.) + ATK) * (1 + CR * CB / 10000.);
}


Stat Character::GenerateStatExceptArtifact()
{
    Stat returnStat;
    
    Stat WeaponMainStat = mWeapon.GetMainStat();
    Stat WeaponSubStat = mWeapon.GetSubStat();
    Stat WeaponSubSubStat = mWeapon.GetSubSubStat();

    for (int i = 0; i < 35; i++)
    {
        returnStat.SetOption(i, 0.);
        returnStat.AddOption(i, mCharacterStat.GetOption(i));
        returnStat.AddOption(i, WeaponMainStat.GetOption(i));
        returnStat.AddOption(i, WeaponSubStat.GetOption(i));
        returnStat.AddOption(i, WeaponSubSubStat.GetOption(i));
    }

    return returnStat;
}


int FindNthLargestOption(double* damArray, int nth)
{
    int tempList[10];
    // std::copy_n(std::begin(damArray), 10, std::begin(tempList));
    std::copy_n(damArray, 10, tempList);
    std::sort(tempList, tempList + 10);
    return tempList[nth - 1];
}


void Character::MakeScoreFunction()
{
    int mainOp[10] = { 0 }; // ���οɼǿ� ���������� �ִ��� Ȯ���Ѵ�. (mainOp[10])
    int numArray[10] = { 0 }; // �� �ɼ��� ��� ������ ����� ��̸� �����. (numArray[10])
    double damArray[10] = { 0 }; // �� �ɼ��� �߰��Ǿ��� ���� �������� ����� ��̸� �����. (damArray[10])
    double nowDamage = GetDamage(); // ���� ������ ����Ѵ�.

    // �� �ɼ��� 1ȸ ���׷��̵�� �ö󰡴� ��ġ
    double plusArray[10] = { 3.88999991118908, 7.76999965310097, 5.82999996840954,
                             19.4500007629395, 6.4800001680851,  5.82999996840954,
                             298.75,           23.3099994659424, 7.28999972343445,
                             23.1499996185303 };

    Stat tempStat = GenerateStatExceptArtifact(); // �������� �ʱ�ȭ�� ���ο� ������ �����Ѵ�.
    Stat tempStatArray[10] = { tempStat, tempStat, tempStat, tempStat, tempStat,
                               tempStat, tempStat, tempStat, tempStat, tempStat };

    mSavedFunction[0] = GetDamage(tempStat);

    for (int i = 0; i < 45; i++) // for������ 45ȸ����, 
    {
        double beforeDamage = GetDamage(tempStat); // ���� ���ȿ� ���� �������� ����ϰ�,
        double difEC = mTargetEC - tempStat.GetOption(4); // ���� ������ ����䱸��ġ���� ������ üũ�Ѵ�.
        bool whetherNotEnoughEC = (difEC > 0);

        if (i == 1)
        {
            for (int j = 0; j < 10; j++)
            {
                tempStatArray[j] = tempStat;
                tempStatArray[j].AddOption(j, plusArray[j]);
                damArray[j] = GetDamage(tempStatArray[j]);
                mEffectionArray[j] = damArray[j] - nowDamage;
            }
        }

        if (i <= 20)
        {
            // ������ ���� �ϸ� ������ �ִ´�.
            if (whetherNotEnoughEC && (5 - mainOp[4] > numArray[4]))
            {
                tempStat.AddOption(4, plusArray[4]);
                numArray[4] += 1;
            }
            else // �׷��� �ʴٸ�
            {
                // 10���� �οɿ� ���ؼ� �ϳ��� �߰��ϸ� ������ ��� �Ǵ��� Ȯ���Ѵ�.
                for (int j = 0; j < 10; j++) 
                {
                    tempStatArray[j] = tempStat;
                    tempStatArray[j].AddOption(j, plusArray[j]);
                    damArray[j] = GetDamage(tempStatArray[j]);
                }

                // ���� ������ ���� ���ȿ� ���ؼ� ((5 - �ֿɿ���) ���� ���� ä���°�?)�� Ȯ���ϰ� ä���.
                    // �Ұ����� ���
                        // ���� ������ ���� ���ȿ� ���ؼ� Ȯ���Ѵ�. (�ִ� 5ȸ �ݺ�)
                for (int j = 1; j <= 5; j++)
                {
                    int largeStat = FindNthLargestOption(damArray, j); 
                    if (5 - mainOp[largeStat] > numArray[largeStat])
                    {
                        tempStat.AddOption(largeStat, plusArray[largeStat]);
                        numArray[largeStat] += 1;
                        break;
                    }
                }
            }
        }
        else
        {
            if (whetherNotEnoughEC)
            {
                tempStat.AddOption(4, plusArray[4]);
                numArray[4] += 1;
            }
            else
            {
                for (int j = 0; j < 10; j++)
                {
                    tempStatArray[j] = tempStat;
                    tempStatArray[j].AddOption(j, plusArray[j]);
                    damArray[j] = GetDamage(tempStatArray[j]);
                }

                for (int j = 1; j <= 2; j++)
                {
                    int largeStat = FindNthLargestOption(damArray, j);
                    if (30 - mainOp[largeStat] != numArray[largeStat])
                    {
                        tempStat.AddOption(largeStat, plusArray[largeStat]);
                        numArray[largeStat] += 1;
                        break;
                    }
                }
            }
        }
        mSavedFunction[i + 1] = GetDamage(tempStat);
    }
}


double Character::GetScore()
{
    double spec = GetDamage();
    if (mSavedFunction[0] == spec) return 0.;

    double score;
    for (int i = 0; i < 45; i++)
    {
        if ((mSavedFunction[i] < spec) && (spec <= mSavedFunction[i + 1]))
        {
            score = i + (spec - mSavedFunction[i]) / (mSavedFunction[i + 1] - mSavedFunction[i]);
            if (mTargetEC - mStat.GetElementCharge() > 0)
            {
                score -= (mTargetEC - mStat.GetElementCharge()) / 6.4800001680851;
            }
            break;
        }
    }
	return score;
}


void Character::SetArtifact(ArtFlower flower, ArtFeather feather, ArtClock clock, 
							ArtCup cup, ArtCrown crown)
{
	fArtFlower = flower;
	fArtFeather = feather;
	fArtClock = clock;
	fArtCup = cup;
	fArtCrown = crown;
}