#include <string.h>
#include "testing.h"
#include "character.c"

test roll_bonus_application_test() {
    DEF_TEST("a character's roll bonus is applied to their attack roll");

    attack assault;
    int bonus;

    bonus = 1;
    attack_roll(&assault, 20, bonus, 10);
    DO_TEST(assault.status == CRITICAL, "bonus doesn't matter on CRITICAL");

    bonus = 5;
    attack_roll(&assault, 15, bonus, 10); // total_roll == 20
    DO_TEST(assault.status == HIT, "bonus doesn't create a CRITICAL");

    bonus = 10;
    attack_roll(&assault, 1, bonus, 10);
    DO_TEST(assault.status == HIT, "bonus adds to roll to beat armor class");

    bonus = 5;
    attack_roll(&assault, 1, bonus, 10);
    DO_TEST(assault.status == MISS, "insufficient bonus still creates a MISS");

    END_TEST;
}

test level_roll_bonus_test() {
    DEF_TEST("a character's attack roll bonus increases by 1 for each EVEN "
            "numbered level achieved");

    int level;

    level = 1;
    DO_TEST(calculate_roll_bonus(level) == 0, "no roll bonus at level 1");

    level = 3;
    DO_TEST(calculate_roll_bonus(level) == 1, "roll bonus for level 2");

    level = 5;
    DO_TEST(calculate_roll_bonus(level) == 2, "roll bonus for level 2 and 4");

    END_TEST;
}

test xp_level_test() {
    DEF_TEST("a new character has 0 xp and level is (1 + floor(xp / 1000))");

    int xp;

    character bob;
    new_character(&bob, "bobby hill");
    DO_TEST(bob.xp == 0, "new character has 0 xp");

    xp = 0;
    DO_TEST(calculate_level(xp) == 1, "at 0 xp, level is 1");
    
    xp = 41999;
    DO_TEST(calculate_level(xp) == 42, "at 41999 xp, level is 42");

    xp = 42000;
    DO_TEST(calculate_level(xp) == 43, "at 42000 xp, level is 43");

    END_TEST;
}

test ability_score_test() {
    DEF_TEST("given a new character, ability scores default to 10");

    character bob;
    new_character(&bob, "bilbob");
     
    DO_TEST(bob.stats.strength == 10, "new character has 10 strength");
    DO_TEST(bob.stats.dexterity == 10, "new character has 10 dexterity");
    DO_TEST(bob.stats.constitution == 10, "new character has 10 constitution");
    DO_TEST(bob.stats.wisdom == 10, "new character has 10 wisdom");
    DO_TEST(bob.stats.intelligence == 10, "new character has 10 intelligence");
    DO_TEST(bob.stats.charisma == 10, "new character has 10 charisma");

    END_TEST;
}

test ability_modifier_test() {
    DEF_TEST("given an ability score, calculate the associated modifier "
            "according to the data in these tests");

    DO_TEST(calculate_ability_modifier(1)  == -5, "ability modifier for 1");
    DO_TEST(calculate_ability_modifier(2)  == -4, "ability modifier for 2");
    DO_TEST(calculate_ability_modifier(3)  == -4, "ability modifier for 3");
    DO_TEST(calculate_ability_modifier(4)  == -3, "ability modifier for 4");
    DO_TEST(calculate_ability_modifier(5)  == -3, "ability modifier for 5");
    DO_TEST(calculate_ability_modifier(6)  == -2, "ability modifier for 6");
    DO_TEST(calculate_ability_modifier(7)  == -2, "ability modifier for 7");
    DO_TEST(calculate_ability_modifier(8)  == -1, "ability modifier for 8");
    DO_TEST(calculate_ability_modifier(9)  == -1, "ability modifier for 9");
    DO_TEST(calculate_ability_modifier(10) ==  0, "ability modifier for 10");
    DO_TEST(calculate_ability_modifier(11) ==  0, "ability modifier for 11");
    DO_TEST(calculate_ability_modifier(12) ==  1, "ability modifier for 12");
    DO_TEST(calculate_ability_modifier(13) ==  1, "ability modifier for 13");
    DO_TEST(calculate_ability_modifier(14) ==  2, "ability modifier for 14");
    DO_TEST(calculate_ability_modifier(15) ==  2, "ability modifier for 15");
    DO_TEST(calculate_ability_modifier(16) ==  3, "ability modifier for 16");
    DO_TEST(calculate_ability_modifier(17) ==  3, "ability modifier for 17");
    DO_TEST(calculate_ability_modifier(18) ==  4, "ability modifier for 18");
    DO_TEST(calculate_ability_modifier(19) ==  4, "ability modifier for 19");
    DO_TEST(calculate_ability_modifier(20) ==  5, "ability modifier for 20");

    END_TEST;
}

