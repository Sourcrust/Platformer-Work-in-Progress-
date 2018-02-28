#ifndef CHECK_INPUTS_H
#define CHECK_INPUTS_H
#include "Person.h"
#include "Platform.h"
#include <vector>

extern const int DASH_KEY,DASH_MAG,UP_KEY,LEFT_KEY,DOWN_KEY,RIGHT_KEY,HOLD_KEY,JUMP_KEY;
void dashCase(Person &p,bool keysPressed[343]);
void leftRightConditions(Person &p,bool keysPressed[343]);
void checkInputs(Person &p,bool keysPressed[343], std::vector<Platform> platforms);

#endif // CHECK_INPUTS_H
