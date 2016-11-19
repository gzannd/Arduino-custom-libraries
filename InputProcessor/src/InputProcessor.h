#include "Arduino.h"
#ifndef InputProcessor_h
#define InputProcessor_h


enum INPUT_RESULT
{
      NO_MATCH,
      MATCH,
      INPUT_ERROR
};
    
class InputProcessor
{
  private:
    String accumulator = "";
    int maxInputLength = 5;
    String passwords[10];
  
  public: 
    InputProcessor();
    INPUT_RESULT HandleInput(char inputCharacter);
    void Reset();
    bool EvaluateInput(String inputBuffer);
};

#endif
