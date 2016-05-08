#include <string.h>
#include "testing.h"
#include "character.c"

test damage_test() {
    DEF_TEST("given an attack status, apply the appropriate damage \n"
            "(miss is 0 points, hit is 1 point, critical is double damage) \n"
            "and set the character's living status to dead if no hit points \n"
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
    DEF_TEST("given an attack, attack hits by meeting or beating defending \n"
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
    new_character(&bob);

    DO_TEST(bob.ac == 10, "AC is 10");
    DO_TEST(bob.hp == 5, "HP is 5");

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
    DEF_TEST("given a character is created, I can get and set the \n"
            "character's name");

    character hero;
    hero.name = "dweebus";

    DO_TEST(strcmp(hero.name, "dweebus") == 0, "character name is set");

    END_TEST;
}

int main() {
    RUN_TEST(name_test);
    RUN_TEST(alignment_test);
    RUN_TEST(new_character_ac_hp_test);
    RUN_TEST(attack_test);
    RUN_TEST(damage_test);
}
