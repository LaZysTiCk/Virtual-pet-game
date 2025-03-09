#include <iostream>
#include <fstream>


using namespace std;

class Pet {
protected:
    string name, fav_food, allergic_food;
    int hunger, energy, happiness, points;

public:
    Pet(string n = "", string ff = "", string af = "")
        : name(n), fav_food(ff), allergic_food(af), hunger(100), energy(100), happiness(100), points(0) {}

    void show() {
        cout << "\nPet Name: " << name;
        cout << "\nFavorite Food: " << fav_food;
        cout << "\nAllergic To: " << allergic_food;
        cout << "\nHunger: " << hunger;
        cout << "\nEnergy: " << energy;
        cout << "\nHappiness: " << happiness;
        cout << "\nPoints: " << points;
    }

    void play() {
        if (energy >= 10) {
            cout << "\nYou played with " << name << "! Happiness increased!";
            happiness += 10;
            energy -= 10;
            points += 5;
        } else {
            cout << "\n" << name << " is too tired to play.";
        }
    }

    void feed(string food) {
        if (food == allergic_food) {
            cout << "\nOh no! " << name << " is allergic to " << food << "! Lost happiness.";
            happiness -= 20;
        } else if (food == fav_food) {
            cout << "\n" << name << " loves " << food << "! Gained energy and happiness.";
            happiness += 15;
            energy += 10;
            points += 10;
        } else {
            cout << "\n" << name << " ate " << food << ".";
            hunger -= 10;
            energy += 5;
        }
    }

    int getPoints() { return points; }
    void setPoints(int p) { points = p; }
    virtual string getType() { return "Pet"; }
    virtual void saveProgress(ofstream &file) { file << getType() << " " << name << " " << points << endl; }
};

class Dog : public Pet {
public:
    Dog(string n = "") : Pet(n, "Bone", "Chocolate") {}
    string getType() { return "Dog"; }
};

class Cat : public Pet {
public:
    Cat(string n = "") : Pet(n, "Fish", "Milk") {}
    string getType() { return "Cat"; }
};

class Rabbit : public Pet {
public:
    Rabbit(string n = "") : Pet(n, "Carrot", "Tomato") {}
    string getType() { return "Rabbit"; }
};

class Dragon : public Pet {
public:
    Dragon(string n = "") : Pet(n, "Gems", "Silver") {}
    string getType() { return "Dragon"; }
};

void saveGame(Pet &pet) {
    ofstream file("savegame.txt");
    if (file.is_open()) {
        pet.saveProgress(file);
        file.close();
        cout << "\nGame progress saved!";
    } else {
        cout << "\nError saving game.";
    }
}

Pet* loadGame() {
    ifstream file("savegame.txt");
    if (file.is_open()) {
        string type, name;
        int points;
        file >> type >> name >> points;
        file.close();

        Pet* loadedPet = NULL;
        if (type == "Dog") loadedPet = new Dog(name);
        else if (type == "Cat") loadedPet = new Cat(name);
        else if (type == "Rabbit") loadedPet = new Rabbit(name);
        else if (type == "Dragon") loadedPet = new Dragon(name);

        if (loadedPet) loadedPet->setPoints(points);
        return loadedPet;
    }
    return NULL;
}

int main() {
    Pet* myPet = loadGame();
    if (!myPet) {
        int choice;
        cout << "\nChoose a pet: 1. Dog  2. Cat  3. Rabbit";
        cin >> choice;
        string name;
        cout << "Enter your pet's name: ";
        cin >> name;

        switch (choice) {
            case 1: myPet = new Dog(name); break;
            case 2: myPet = new Cat(name); break;
            case 3: myPet = new Rabbit(name); break;
            default: cout << "\nInvalid choice."; return 0;
        }
    }

    int action;
    do {
        cout << "\n\nChoose an action: 1. Show Stats  2. Play  3. Feed  4. Save & Exit";
        cin >> action;
        switch (action) {
            case 1: myPet->show(); break;
            case 2: myPet->play(); break;
            case 3: {
                string food;
                cout << "Enter food: ";
                cin >> food;
                myPet->feed(food);
                break;
            }
            case 4: saveGame(*myPet); break;
            default: cout << "\nInvalid action.";
        }

        if (myPet->getPoints() >= 50 && myPet->getType() != "Dragon") {
            cout << "\nCongratulations! You've unlocked the Dragon!";
            string dragonName;
            cout << "\nEnter a new name for your Dragon: ";
            cin >> dragonName;
            delete myPet;
            myPet = new Dragon(dragonName);
        }
    } while (action != 4);

    delete myPet;
    return 0;
}

