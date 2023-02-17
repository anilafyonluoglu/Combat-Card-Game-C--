#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <time.h>
#include <chrono>

class Mana{
protected:
    std::string manaColor;
    friend class Player;
    friend class Card;
    bool isFound;
    bool isUsed;
public:
    Mana()= default;
    Mana(std::string col):manaColor(std::move(col)){}

    bool operator==(const Mana &mn){
        if (this->manaColor == mn.manaColor){
            return true;
        } else{
            return false;
        }
    }

    std::string returnColor(){
        return manaColor;
    }


};

static bool operator<(const std::shared_ptr<Mana> &mn1, const std::shared_ptr<Mana> &mn2) {
    if (mn1->returnColor() < mn2->returnColor()){
        return true;
        } else{
        return false;
        }

}

class redMana: public Mana{
public:
    redMana():Mana("Red"){}
};
class greenMana: public Mana{
public:
    greenMana():Mana("Green"){}
};
class blueMana: public Mana{
public:
    blueMana():Mana("Blue"){}
};
class whiteMana: public Mana{
public:
    whiteMana():Mana("White"){}
};
class blackMana: public Mana{
public:
    blackMana():Mana("Black"){}
};

class Player;

class Card{
protected:
    std::string Name;
    std::string Type;
    std::string Color;
    bool isDestroyed;
    friend class Player;
    std::vector<std::shared_ptr<Mana>> manaCost;
    int colorlessManaCount;

public:
    Card()= default;
    Card(std::string na):Name(std::move(na)),isDestroyed(false){}

    virtual void play(std::shared_ptr<Player> &pl, std::shared_ptr<Player> &otherPl) = 0;

    std::string getName(){
        return Name;
    }
    bool allFound(){
        for (int i = 0; i<manaCost.size();i++) {
            if (manaCost[i]->isFound = false){
                return false;
            }
        }
        return true;
    }

    std::string getColor(){
        return this->Color;
    }

    std::string getType(){
        return this ->Type;
    }
};

class landCard: public Card,public std::enable_shared_from_this<landCard>{
protected:
    bool isTapped;
    std::shared_ptr<Mana> givenMana;
    friend class destroyTargetLand;
    friend class Player;
public:

    landCard():Card(){
        Type = "Land";
        isTapped = false;
    }

    landCard(std::string na):Card(std::move(na)){
        Type = "Land";
        isTapped = false;
        colorlessManaCount = 0;
    }

    void play(std::shared_ptr<Player> &pl,std::shared_ptr<Player> &otherPl) override;

    std::shared_ptr<Mana> tap(){
        isTapped = true;
        return givenMana;
    }

    std::shared_ptr<landCard> get_ptr(){
        return shared_from_this();
    }
};

class Forest:   public landCard{
public:
    Forest():landCard("Forest"){
        givenMana = std::make_shared<greenMana>();
        Color = "Green";
    }};
class Island:   public landCard{
public:
    Island():landCard("Island"){
        givenMana = std::make_shared<blueMana>();
        Color = "Blue";
    }};
class Mountain: public landCard{
public:
    Mountain():landCard("Mountain"){
        givenMana = std::make_shared<redMana>();
        Color = "Red";
    }
};
class Plains:   public landCard{
public:
    Plains():landCard("Plains"){
        givenMana = std::make_shared<whiteMana>();
        Color = "White";
    }};
class Swamp:    public landCard{
public:
    Swamp():landCard("Swamp"){
        givenMana = std::make_shared<blackMana>();
        Color = "Black";
    }
};

class creatureCard: public Card, public std::enable_shared_from_this<creatureCard>{
protected:
    int maxHP;
    int currentHP;
    int attackPower;
    bool hasTrample;
    bool hasFirstStrike;
    bool isTapped;
    friend class Player;
    friend class destroyTargetCreat;
    friend class dealDamage;
    friend class effectPowersAll;
    friend class loseTrample;
    friend class gainTrample;
    friend class gainHasFirstStrike;
    friend class loseHasFirstStrike;
    friend void dealDmgCr(std::shared_ptr<creatureCard> &crToBeHarmed, int attPow);
public:

    creatureCard(std::string na):Card(na){
        Type = "Creature";
        isTapped = false;
    }
    std::shared_ptr<creatureCard> get_ptr(){
        return shared_from_this();
    }
    void tap(){
        isTapped = true;
    }
    bool isItTapped(){
        return isTapped;
    }

    void play(std::shared_ptr<Player> &pl,std::shared_ptr<Player> &otherPl) override;

    void attack(std::shared_ptr<Player> &plToBeHarmed,std::shared_ptr<creatureCard> &crToBeHarmed );
};
class soldier       : public creatureCard{
public:
    soldier():creatureCard("Soldier"){
        manaCost = {std::make_shared<whiteMana>()};
        colorlessManaCount = 0;
        Color = "White";
        attackPower = 1;
        maxHP = 1;
        hasFirstStrike = false;
        hasTrample = false;
    }
};
class armoredPegasus: public creatureCard{
public:
    armoredPegasus():creatureCard("Armored Pegasus"){
        manaCost = {std::make_shared<whiteMana>()};
        colorlessManaCount = 1;
        Color = "White";
        attackPower = 1;
        maxHP = 2;
        hasFirstStrike = false;
        hasTrample = false;
    }};
