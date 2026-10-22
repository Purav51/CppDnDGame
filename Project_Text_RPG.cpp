#include <iostream>
#include <random>
#include <chrono> // For high-precision time seeding
using namespace std;

class Character
{
private:
    // Static stats
    int health;
    int minDamage;
    int maxDamage;

public:
    Character(int health, int minDamage, int maxDamage)
    {
        this->health = health;
        this->minDamage = minDamage;
        this->maxDamage = maxDamage;
    };
    int GetHealth()
    {
        return health;
    }
    int DealDamage()
    {
        random_device rd;                                            // source of the random value to provide the seed.
        static mt19937 gen(rd());                                    // the engine that uses the seed to generate psuedorandom numbers squence (raw)
        uniform_int_distribution<int> distrib(minDamage, maxDamage); // converts the raw psuedorandom number sequence into a uniform distribution / range
        return distrib(gen);
    }
    void ReduceHealth(int damage)
    {
        health -= damage;
    }
};
int DiceRoll()
{
    // static ensures this initialization block only executes ONCE on the 1st call
    static mt19937 gen([]()
                       {
        // 1. Try to get a hardware value
        random_device rd;
        
        // 2. Get the current exact time in nanoseconds
        auto time_seed = chrono::high_resolution_clock::now().time_since_epoch().count();
        
        // 3. Get a unique memory address state 
        // (The OS loads the executable at a different memory slot every run)
        uintptr_t mem_address = reinterpret_cast<uintptr_t>(&time_seed);

        // 4. Blend them together using XOR (^). If rd() fails and returns a fixed 
        // number, the time and memory pointer will overwrite it with massive entropy.
        unsigned int final_seed = static_cast<unsigned int>(rd() ^ time_seed ^ mem_address);
        
        return mt19937(final_seed); }());

    uniform_int_distribution<int> distrib(1, 20);

    return distrib(gen);
}
class Player : public Character
{
private:
    // increase at start
    int stren = 8;
    int dext = 8;
    int intel = 8;

