#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <string>

class Organism {
public:
    virtual void eat() = 0;
    virtual std::string getType() = 0;
};

class Predator : public Organism {
public:
    Predator(std::string type) : type(type) {}
    void eat() {
        std::cout << type << " съел " << target->getType() << std::endl;
    }
    std::string getType() {
        return type;
    }
    void setTarget(Organism* t) {
        target = t;
    }
private:
    std::string type;
    Organism* target;
};

class Herbivore : public Organism {
public:
    Herbivore(std::string type) : type(type) {}
    void eat() {
        std::cout << type << " съело " << target->getType() << std::endl;
    }
    std::string getType() {
        return type;
    }
    void setTarget(Organism* t) {
        target = t;
    }
private:
    std::string type;
    Organism* target;
};

class Plant : public Organism {
public:
    Plant(std::string type) : type(type) {}
    void eat() {
        std::cout << type << " было съедено" << std::endl;
    }
    std::string getType() {
        return type;
    }
private:
    std::string type;
};

std::vector<Organism*> organisms;

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void feed() {
    clearScreen();
    if (organisms.empty()) {
        std::cout << "Все померли" << std::endl;
        return;
    }

    auto predatorIt = organisms.begin();
    std::advance(predatorIt, rand() % organisms.size());
    Predator* predator = dynamic_cast<Predator*>(*predatorIt);

    if (predator) {
        auto targetIt = organisms.begin();
        while (targetIt == predatorIt || dynamic_cast<Plant*>(*targetIt) != nullptr || dynamic_cast<Predator*>(*targetIt) != nullptr) {
            targetIt = organisms.begin();
            std::advance(targetIt, rand() % organisms.size());
        }
        Organism* target = *targetIt;
        predator->setTarget(target);
        predator->eat();
        organisms.erase(targetIt);
    }
    else {
        auto herbivoreIt = organisms.begin();
        std::advance(herbivoreIt, rand() % organisms.size());
        Herbivore* herbivore = dynamic_cast<Herbivore*>(*herbivoreIt);

        if (herbivore) {
            auto targetIt = organisms.begin();
            while (dynamic_cast<Plant*>(*targetIt) == nullptr) {
                targetIt = organisms.begin();
                std::advance(targetIt, rand() % organisms.size());
            }
            Plant* plant = dynamic_cast<Plant*>(*targetIt);
            herbivore->setTarget(plant);
            herbivore->eat();
            organisms.erase(targetIt);
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));

    int numPredators;
    std::cout << "Введите количество хищников: ";
    std::cin >> numPredators;
    std::cin.ignore(); // Очищаем символ новой строки из входного буфера
    for (int i = 0; i < numPredators; ++i) {
        std::string type;
        std::cout << "Введите тип хищника " << i + 1 << ": ";
        std::getline(std::cin, type);
        organisms.push_back(new Predator(type));
    }

    int numHerbivores;
    std::cout << "Введите количество травоядных: ";
    std::cin >> numHerbivores;
    std::cin.ignore(); // Очищаем символ новой строки из входного буфера
    for (int i = 0; i < numHerbivores; ++i) {
        std::string type;
        std::cout << "Введите тип травоядного " << i + 1 << ": ";
        std::getline(std::cin, type);
        organisms.push_back(new Herbivore(type));
    }

    int numPlants;
    std::cout << "Введите количество растений: ";
    std::cin >> numPlants;
    std::cin.ignore(); // Очищаем символ новой строки из входного буфера
    for (int i = 0; i < numPlants; ++i) {
        std::string type;
        std::cout << "Введите тип растения " << i + 1 << ": ";
        std::getline(std::cin, type);
        organisms.push_back(new Plant(type));
    }

    while (!organisms.empty()) {
        std::cout << "Нажмите 1 для продолжения или 2 для кормления: ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "1") {
            continue;
        }
        else if (input == "2") {
            feed();
        }
        else {
            std::cout << "Неверный выбор!" << std::endl;
        }
    }

    std::cout << "Описание процесса:" << std::endl;
    for (Organism* organism : organisms) {
        std::cout << organism->getType() << std::endl;
    }

    // Очистка памяти
    for (Organism* organism : organisms) {
        delete organism;
    }

    return 0;
}