class whiteKnight   : public creatureCard{
public:
    whiteKnight():creatureCard("White Knight"){
        manaCost = {std::make_shared<whiteMana>(),std::make_shared<whiteMana>()};
        colorlessManaCount = 0;
        Color = "White";
        attackPower = 2;
        maxHP = 2;
        hasFirstStrike = true;
        hasTrample = false;
    }};
class angryBear     : public creatureCard{
public:
    angryBear():creatureCard("Angry Bear"){
        manaCost = {std::make_shared<greenMana>()};
        colorlessManaCount = 2;
        Color = "Green";
        attackPower = 3;
        maxHP = 2;
        hasFirstStrike = false;
        hasTrample = true;
    }
};
class guard         : public creatureCard{
public:
    guard():creatureCard("Guard"){
        manaCost = {std::make_shared<whiteMana>(),std::make_shared<whiteMana>()};
        colorlessManaCount = 2;
        Color = "White";
        attackPower = 2;
        maxHP = 5;
        hasFirstStrike = false;
        hasTrample = false;
    }
};
class werewolf      : public creatureCard{
public:
    werewolf():creatureCard("Werewolf"){
        manaCost = {std::make_shared<greenMana>(),std::make_shared<whiteMana>()};
        colorlessManaCount = 2;
        Color = "Green";
        attackPower = 4;
        maxHP = 6;
        hasFirstStrike = false;
        hasTrample = true;
    }
};
class skeleton      : public creatureCard{
public:
    skeleton():creatureCard("Skeleton"){
        manaCost = {std::make_shared<blackMana>()};
        colorlessManaCount = 0;
        Color = "Black";
        attackPower = 1;
        maxHP = 1;
        hasFirstStrike = false;
        hasTrample = false;
    }
};
class ghost         : public creatureCard{
public:
    ghost():creatureCard("Ghost"){
        manaCost = {std::make_shared<blackMana>()};
        colorlessManaCount = 1;
        Color = "Black";
        attackPower = 2;
        maxHP = 1;
        hasFirstStrike = false;
        hasTrample = false;
    }
};
class blackKnight   : public creatureCard{
public:
    blackKnight():creatureCard("Black Knight"){
        manaCost = {std::make_shared<blackMana>(),std::make_shared<blackMana>()};
        colorlessManaCount = 0;
        Color = "Black";
        attackPower = 2;
        maxHP = 2;
        hasFirstStrike = true;
        hasTrample = false;
    }
};
class orcManiac     : public creatureCard{
public:
    orcManiac():creatureCard("Orc Maniac"){
        manaCost = {std::make_shared<redMana>()};
        colorlessManaCount = 2;
        Color = "Red";
        attackPower = 4;
        maxHP = 1;
        hasFirstStrike = false;
        hasTrample = false;
    }
};
class hobgoblin     : public creatureCard{
public:
    hobgoblin():creatureCard("Hobgoblin"){
        manaCost = {std::make_shared<blackMana>(),std::make_shared<redMana>()};
        colorlessManaCount = 1;
        Color = "Red";
        attackPower = 3;
        maxHP = 3;
        hasFirstStrike = false;
        hasTrample = false;
    }
};
class vampire       : public creatureCard{
public:
    vampire():creatureCard("Vampire"){
        manaCost = {std::make_shared<blackMana>()};
        colorlessManaCount = 3;
        Color = "Black";
        attackPower = 6;
        maxHP = 3;
        hasFirstStrike = false;
        hasTrample = false;
    }
};
class Effect{
public:
    Effect()=default;
    virtual void theEffect(std::shared_ptr<Player> &thePl)=0;
};

class enchantmentCard:public Card, public std::enable_shared_from_this<enchantmentCard>{
protected:
    std::shared_ptr<Effect> effect;
    friend class destroyTargetEnch;
public:
    enchantmentCard(std::string na):Card(na){
        Type = "Enchantment";
    }
    void play(std::shared_ptr<Player> &pl,std::shared_ptr<Player> &otherPl) override;

    std::shared_ptr<enchantmentCard> get_ptr(){
        return shared_from_this();
    }

};
class sorceryCard    :public Card, public std::enable_shared_from_this<sorceryCard>{
protected:
    std::unique_ptr<Effect> effect;
public:
    sorceryCard(std::string na):Card(na){}

    std::shared_ptr<sorceryCard> get_ptr(){
        return shared_from_this();
    }

    void play(std::shared_ptr<Player> &pl,std::shared_ptr<Player> &otherPl) override;

};

class Player{
protected:
    int HP;
    bool hasPlayedLandCard;
    friend class dealDamage;
    friend class returnCharacter;
    friend class destroyTargetLand;
    friend class effectPowersAll;
    friend class gainTrample;
    friend class loseTrample;
    friend class destroyTargetEnch;
    friend class destroyTargetCreat;
    friend class loseHasFirstStrike;
    friend class creatureCard;
    friend class landCard;
    friend class enchantmentCard;
    friend class sorceryCard;
    friend void dealDmgPlyr(std::shared_ptr<Player> &plToBeHarmed, int attPow);

