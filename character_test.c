#include <string.h>
#include "testing.h"
#include "character.c"

test attack_test() {
    DEF_TEST("character can attack by beating AC with D20 roll");

    character defender;
    build_character(&defender);

    attack(20, defender);

    END_TEST;
}

test ac_hp_test() {
    DEF_TEST("build a character with a default AC (10) and HP (5)");

    character bob;
    build_character(&bob);

    DO_TEST(bob.ac == 10, "AC is 10");
    DO_TEST(bob.hp == 5, "HP is 5");

    END_TEST;
}

test alignment_test() {
    DEF_TEST("given a character, i can get and set the alignment");
    
    character hero;
    hero.alignment = GOOD;

    DO_TEST(hero.alignment == GOOD, "hero is good");

    END_TEST;
}

test name_test() {
    DEF_TEST("given a character is created, i can get"
                    " and set the character's name");

    character hero;
    hero.name = "dweebus";

    DO_TEST(strcmp(hero.name, "dweebus") == 0,
                    "name is as it was set");

    END_TEST;
}

int main() {
    RUN_TEST(name_test);
    RUN_TEST(alignment_test);
    RUN_TEST(ac_hp_test);
}
