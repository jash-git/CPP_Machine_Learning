#include <iostream>
#include <cstdio>
#include "LineReg.h"

using namespace std;

void Pause()
{
    printf("Press Enter key to continue¡K");
    fgetc(stdin);
}

int main()
{
    LineReg();
    Pause();
    return 0;
}