    std::vector<std::shared_ptr<Card>> deck;
    std::vector<std::shared_ptr<Card>> hand;
    std::vector<std::shared_ptr<Card>> discard;
    std::vector<std::shared_ptr<creatureCard>> inPlayCreature;
    std::vector<std::shared_ptr<landCard>> inPlayLand;
    std::vector<std::shared_ptr<enchantmentCard>> inPlayEnch;


    std::vector<std::shared_ptr<Mana>> wallet;

public:
    Player(std::vector<std::shared_ptr<Card>> &cards):HP(15),hasPlayedLandCard(false){
        for (auto & card : cards) {
            deck.emplace_back(card);
        }
    }

    Player(const Player &pl){
        HP=pl.HP;
        hasPlayedLandCard= pl.hasPlayedLandCard;


        for (auto & i : pl.wallet) {
            wallet.emplace_back(i);
        }
        for (auto & i : pl.deck) {
        deck.emplace_back(i);
        }
        for (auto & i : pl.hand) {
            hand.emplace_back(i);
        }
        for (auto & i : pl.inPlayCreature) {
            inPlayCreature.emplace_back(i);
        }
        for (auto & i : pl.inPlayLand) {
            inPlayLand.emplace_back(i);
        }
        for (auto & i : pl.inPlayEnch) {
            inPlayEnch.emplace_back(i);
        }
        for (auto & i : pl.discard) {
            discard.emplace_back(i);
        }
    }

    int getHP(){
        return this->HP;
    }

    bool playedLandCard(){
        return hasPlayedLandCard;
    }

    std::vector<std::shared_ptr<Card>> getHand(){
        return hand;
    }

    std::vector<std::shared_ptr<creatureCard>>getCreatureCards(){
        return this->inPlayCreature;
    }

    void showCreatureCards(){
        if (!inPlayCreature.empty()){
            for (int i = 0; i < inPlayCreature.size(); ++i) {
                std::cout << inPlayCreature[i]->getName() << ", ";
            }
        } else{
            std::cout << "No creature card to show..." << std::endl;
        }
    }


    bool payManaFunc(std::shared_ptr<Card> &card){
        std::string input;
        int indx;
        std::vector<int> indexes;

        if (wallet.empty()){
            return false;

        } else if ((wallet.size() - (card->manaCost.size() + card->colorlessManaCount)) < 0) {
            return false;

        } else {
            for (int i = 0; i < card->manaCost.size(); ++i) {
                for (int j = 0; j < wallet.size(); ++j) {
                    if (card->manaCost[i] == wallet[j]){
                        if (!(wallet[j]->isUsed)){
                            card->manaCost[i]->isFound = true;
                            wallet[j]->isUsed = true;
                            indexes.push_back(j);
                        } else{
                            continue;
                        }
                    }
                }
            }

            if (card->allFound()){
                for (int i = 0; i < indexes.size(); ++i) {
                    wallet.erase(wallet.begin()+indexes[i]);
                }

                for (int i = 0; i < card->colorlessManaCount; ++i) {
                    std::srand((unsigned )time(nullptr));
                    int indColorless = int(std::rand() % (wallet.size() + 1));
                    wallet.erase(wallet.begin()+indColorless);
                }

                return true;
            } else{
                return false;

            }
        }
    }

    void resetForEndPhase(){

        wallet.clear();
        hasPlayedLandCard = false;
    };

    void tapAndGainMana(){
        for (auto  & it : inPlayLand ) {
            wallet.push_back(it->tap());
        }
        for (auto &it : wallet) {
            std::cout << it->returnColor() << ", ";
        }
    }

    void unTap(){
        for (auto & it1 : inPlayCreature) {
            it1->isTapped = false;
        }
        for (auto & it2 : inPlayLand) {
            it2->isTapped = false;
        }
    }

    void Draw(){
        if (hand.size() < 7){
            if (!deck.empty()){
                hand.push_back(deck.back());
                deck.erase(deck.end()-1);
            } else{
                std::cout << "You do not have any cards..." << std::endl;
            }


        } else{
            std::cout<< "Your hand is full, can not draw a card..." << std::endl;
        }
    }

    void resetForPhase(){

        for (int i = 0; i < inPlayLand.size(); ++i) {
            if (inPlayLand[i]->isDestroyed){
                discard.emplace_back(inPlayLand[i]);
                inPlayLand.erase(inPlayLand.begin()+i);
            }
        }

        for (int j = 0; j < inPlayCreature.size(); ++j) {
            if (inPlayCreature[j]->isDestroyed){
                discard.emplace_back(inPlayCreature[j]);
                inPlayCreature.erase(inPlayCreature.begin()+j);
            } else{
                inPlayCreature[j]->currentHP = inPlayCreature[j]->maxHP;
            }
        }

        for (int k = 0; k < inPlayEnch.size(); ++k) {
            if (inPlayEnch[k]->isDestroyed){
                discard.emplace_back(inPlayEnch[k]);
                inPlayEnch.erase(inPlayEnch.begin()+k);
            }
        }




    }

};

