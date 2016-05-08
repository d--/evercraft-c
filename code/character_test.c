#include <string.h>
#include "testing.h"
#include "character.c"

test ability_score_test() {
    DEF_TEST("given a character, that character has ability scores that are "
            "able to be set, range from 1 to 20, default to 10, and have "
            "their modifiers set according to ability_modifier_test");

    character bob;
    new_character(&bob, "bilbob");
     
//    DO_TEST(bob.strength.score == 10,
//            "new character has default 10 strength");
//
//    modify_strength(&bob, 15); 
//    DO_TEST(bob.strength.score == 15, "bob's new strength is now 15");
//    DO_TEST(bob.strength.modifier == 2, "bob's strength modifier is now 2");

    END_TEST;
}

test ability_modifier_test() {
    DEF_TEST("given an ability score, calculate the associated modifier "
            "according to the data in these tests");

    DO_TEST(calculate_ability_modifier(1)  == -5, "1:  -5");
    DO_TEST(calculate_ability_modifier(2)  == -4, "2:  -4");
    DO_TEST(calculate_ability_modifier(3)  == -4, "3:  -4");
    DO_TEST(calculate_ability_modifier(4)  == -3, "4:  -3");
    DO_TEST(calculate_ability_modifier(5)  == -3, "5:  -3");
    DO_TEST(calculate_ability_modifier(6)  == -2, "6:  -2");
    DO_TEST(calculate_ability_modifier(7)  == -2, "7:  -2");
    DO_TEST(calculate_ability_modifier(8)  == -1, "8:  -1");
    DO_TEST(calculate_ability_modifier(9)  == -1, "9:  -1");
    DO_TEST(calculate_ability_modifier(10) ==  0, "10:  0");
    DO_TEST(calculate_ability_modifier(11) ==  0, "11:  0");
    DO_TEST(calculate_ability_modifier(12) ==  1, "12:  1");
    DO_TEST(calculate_ability_modifier(13) ==  1, "13:  1");
    DO_TEST(calculate_ability_modifier(14) ==  2, "14:  2");
    DO_TEST(calculate_ability_modifier(15) ==  2, "15:  2");
    DO_TEST(calculate_ability_modifier(16) ==  3, "16:  3");
    DO_TEST(calculate_ability_modifier(17) ==  3, "17:  3");
    DO_TEST(calculate_ability_modifier(18) ==  4, "18:  4");
    DO_TEST(calculate_ability_modifier(19) ==  4, "19:  4");
    DO_TEST(calculate_ability_modifier(20) ==  5, "20:  5");

    END_TEST;
}

test damage_test() {
    DEF_TEST("given an attack status, apply the appropriate damage "
            "(miss is 0 points, hit is 1 point, critical is double damage) "
            "and set the character's living status to dead if no hit points "
            "remain");

    character defender;
    defender.ac = 1;
    defender.hp = 4;
    defender.life = LIVING;

    attack assault;
    assault.damage = 1;

    assault.status = HIT;
    apply_attack(&defender, assault);
    DO_TEST(defender.hp == 3, "HP is minus one on HIT");

    assault.status = CRITICAL;
    apply_attack(&defender, assault);
    DO_TEST(defender.hp == 1, "damage was double on CRITICAL hit");

    assault.status = MISS;
    apply_attack(&defender, assault);
    DO_TEST(defender.hp == 1, "zero damage on MISS");

    assault.status = HIT;
    apply_attack(&defender, assault);
    DO_TEST(defender.life == DEAD, "character is dead if HP is 0");

    assault.status = CRITICAL;
    apply_attack(&defender, assault);
    DO_TEST(defender.hp == 0, "still just dead after attacking corpse");

    END_TEST;
}

test attack_test() {
    DEF_TEST("given an attack, attack hits by meeting or beating defending "
            "character's AC with D20 roll");

    character defender;
    defender.ac = 5;
    defender.hp = 9999;

    attack assault;

    attack_roll(&assault, 20, defender);
    DO_TEST(assault.status == CRITICAL, "CRITICAL when roll is 20");

    attack_roll(&assault, 10, defender);
    DO_TEST(assault.status == HIT, "HIT when roll is above AC");

    attack_roll(&assault, 5, defender);
    DO_TEST(assault.status == HIT, "HIT when roll equals AC");

    attack_roll(&assault, 1, defender);
    DO_TEST(assault.status == MISS, "MISS when roll is below AC");

    END_TEST;
}

test new_character_ac_hp_test() {
    DEF_TEST("new character has a default AC (10) and HP (5)");

    character bob;
    new_character(&bob, "bobbit");

    DO_TEST(bob.ac == 10, "AC is 10");
    DO_TEST(bob.hp == 5, "HP is 5");

    END_TEST;
}

test alignment_test() {
    DEF_TEST("given a character, I can get and set the alignment");
    
    character hero;
    align_character(&hero, GOOD);

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
    RUN_TEST(name_test);
    RUN_TEST(alignment_test);
    RUN_TEST(new_character_ac_hp_test);
    RUN_TEST(attack_test);
    RUN_TEST(damage_test);
    RUN_TEST(ability_modifier_test);
    RUN_TEST(ability_score_test);
}
