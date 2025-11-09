# Object-Oriented Programming (OOP) in Pure C

This repository contains a single C program that demonstrates how to implement core Object-Oriented Programming (OOP) concepts in standard C, which does not natively support them.

This project is an academic exercise to show how OOP-like systems (such as GObject or parts of the Linux kernel) can be built from the ground up. It manually implements the "machinery" that a C++ compiler would normally handle automatically.

## 🚀 Core Concepts Demonstrated

This program simulates all major OOP principles using only C's built-in features:

* **Encapsulation:** Achieved by using `struct` to bundle data. True encapsulation (hiding data) can be achieved by forward-declaring `struct` in a public header file and only defining it in the source file, forcing users to interact via public functions.
* **Inheritance:** Simulated using **struct composition**. The "derived" struct (e.g., `Warrior`) contains the "base" struct (e.g., `Entity`) as its *very first member*. This guarantees that a pointer to `Warrior` can be safely cast to a pointer to `Entity`.
* **Polymorphism (Dynamic Dispatch):** Implemented by creating a manual **v-table** (virtual table).
    * The `Entity_vtable` is a `struct` containing function pointers (`attack`, `displayStatus`, `destroy`).
    * Each "base" `Entity` struct contains a pointer to a v-table.
    * "Constructors" (like `Warrior_create`) assign this pointer to the *specific* v-table for that type (e.g., `&WARRIOR_VTABLE`).
    * When we call `entity->vtable->attack(entity)`, it dynamically calls the correct function (`Warrior_attack` or `Mage_attack`) at runtime, based on which v-table the object points to.

## 🔧 How to Compile and Run

This program is a single file and has no external dependencies. You can compile it with any standard C compiler, such as `gcc`.

### 1. Clone the repository (or copy the code)

```bash
git clone https://github.com/DEMiHAT/c-vtable-demo
cd c-vtable-demo
```

### 2. Compile the Program

```bash
# 'gcc' is the compiler
# 'oop_in_c.c' is the source file
# '-o oop_demo' specifies the name of the output executable
# '-std=c99' (Optional) ensures it compiles with the C99 standard
gcc oop_in_c.c -o oop_demo -std=c99
```

### 3. Run the Executable

```bash
./oop_demo
```

### Example Output

You will see a log demonstrating the creation of "objects," the polymorphic calls to `attack()`, and the final "virtual" cleanup process.

```
--- 1. CREATING "OBJECTS" ---
[Entity 'Grom' initialized]
  -> Warrior 'Grom' created!
[Entity 'Jaina' initialized]
  -> Mage 'Jaina' created!

--- 2. DEMONSTRATING POLYMORPHISM ---
Calling attack() for: Grom
Grom swings their axe! (Rage: 50)
Calling attack() for: Jaina
Jaina casts Fireball! (Mana: 100)

--- 3. DEMONSTRATING ENCAPSULATION ---
Calling 'public' method Entity_takeDamage() on Grom.
Grom takes 30 damage.
Displaying updated status:
== Warrior == | Name: Grom | Health: 120 | Rage: 60

--- 4. POLYMORPHIC CLEANUP ---
  -> Warrior 'Grom' is being destroyed.
[Entity 'Grom' cleaned up]
  -> Mage 'Jaina' is being destroyed.
[Entity 'Jaina' cleaned up]
```