void landCard       ::play(std::shared_ptr<Player> &pl,std::shared_ptr<Player> &otherPl){
    for (int i = 0; i < pl->hand.size(); ++i) {
        if (pl->hand[i] == get_ptr()){
            pl->inPlayLand.emplace_back(get_ptr());
            pl->hand.erase(pl->hand.begin()+i);
            pl->hasPlayedLandCard= true;
        }
    }
}
void creatureCard   ::play(std::shared_ptr<Player> &pl,std::shared_ptr<Player> &otherPl) {
    for (int i = 0; i < pl->hand.size(); ++i) {
        if (pl->hand[i] == get_ptr()){
            pl->inPlayCreature.emplace_back(get_ptr());
            pl->hand.erase(pl->hand.begin()+i);

        }
    }

}
void enchantmentCard::play(std::shared_ptr<Player> &pl,std::shared_ptr<Player> &otherPl){
    for (int i = 0; i < pl->hand.size(); ++i) {
        if (pl->hand[i] == get_ptr()){
            if (get_ptr()->getName() == "Holy Light" || get_ptr()->getName() == "Restrain" || get_ptr()->getName() ==  "Slow"){
                effect->theEffect(otherPl);
            } else{
                effect->theEffect(pl);
            }
            pl->inPlayEnch.emplace_back(get_ptr());
            pl->hand.erase(pl->hand.begin()+i);
            break;

        }
    }
}
void sorceryCard    ::play(std::shared_ptr<Player> &pl,std::shared_ptr<Player> &otherPl){

    for (int i = 0; i < pl->hand.size(); ++i) {
        if (pl->hand[i] == get_ptr()){

            if (pl->hand[i]->getName() == "Reanimate"){
                effect->theEffect(pl);
            }else{
                effect->theEffect(otherPl);
            }
            pl->discard.emplace_back(get_ptr());
            pl->hand.erase(pl->hand.begin()+i);
            break;


        }
    }
}

void dealDmgPlyr(std::shared_ptr<Player> &plToBeHarmed, int attPow){
    plToBeHarmed->HP -= attPow;
}
void dealDmgCr(std::shared_ptr<creatureCard> &crToBeHarmed, int attPow){
    crToBeHarmed->currentHP -= attPow;
}

class destroyTargetLand : public Effect{
public:
    destroyTargetLand():Effect(){}
    void theEffect(std::shared_ptr<Player> &thePl) override{
        srand(time(nullptr));
        int index = int(rand()%(thePl->inPlayLand.size()+1));
        thePl->inPlayLand[index]->isDestroyed = true;
        std::cout<< "Land card "<< thePl->inPlayLand[index]->getName() << " is destroyed..."<< std::endl;
    }
};
class destroyTargetEnch : public Effect{
public:
    destroyTargetEnch():Effect(){}

    void theEffect(std::shared_ptr<Player> &thePl) override{
        std::string input;
        std::cout << "Choose an enchantment card to destroy..." << std::endl;
        for (int i = 0; i < thePl->inPlayEnch.size(); ++i) {
            std::cout << thePl->inPlayEnch[i] << ",";
        }

        std::cout << std::endl;
        std::getline(std::cin, input);

        for (int i = 0; i < thePl->inPlayEnch.size(); ++i) {
            if (input == thePl->inPlayEnch[i]->getName()){
                thePl->inPlayEnch[i]->isDestroyed = true;
                std::cout<< "Enchantment card "<< thePl->inPlayEnch[i]->getName() << " is destroyed..."<< std::endl;
                break;
            }
        }
    }
};
class destroyTargetCreat: public Effect{
public:
    destroyTargetCreat():Effect(){}
    void theEffect(std::shared_ptr<Player> &thePl) override {
        std::string input;
        std::cout << "Choose an creature card to destroy..." << std::endl;
        for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
            std::cout << thePl->inPlayCreature[i]->getName() << ", ";
        }

        std::cout << std::endl;
        std::getline(std::cin, input);

        for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
            if (input == thePl->inPlayCreature[i]->getName()) {
                std::cout<< "Creature card "<< thePl->inPlayCreature[i]->getName() << " is destroyed..."<< std::endl;
                thePl->inPlayCreature[i]->isDestroyed = true;
                break;
            }
        }
    }
};
class returnCharacter   : public Effect{
public:
    returnCharacter():Effect(){}
    void theEffect(std::shared_ptr<Player> &thePl) override{
        std::string input;
        if (thePl->hand.size() < 7){
            std::cout << "Pick a card to bring from discard..." << std::endl;
            for (int i = 0; i < thePl->discard.size(); ++i) {
                if (i != thePl->discard.size()-1){
                    std::cout << thePl->discard[i]->getName() << ", ";
                } else{
                    std::cout << thePl->discard[i]->getName() << std::endl;
                }
            }
            std::cout << std::endl;
            std::getline(std::cin,input);

            for (int i = 0; i < thePl->discard.size(); ++i) {
                if (input == thePl->discard[i]->getName()){
                    thePl->hand.push_back(thePl->discard[i]);
                    thePl->discard.erase(thePl->discard.begin()+i);
                }
            }
        } else{
            std::cout << "You already have 7 cards, you can not pick a new card." << std::endl;
        }
    }
};
class dealDamage        : public Effect{
protected:
    int attackPow;
public:
    dealDamage():Effect(){}

