#ifndef SOLARPEARL_HH
#define SOLARPEARL_HH


#include "../Weapon.hh"


class SolarPearl : public Weapon
{
public:
	SolarPearl()
    {
        for (int i = 0; i < 35 ; i++)
        {
            mMainStat.SetOption(i, 0.);
            mSubStat.SetOption(i, 0.);
            mSubSubStat.SetOption(i, 0.);
        }
        mMainStat.SetOption(3, 510.);
        mSubStat.SetOption(0, 27.6);
        mSubSubStat.SetOption(19, 40.);
        mSubSubStat.SetOption(22, 40.);
        mSubSubStat.SetOption(23, 40.);
    }
	~SolarPearl() {}

};

#endif