    void allocateStats()
    {
        int ptsRemaining = 2;
        while (ptsRemaining > 0)
        {
            cout << "Pick where to invest the points and enter the number of points.  1. Strength 2. Intelligence 3. Dexterity. " << endl
                 << endl;
            int statChoice;
            cout << "Choose your stat: 1. 2. or 3.: ";
            cin >> statChoice;
            int ptsinput;
            cout << "Enter the number of points to input 1. or 2.: ";
            cin >> ptsinput;
            if (statChoice < 1 || statChoice > 3 || ptsinput < 1 || ptsinput > 2)
            {
                cout << "invalid input" << endl
                     << endl;
            }
            else
            {
                if (ptsRemaining >= ptsinput)
                {
                    ptsRemaining -= ptsinput;

                    switch (statChoice)
                    {
                    case 1:
                        stren += ptsinput;
                        break;
                    case 2:
                        intel += ptsinput;
                        break;
                    case 3:
                        dext += ptsinput;
                        break;
                    }

                    cout << "Points Remaining: " << ptsRemaining << endl
                         << endl;
                }
                else
                {
                    cout << endl
                         << "You don't have enough points remaining!" << endl
                         << endl;
                }
            }
        }
    }

public:
    Player()
        : Character(30, 10, 15)
    {
    }
    void displayStats()
    {
        cout << endl
             << "Strength is: " << stren << endl
             << endl;
        cout << "Intelligence is: " << intel << endl
             << endl;
        cout << "Dexterity is: " << dext << endl
             << endl;
    }
    void BuildPlayer()
    {
        allocateStats();
        displayStats();
    }
    int GetIntelligence()
    {
        return intel;
    }
    int GetDexterity()
    {
        return dext;
    }
};
class Enemy : public Character
{
public:
    Enemy()
        : Character(40, 10, 20)
    {
    }
};
class TutorialRoom
{
private:
    Player *player = nullptr; // decalration of a pointer that later on will point to the respective player.
public:
    void BeginRoomTutorial(Player *CurrPlayer)
    {
        this->player = CurrPlayer;
        cout << "You wake up in a dark room illuminated by a few candlelights, hanging in the air. Your vision that was blurry starts to settle. What do you do?" << endl
             << endl;
        cout << "1. Get up and Look around" << endl
             << "2. Check yourself" << endl;
        ProcessChoice1();
        cout << "Around the room you notice 3 things along with a bed, a chest, a book and a small shiv stuck inside a jar. What do you do?" << endl
             << endl;
        cout << "1. Go to bed. " << endl
             << "2. Try to open the chest" << endl
             << "3. Examine the book." << endl
             << "4. Try to remove the shiv" << endl;
        ProcessChoice2();
    }
    void ProcessChoice1()
    {
        bool TutFinished = false;

        // a while loop that runs infinitely until the player picks the right choice
        while (TutFinished != true)
        {
            int Choice;
            cin >> Choice;
            switch (Choice)
            {
            case 1:
                cout << "You try to jolt up normally but an ache in the backside of your head prevents you from sudden movements,you slowly move up and stand on your feet." << endl;
                TutFinished = true;
                break;
            case 2:
                player->displayStats();
                cout << endl
                     << "You seem to be fine for the most part, other than a headache you don't feel particularly bad. What do you do? 1. Get up and look around." << endl
                     << "Make a pick (1 or 2): ";
                break;
            default:
                cout << "please enter 1 or 2.";
                break;
            }
        }
    }
    void ProcessChoice2()
    {
        bool TutFinished = false;
        while (TutFinished != true)
        {
            int Choice;
            cin >> Choice;

            switch (Choice)
            {
            case 1:
            {
                cout << "You slowly move and lie down face first on the bed. "
                     << "Within a few seconds your eyes shut."
                     << endl;

                TutFinished = true;
                break;
            }

            case 2:
            {
                cout << endl
                     << "Strength Check: need to roll 12 or higher"
                     << endl;

                int result = DiceRoll();

                cout << "You rolled: " << result << endl;

                if (result >= 12)
                {
                    cout << "Success!" << endl;
                    cout << "The chest opens and you find a bottle of water "
                         << "and some clean clothes inside."
                         << endl;
                }
                else
                {
                    cout << "The chest doesn't budge." << endl;
                }

                cout << endl
                     << "What do you do now?"
                     << endl
                     << "1. Go to bed"
                     << endl
                     << "3. Examine the book"
                     << endl
                     << "4. Try to remove the shiv"
                     << endl;

                cin >> Choice;

                if (Choice == 1 || Choice == 3 || Choice == 4)
                {
                    TutFinished = true;
                    break;
                }
                else
                {
                    cout << "Please pick 1, 3 or 4."
                         << endl;
                }

                break;
            }
            case 3:
            {
                cout << endl
                     << "Intelligence Check: need to roll 12 or higher"
                     << endl;

                int result = DiceRoll();
                cout << "You rolled: " << result << endl;

                if (result >= 12)
                {
                    cout << "Success!" << endl;
                    cout << "The book is a famous tome containing the history of the land. You pass the pages and  find the mention of a paladin's dungeon where a legendary sword was burried." << endl;
                }
                else
                {
                    cout << "You aren't able to figure out everything but you do see a map and a location marked with a red crossed and the name of the place called The Paladin's Rest." << endl;
                }
                cout << endl
                     << "What do you do now?"
                     << endl
                     << "1. Go to bed"
                     << endl
                     << "2. Try to Open the Chest"
                     << endl
                     << "4. Try to remove the shiv"
                     << endl;

                cin >> Choice;

                if (Choice == 1 || Choice == 2 || Choice == 4)
                {
                    TutFinished = true;
                    break;
                }
                else
                {
                    cout << "Please pick 1, 2 or 4."
                         << endl;
                }
                break;
            }
            default:
            {
                cout << "Please enter 1, 2, 3 or 4."
                     << endl;

                break;
            }
            }
        }
    }
};
class Level1
{
private:
    Player *p = nullptr;
    bool incantationRecognized = false;

public:
    void Entrance(Player *player)
    {
        this->p = player;

        cout << endl
             << "You find yourself staring at the mouth of a giant cave. "
             << "According to the map you recovered from the chest in that tavern, "
             << "this is where the sword of the king's paladin is kept, along with his treasures."
             << endl;

        cout << endl
             << "What do you do?" << endl
             << "1. Enter the cave." << endl
             << "2. Look around." << endl;

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            cout << endl
                 << "You grip your torch, check your gear, take a breath and..."
                 << endl;
            incantationRecognized = false;
        }

