#include <iostream>
#include <ctime>

void PrintIntroduction(int Difficulty) {
    std::cout << "\n\nYou are a secret agent breaking into a level " << Difficulty << " secure server room...\n";
    std::cout << "You need to enter the correct codes to continue...\n";
    std::cout << "There are 3 numbers in the code\n\n";
}

bool PlayGame(int Difficulty) 
{
    PrintIntroduction(Difficulty);

    const int CodeA = rand() % Difficulty + Difficulty;
    const int CodeB = rand() % Difficulty + Difficulty;
    const int CodeC = rand() % Difficulty + Difficulty;

    const int CodeSum = CodeA + CodeB + CodeC;
    const int CodeProduct = CodeA * CodeB * CodeC;

    std::cout << "The codes add up to: " << CodeSum;
    std::cout << "\nThe codes multiply to give: " << CodeProduct << std::endl;

    int GuessA, GuessB, GuessC;
    std::cin >> GuessA;
    std::cin >> GuessB;
    std::cin >> GuessC;
    
    int GuessSum = GuessA + GuessB + GuessC;
    int GuessProduct = GuessA * GuessB * GuessC;

    if (GuessSum == CodeSum && GuessProduct == CodeProduct) 
    {
        std::cout << "\nYou Win! Onto the next level";
        return true;
    }
    else 
    {
        std::cout << "\nWrong Guess! Retry this level";
        return false;
    }
}

int main() 
{
    srand(time(NULL));  // create new random sequence based on time of day

    const int MaxLevelDifficulty = 5;
    int LevelDifficulty = 1;

    while (LevelDifficulty <= MaxLevelDifficulty) 
    {
        bool bLevelComplete = PlayGame(LevelDifficulty);
        std::cin.clear();  // clears any errors in relation to the input
        std::cin.ignore();  // discards the buffer

        if (bLevelComplete) 
        {
            // increase level difficulty
            ++LevelDifficulty;
        }
    }

    std::cout << "\nCongrats on finishing the game!";

    return 0;
}