    dealDamage(int x):Effect(),attackPow(x){}
    void theEffect(std::shared_ptr<Player> &thePl) override{
        char input;
        std::cout << "Will you be damaging a player or a creature?(Type P for player, C for creature...)"<< std::endl;
        std::cin >> input;
        if (toupper(input) =='P'){
            thePl->HP -= attackPow;
        }else if(toupper(input)=='C'){
            std::string inputStr;
            for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
                std::cout << thePl->inPlayCreature[i]->getName() << ", ";
            }
            std::cout << "Which creature will you damage? " << std::endl;
            std::cin >> input;
            for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
                if (inputStr == thePl->inPlayCreature[i]->getName()){
                    thePl->inPlayCreature[i]->currentHP -= attackPow;
                }
            }
        }else{
            std::cout << "Wrong input..." << std::endl;
        }
    }

};
class effectPowersAll   : public Effect{
protected:
    int changeInHP;
    int changeInAttPow;
    std::string color;
public:
    effectPowersAll(int chHp, int chAtPow, std::string col):Effect(),changeInHP(chHp),changeInAttPow(chAtPow),color(col){}

    void theEffect(std::shared_ptr<Player> &thePl) override{
        for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
            if (thePl->inPlayCreature[i]->getColor() == color){
                thePl->inPlayCreature[i]->currentHP += changeInHP;
                thePl->inPlayCreature[i]->attackPower += changeInAttPow;
            }
        }
    }
};
class gainTrample       : public Effect{
public:
    gainTrample():Effect(){}

    void theEffect(std::shared_ptr<Player> &thePl) override{
        std::string input;
        std::cout << "Which creature will gain Trample?" << std::endl;
        for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
            std::cout << thePl->inPlayCreature[i]->getName() << ", ";
        }
        std::cin >> input;
        for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
            if (input == thePl->inPlayCreature[i]->getName()){
                thePl->inPlayCreature[i]->hasTrample = true;
            }
        }
    }
};
class loseTrample       : public Effect{
protected:
    std::string color;
public:
    loseTrample(std::string co):Effect(),color(co){}

    void theEffect(std::shared_ptr<Player> &thePl) override{
        for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
            if (color == thePl->inPlayCreature[i]->getColor()){
                thePl->inPlayCreature[i]->hasTrample = false;
            }
        }
    }
};
class loseHasFirstStrike: public Effect{
public:
    loseHasFirstStrike():Effect(){}
    void theEffect(std::shared_ptr<Player> &thePl) override{
        std::string input;
        std::cout << "Which creature will lose First Strike ability?" << std::endl;
        for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
            std::cout << thePl->inPlayCreature[i]->getName() << ", ";
        }
        std::cin >> input;
        for (int i = 0; i < thePl->inPlayCreature.size(); ++i) {
            if (input == thePl->inPlayCreature[i]->getName()){
                thePl->inPlayCreature[i]->hasFirstStrike = false;
            }
        }
    }
};

