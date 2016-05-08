typedef enum {
    GOOD,
    NEUTRAL,
    EVIL
} alignment;

typedef struct {
    char *name;
    alignment alignment;
    int ac;
    int hp;
} character;

void build_character(character *c) {
    c->ac = 10;
    c->hp = 5;
}
