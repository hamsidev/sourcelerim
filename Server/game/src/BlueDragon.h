extern int BlueDragon_StateBattle (LPCHARACTER);
extern time_t UseBlueDragonSkill (LPCHARACTER, unsigned int);
#ifdef on
extern int64_t BlueDragon_Damage (LPCHARACTER me, LPCHARACTER attacker, int64_t dam);
#else
extern int BlueDragon_Damage (LPCHARACTER me, LPCHARACTER attacker, int dam);
#endif
// vaffanculo a chi t'e morto martysama
