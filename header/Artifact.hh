#ifndef ARTIFACT_HH
#define ARTIFACT_HH


#include "Stat.hh"
#include <vector>


// double artInitTimeList[5] = { 0. }; // global
constexpr double MAXMAINOPTIONLIST[19]
		= { 31.1, 62.2, 46.6, 311., 51.8,
			46.6, 4780., 187., 58.3, 0.,
			46.6, 46.6, 46.6, 46.6, 46.6,
			46.6, 46.6, 46.6, 35.9 };

constexpr double SUBOPTPROB[10] = { 3, 3, 4, 6, 4, 4, 6, 4, 4, 6 };
constexpr double OPTIONARRAY[10][4] =
{ {2.72000003606081, 3.10999993234873, 3.50000001490116, 3.88999991118908},
	{5.44000007212162, 6.21999986469746, 6.98999986052513, 7.76999965310097},
	{4.08000014722347, 4.65999990701675, 5.24999983608723, 5.82999996840954},
	{13.6199998855591, 15.5600004196166, 17.5100002288818, 19.4500007629395},
	{4.52999994158745, 5.18000014126301, 5.82999996840954, 6.4800001680851 },
	{4.08000014722347, 4.65999990701675, 5.24999983608723, 5.82999996840954},
	{209.130004882813, 239.            , 268.880004882813, 298.75          },
	{16.3199996948242, 18.6499996185303, 20.9799995422363, 23.3099994659424},
	{5.09999990463257, 5.82999996840954, 6.56000003218651, 7.28999972343445},
	{16.2000007629395, 18.5200004577637, 20.8299999237061, 23.1499996185303} };


class Artifact
{
public:
	Artifact() {}
	~Artifact() {}

	void Generation();
	void Generation(int mainType);
	virtual int GetType() { return mType; }
	int GetMainType() { return mMainType; }
	void SetMainType(int mainType);

	Stat GetMainStat() { return mMainStat; }
	Stat GetSubStat() { return mSubStat; }

protected:
	int mType = 0;
	std::vector<int> mProbabiltyWeight;
	std::vector<int> mCummulatedWeight;

private:
	void GenerateMainOption();
	void GenerateSubOption();

	void             FullMainOption(int mainType);
	int              UseCummulatedWeight(std::vector<int> cummulatedWeight);
	std::vector<int> GenerateCummulatedWeight();
	bool             Selected3or4OptStart();
	std::vector<int> GenerateStartOpt(std::vector<int> cummulatedWeight);
	void             UpgradeSubOption(std::vector<int> startOptList, bool whether4OptStart);

	int mMainType = 0;

	Stat mMainStat;
	Stat mSubStat;
};

#endif