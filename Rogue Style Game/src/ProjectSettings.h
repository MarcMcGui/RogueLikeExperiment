#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

enum Direciton
{
    foward = 0,
    back = 1,
    left = 2,
    right = 3
};

extern char WallSymbol;
extern char FreeSpace;
extern char HallSpace;
extern char PlayerSymbol;

//enemy symbols
extern char EnemySymbol;
extern char EnemySymbol2;
extern char EnemySymbol3;

extern int StartDamage;
extern int StartHealth;
extern int StartArmor;

extern float DamageLevelMult;
extern float HealthLevelMult;
extern float ArmorLevelMult;

#endif