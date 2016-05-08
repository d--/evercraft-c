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
    char *name;
    alignment_status alignment;
    int ac;
    int hp;
    life_status life;
} character;

typedef struct {
    attack_status status;
    int damage;
} attack;

void new_character(character *c, char *name) {
    c->ac = 10;
    c->hp = 5;
    c->name = name;
    c->life = LIVING;
}

void align_character(character *c, alignment_status alignment) {
    c->alignment = alignment;
}

void attack_roll(attack *a, int roll, character c) {
    if (roll == 20) {
        a->status = CRITICAL;
    } else if (roll >= c.ac) {
        a->status = HIT;
    } else {
        a->status = MISS;
    }
}

void apply_attack(character *c, attack a) {
    if (c->hp == 0 || a.status == MISS) {
        return;
    }
    if (a.status == HIT) {
        c->hp = c->hp - a.damage;
    }
    if (a.status == CRITICAL) {
        c->hp = c->hp - (a.damage * 2);
    }
    if (c->hp == 0) {
        c->life = DEAD;
    }
} 

int calculate_ability_modifier(int score) {
    if (score < 11) {
        return -1 * (11 - score) / 2;
    } else {
        return (score - 10) / 2;
    }
}
