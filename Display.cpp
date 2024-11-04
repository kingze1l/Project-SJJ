#include "Display.h"
#include <iostream>
#include <sstream>
#include <windows.h>
#include <cstdlib> 

using namespace std;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void displayLogo() {
    setColor(3);
    cout << "    ***************************************************************************************************************\n";
    setColor(9);
    cout << "                    MMP\"\"MM\"\"YMM       7MM                     mm    7MMF    7MMF           7MM        \n";
    setColor(11);
    cout << "                    P'   MM   `7        MM                     MM     MM      MM             MM         \n";
    setColor(14);
    cout << "                         MM   ,6\"Yb.    MM  .gP\"Ya  7MMpMMMb.mmMMmm   MM      MM  7MM   7MM  MM,dMMb.  \n";
    setColor(10);
    cout << "                         MM  8)   MM    MM ,M'   Yb  MM    MM  MM     MMmmmmmmMM   MM    MM  MM    `Mb \n";
    setColor(13);
    cout << "                         MM   ,pm9MM    MM 8M\"\"\"\"\"\"  MM    MM  MM     MM      MM   MM    MM  MM     M8 \n";
    setColor(12);
    cout << "                         MM  8M   MM    MM YM.    ,  MM    MM  MM     MM      MM   MM    MM  MM.   ,M9 \n";
    setColor(14);
    cout << "                        JMML `Moo9^Yo  JMML `Mbmmd' JMML  JMML  Mbmo JMML    JMML  `Mbod\"YML.P^YbmdP'  \n";
    setColor(3);
    cout << "    ***************************************************************************************************************\n";
    setColor(15);
}

void displayMenu() {
    setColor(11); // Cyan
    cout << "\n                           1. ";
    setColor(10); // Green
    cout << "Student Sign-Up";
    setColor(11); // Cyan
    cout << "\n                           2. ";
    setColor(14); // Yellow
    cout << "Sign In";
    setColor(11); // Cyan
    cout << "\n                           3. ";
    setColor(12); // Red
    cout << "Exit";
    setColor(15); // Reset to white
    cout << "\n                           Choose an option: ";
}
