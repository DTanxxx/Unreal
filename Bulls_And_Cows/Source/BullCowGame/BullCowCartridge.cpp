// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    // Load the HiddenWordList text file's content into a TArray member variable, according to a predicate
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate
    (
        Isograms, 
        *WordListPath,
        [](const FString& Word) 
        {
            return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word);
        }
    );

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame() 
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; 
    Lives = HiddenWord.Len() * 2; 
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull-Cows!"));
    PrintLine(FString::Printf(TEXT("Guess the %i letter word"), HiddenWord.Len()));
    PrintLine(TEXT("Type in your guess and press Enter"));
    PrintLine(FString::Printf(TEXT("You have %i lives"), Lives));  
    // PrintLine(FString::Printf(TEXT("Word: %s; %i characters long"), *HiddenWord, HiddenWord.Len()));  // Debug line
}

void UBullCowCartridge::EndGame() 
{
    bGameOver = true;
    PrintLine(TEXT("\nPress Enter to play again..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord) 
    {
        ClearScreen();
        PrintLine(TEXT("You Won!"));
        EndGame();
        return;
    }

    // Check if correct number of letters
    if (Guess.Len() != HiddenWord.Len()) 
    {
        PrintLine(TEXT("Wrong number of letters! \nLives will not be reduced"));
        PrintLine(FString::Printf(TEXT("Hidden word is %i letters long"), HiddenWord.Len()));
        return;
    }

    // Check if isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("There should not be any repeating letters"));
        PrintLine(TEXT("Guess again"));
        return;
    }

    // Lose a life here
    --Lives;
    PrintLine(TEXT("You lost a life!"));

    // Check if no lives left
    if (Lives <= 0) 
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(FString::Printf(TEXT("Hidden word was: %s"), *HiddenWord));
        EndGame();
        return;        
    }

    // Show the player Bulls and Cows counts
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(FString::Printf(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows));
    PrintLine(FString::Printf(TEXT("Guess again, you have %i lives left"), Lives));
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 Index = 0; Index < Word.Len() - 1; ++Index) 
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); ++Comparison)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    
    return true;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); ++GuessIndex)
    {
        // If index in Guess is same as index in HiddenWord, incre BullCount
        if (Guess[GuessIndex] == HiddenWord[GuessIndex]) 
        {
            ++Count.Bulls;
            continue;
        }
        
        // If not a bull check for cow -- letter in Guess appears in HiddenWord
        for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); ++HiddenWordIndex)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenWordIndex]) 
            {
                ++Count.Cows;
                break;
            }
        }
    }

    return Count;
}

// TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const 
// {
//     TArray<FString> ValidWords;

//     for (FString Word : WordList)
//     {
//         if (!IsIsogram(Word)) 
//         {
//             continue;
//         }

//         if (Word.Len() >= 4 && Word.Len() <= 8) 
//         {
//             ValidWords.Emplace(Word);
//         }
//     }

//     return ValidWords;
// }
