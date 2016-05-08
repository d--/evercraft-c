typedef struct {
    int score;
    int modifier;
} ability;

typedef struct {
    int armor_class;
    int hit_points;
    ability strength;
    ability dexterity;
    ability constitution;
    ability wisdom;
    ability intelligence;
    ability charisma;
} character_statistics;

typedef enum {
    GOOD,
    NEUTRAL,
    EVIL
} alignment_status;

typedef enum {
    LIVING,
    DEAD 
} life_status;

typedef enum {
    HIT,
    MISS,
    CRITICAL
} attack_status;

typedef struct {
    char const *name;
    alignment_status alignment;
    life_status life;
    character_statistics stats;
} character;

typedef struct {
    attack_status status;
    int damage;
} attack;

int calculate_ability_modifier(int score) {
    if (score < 11) {
        return -1 * (11 - score) / 2;
    } else {
        return (score - 10) / 2;
    }
}

void modify_ability(ability *a, int score) {
    a->score = score;
    a->modifier = calculate_ability_modifier(score);
}

void new_character(character *c, char const *name) {
    c->stats.armor_class = 10;
    c->stats.hit_points = 5;
    c->name = name;
    c->life = LIVING;
    modify_ability(&c->stats.strength, 10);
    modify_ability(&c->stats.dexterity, 10);
    modify_ability(&c->stats.constitution, 10);
    modify_ability(&c->stats.wisdom, 10);
    modify_ability(&c->stats.intelligence, 10);
    modify_ability(&c->stats.charisma, 10);
}

void align_character(character *c, alignment_status alignment) {
    c->alignment = alignment;
}

void attack_roll(attack *a, int roll, character const *character) {
    if (roll == 20) {
        a->status = CRITICAL;
    } else if (roll >= character->stats.armor_class) {
        a->status = HIT;
    } else {
        a->status = MISS;
    }
}

void apply_attack(character *c, attack const *attack) {
    if (c->stats.hit_points == 0 || attack->status == MISS) {
        return;
    }
    if (attack->status == HIT) {
        c->stats.hit_points = c->stats.hit_points - attack->damage;
    }
    if (attack->status == CRITICAL) {
        c->stats.hit_points = c->stats.hit_points - (attack->damage * 2);
    }
    if (c->stats.hit_points == 0) {
        c->life = DEAD;
    }
} 