test damage_test() {
    DEF_TEST("given an attack status, apply the appropriate damage "
            "(miss is 0 points, hit is 1 point, critical is double damage) "
            "and set the character's living status to dead if no hit points "
            "remain");

    character defender;
    attack assault;

    defender.stats.hit_points = 4;
    assault.status = HIT;
    assault.damage = 1;
    apply_attack(&defender, &assault);
    DO_TEST(defender.stats.hit_points == 3, "HP is minus one on HIT");

    defender.stats.hit_points = 7;
    assault.status = CRITICAL;
    assault.damage = 1;
    apply_attack(&defender, &assault);
    DO_TEST(defender.stats.hit_points == 5, "damage was double on CRITICAL");

    defender.stats.hit_points = 1;
    assault.status = MISS;
    assault.damage = 9999;
    apply_attack(&defender, &assault);
    DO_TEST(defender.stats.hit_points == 1, "zero damage on MISS");

    defender.life = LIVING;
    defender.stats.hit_points = 1;
    assault.status = HIT;
    assault.damage = 1;
    apply_attack(&defender, &assault);
    DO_TEST(defender.life == DEAD, "character is dead if HP is 0");

    defender.life = DEAD;
    defender.stats.hit_points = 0;
    assault.status = CRITICAL;
    assault.damage = 9999;
    apply_attack(&defender, &assault);
    DO_TEST(defender.stats.hit_points == 0, "still dead after corpse is hit");

    END_TEST;
}

test attack_test() {
    DEF_TEST("given an attack, attack hits by meeting or beating defending "
            "character's AC with D20 roll");

    attack assault;
    int armor_class = 5;
    int bonus = 0;

    attack_roll(&assault, 20, bonus, armor_class);
    DO_TEST(assault.status == CRITICAL, "CRITICAL when roll is 20");

    attack_roll(&assault, 10, bonus, armor_class);
    DO_TEST(assault.status == HIT, "HIT when roll is above AC");

    attack_roll(&assault, 5, bonus, armor_class);
    DO_TEST(assault.status == HIT, "HIT when roll equals AC");

    armor_class = 25;
    attack_roll(&assault, 20, bonus, armor_class);
    DO_TEST(assault.status == MISS, "MISS when roll is below AC");

    END_TEST;
}

test new_character_ac_hp_test() {
    DEF_TEST("new character has a default AC (10) and HP (5)");

    character bob;
    new_character(&bob, "bobbit");

    DO_TEST(bob.stats.armor_class == 10, "AC is 10");
    DO_TEST(bob.stats.hit_points == 5, "HP is 5");

    END_TEST;
}

test alignment_test() {
    DEF_TEST("given a character, I can get and set the alignment");
    
    character hero;
    hero.alignment = GOOD;

    DO_TEST(hero.alignment == GOOD, "character alignment is set");

    END_TEST;
}

test name_test() {
    DEF_TEST("given a character is created, I can get and set the character's "
            "name");

    character hero;
    new_character(&hero, "dweebus");

    DO_TEST(strcmp(hero.name, "dweebus") == 0, "character name is set");

    END_TEST;
}

int main() {
    RUN_TEST(roll_bonus_application_test);
    RUN_TEST(level_roll_bonus_test);
    RUN_TEST(name_test);
    RUN_TEST(alignment_test);
    RUN_TEST(new_character_ac_hp_test);
    RUN_TEST(attack_test);
    RUN_TEST(damage_test);
    RUN_TEST(ability_modifier_test);
    RUN_TEST(ability_score_test);
    RUN_TEST(xp_level_test);
}
