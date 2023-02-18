#include "../header/Character.hh"
#include "../header/ArtSetStat.hh"


void ArtSetStat::SaveCharacterPointer(Character* character)
{
    mCharactersUsingThis.emplace_back(character);
}


void ArtSetStat::DeleteCharacterPointer(const Character* character)
{
    std::size_t index = 0;
    std::size_t size = mCharactersUsingThis.size();
    for(std::size_t i = 0; i < size; i++)
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
	std::size_t length = mCharactersUsingThis.size();
	for (std::size_t i = 0; i < length; i++)
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