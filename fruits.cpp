/* Add ALL of your code to this file. */

/* The declarations of the class template ’set’ are below. Do NOT uncomment them.

template <typename T> class set {
  public:
    set(); // constructor that creates an empty set
    void insert(const T& item); // adds item to the set
    set<T>::constant_iterator begin(); // returns constant iterator
    set<T>::constant_iterator end(); // returns constant iterator
    void erase(set<T>::const_iterator); // removes specified item
    bool empty(); // returns true if the set is empty
};

*/

/* Do NOT add any other header files. */

#include<iostream>
#include<string>
#include<set>

/* Add your code below this line. */

// Question b
class Fruit {
protected:    
    int weight = 0; // in grams
    int bestBeforeDate = 0;
    std::string variety;
    bool premium = false;
public:
    Fruit() {};
    Fruit(int weight, int bestBeforeDate, std::string variety)
    : weight(weight), bestBeforeDate(bestBeforeDate), variety(variety) {};

    std::string getVariety() { return variety; }

    void print() {
        std::cout << variety << " " << weight << " " << bestBeforeDate << std::endl;
    }

    std::string getName() { return variety; }
    int getBestBefore() { return bestBeforeDate;}
    int getPremium() { return premium; }
};

class Apple : public Fruit {
private:
    int percRed;

public:
    Apple() {};
    Apple(int weight, int bestBeforeDate, std::string variety, int percRed) 
    : Fruit(weight, bestBeforeDate, variety), percRed(percRed) {
        if (percRed > 0.75 && weight > 90) {
            premium = true;
        }
        else if (percRed > 0.5 && weight > 90) {
            premium = true;
        }
    };
};

class Pear : public Fruit {
private:
    bool stalk = false; // false means not intact

public:
    Pear(int weight, int bestBeforeDate, std::string variety, bool stalk)
        : Fruit(weight, bestBeforeDate, variety), stalk(stalk) {
        if (stalk && weight > 130) premium = true;
    }
};

template<class T, const int maxNumber>
class Basket {
private:
    static const int maxNumber;
    T* itemsArray[maxNumber]; //set<T>* itemsArray;
    int itemsCount = 0;
public:
    Basket() {}

    void addItem(const T* t) {
        if (itemsCount == maxNumber) return;
        itemsArray[itemsCount] = t;
        itemsCount++;
    }

    void removeItem(const T* t) { // How to use set to erase?
        if (itemsCount == 0) return;
        for (int i = 0; i < itemsCount; i++) {
            if (*itemsCount[i].getName() == *t.getName()) {
                itemsArray[i] == itemsArray[itemsCount]; // swap from last position in array
                itemsCount--;
                return;
            }
        }
    }

    T* getItem(const int i) {
        if (i < 0 || i > itemsCount) return nullptr;
        return itemsArray[i];
    }

    void printBasket() {
        for (int i = 0; i < itemsCount; i++) {
            itemsArray[i]->print(); 
        }
    }

    int getItemsCount() { return itemsCount; }

    bool checkItem(const int i) {
        if (i < 0 || i > itemsCount) return false;
        // check item
        if (itemsArray[i]->getBestBefore() >= 3 && itemsArray[i]->getPremium()) return true;
        return false;
    }

    ~Basket() {
        delete[] itemsArray;
    }
};

// Question c

int main() {
    // part 1
    Basket<Fruit, 30>* bigFruitBasket = new Basket<Fruit, 30>;
    Apple* redApple = new Apple(95, 7, "Gala", 80);
    Apple* mixedApple = new Apple(80, 8, "Cox", 60);
    Pear* pear = new Pear(140, 2, "Ambrosia", true);

    bigFruitBasket->addItem(redApple);
    bigFruitBasket->addItem(mixedApple);
    bigFruitBasket->addItem(pear);
    bigFruitBasket->printBasket();

    // part 2
    Basket<Fruit, 10>* smallFruitBasket = new Basket<Fruit, 10>;
    for (int i = 1; i < bigFruitBasket->getItemsCount(); i++) {
        Fruit* target = bigFruitBasket->getItem(i);
        if (bigFruitBasket->checkItem(i)) {
            smallFruitBasket->addItem(target);
        }
        bigFruitBasket->removeItem(target);
    }

    bigFruitBasket->printBasket();
    smallFruitBasket->printBasket();

}