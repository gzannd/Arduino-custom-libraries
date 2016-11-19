#include "Arduino.h"
#include "InputProcessor.h"



InputProcessor::InputProcessor()
{
    //Eventually want to add a repository.
    passwords[0] = "12345";
    passwords[1] = "11111";
    passwords[2] = "98765";
}

bool InputProcessor::EvaluateInput(String inputBuffer)
{
  //Evaluate input
  for(int i=0;i<3;i++)
  {
    if(passwords[i] == accumulator)
    {
      return true;
    }
  }

  return false;
}

void InputProcessor::Reset()
{
  accumulator = "";  
}

INPUT_RESULT InputProcessor::HandleInput(char inputCharacter)
{
   INPUT_RESULT inputResult;
  
   if(inputCharacter == '*')
   {
      Reset();
      inputResult = NO_MATCH;
   }
   else if(inputCharacter == '#')
   {
      bool result = EvaluateInput(accumulator);
      
      if(result == true)
      {
        inputResult = MATCH;
      }
      else
      {
        inputResult = INPUT_ERROR;
      }
   }
   else
   {
      if(accumulator.length() < maxInputLength)
      {
        accumulator += inputCharacter;
        inputResult = NO_MATCH;
      }
      else
      {
        //Indicate some sort of error.
        inputResult = INPUT_ERROR;
      }
   }

   if(inputResult == MATCH || inputResult == INPUT_ERROR)
   {
      Reset();
   }

   return inputResult;
}