void creatureCard::attack(std::shared_ptr<Player> &plToBeHarmed, std::shared_ptr<creatureCard> &crToBeHarmed) {
    std::shared_ptr<creatureCard> thisCreature = get_ptr();

    if (crToBeHarmed == nullptr ){
        dealDmgPlyr(plToBeHarmed, attackPower);
        if (plToBeHarmed->getHP() <= 0){
            std::cout << "The opposing player has been defeated, you won!" << std::endl;
        }
    } else{
        if (hasTrample){
            if ((attackPower-crToBeHarmed->currentHP) > 0){
                int excPow = (attackPower - crToBeHarmed->currentHP);
                dealDmgPlyr(plToBeHarmed,excPow);
                if (plToBeHarmed->getHP() <=0){
                    std::cout << "The opposing player has been defeated, you won!" << std::endl;
                }
            }
        }
        if(hasFirstStrike){
            if (crToBeHarmed->hasFirstStrike){
                dealDmgCr(crToBeHarmed,attackPower);
                std::cout << thisCreature->getName() << " hit " << crToBeHarmed->getName() << ", current HP is:" << thisCreature->currentHP << std::endl;
                if (crToBeHarmed->currentHP <= 0){
                    std::cout << crToBeHarmed->getName() << " is destroyed..." << std::endl;
                    crToBeHarmed->isDestroyed= true;
                }else{
                    dealDmgCr(thisCreature,crToBeHarmed->attackPower);
                    if (currentHP <=0){
                        std::cout << thisCreature->getName() << " is destroyed..." << std::endl;
                        thisCreature->isDestroyed=true;
                    }
                    std::cout <<crToBeHarmed->getName() << " hit " <<  thisCreature->getName() << ", current HP is:" << thisCreature->currentHP << std::endl;
                }

            }
            else{
                crToBeHarmed->currentHP -= attackPower;
                std::cout << thisCreature->getName() << " hit " << crToBeHarmed->getName()  << ", current HP is:" << crToBeHarmed->currentHP << std::endl;
                if (crToBeHarmed->currentHP <= 0){
                    crToBeHarmed->isDestroyed = true;
                    std::cout << crToBeHarmed->getName() << " is destroyed..." << std::endl;
                }
            }
        }
        else{
            if (!(crToBeHarmed->hasFirstStrike)){
                crToBeHarmed->currentHP -= attackPower;
                std::cout << thisCreature->getName() << " hit " << crToBeHarmed->getName() << ", current HP is:" << crToBeHarmed->currentHP << std::endl;
                if (crToBeHarmed->currentHP <= 0){
                    crToBeHarmed->isDestroyed = true;
                    std::cout << crToBeHarmed->getName() << " is destroyed..." << std::endl;

                }else{
                    currentHP -= crToBeHarmed->attackPower;
                    std::cout << crToBeHarmed->getName() << " hit " << thisCreature->getName() << ", current HP is:" <<thisCreature->currentHP << std::endl;
                    if (currentHP <=0){
                        std::cout << thisCreature->getName() << " is destroyed..." << std::endl;
                        thisCreature->isDestroyed = true;
                    }
                }

            }
            else{
                currentHP -= crToBeHarmed->attackPower;
                std::cout << crToBeHarmed->getName() << " hit " << thisCreature->getName() << ", current HP is:" <<thisCreature->currentHP << std::endl;
                if (currentHP <= 0){
                    std::cout << thisCreature->getName() << " is destroyed..." << std::endl;
                   thisCreature->isDestroyed = true;

                }
            }

        }
    }
    for (int i = 0; i < plToBeHarmed->inPlayCreature.size(); ++i) {
        if (plToBeHarmed->inPlayCreature[i]->isDestroyed){
            plToBeHarmed->discard.emplace_back(plToBeHarmed->inPlayCreature[i]);
            plToBeHarmed->inPlayCreature.erase(plToBeHarmed->inPlayCreature.begin()+i);
        }
    }
}

class Rage     : public enchantmentCard{
public:
    Rage():enchantmentCard("Rage"){
        manaCost = {std::make_shared<greenMana>()};
        colorlessManaCount = 0;
        Color = "Green";
        effect = std::make_unique<gainTrample>();
    }
};
class holyWar  : public enchantmentCard{
public:
    holyWar():enchantmentCard("Holy War"){
        manaCost = {std::make_shared<whiteMana>()};
        colorlessManaCount = 1;
        Color = "White";
        effect = std::make_unique<effectPowersAll>(1,1,"White");
    }
};
class holyLight: public enchantmentCard{
public:
    holyLight():enchantmentCard("Holy Light"){
        manaCost = { std::make_shared<whiteMana>()};
        colorlessManaCount = 1;
        Color = "White";
        effect = std::make_unique<effectPowersAll>(-1,-1,"Black");
    }
};
class unholyWar: public enchantmentCard{
public:
    unholyWar():enchantmentCard("Unholy War"){
        manaCost = {std::make_shared<blackMana>()};
        colorlessManaCount = 1;
        Color = "Black";
        effect = std::make_unique<effectPowersAll>(2,0,"Black");
    }
};
class Restrain : public enchantmentCard{
public:
    Restrain():enchantmentCard("Restrain"){
        manaCost ={std::make_shared<redMana>()};
        colorlessManaCount = 2;
        Color = "Red";
        effect = std::make_unique<loseTrample>("Green");
    }
};
class Slow     : public enchantmentCard{
public:
    Slow():enchantmentCard("Slow"){
        manaCost = {std::make_shared<blackMana>()};
        colorlessManaCount = 0;
        Color = "Black";
        effect = std::make_unique<loseHasFirstStrike>();
    }
};

class Disenchant    : public sorceryCard{
public:
    Disenchant():sorceryCard("Disenchant"){
        manaCost = {std::make_shared<whiteMana>()};
        colorlessManaCount = 1;
        Color = "White";
        effect = std::make_unique<destroyTargetEnch>();
    }
};
class lightningBolt : public sorceryCard{
public:
    lightningBolt():sorceryCard("Lightning Bolt"){
        manaCost= {std::make_shared<greenMana>()};
        colorlessManaCount = 1;
        Color = "Green";
        effect = std::make_unique<dealDamage>(2);

    }
};
class Flood         : public sorceryCard{
public:
    Flood ():sorceryCard("Flood"){
        manaCost = {std::make_shared<greenMana>(),std::make_shared<whiteMana>()};
        colorlessManaCount = 1;
        Color = "Green";
        effect = std::make_unique<destroyTargetLand>();
    }
};
class Reanimate     : public sorceryCard{
public:
    Reanimate():sorceryCard("Reanimate"){
        manaCost = {std::make_shared<blackMana>()};
        colorlessManaCount = 0;
        Color = "Black";
        effect = std::make_unique<returnCharacter>();
    }
};
class Plague        : public sorceryCard{
public:
    Plague():sorceryCard("Plague"){
        manaCost = {std::make_shared<blackMana>()};
        colorlessManaCount = 2;
        Color = "Black";
        effect = std::make_unique<dealDamage>(1);
    }
};
class Terror        : public sorceryCard{
public:
    Terror():sorceryCard("Terror"){
        manaCost = {std::make_shared<blackMana>()};
        colorlessManaCount = 1;
        Color = "Black";
        effect = std::make_unique<destroyTargetCreat>();
    }
};