        if (choice == 2)
        {
            int intelligenceModifier = p->GetIntelligence() - 8;
            cout << endl
                 << "Intelligence roll: need 14 or higher." << endl
                 << "Your intelligence modifier is: +" << intelligenceModifier << endl;

            int rolledNumber = DiceRoll();

            cout << endl
                 << "You rolled: " << rolledNumber << endl;

            int finalRolledNumber = rolledNumber + intelligenceModifier;

            cout << "After adding your modifier: +" << intelligenceModifier << endl;
            cout << "Your final rolled number is: " << finalRolledNumber << endl;

            if (finalRolledNumber >= 14)
            {
                cout << endl
                     << "You notice a carving on the wall, written in a strange language, "
                     << "but you recognize a few words written in   elven script that read as an incantation of sorts. This can be useful. "
                     << endl;
                incantationRecognized = true;
            }
            else
            {
                cout << endl
                     << "You notice a carving on the wall, written in a strange language, "
                     << "but you can't recognize it."
                     << endl;
                incantationRecognized = false;
            }
        }

        cout << endl
             << "incantation recognized? " << incantationRecognized;
        cout << endl
             << "You venture forth." << endl;
    }
    void TrappedDoor(Player *player)
    {
        this->p = player;

        if (incantationRecognized == true)
        {
            cout << endl
                 << "Trap disabled" << endl;
        }
        else
        {
            cout << endl
                 << "Trap fired!! Dexterity saving throw needed" << endl;
            int dexterityModifier = p->GetDexterity() - 8;
            cout << endl
                 << "Dexterity Roll: need 14 or higher." << endl
                 << "Your Dexterity modifier is: +" << dexterityModifier << endl;

            int rolledNumber = DiceRoll();

            cout << endl
                 << "You rolled: " << rolledNumber << endl;

            int finalRolledNumber = rolledNumber + dexterityModifier;

            cout << "After adding your modifier: +" << dexterityModifier << endl;
            cout << "Your final rolled number is: " << finalRolledNumber << endl;

            if (finalRolledNumber < 14)
            {
                cout << endl
                     << "Your current health is: " << p->GetHealth() << endl;
                p->ReduceHealth(2);
                cout << endl
                     << "The trap hits you and does 2 points of damage." << endl
                     << "your health is: " << p->GetHealth();
            }
            else
            {
                cout << endl
                     << "You avoid the trap sucessfuly." << endl;
            }
        }
    }
};
class Game
{
private:
    Player player;
    Enemy enemy;
    TutorialRoom room;
    Level1 lvl1;

public:
    void StartGame()
    {
        // Building the character
        cout << endl
             << "Welcome!!" << endl
             << endl;
        cout << endl
             << "to start off please build your character: " << endl
             << endl;
        player.BuildPlayer();

        //  level 1.
        lvl1.Entrance(&player);
        lvl1.TrappedDoor(&player);
    }

    // Combat Mechanics
    void AttackPlayer()
    {
        int damage = enemy.DealDamage();
        cout << "Player health is: " << player.GetHealth() << endl;
        player.ReduceHealth(damage);
        cout << "Enemy attacked the player for "
             << damage << " damage." << endl;
        cout << "Player health is now: "
             << player.GetHealth() << endl;
    }
    void AttackEnemy()
    {
        int damage = player.DealDamage();
        cout << "Enemy health is: " << enemy.GetHealth() << endl;
        enemy.ReduceHealth(damage);
        cout << "Player attacked enemy for " << damage << " damage." << endl;
        cout << "Enemy health is now: " << enemy.GetHealth() << endl;
    }
    void StartTutorial()
    {
        room.BeginRoomTutorial(&player);
    }
};

int main()
{
    Game game;
    game.StartGame();
    // game.StartTutorial();
    return 0;
}

void checkRoll()
{
    int res = DiceRoll();
    cout << res << endl;
    if (res >= 13)
    {
        cout << "Success!!" << endl;
    }
    else
    {
        cout << "Fail!!" << endl;
    }
}
