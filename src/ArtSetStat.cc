#include "../header/Character.hh"
#include "../header/ArtSetStat.hh"


void ArtSetStat::SaveCharacterPointer(Character* character)
{
    mCharactersUsingThis.emplace_back(character);
}


void ArtSetStat::DeleteCharacterPointer(const Character* character)
{
    int index = 0;
    int size = mCharactersUsingThis.size();
    for(int i = 0; i < size; i++)
    {
        if (mCharactersUsingThis[index] == character)
        {
            mCharactersUsingThis.erase(mCharactersUsingThis.begin() + index);
        }
        else index++;
    }
}


bool ArtSetStat::IsUsingThis(const Character* character) const
{
	bool returnBool = false;
	int length = mCharactersUsingThis.size();
	for (int i = 0; i < length; i++)
	{
		if (character == mCharactersUsingThis[i])
		{
			returnBool = true;
			break;
		}
	}
	return returnBool;
}


void ArtSetStat::AlertModified() const
{
    for(auto& character : mCharactersUsingThis)
    {
        character->ConfirmArtSetStatModified();
    }
}