int main() {

    unsigned seed = std::chrono::system_clock::now()
            .time_since_epoch()
            .count();
    std::string inputMain;

    std::vector<std::shared_ptr<Card>> cardsForPlayerOne{std::make_shared<Plains>(), std::make_shared<Plains>(),std::make_shared<Plains>(),std::make_shared<Plains>(),std::make_shared<Plains>(),
                                                         std::make_shared<Forest>(),std::make_shared<Forest>(),std::make_shared<Forest>(),std::make_shared<Island>(),
                                                         std::make_shared<soldier>(),std::make_shared<soldier>(),std::make_shared<soldier>(),std::make_shared<armoredPegasus>(),std::make_shared<armoredPegasus>(),
                                                         std::make_shared<whiteKnight>(),std::make_shared<whiteKnight>(),
                                                         std::make_shared<angryBear>(),std::make_shared<guard>(),std::make_shared<werewolf>(),
                                                         std::make_shared<Disenchant>(),std::make_shared<lightningBolt>(),std::make_shared<Flood>(), std::make_shared<Flood>(),
                                                         std::make_shared<Rage>(),std::make_shared<holyWar>(), std::make_shared<holyLight>()};


    std::vector<std::shared_ptr<Card>> cardsForPlayerTwo{std::make_shared<Swamp>(),std::make_shared<Swamp>(),std::make_shared<Swamp>(),std::make_shared<Swamp>(),std::make_shared<Swamp>(),
                                                         std::make_shared<Mountain>(),std::make_shared<Mountain>(),std::make_shared<Mountain>(),std::make_shared<Island>(),
                                                         std::make_shared<skeleton>(),std::make_shared<skeleton>(),std::make_shared<skeleton>(),std::make_shared<ghost>(),std::make_shared<ghost>(),
                                                         std::make_shared<blackKnight>(),std::make_shared<blackKnight>(),
                                                         std::make_shared<orcManiac>(),std::make_shared<hobgoblin>(),std::make_shared<vampire>(),
                                                         std::make_shared<Reanimate>(),std::make_shared<Plague>(),std::make_shared<Terror>(),std::make_shared<Terror>(),
                                                         std::make_shared<unholyWar>(),std::make_shared<Restrain>(),std::make_shared<Slow>()};

    std::shuffle(cardsForPlayerOne.begin(),cardsForPlayerOne.end(),std::default_random_engine(seed));
    std::shuffle(cardsForPlayerTwo.begin(),cardsForPlayerTwo.end(),std::default_random_engine(seed));

    std::shared_ptr<Player> player1 = std::make_shared<Player>(cardsForPlayerOne);
    std::shared_ptr<Player> player2 = std::make_shared<Player>(cardsForPlayerTwo);

    std::vector<std::shared_ptr<Player>> players;
    players.emplace_back(player1);
    players.emplace_back(player2);

    for (int i = 0; i < 7; ++i) {
        player1->Draw();
        player2->Draw();
    }

    for (int round=1; (player1->getHP() > 0 && player2->getHP() > 0); round++ ) {

        std::cout << "Round " << round << " is starting..." << std::endl;

        for (int playerIndex = 0; playerIndex < players.size() && (player1->getHP() > 0 && player2->getHP() > 0) ; ++playerIndex) {

            std::cout << "Turn for player " << playerIndex+1 << "..." << std::endl;

            for (int phase = 0; phase < 6; ++phase) {
                if (phase == 0) {

                    players[playerIndex]->Draw();

                } else if (phase == 1) {

                    players[playerIndex]->unTap();

                } else if (phase == 2) {

                    std::cout << "Current Mana(s): ";
                    players[playerIndex]->tapAndGainMana();

                    std::cout << "Choose a card to play..." << std::endl;
                    for (int i = 0; i < players[playerIndex]->getHand().size(); ++i) {
                        if (i != (players[playerIndex]->getHand().size()-1)){
                            std::cout << players[playerIndex]->getHand()[i]->getName() << "," ;
                        } else{
                            std::cout << players[playerIndex]->getHand()[i]->getName() << std::endl;
                        }
                    }

                    std::getline(std::cin, inputMain);

                    for (int it = 0; it < players[playerIndex]->getHand().size(); it++) {
                        if (!(players[playerIndex]->getHand()[it]->getName() == inputMain))continue;
                        if (players[playerIndex]->getHand()[it]->getType()!= "Land"){
                            if (players[playerIndex]->payManaFunc(players[playerIndex]->getHand()[it])) {
                                players[playerIndex]->getHand()[it]->play(players[playerIndex],
                                                                          players[1 - playerIndex]);
                                break;
                            } else {
                                std::cout << "You do not have enough mana..." << std::endl;
                            }
                        } else{
                            players[playerIndex]->getHand()[it]->play(players[playerIndex],players[1-playerIndex]);
                        }
                        break;
                    }

                } else if (phase == 3) {

                    std::vector<std::string> namesAttack;
                    std::vector<std::string> namesDefend;
                    std::string input;
                    int num;
                    std::shared_ptr<creatureCard> forNull = nullptr;


                    players[playerIndex]->showCreatureCards();

                    if (players[playerIndex]->getCreatureCards().empty()){
                        std::cout << "You have no creature cards to attack with..." << std::endl;
                    }else{
                        std::cout << "You have " << players[playerIndex]->getCreatureCards().size() << " creature card(s)..."<< std::endl;
                        std::cout << "How many creature cards will you attack with? "<< std::endl;
                        std::cin >> num;

                        if (num > 0){
                            for (int i = 0; i < num; ++i) {
                                std::getline(std::cin, input);
                                namesAttack.emplace_back(input);
                            }

                            if (!(players[1-playerIndex]->getCreatureCards().empty())){
                                std::cout << "Opposing player shall choose defending creatures...Type 'none' for not choosing one. " << std::endl;
                                players[1-playerIndex]->showCreatureCards();

                                for (int i = 0; i < num; ++i) {
                                    std::getline(std::cin, input);
                                    namesDefend.emplace_back(input);
                                }
                            } else{
                                for (int i=0; i< namesAttack.size(); i++) {
                                    for (int j = 0; j < players[playerIndex]->getCreatureCards().size(); ++j) {
                                        if(namesAttack[i] == players[playerIndex]->getCreatureCards()[j]->getName()){
                                            players[playerIndex]->getCreatureCards()[j]->attack((players[1-playerIndex]),forNull);
                                        }
                                    }
                                }
                            }

                            for (int indexStrings = 0; indexStrings < num; ++indexStrings) {
                                for (int indexAttacker = 0; indexAttacker < num; ++indexAttacker) {
                                    for (int indexDefender = 0; indexDefender < num; ++indexDefender) {
                                        if (namesAttack[indexStrings] == players[playerIndex]->getCreatureCards()[indexAttacker]->getName()){
                                            players[playerIndex]->getCreatureCards()[indexAttacker]->tap();
                                            if (namesDefend[indexStrings] != "null" && namesDefend[indexStrings] == players[1-playerIndex]->getCreatureCards()[indexDefender]->getName()){
                                                players[playerIndex]->getCreatureCards()[indexAttacker]->attack(players[1-playerIndex],players[1-playerIndex]->getCreatureCards()[indexDefender]);

                                            }else if(namesDefend[indexStrings] == "null"){
                                                players[playerIndex]->getCreatureCards()[indexAttacker]->attack(players[1-playerIndex], forNull);

                                            }
                                        }
                                    }
                                }
                            }
                        }
                        for (int i = 0; i < players.size(); ++i) {
                            if (players[playerIndex]->getHP()<=0){
                                std::cout << "Player "<< playerIndex+1 << " is destroyed, Player " << (1-playerIndex)+1 << " is the winner!"<<std::endl;
                                break;
                            }
                        }
                    }

                } else if (phase == 4) {

                    std::cout << "Choose a card to play..." << std::endl;
                    for (int i = 0; i < players[playerIndex]->getHand().size(); ++i) {
                        if (i != (players[playerIndex]->getHand().size()-1)){
                            std::cout << players[playerIndex]->getHand()[i]->getName() << "," ;
                        } else{
                            std::cout << players[playerIndex]->getHand()[i]->getName() << std::endl;
                        }
                    }

                    std::getline(std::cin, inputMain);

                    for (int it = 0 ; it < players[playerIndex]->getHand().size();it++) {
                        if (players[playerIndex]->getHand()[it]->getName() == inputMain) {
                            if (players[playerIndex]->getHand()[it]->getType() == "Land") {
                                if (players[playerIndex]->playedLandCard()) {
                                    std::cout << "You can not play land card..." << std::endl;
                                    break;
                                } else {
                                    players[playerIndex]->getHand()[it]->play(players[playerIndex], players[1 - playerIndex]);
                                    break;
                                }
                            } else {
                                if (players[playerIndex]->payManaFunc(players[playerIndex]->getHand()[it])) {
                                    players[playerIndex]->getHand()[it]->play(players[playerIndex], players[1 - playerIndex]);
                                    break;
                                } else {
                                    std::cout << "You do not have enough mana..." << std::endl;
                                }
                            }
                        }
                    }

                } else {
                    players[playerIndex]->resetForEndPhase();
                }

                for (int i = 0; i < players.size(); ++i) {
                    players[i]->resetForPhase();
                }
            }
        }

        std::cout << "End of round " << round << "..." << std::endl;
    }

    return 0;
}