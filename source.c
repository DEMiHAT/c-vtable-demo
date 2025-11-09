#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Entity;

typedef struct {
    void (*attack)(struct Entity* self);
    void (*displayStatus)(struct Entity* self);
    void (*destroy)(struct Entity* self);
} Entity_vtable;

typedef struct Entity {
    const Entity_vtable* vtable;
    char name[50];
    int health;
} Entity;

void Entity_takeDamage(Entity* self, int damage) {
    if (self == NULL) return;
    
    self->health -= damage;
    printf("%s takes %d damage.\n", self->name, damage);
    
    if (self->health < 0) {
        self->health = 0;
    }
    
    if (self->health == 0) {
        printf("%s has fallen!\n", self->name);
    }
}

void Entity_default_attack(Entity* self) {
    printf("%s does a generic attack.\n", self->name);
}

void Entity_default_displayStatus(Entity* self) {
    printf("== Entity == | Name: %s | Health: %d\n", self->name, self->health);
}

void Entity_init(Entity* self, const char* name, int health) {
    self->vtable = NULL; 
    strncpy(self->name, name, 49);
    self->name[49] = '\0';
    self->health = health;
    printf("[Entity '%s' initialized]\n", self->name);
}

void Entity_cleanup(Entity* self) {
    printf("[Entity '%s' cleaned up]\n", self->name);
}

typedef struct {
    Entity base;
    int rage;
} Warrior;

void Warrior_attack(struct Entity* self) {
    Warrior* w = (Warrior*)self;
    printf("%s swings their axe! (Rage: %d)\n", w->base.name, w->rage);
    w->rage += 10;
}

void Warrior_displayStatus(struct Entity* self) {
    Warrior* w = (Warrior*)self;
    printf("== Warrior == | Name: %s | Health: %d | Rage: %d\n",
        w->base.name, w->base.health, w->rage);
}

void Warrior_destroy(struct Entity* self) {
    Warrior* w = (Warrior*)self;
    printf("  -> Warrior '%s' is being destroyed.\n", w->base.name);
    Entity_cleanup(self);
    free(w);
}

const Entity_vtable WARRIOR_VTABLE = {
    .attack = Warrior_attack,
    .displayStatus = Warrior_displayStatus,
    .destroy = Warrior_destroy
};

Warrior* Warrior_create(const char* name, int health, int rage) {
    Warrior* w = (Warrior*)malloc(sizeof(Warrior));
    if (w == NULL) return NULL;
    
    Entity_init(&w->base, name, health);
    w->base.vtable = &WARRIOR_VTABLE;
    w->rage = rage;
    
    printf("  -> Warrior '%s' created!\n", name);
    return w;
}

typedef struct {
    Entity base;
    int mana;
} Mage;

void Mage_attack(struct Entity* self) {
    Mage* m = (Mage*)self;
    if (m->mana >= 20) {
        printf("%s casts Fireball! (Mana: %d)\n", m->base.name, m->mana);
        m->mana -= 20;
    } else {
        printf("%s tries to cast, but is out of mana!\n", m->base.name);
    }
}

void Mage_displayStatus(struct Entity* self) {
    Mage* m = (Mage*)self;
    printf("== Mage ==    | Name: %s | Health: %d | Mana: %d\n",
        m->base.name, m->base.health, m->mana);
}

void Mage_destroy(struct E_ntity* self) {
    Mage* m = (Mage*)self;
    printf("  -> Mage '%s' is being destroyed.\n", m->base.name);
    Entity_cleanup(self);
    free(m);
}

const Entity_vtable MAGE_VTABLE = {
    .attack = Mage_attack,
    .displayStatus = Mage_displayStatus,
    .destroy = Mage_destroy
};

Mage* Mage_create(const char* name, int health, int mana) {
    Mage* m = (Mage*)malloc(sizeof(Mage));
    if (m == NULL) return NULL;
    
    Entity_init(&m->base, name, health);
    m->base.vtable = &MAGE_VTABLE;
    m->mana = mana;
    
    printf("  -> Mage '%s' created!\n", name);
    return m;
}

int main() {
    printf("--- 1. CREATING \"OBJECTS\" ---\n");
    Warrior* grom = Warrior_create("Grom", 150, 50);
    Mage* jaina = Mage_create("Jaina", 100, 100);

    printf("\n--- 2. DEMONSTRATING POLYMORPHISM ---\n");
    
    Entity* entity_list[2];
    entity_list[0] = (Entity*)grom;
    entity_list[1] = (Entity*)jaina;

    for (int i = 0; i < 2; i++) {
        Entity* e = entity_list[i];
        printf("Calling attack() for: %s\n", e->name);
        e->vtable->attack(e);
    }

    printf("\n--- 3. DEMONSTRATING ENCAPSULATION ---\n");
    printf("Calling 'public' method Entity_takeDamage() on Grom.\n");
    
    Entity_takeDamage((Entity*)grom, 30);

    printf("Displaying updated status:\n");
    grom->base.vtable->displayStatus((Entity*)grom);
    
    printf("\n--- 4. POLYMORPHIC CLEANUP ---\n");
    for (int i = 0; i < 2; i++) {
        entity_list[i]->vtable->destroy(entity_list[i]);
    }
    
    return 0;
}
