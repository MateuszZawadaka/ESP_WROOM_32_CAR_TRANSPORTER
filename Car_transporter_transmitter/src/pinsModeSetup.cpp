#include<pinsModeSetup.h>


void pinsModeSetup::setInputMode(int elementsInArray, const int elementsPin[]){
    for (int i = 0; i < elementsInArray; i++)
    {
        pinMode(elementsPin[i], INPUT_PULLUP);
    }
    
}

void pinsModeSetup::setOutputMode(int elementsInArray, const int elementsPin[]){
    for (int i = 0; i < elementsInArray; i++)
    {
        pinMode(elementsPin[i], OUTPUT);
    }
    

}