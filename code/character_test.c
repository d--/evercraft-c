#include <string.h>
#include "testing.h"
#include "character.c"

test xp_level_test() {
    DEF_TEST("a new character has 0 xp and level is (1 + floor(xp / 1000))");

    character bob;
    new_character(&bob, "bobby hill");

    DO_TEST(bob.xp == 0, "new character has 0 xp");
    DO_TEST(calculate_level(bob.xp) == 1, "at 0 xp, character level 1");
    
    bob.xp = 41999;
    DO_TEST(calculate_level(bob.xp) == 42, "at 41999 xp, character level 42");

    bob.xp = 42000;
    DO_TEST(calculate_level(bob.xp) == 43, "at 42000 xp, character level 43");

    END_TEST;
}

test ability_score_test() {
    DEF_TEST("given a new character, ability scores default to 10");

    character bob;
    new_character(&bob, "bilbob");
     
    DO_TEST(bob.stats.strength == 10,
            "new character has 10 strength");
    DO_TEST(bob.stats.dexterity == 10,
            "new character has 10 dexterity");
    DO_TEST(bob.stats.constitution == 10,
            "new character has 10 constitution");
    DO_TEST(bob.stats.wisdom == 10,
            "new character has 10 wisdom");
    DO_TEST(bob.stats.intelligence == 10,
            "new character has 10 intelligence");
    DO_TEST(bob.stats.charisma == 10,
            "new character has 10 charisma");

    END_TEST;
}

test ability_modifier_test() {
    DEF_TEST("given an ability score, calculate the associated modifier "
            "according to the data in these tests");

    int (*m)(int);
    m = &calculate_ability_modifier;

    DO_TEST(m(1)  == -5, "1");
    DO_TEST(m(2)  == -4, "2");
    DO_TEST(m(3)  == -4, "3");
    DO_TEST(m(4)  == -3, "4");
    DO_TEST(m(5)  == -3, "5");
    DO_TEST(m(6)  == -2, "6");
    DO_TEST(m(7)  == -2, "7");
    DO_TEST(m(8)  == -1, "8");
    DO_TEST(m(9)  == -1, "9");
    DO_TEST(m(10) ==  0, "10");
    DO_TEST(m(11) ==  0, "11");
    DO_TEST(m(12) ==  1, "12");
    DO_TEST(m(13) ==  1, "13");
    DO_TEST(m(14) ==  2, "14");
    DO_TEST(m(15) ==  2, "15");
    DO_TEST(m(16) ==  3, "16");
    DO_TEST(m(17) ==  3, "17");
    DO_TEST(m(18) ==  4, "18");
    DO_TEST(m(19) ==  4, "19");
    DO_TEST(m(20) ==  5, "20");

    END_TEST;
}

test damage_test() {
    DEF_TEST("given an attack status, apply the appropriate damage "
            "(miss is 0 points, hit is 1 point, critical is double damage) "
            "and set the character's living status to dead if no hit points "
            "remain");

    character defender;
    defender.stats.armor_class = 1;
    defender.stats.hit_points = 4;
    defender.life = LIVING;

    attack assault;
    assault.damage = 1;

    assault.status = HIT;
    apply_attack(&defender, &assault);
    DO_TEST(defender.stats.hit_points == 3, "HP is minus one on HIT");

    assault.status = CRITICAL;
    apply_attack(&defender, &assault);
    DO_TEST(defender.stats.hit_points == 1, "damage was double on CRITICAL");

    assault.status = MISS;
    apply_attack(&defender, &assault);
    DO_TEST(defender.stats.hit_points == 1, "zero damage on MISS");

    assault.status = HIT;
    apply_attack(&defender, &assault);
    DO_TEST(defender.life == DEAD, "character is dead if HP is 0");

    assault.status = CRITICAL;
    apply_attack(&defender, &assault);
    DO_TEST(defender.stats.hit_points == 0, "still dead after corpse is hit");

    END_TEST;
}

test attack_test() {
    DEF_TEST("given an attack, attack hits by meeting or beating defending "
            "character's AC with D20 roll");

    character defender;
    defender.stats.armor_class = 5;
    defender.stats.hit_points = 9999;

    attack assault;

    attack_roll(&assault, 20, &defender);
    DO_TEST(assault.status == CRITICAL, "CRITICAL when roll is 20");

    attack_roll(&assault, 10, &defender);
    DO_TEST(assault.status == HIT, "HIT when roll is above AC");

    attack_roll(&assault, 5, &defender);
    DO_TEST(assault.status == HIT, "HIT when roll equals AC");

    attack_roll(&assault, 1, &defender);
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
    RUN_TEST(xp_level_test);
}
