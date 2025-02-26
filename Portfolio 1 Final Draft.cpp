#include <iostream>
#include <random>
#include <string>
//these includes are so I can do a wait command so the screen isnt just flooded with text.
#include <thread>
#include <chrono>

    //this is my waitinme function
void wait(int waittime);

//this is a function for transistionary dialouge inbetween eventing
void transtxt(int transtype);


//Vital functions pieces for HP tracking
int PLAYERHP = 20;
int remHP = 20;
void HPDISPLAY(int DMG);
bool incombat;

//These are the vars for tracking the score screen and if the new score is higher or lower then the previous one.
int UnoDos;
int playattempts;
void scorescreen(int playattempts);
int score;
int StoredScore;
int bscalc;
int mathfix;
int dropamount;

//Enemy encounter vars
bool didrun = false;

//Adventurer encounter var
void FELLOWADVENTURE(int adventype);

//These are the treasure variables
void TREASUREROLL(int treasurechance);
int occult_doll;
bool have_ancient_bowl = false;
bool have_magic_sword = false;
bool dungeonkey = false;

//enemy drop variables
int onions;
int pieceofheart;
int snakeegg;

int rarespice;
int sparemeat;
bool dragonheart = false;

//Other Variables
bool gameloop = true;
bool InvalidNumInput = true;
int yesno;
int rngdice;
int phpmax;
char EnemyGroup;


//this is my enemy class, its to reduce on code bloat and it looks nice in int main.
class Foes {
public:
    std::string eniNM;
    std::string eniTXT;
    int eniHP;
    int eniDMG;
    int eniFLEE;
    int eniHIT;
    int eniSCR;
    int eniENTtxt;
    int eniAFtxt;
    int whatdrops;
    int dropchance;
    std::string dropname;

    void ENEMYENC() {
        transtxt(eniENTtxt);
        std::cout << std::endl << "Oh no! A " << eniNM << " blocks your path!" << std::endl;
        wait(500);
        didrun = false;
        incombat = true;
        InvalidNumInput = true;
        while (InvalidNumInput || eniHP > 0) {
            std::cout << "\n1) Attack\n2) Run";
            std::cout << "\nWhat do you do: ";
            std::cin >> yesno;
            if (std::cin.fail()) {
                std::cin.clear(); std::cin.ignore(512, '\n');
                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2..." << std::endl;
            }
            else if (yesno != 1 && yesno != 2) {
                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2..." << std::endl;
            }
            else if (yesno == 2) {
                std::cout << "\nYou attempt to run past it..." << std::endl;
                rngdice = ((rand() % eniFLEE));
                if (rngdice == 0) {
                    //succussfull getaway. yields no enemy drops or increased score from vanquishing
                    std::cout << "You get away from the " << eniNM << "! And continue." << std::endl;
                    didrun = true;
                    InvalidNumInput = false;
                    break;
                }
                else {
                    //fail to run and take damage
                    std::cout << "But the " << eniNM << " catches up to you and " << eniTXT << "!" << std::endl;
                    //taking damage
                    HPDISPLAY(eniDMG);
                }
            }
            else {
                //attack
                    //The Magic Sword has both a higher damage and crit chance.
                InvalidNumInput = false;
                if (have_magic_sword) {
                    rngdice = ((rand() % 2) + 5);
                    std::cout << "You attack with your Magic Sword!" << std::endl;
                    if (rngdice == 7) {
                        //crit                            
                        std::cout << "Woah! A critical hit! Massive damage inflicted on the " << eniNM << "!" << std::endl;
                        eniHP = eniHP - 9;
                    }
                    else {
                        eniHP = eniHP - 3;
                    }
                }
                else {
                    //standard sword attack
                    rngdice = ((rand() % 4) + 4);
                    std::cout << "You attack with your Sword!" << std::endl;
                    if (rngdice == 7) {
                        //crit                            
                        std::cout << "Woah! A critical hit! Massive damage inflicted on the " << eniNM << "!" << std::endl;
                        eniHP = eniHP - 3;
                    }
                    else {
                        eniHP = eniHP - 1;
                    }
                }
                wait(500);
                //This is where enemy hit chance happens

                if (eniHP > 0) {
                    std::cout << "\nThe " << eniNM << " moves to attack you!" << std::endl;
                    //hit chance
                    rngdice = (rand() % (eniHIT - 1));
                    if (rngdice == (rand() % (eniHIT - 1))) {
                        std::cout << "The " << eniNM << " attacks you and " << eniTXT << "!" << std::endl;
                        HPDISPLAY(eniDMG);
                    }
                    else {
                        std::cout << "The " << eniNM << " attacks you and misses!" << std::endl;
                    }
                }
            }

        }
        //this is for the score increase after the encouter is cleared. it only happens if you didn't run.
        if (didrun == false) {
            //foe deafeted!
            std::cout << "Success! The " << eniNM << " was vanquished!" << std::endl << std::endl;

            score = score + (rand() % (eniSCR / 30) + eniSCR);
            //this is how loot drops are calculated
            if (dropchance > 0 && dropchance <= 100) {
                rngdice = (rand() % 99) + 1;
                if (dropchance >= rngdice) {
                    std::cout << "Oh! Theres something here!" << std::endl;
                    std::cout << "Picked up a" << dropname << "." << std::endl;
                    whatdrops++;
                    if (pieceofheart == 2) {
                        //for when you fight skelli
                        wait(500);
                        std::cout << "Woah! It looks like two of these strange red crytsals you found fit together!" << std::endl;
                        std::cout << "The completed crystal, now with a visible heart shape, melts into your body..." << std::endl;
                        std::cout << "What change could this have brought about..." << std::endl;
                        phpmax = phpmax + 2;
                        PLAYERHP = PLAYERHP + 2;
                        pieceofheart = 0;
                    }
                }
            }
            else if (dropchance > 100) {
                //for things with multidrop
                //I ended up only having one multidrop enemy so it feels kinda pointless
                rngdice = dropchance / 100;
                mathfix = dropchance - (rngdice * 100);

                //this garuntees at least 2 drops when the number was above 300
                if (rngdice >= 3) {
                    dropamount++;
                    dropamount++;
                    whatdrops++;
                    whatdrops++;
                    rngdice--;
                    rngdice--;
                }
                for (dropchance = rngdice; dropchance > 0; dropchance--) {
                    rngdice = (rand() % 99) + 1;
                    if (mathfix >= rngdice) {
                        dropamount++;
                        whatdrops++;
                    }
                }
                std::cout << "Oh! Theres something here!" << std::endl;
                std::cout << "Picked up " << dropamount << " " << dropname << "." << std::endl;
            }
            else {
                //no drops
            }
            transtxt(eniAFtxt);
        }
        else {
            //you do get points for surviving by running, but its a random number between 25 and 50
            score = score + ((rand() % 26) + 25);
            transtxt(eniAFtxt);
        }
        incombat = false;

    }
    //this is the enemy constuctor, its how I tell c++ to take in data when I make an enemy later on (like at the start of int main() )
    Foes(std::string name, std::string txt, int hp, int dph, int flee, int hit, int score, int nttxt, int xttxt, int dropvar, int chance, std::string item) {
        //if i put the brackets not here it doesnt work or something i don't fully understand why...
        eniNM = name;
        eniTXT = txt;
        eniHP = hp;
        eniDMG = dph;
        eniFLEE = flee;
        eniHIT = hit;
        eniSCR = score;
        eniENTtxt = nttxt;
        eniAFtxt = xttxt;
        whatdrops = dropvar;
        dropchance = chance;
        dropname = item;
    };
};




int main() {

    //these are the enemies you fight throughout the game, i define them right here.
    //formatting is
    //Foes(Name, attack, HP, DMG, Flee chance 1/x, Hit chance 1/x, base score for defeat, What entry dialouge to use, What after event dialouge to use, Which item drops, how often, dropped item name)
   
    Foes goblin = Foes("Goblin", "swings a bat", 2, 1, 1, 2, 200, 1, 2, sparemeat, 5, " bag of meat scraps");
    Foes skeleton = Foes("Skeleton", "swings a rusted sword", 3, ((rand() % 1) + 1), 3, 4, 300, 1, 2, pieceofheart, 30, " strange red crystal");
    Foes onionsmall = Foes("Walking Onion", "tackles", 2, 1, 1, 5, 420, 1, 2, onions, 100, "n onion");

    Foes skeletondangerous = Foes("Dangerous looking Skeleton", "swings a pristine sword", ((rand() % 6) + 1), ((rand() % 1) + 2), 3, 4, 500, 1, 2, pieceofheart, 75, "strange red crystal");
    Foes snake = Foes("Large Snake", "bites", 5, 2, 4, 5, 370, 1, 2, snakeegg, 60, " strange egg");
    Foes minos = Foes("Minotaur", "swings an axe", 7, 4, 2, 6, 900, 1, 2, sparemeat, 50, " bag of meat scraps");
    Foes sqrl = Foes("Huge Squirrel", "throws a nut", 8, 3, 6, 7, 900, 1, 2, rarespice, 100, " bag of rare spices");

    Foes onionlarge = Foes("Onion Lord", "spins like a top", 12, 3, 1, 3, 2222, 1, 2, onions, 567, "onions");
    Foes lizard = Foes("Huge Lizard", "lunges and bites", 10, 2, 10, 3, 1111, 1, 2, snakeegg, 100, " strange egg");

    Foes goblinlarge = Foes("Armored Goblin", "swings a mace", 15, 5, 5, 7, 1500, 1, 2, 0, 0, "");

    Foes goblinking = Foes("Goblin 'King'", "swings viciously", 20, 4, 5, 5, 3000, 4, 4, 0, 0, "");
    Foes dragonred = Foes("Red Dragon", "breathes fire", 25, 5, 6, 3, 5000, 4, 4, 0, 0, "");

    std::cout << R"(
                                                    &&
                                                    &&
 __________________________________________________&&&&              &
/__________________________________________________&@@@@@@@@@@@@@@@@&&}
\__________________________________________________&@@@@@@@@@@@@@@@@&&}
                                                   &&&&              &
                                                    &&
                                                    &&
)" << '\n';
    std::cout << "\nThis game was made possible due to many numerous contributiors to the ASCII art archive." << std::endl;
    std::cout << "Welcome to Bountiful Dungeons!!!" << std::endl << "The goal of this game is too:\n1. Explore a dungeon \n2. Collect monster parts and treasure\n3. Cook up a sick meal with what you got \n4. Aim for the highest score!" << std::endl;
    while (gameloop) {
        while (InvalidNumInput) {
            std::cout << "\nWould you like to start the game?\n1) Start Game\n2) Close Game";
            std::cout << "\nYour answer: ";
            std::cin >> yesno;
            //this first branch of the if/else statement is for valididating user inputs, it checks for a non int input
            if (std::cin.fail()) {
                //the cin.clear clears the cin buffer, basically its what makes it so the program allows for cin >> gamestart to occour again.
                std::cin.clear();
                std::cin.ignore(1024, '\n');
                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
            }
            else if (yesno == 2) {
                std::cout << "\nUnderstood...";
                std::cout << "\n\nClosing the Game!\n";
                return 0;
            }
            else if (yesno != 2 && yesno != 1) {
                std::cin.clear();
                std::cin.ignore(1024, '\n');
                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
            }
            else {
                //for when a 1 is inputted
                InvalidNumInput = false;
            }
        }
        //these are the reset for variables that need to be reset upon replay
                // realisitcally, a lot of these don't need to be reset, but in case I mess something up they are kinda a failsafe, 
                // though that could also make error checking hard so I may just eliminate them if I can
        srand((unsigned int)time(NULL));
        PLAYERHP = 20;
        phpmax = PLAYERHP;
        score = 0;
        bool have_magic_sword = false;
        bool have_ancient_bowl = false;
        int occult_doll = 0;
        bool InvalidNumInput = true;
        bool didrun = false;
        EnemyGroup = 'X';
        incombat = false;


        onions = 0;
        pieceofheart = 0;
        snakeegg = 0;
        rarespice = 0;
        sparemeat = 0;
        dungeonkey = false;
        dragonheart = false;

        wait(500);

        //The following are randomized intro cutscenes, I kinda hate how they turned out but I'm not cutting them for whatever reason

        rngdice = (rand() % 4);
        if (rngdice == 0) {
            std::cout << std::endl;
            //this gives you an increased chance at getting a chance at th first treasure.
            //it does this by inscreasing your base score!!!
            std::cout << R"(
   ...    *    .   _  .   
*  .  *     .   * (_)   *
  .      |*  ..   *   ..
   .  * \|  *  ___  . . *
*   \/   |/ \/{o,o}     .
  _\_\   |  / /)  )* _/_ *
      \ \| /,--"-"---  ..
_-----`  |(,__,__/__/_ .
       \ ||      ..
        ||| .            *
        |||
ejm98   |||
  , -=-~' .-^- _
)" << '\n';
            std::cout << "\nAn beautiful owl sits on a tree outside a ominous dungeon...\nPerhaps this is a sign of good things to come!";
            score = score + (rand() % 36) + 470;
        }
        else if (rngdice == 1 || rngdice == 3) {
            std::cout << "\nYou feel a tug on your fishing line!\nThen reel in your line...\nBut then suddenly a fox comes out of nowhere!";
            std::cout << R"(
|\_/|,,_____,~~`
(.".)~~     )`~}}
 \o/\ /---~\\ ~}}
   _//    _// ~}
)" << '\n';
            std::cout << "\nDrats! The little fella stole your fish and ran off...";
            std::cout << "\nYou will have to go into the dungeon on an empty stomach, how sad...";
        }
        else if (rngdice == 2 || rngdice == 4) {
            std::cout << "\nWow! An ominous dungeon with an entrance through a hole in huge tree trunk!\nSounds perfect for exploring!" << std::endl;
        }
        wait(1000);

        std::cout << "\nAs you enter the dungeon you can feel a cold breeze coming from deep inside." << std::endl;

        wait(500);


        //Encounter A Skeleton|| Goblin|| Onion|
        EnemyGroup = 'A';
        rngdice = (rand() % 4);
        if (rngdice == 0 || rngdice == 1) {
            //combat!
            skeleton.ENEMYENC();
        }
        else if (rngdice == 3 || rngdice == 4) {
            goblin.ENEMYENC();
        }
        else if (rngdice == 2) {
            onionsmall.ENEMYENC();
        }
        else {
            //this branch should in theroy never fire off, but its here so that if it does happen, I know where to look.
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }

        //treasure roll A (high roll)
        TREASUREROLL(777);

        //Encounter B D-Skeleton| Skeleton|| Goblin|||| Huge Snake|| Onion|
        EnemyGroup = 'B';
        rngdice = (rand() % 9);
        if (rngdice == 1 || rngdice == 2) {
            skeleton.ENEMYENC();
        }
        else if (rngdice == 3 || rngdice == 4 || rngdice == 5 || rngdice == 8) {
            goblin.ENEMYENC();

        }
        else if (rngdice == 7) {
            onionsmall.ENEMYENC();
        }
        else if (rngdice == 0) {
            skeletondangerous.ENEMYENC();
        }
        else if (rngdice == 6 || rngdice == 9) {
            snake.ENEMYENC();
        }
        else {
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }

        //treasure roll B (low roll)
        TREASUREROLL(300);

        FELLOWADVENTURE(1);


        //treasure roll c (high roll)
        TREASUREROLL(1000);


        //Encounter C D-Skeleton| Skeleton| Goblin| Huge Snake| Onion||||||
        EnemyGroup = 'C';
        rngdice = (rand() % 9);
        if (rngdice == 1) {
            skeleton.ENEMYENC();
        }
        else if (rngdice == 8) {
            goblin.ENEMYENC();

        }
        else if (rngdice == 7 || rngdice == 2 || rngdice == 3 || rngdice == 4 || rngdice == 5 || rngdice == 6) {
            onionsmall.ENEMYENC();
        }
        else if (rngdice == 0) {
            skeletondangerous.ENEMYENC();
        }
        else if (rngdice == 9) {
            snake.ENEMYENC();
        }
        else {
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }



        //The following is a dev test line so I can balance the score
        //std::cout << std::endl << std::endl << "The current score is " << score << std::endl << std::endl;


        TREASUREROLL(1300);



        FELLOWADVENTURE(2);

        TREASUREROLL(3100);

        //Encounter D D-Skeleton| Huge Snake|
        EnemyGroup = 'D';
        rngdice = (rand() % 1);
        if (rngdice == 0) {
            skeletondangerous.ENEMYENC();
        }
        else if (rngdice == 1) {
            snake.ENEMYENC();
        }
        else {
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }

        TREASUREROLL(2200);

        FELLOWADVENTURE((rand() % 1) + 1);


        //Encounter E Huge Snake| Skeleton| Goblin || Huge Squirrel|
        EnemyGroup = 'E';
        rngdice = (rand() % 4);
        if (rngdice == 0) {
            skeleton.ENEMYENC();
        }
        else if (rngdice == 1) {
            snake.ENEMYENC();
        }
        else if (rngdice == 2 || rngdice == 3) {
            goblin.ENEMYENC();
        }
        else if (rngdice == 4) {
            sqrl.ENEMYENC();
        }
        else {
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }

        TREASUREROLL(1850);

        FELLOWADVENTURE((rand() % 1) + 1);

        //Encounter F Minos| Onion| D-Skeleton|
        EnemyGroup = 'F';
        rngdice = (rand() % 2);
        if (rngdice == 0) {
            minos.ENEMYENC();
        }
        else if (rngdice == 1) {
            onionsmall.ENEMYENC();
        }
        else if (rngdice == 2) {
            sqrl.ENEMYENC();
        }
        else {
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }

        TREASUREROLL(3250);
        FELLOWADVENTURE(2);


        //Encounter G Huge Snake| Goblin| Huge Squirrel|
        EnemyGroup = 'G';
        rngdice = (rand() % 2);
        if (rngdice == 0) {
            snake.ENEMYENC();
        }
        else if (rngdice == 1) {
            goblin.ENEMYENC();
        }
        else if (rngdice == 2) {
            sqrl.ENEMYENC();
        }
        else {
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }
        TREASUREROLL(2460);

        //Encounter H Skeleton| Onion| Goblin|
        EnemyGroup = 'H';
        rngdice = (rand() % 2);
        if (rngdice == 0) {
            skeleton.ENEMYENC();
        }
        else if (rngdice == 1) {
            onionsmall.ENEMYENC();
        }
        else if (rngdice == 2) {
            goblin.ENEMYENC();
        }
        else {
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }



        //The following is a dev test line so I can balance the score
        //std::cout << std::endl << std::endl << "The current score is " << score << std::endl << std::endl;


        TREASUREROLL(5000);



        std::cout << std::endl << "As you make your way down the dungeon you here what sounds like nails on a chalkboard." << std::endl;
        wait(1000);
        std::cout << "Then suddenly a mass leaps out of the shadow and bites you!";
        HPDISPLAY(3);
        std::cout << "Ouch! Your eyes adjust and you can see...";
        //Encounter I Huge Lizard
        EnemyGroup = 'I';
        lizard.ENEMYENC();

        FELLOWADVENTURE(1);

        //Encounter J King Onion
        EnemyGroup = 'J';
        onionlarge.ENEMYENC();

        TREASUREROLL(6000);

        //Encounter K Huge Squirrel| Minos| Huge Lizard|
        EnemyGroup = 'K';
        rngdice = (rand() % 2);
        if (rngdice == 0) {
            sqrl.ENEMYENC();
        }
        else if (rngdice == 1) {
            minos.ENEMYENC();
        }
        else if (rngdice == 2) {
            lizard.ENEMYENC();
        }
        else {
            std::cerr << "A fatal error has occoured!" << std::endl;
            std::cerr << "Program ended up not having a allocated monster to the monster roll!!!" << std::endl;
            std::cerr << "Program is in Enemy Group" << EnemyGroup << std::endl;
            return 0;
        }




        std::cout << std::endl << "As you make your way down the dungeon you here loud thumps!\nYou must be approaching something strong!" << std::endl;

        //Encounter L Armored Goblin
        EnemyGroup = 'L';
        goblinlarge.ENEMYENC();
        std::cout << "It appears that the Armored Goblin was nearby something interesting!" << std::endl;
        TREASUREROLL(500);


        FELLOWADVENTURE((rand() % 1) + 1);

        TREASUREROLL(6000);


        std::cout << "You can hear distant thumping as you go deeper into the dungeon..." << std::endl;

        TREASUREROLL(7777);

        wait(1000);

        //Final Boss

        std::cout << "You continue your way through the dungeon, but that thumping grows louder and louder..." << std::endl;
        std::cout << "You turn a corner and can see a huge door, is this the end of the dungeon?" << std::endl;

        wait(1000);
        std::cout << "Then from a hallway ahead of you a Goblin unlike any you've seen before rounds the corner!" << std::endl;

        wait(1000);
        std::cout << "They have a crown atop thier head and a hammer encrusted in gold." << std::endl;
        std::cout << "Yet, in contrast to this regality the equipment tells stories of countless battles, and countless victories..." << std::endl;
        std::cout << "You wonder to yourself, will you be just one more etch on that crown?" << std::endl;

        wait(1000);
        std::cout << "You snap yourself out of it! Because ultimately..." << std::endl;

        goblinking.ENEMYENC();

        if (didrun == true) {

            wait(1000);
            std::cout << "\nBut theres nowhere to go but backwards..." << std::endl;
            std::cout << "As you run away you swear you can hear that Goblin just behind you." << std::endl;
            std::cout << "But you just keep running." << std::endl;
            std::cout << "Cause in the end, you didn't lose. You're alive." << std::endl;

            wait(1000);

            std::cout << std::endl << "GAME CLEAR" << std::endl;
            playattempts++;
            scorescreen(playattempts);
        }
        else {
            //if you didn't run
            std::cout << std::endl << "You approach the huge door that the Goblin stood in the way of." << std::endl;
            std::cout << "You attempt to push at it, but it show no signs of budging." << std::endl;

            wait(1000);
            std::cout << "You investagate around a little bit, and eventully find a keyhole.";
            if (dungeonkey) {
                std::cout << "It looks like that ominous key you got could fit in here...";
                InvalidNumInput = true;
                while (InvalidNumInput) {
                    std::cout << "\nUse key to unlock door?\n1) Use key\n2) Go back";
                    std::cout << "\nWhat will you do: ";
                    std::cin >> yesno;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(1024, '\n');
                        std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
                    }
                    else if (yesno == 2) {
                        InvalidNumInput = false;
                        //ending opp 1
                        std::cout << "...";

                        wait(1000);
                        std::cout << "It's a choice lacking in dramtic flare.";
                        std::cout << "But in the end, you triumphed where many adventures had failed. You're alive." << std::endl;

                        wait(1000);;
                        std::cout << std::endl << "GAME CLEAR" << std::endl;
                        playattempts++;
                        scorescreen(playattempts);

                    }
                    else if (yesno != 2 && yesno != 1) {
                        std::cin.clear();
                        std::cin.ignore(1024, '\n');
                        std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
                    }
                    else {
                        //continue
                        InvalidNumInput = false;

                        std::cout << std::endl << "The key pushes in perfectally." << std::endl;
                        std::cout << "As you finish turning it you can hear the distinct sound of machinery." << std::endl;
                        std::cout << "The door, on its own, begins to open. Pulling the doors inward." << std::endl;

                        wait(1000);
                        std::cout << "You wonder if the sound of the stone scraping across the ground will attract monsters..." << std::endl;

                        wait(1000);
                        std::cout << "Your snapped out of your train of thought by the THUD of the doors finishing opening." << std::endl;
                        std::cout << "So, what will you do? " << std::endl;
                        InvalidNumInput = true;
                        while (InvalidNumInput) {
                            std::cout << "\n1) Continue onward\n2) Go back";
                            std::cout << "\nWhat will you do: ";
                            std::cin >> yesno;
                            if (std::cin.fail()) {
                                std::cin.clear();
                                std::cin.ignore(1024, '\n');
                                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
                            }
                            else if (yesno == 2) {
                                InvalidNumInput = false;
                                //ending opp 1
                                std::cout << "...";

                                wait(1000);
                                std::cout << "It's a choice leaving a huge mystery unsolved.";
                                std::cout << "But in the end, you triumphed where many adventures had failed. You're alive." << std::endl;

                                wait(500);
                                std::cout << std::endl << "GAME CLEAR" << std::endl;
                                playattempts++;
                                scorescreen(playattempts);

                            }
                            else if (yesno != 2 && yesno != 1) {
                                std::cin.clear();
                                std::cin.ignore(1024, '\n');
                                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
                            }
                            else {
                                //continue
                                std::cout << std::endl << "You take a step foward. The air here is cold..." << std::endl;
                                std::cout << "Far colder then the air in the previous part of the dungeon..." << std::endl;
                                std::cout << "Not to mention the noises, or rather lack of them." << std::endl;
                                std::cout << "It's a maddening silence, one you haven't been used too for hours." << std::endl;

                                wait(1000);

                                std::cout << "Suddenly, you've been walking for feels like hours..." << std::endl;
                                std::cout << "You realize you can barely see your own hands in front of you." << std::endl;
                                std::cout << "It's gotten dark... Very dark..." << std::endl;

                                wait(1000);


                                std::cout << "So, what will you do? " << std::endl;
                                InvalidNumInput = true;
                                while (InvalidNumInput) {
                                    std::cout << "\n1) Continue onward\n2) Go back";
                                    std::cout << "\nWhat will you do: ";
                                    std::cin >> yesno;
                                    if (std::cin.fail()) {
                                        std::cin.clear();
                                        std::cin.ignore(1024, '\n');
                                        std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
                                    }
                                    else if (yesno == 2) {
                                        InvalidNumInput = false;
                                        //ending opp 1
                                        std::cout << "...";

                                        wait(1000);
                                        std::cout << "You don't even know which way is back anymore to be honest...";
                                        std::cout << "But you confidently turn your 180 degrees on your feet and just book it in the other direction...";

                                        wait(1000);
                                        std::cout << "Then sometime later, its starts to get a little brighter.";
                                        std::cout << "But in the end, you triumphed where many adventures had failed. You're alive." << std::endl;

                                        wait(500);
                                        std::cout << std::endl << "GAME CLEAR" << std::endl;
                                        playattempts++;
                                        scorescreen(playattempts);

                                    }
                                    else if (yesno != 2 && yesno != 1) {
                                        std::cin.clear();
                                        std::cin.ignore(1024, '\n');
                                        std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
                                    }
                                    else {
                                        //continue
                                        std::cout << std::endl << "You take another step foward. Then another" << std::endl;
                                        for (rngdice = ((rand() % 5) + 5); rngdice > 0; rngdice--) {
                                            std::cout << "Then another" << std::endl;

                                            wait(500);
                                        }
                                        std::cout << "\n\nThen darkness is broken by a brief brilliant flash of light." << std::endl;
                                        std::cout << "You had seen it before, it was the unforgettable terror of a Dragons' fire." << std::endl;

                                        wait(1000);
                                        std::cout << "This hallway had been a pathway inbetween the dungeon and this Dragon here." << std::endl;

                                        wait(1000);
                                        std::cout << "What do you do? Perhaps it hadn't noticed you yet...";


                                        InvalidNumInput = true;
                                        while (InvalidNumInput) {
                                            std::cout << "\n1) Continue onward\n2) Attempt to escape";
                                            std::cout << "\nWhat will you do: ";
                                            std::cin >> yesno;
                                            if (std::cin.fail()) {
                                                std::cin.clear();
                                                std::cin.ignore(1024, '\n');
                                                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
                                            }
                                            else if (yesno == 2) {
                                                InvalidNumInput = false;
                                                //ending opp 1
                                                std::cout << "...";

                                                wait(1000);
                                                //wait command for 1/2 second

                                                wait(1000);
                                                std::cout << "It probably hadn't seen you yet...";
                                                std::cout << "You turn around as quietly as possible, and start to go back the way you came.";

                                                wait(1000);
                                                //wait command for 1/2 second

                                                wait(1000);
                                                std::cout << "But moments later that brilliant terror creeps up from behind.";
                                                std::cout << "You are engulfed in a fury of fire.";

                                                //death
                                                occult_doll = 0;
                                                HPDISPLAY(9999999);

                                            }
                                            else if (yesno != 2 && yesno != 1) {
                                                std::cin.clear();
                                                std::cin.ignore(1024, '\n');
                                                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
                                            }
                                            else {
                                                std::cout << "\nYou steel your nerves. Then move your right foot foward." << std::endl;

                                                wait(500);
                                                std::cout << "But before you can move your left the room is set alight by the Dragons' fire." << std::endl;
                                                std::cout << "You where able to dodge that attack!" << std::endl;
                                                //wait command for 1/2 second
                                                wait(500);
                                                std::cout << "But..." << std::endl;
                                                //True Final Boss
                                                dragonred.ENEMYENC();
                                                if (didrun == true) {
                                                    wait(500);
                                                    std::cout << "\nBut you haven't a clue where to go next..." << std::endl;
                                                    std::cout << "You feel confident you've gotten away..." << std::endl;
                                                    wait(500);
                                                    std::cout << "But your eyes must be playing tricks on you..." << std::endl;
                                                    std::cout << "Cause it looks like right in your path is the Red Dragon!" << std::endl;
                                                    std::cout << "And it looks pissed!" << std::endl;
                                                    std::cout << "An insurmountable amount of fire comes blasting towards you." << std::endl;
                                                    wait(500);
                                                    //death
                                                    occult_doll = 0;
                                                    HPDISPLAY(phpmax + 1000);
                                                }
                                                else {
                                                    //if you didn't run

                                                    std::cout << "The Dragon lets out one last lick of fire from its mouth before falling to the ground..." << std::endl;
                                                    std::cout << "The room is once again devoid of light." << std::endl;
                                                    wait(500);
                                                    std::cout << "But that doesn't matter to you does it?" << std::endl;
                                                    std::cout << "You came here to complete a goal..." << std::endl;
                                                    std::cout << "After all, monsters should know by now, they can lick thier lips, sharpen thier claws and show off thier fire..." << std::endl;
                                                    wait(500);
                                                    std::cout << "But the human spirit is indomitable." << std::endl;
                                                    std::cout << "It can only be supressed, never extingushed." << std::endl;
                                                    std::cout << "Inevitably the embers will coalesce and burn away the darkness." << std::endl;
                                                    wait(1000);
                                                    std::cout << std::endl << "TRUE GAME CLEAR" << std::endl;
                                                    wait(500);
                                                    dragonheart = true;
                                                    playattempts++;
                                                    scorescreen(playattempts);
                                                }
                                            }

                                        }

                                    }
                                }

                            }
                        }
                    }
                }



            }
            else {
                std::cout << "Alas, you have no key to any doors in this dungeon..." << std::endl;
                std::cout << "I guess that means an end to this journey, and a start to a new one." << std::endl;
                wait(500);
                std::cout << "As for now, you make your way backwards, out of the dungeon." << std::endl;
                wait(1000);
                std::cout << std::endl << "GAME CLEAR" << std::endl;
                playattempts++;
                scorescreen(playattempts);
            }

        }

    }

    std::cout << "This is some sort of error!!! Please report!";
    std::cout << "Error Code: reached end of gameloop";
    return 0;
}


// Everything past here are functions i made myself. I can explain how they work and they work.
    // The reason I'm using them is so that I don't have hundreds of nested if/else statements for every possibility, but instead a schematic made by me that I can call
    // This also cuts down on code bloat!
        // ALSO I personally think it makes it easier to understand what each procedure is doing. It does make the program less linear to read though, which is a trade off.

//treasure rolls are made my comparing the score to treasure chance, and if the score is greater then or equal to it the roll succeds.
//The treasure chance is a number I spescify that I then randomly generate in the spesifeid range (not including 0).
//if you miss the roll the treasure chest is empty
void TREASUREROLL(int treasurechance) {
    rngdice = ((rand() % treasurechance) + 1);
    if (score >= rngdice) {
        //successful treasure roll
        //You can choose not to open the chest as there is a small chance of getting ambushed by a mimic and dying.
        std::cout << "Woah! You found came across a treasure chest!!!\nDo you want to open it?";
        InvalidNumInput = true;
        while (InvalidNumInput == true) {
            std::cout << "\n1) Open Chest\n2) Leave it alone";
            std::cout << "\nWhat do you do: ";
            std::cin >> yesno;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1024, '\n');
                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
            }
            else if (yesno == 2) {
                std::cout << "\n\nYou chose not to open the chest...\nPerhaps a correct choice...";
                InvalidNumInput = false;
            }
            else if (yesno != 1) {
                //if num isnt a 1 or 2
                std::cin.clear();
                std::cin.ignore(1024, '\n');
                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
            }
            else {
                InvalidNumInput = false;
                std::cout << "\nYou approach the chest and begin to search inside..." << std::endl;
                //what treasure will it be!!!
                //basically rolling a d20
                rngdice = ((rand() % 19) + 1);

                wait(500);

                //GOLD CHANCE 50%
                if (rngdice <= 10) {
                    //this is the gold treasure chest branch the most common one
                    //this is determine which of the three tiers of gold sums you will get, and how much score you get
                    rngdice = ((rand() % 5) + 1);
                    if (rngdice <= 3) {
                        score = score + ((rand() % 100) + 1) + 200;
                        std::cout << "Inside it was a small amount of gold!" << std::endl;
                    }
                    else if (rngdice <= 5) {
                        score = score + ((rand() % 299) + 1) + 700;
                        std::cout << "Inside it was a decent amount of gold!!" << std::endl;
                    }
                    else if (rngdice == 6) {
                        score = score + ((rand() % 299) + 1) + 1700;
                        std::cout << "Inside it was a huge amount of gold!!!" << std::endl;
                    }

                }
                //MAGIC SWORD 15%
                else if (rngdice <= 13) {
                    //checks if it was already obtained.
                    if (!(have_magic_sword)) {
                        have_magic_sword = true;
                        score = score + ((rand() % 100) + 1) + 200;
                        std::cout << "Inside it was a Magic Sword!!!" << std::endl;
                        std::cout << "You will probablly be able to use this to vanquish monsters more easily..." << std::endl;
                    }
                    else {
                        //if you already have the sword you go here
                        score = score + ((rand() % 299) + 1) + 700;
                        std::cout << "Inside it was a decent amount of gold!!" << std::endl;
                    }
                }
                //OCCULT DOLL 15%
                else if (rngdice <= 16) {
                    occult_doll = occult_doll + 1;
                    std::cout << "Inside it was a strange doll..." << std::endl;
                    std::cout << "You wonder if it will be of any use..." << std::endl;
                }
                //MIMIC 5%
                else if (rngdice == 17) {
                    std::cout << "As you are searching inside the chest you suddenly feel something sharp begin to press into you..." << std::endl;
                    std::cout << "CHOMP!!!!" << std::endl;
                    wait(1000);
                    std::cout << "The Chest must have been a Mimic..." << std::endl;
                    //DEATH
                    HPDISPLAY(phpmax);

                }
                //health potion 10%
                else if (rngdice == 18 || rngdice == 19) {
                    std::cout << "Inside it was a Health Potion!!!" << std::endl;
                    std::cout << "Your health has been restored!" << std::endl;
                    PLAYERHP = (PLAYERHP + (.5 * phpmax));
                    //makes sure players hp doesn't excceed max.
                    if (PLAYERHP >= phpmax) {
                        PLAYERHP = phpmax;
                    }
                }
                //ANCIENT BOWL 5%
                else if (rngdice == 20) {
                    //checks if it was already obtained.
                    if (!(have_ancient_bowl)) {
                        have_ancient_bowl = true;
                        std::cout << "Inside it was a Ancient Bowl!!!" << std::endl;
                        std::cout << "You wonder what you can do with it..." << std::endl;
                    }
                    else {
                        //if you already have the bowl you go here and force encounter with a mimic :)
                        std::cout << "As you are searching inside the chest you suddenly feel something sharp begin to press into you..." << std::endl;
                        std::cout << "CHOMP!!!!" << std::endl;
                        wait(1000);
                        std::cout << "The Chest must have been a Mimic..." << std::endl;
                        //DEATH
                        HPDISPLAY(phpmax);
                    }
                }
                else {
                    //this is a failsafe branch for error checking
                    std::cerr << "FATAL ERROR: Ended up in undefined Chest Lootable";
                    exit(0);
                }


            }
        }

    }
    else {
        std::cout << "Woah! You found came across a treasure chest!!!\nDo you want to open it?";
        InvalidNumInput = true;

        while (InvalidNumInput == true) {
            std::cout << "\n1) Open Chest\n2) Leave it alone";
            std::cout << "\nWhat do you do: ";
            std::cin >> yesno;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1024, '\n');
                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
            }
            else if (yesno == 2) {
                std::cout << "\n\nYou chose not to open the chest...\nPerhaps a correct choice...";
                InvalidNumInput = false;
            }
            else if (yesno != 1) {
                //if num isnt a 1 or 2
                std::cin.clear();
                std::cin.ignore(1024, '\n');
                std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2...";
            }
            else {
                InvalidNumInput = false;
                std::cout << "\nYou approach the chest and begin to search inside..." << std::endl;
                std::cout << "Inside it was..." << std::endl;

                wait(500);
                std::cout << "nothing..." << std::endl;

                wait(500);
                std::cout << "Someone or something else must of have gotten to this treasure first." << std::endl;
            }

        }
    }
    wait(500);
    transtxt(2);
    wait(1000);
    //end of treasure command
    std::cout << std::endl;
}

//There is two types of adventuered, those that give you free things, usually minor stuff. And those that give you better things, if you have what they want, and if you don't have it
    //they'll give you a hint on how you could get it.
            //these could also be other functions that are called by this function for efficeny
void FELLOWADVENTURE(int adventype) {
    std::cout << std::endl << "As you make your way through the dungeon you see a feint light ahead!" << std::endl;

    wait(500);

    //randomized encounter text for variability
    rngdice = (rand() % 1);
    if (rngdice == 0) {
        std::cout << "Its just around the corner, about five feet away." << std::endl;
        wait(500);
        std::cout << "You won't have a chance to get away! Guess you'll have to brace yourself!" << std::endl;
        wait(2500);
        std::cout << "You brace yourself for the worst to come round the corner..." << std::endl;
        wait(500);
        std::cout << "And then, from round the corner, a fellow adventurer emerges." << std::endl << std::endl;
        wait(1000);
    }
    else if (rngdice == 1) {
        std::cout << "It's getting closer! Something is running up the staircase ahead of you!" << std::endl;
        wait(500);
        std::cout << "You won't have a chance to get away! Guess you'll have to brace yourself!" << std::endl;
        wait(2500);
        std::cout << "You brace yourself for the worst to come up from the darkness..." << std::endl;
        wait(500);
        std::cout << "And then, from the depths of the dungeon, a fellow adventurer emerges." << std::endl << std::endl;
        wait(1000);
    }
    if (adventype == 1) {
        rngdice = (rand() % 3) + 1;
        if (rngdice == 1) {
            //give you occult doll
            std::cout << "This adventurer, ragged and tired, locks eyes with you..." << std::endl;
            std::cout << "They then speak, somewhat meekly. ''Hey stranger? Could I ask you a strange favor?''" << std::endl;
            InvalidNumInput = true;
            while (InvalidNumInput) {
                std::cout << "Accept strangers favor?\n1) Accept\n2) Deny";
                std::cout << "\nWhat will you do: ";
                std::cin >> yesno;
                //this first branch of the if/else statement is for valididating user inputs, it checks for a non int input
                if (std::cin.fail()) {
                    //the cin.clear clears the cin buffer, basically its what makes it so the program allows for cin >> gamestart to occour again.
                    std::cin.clear();
                    std::cin.ignore(1024, '\n');
                    std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                }
                else if (yesno == 2) {
                    InvalidNumInput = false;
                    std::cout << std::endl << "You tell the fellow adventurer that you can't do any favors right now." << std::endl;
                    wait(500);
                    std::cout << "The adventurer seems saddened, then says ''Oh well... Guess on I'll be on my way then...''" << std::endl;
                    wait(500);
                    std::cout << "They then start walking the way you came, they must be looking to leave the dungeon." << std::endl;


                }
                else if (yesno != 2 && yesno != 1) {
                    std::cin.clear();
                    std::cin.ignore(1024, '\n');
                    std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                }
                else {
                    //for when a 1 is inputted
                    InvalidNumInput = false;
                    std::cout << std::endl << "The adenturers mood seems to lighten considerably." << std::endl;
                    wait(1000);
                    std::cout << "The adventurer then says ''Thank you so much, I found this strange doll deeper in, and its giving me bad vibes''" << std::endl;
                    wait(500);
                    std::cout << "The adventurer then puts the strange doll in your hands and trots off, back the way you came, they must be looking to leave." << std::endl;
                    wait(500);
                    std::cout << "You wonder if the strange doll will be of any use..." << std::endl;
                    occult_doll++;

                }
            }

        }
        else if (rngdice == 2) {
            //spare meat
            std::cout << "They seem well seasoned, they walk with confidence towards you with an outstretched hand." << std::endl;
            wait(500);
            std::cout << "The adventurer then says ''Howdy! You're pretty deep in here for a new face!''" << std::endl;
            wait(500);
            std::cout << "They outstrech thier hand and pass you something." << std::endl;
            sparemeat++;
            wait(500);
            std::cout << "''This heres some spare meat I had on me''" << std::endl;
            wait(500);
            std::cout << "They continue on, ''Considering this a welcoming gift!''" << std::endl;
            wait(500);
            std::cout << "The adventurer then waves goodbye and continues onward in the direction they where going." << std::endl;

        }
        else if (rngdice == 3) {
            //spices
            std::cout << "They beckon towards you." << std::endl;
            wait(1500);
            std::cout << "The adventurer then says ''Heyyyy! I think I collected, way too many of this rare spice here!''" << std::endl;
            std::cout << "''Do you want some?''" << std::endl;
            InvalidNumInput = true;
            while (InvalidNumInput) {
                std::cout << "Accept strangers favor?\n1) Accept\n2) Deny";
                std::cout << "\nWhat will you do: ";
                std::cin >> yesno;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(1024, '\n');
                    std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                }
                else if (yesno == 2) {
                    InvalidNumInput = false;
                    std::cout << std::endl << "You tell the fellow adventurer that you can't do any favors right now." << std::endl;
                    wait(500);
                    std::cout << "The adventurer seems saddened, then says ''Oh well... Guess on I'll be on my way then...''" << std::endl;
                    wait(500);
                    std::cout << "They then start walking the way you came, they must be looking to leave the dungeon." << std::endl;

                }
                else if (yesno != 2 && yesno != 1) {
                    std::cin.clear();
                    std::cin.ignore(1024, '\n');
                    std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                }
                else {
                    InvalidNumInput = false;
                    std::cout << std::endl << "The adenturers mood seems to lighten considerably." << std::endl;
                    wait(500);
                    std::cout << "The adventurer then says ''Thank you so much''" << std::endl;
                    wait(500);
                    std::cout << "The adventurer hands you a bag of rare spices." << std::endl;
                    rarespice++;

                }
            }
        }
        else if (rngdice == 4) {
            //hp restore
                    //has a fail chance
            std::cout << "They are running and seem panicked." << std::endl;
            wait(500);
            std::cout << "They run past you at a high speed, not even awknowleging your existence." << std::endl;
            wait(500);
            std::cout << "However, as they passed by you it seems like they dropped something." << std::endl;
            std::cout << "It appears to be a small potion. It didn't break open, so you could drink it." << std::endl;
            wait(500);
            InvalidNumInput = true;
            while (InvalidNumInput) {
                std::cout << "Drink the unknown potion?\n1) Drink\n2) Don't drink";
                std::cout << "\nWhat will you do: ";
                std::cin >> yesno;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(1024, '\n');
                    std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                }
                else if (yesno == 2) {
                    InvalidNumInput = false;
                    std::cout << std::endl << "You decide against drinking the potion." << std::endl;
                    wait(500);
                    std::cout << "Perhaps a good idea..." << std::endl;
                    wait(500);
                }
                else if (yesno != 2 && yesno != 1) {
                    std::cin.clear();
                    std::cin.ignore(1024, '\n');
                    std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                }
                else {
                    InvalidNumInput = false;
                    std::cout << "The decide to drink the potion..." << std::endl;
                    wait((rand() % 1000) + 1000);
                    rngdice = (rand() % 5);
                    if (rngdice == 0) {
                        //damage
                        std::cout << "Perhaps a huge mistake...\nYou can feel your stomach churn and your visions blurs" << std::endl;
                        HPDISPLAY((rand() % 6) + 1);

                    }
                    else if (rngdice < 5) {
                        std::cout << "It had a very nice cherry taste to it..." << std::endl;
                        wait(500);
                        std::cout << "You feel healthier." << std::endl;
                        PLAYERHP = (PLAYERHP + 20);
                        //makes sure players hp doesn't excceed max.
                        if (PLAYERHP >= phpmax) {
                            PLAYERHP = phpmax;
                        }

                    }
                    else {
                        std::cout << "It tasted kind of like lemonade." << std::endl;
                        wait(500);
                        std::cout << "You don't feel any different." << std::endl;

                    }


                }
            }

        }
    }
    else {
        rngdice = (rand() % 3) + 1;
        if (rngdice == 1 && dungeonkey == false) {
            //dungeon key
            std::cout << "This adventurer, smiles and locks eyes with you..." << std::endl;
            wait(500);
            std::cout << "They then speak ''Hey stranger? How about a deal?''" << std::endl;
            std::cout << "''This rare dungeon key I found, for 1 onion?''" << std::endl;
            InvalidNumInput = true;
            while (InvalidNumInput) {
                std::cout << "Accept deal?\n1) Accept\n2) Deny";
                std::cout << "\nWhat will you do: ";
                std::cin >> yesno;
                //this first branch of the if/else statement is for valididating user inputs, it checks for a non int input
                if (std::cin.fail()) {
                    //the cin.clear clears the cin buffer, basically its what makes it so the program allows for cin >> gamestart to occour again.
                    std::cin.clear();
                    std::cin.ignore(1024, '\n');
                    std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                }
                else if (yesno == 2) {
                    InvalidNumInput = false;
                    std::cout << std::endl << "You tell the fellow adventurer that you can't part with any onions..." << std::endl;
                    wait(500);
                    std::cout << "The adventurer seems saddened, then says ''Oh well... back to searching for one...''" << std::endl;
                    wait(500);
                    std::cout << "''Those little walking Onions can be hard to find.''\n''I'd understand why someone wouldn't want to part with one''" << std::endl;
                    std::cout << "They then walk away." << std::endl;

                }
                else if (yesno != 2 && yesno != 1) {
                    std::cin.clear();
                    std::cin.ignore(1024, '\n');
                    std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                }
                else {
                    //for when a 1 is inputted
                    InvalidNumInput = false;
                    std::cout << std::endl << "The adenturers mood seems to lighten considerably." << std::endl;
                    wait(500);
                    std::cout << "''Perfect! I've been looking for one all day!''" << std::endl;
                    wait(1500);
                    if (onions > 0) {
                        std::cout << "You hand them an onion.\nThey take it and place an ominous looking key in your hand." << std::endl;
                        wait(500);
                        std::cout << "''Thanks again! These just taste better then the ones that grow closer to the surface!''" << std::endl;
                        onions--;
                        dungeonkey = true;
                        wait(500);
                        std::cout << "You wonder if this key will be of any use..." << std::endl;

                    }
                    else {
                        std::cout << "You start searching your belongings as seconds turn into minutes." << std::endl;
                        wait(500);
                        std::cout << "The adventuer then awkwardly asks if you even had one?" << std::endl;
                        std::cout << "You thought you had one, but you didn't." << std::endl;
                        wait(500);
                        std::cout << "The adventuer seems mildly annoyed then says..." << std::endl;
                        std::cout << "''Oh well... back to searching for one...''" << std::endl;
                        std::cout << "''Those little walking Onions can be hard to find.''" << std::endl;
                        wait(500);
                        std::cout << "They then walk away." << std::endl;

                    }



                }
            }
        }
        else if (rngdice <= 2 || have_magic_sword == false) {
            //bowl
            std::cout << "This adventurer has a huge backpack.\nIts so heavy looking your surprised they can carry it." << std::endl;
            wait(500);
            std::cout << "They then speak ''Hello. By chance you haven't happened to find any of those relic bowls around here?''" << std::endl;
            wait(500);
            if (have_ancient_bowl) {
                std::cout << "You look at them, and start to shake your head no, but then remember the strange bowl you collected." << std::endl;
                wait(500);
                std::cout << "You take it out and show it to the adventurer." << std::endl;
                std::cout << "''I am gobsmacked! That's the exact type of relic I'm looking for!''" << std::endl;
                wait(500);
                std::cout << "''Tell you what good fella, I'll give you this Magic Sword I found.''" << std::endl;
                wait(500);
                std::cout << "''If you trade me that exquisite bowl you have.''" << std::endl;
                InvalidNumInput = true;
                while (InvalidNumInput) {
                    std::cout << "Accept deal?\n1) Accept\n2) Deny";
                    std::cout << "\nWhat will you do: ";
                    std::cin >> yesno;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(1024, '\n');
                        std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                    }
                    else if (yesno == 2) {
                        InvalidNumInput = false;
                        std::cout << std::endl << "You tell the fellow adventurer that you wonn't part with the bowl..." << std::endl;
                        wait(500);
                        std::cout << "The adventurer seems deeply saddened, then says ''Oh well... just as I thought my years long quest would be over...''" << std::endl;
                        std::cout << "''If I come across you again, don't be afraid to change your mind...''" << std::endl;
                        wait(500);
                        std::cout << "They then walk away." << std::endl;


                    }
                    else if (yesno != 2 && yesno != 1) {
                        std::cin.clear();
                        std::cin.ignore(1024, '\n');
                        std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                    }
                    else {
                        //for when a 1 is inputted
                        InvalidNumInput = false;
                        std::cout << std::endl << "The adenturers falls to thier knees." << std::endl;
                        wait(2500);
                        std::cout << "''Oh! It's over...''" << std::endl;
                        wait(500);
                        std::cout << "You hand them the strange bowl.\nThey take it and hand you a sword that you can tell has magic properties." << std::endl;
                        wait(500);
                        std::cout << "''Thank you again chap! This here bowl completes my set...''" << std::endl;
                        wait(500);
                        have_ancient_bowl = false;
                        have_magic_sword = true;
                        std::cout << "You know that this sword will be of use!" << std::endl;

                    }
                }

            }
            else {
                std::cout << "You look at them, and shake your head no." << std::endl;
                wait(500);
                std::cout << "''Curses! Back to scavanging every treasure trove in this whole bloody dungeon it is for me then!''" << std::endl;
                wait(500);
                std::cout << "''But those mimics can be very convincing... I'll have to be on gaurd...''" << std::endl;
                wait(500);
                std::cout << "The adventurer goes on thier way." << std::endl;

            }
        }
        else if (rngdice <= 3 || have_ancient_bowl == false) {
            //money for bowl
            std::cout << "They seems young, they're a simmilar age to you." << std::endl;
            wait(500);
            std::cout << "They then speak ''Hi. You wouldn't happen to want this relic I found would ya?''" << std::endl;
            wait(1000);
            std::cout << "They continue ''I'm really just looking for gold, if you have 2500G its yours!''" << std::endl;
            if (score >= 2500) {
                std::cout << "You check your pockets. You have the money." << std::endl;
                InvalidNumInput = true;
                while (InvalidNumInput) {
                    std::cout << "Accept deal?\n1) Accept\n2) Deny";
                    std::cout << "\nWhat will you do: ";
                    std::cin >> yesno;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(1024, '\n');
                        std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                    }
                    else if (yesno == 2) {
                        InvalidNumInput = false;
                        std::cout << std::endl << "You tell the fellow adventurer that you don't feel like you like you want the bowl..." << std::endl;
                        std::cout << "The adventurer seems deeply saddened...''" << std::endl;
                        wait(500);
                        std::cout << "They then walk away." << std::endl;

                    }
                    else if (yesno != 2 && yesno != 1) {
                        std::cin.clear();
                        std::cin.ignore(1024, '\n');
                        std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                    }
                    else {
                        //for when a 1 is inputted
                        InvalidNumInput = false;
                        std::cout << std::endl << "The adenturers seems happy." << std::endl;
                        wait(500);
                        std::cout << "''Perfect!'' They say." << std::endl;
                        wait(500);
                        std::cout << "You hand them 2500G and they hand you a strange bowl." << std::endl;
                        std::cout << "''Thanks friend!''" << std::endl;
                        wait(500);
                        std::cout << "The andventurer then strolls away." << std::endl;
                        have_ancient_bowl = true;
                        score = score - 2500;
                        std::cout << "You wonder if this bowl will be of any use!" << std::endl;

                    }
                }
            }
            else {
                std::cout << "You check your pockets. You don't have the money." << std::endl;
                wait(500);
                std::cout << "''Oh well! Well guess I'm off, nice meeting you!''" << std::endl;
                wait(500);
                std::cout << "The adventurer goes on thier way." << std::endl;

            }

        }
        else {
            //eggs 4 potion
            std::cout << "They look right at you and approach." << std::endl;
            wait(500);
            std::cout << "They then speak ''Greetings! You don't happen to have any Eggs on you.''" << std::endl;
            wait(500);
            std::cout << "They continue ''I'm really just looking any old egg, I'll give you a big potion for an egg!''" << std::endl;
            wait(500);
            if (snakeegg > 0) {
                std::cout << "You check your pockets. You have an egg." << std::endl;
                InvalidNumInput = true;
                while (InvalidNumInput) {
                    std::cout << "Accept deal?\n1) Accept\n2) Deny";
                    std::cout << "\nWhat will you do: ";
                    std::cin >> yesno;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(1024, '\n');
                        std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                    }
                    else if (yesno == 2) {
                        InvalidNumInput = false;
                        std::cout << std::endl << "You tell the fellow adventurer that you wonn't part with an egg..." << std::endl;
                        wait(500);
                        std::cout << "''If I come across you again, don't be afraid to change your mind...''" << std::endl;
                        std::cout << "They then walk away." << std::endl;

                    }
                    else if (yesno != 2 && yesno != 1) {
                        std::cin.clear();
                        std::cin.ignore(1024, '\n');
                        std::cout << "\nYou have failed to input a valid response. Please input a 1 or a 2." << std::endl;
                    }
                    else {
                        //for when a 1 is inputted
                        InvalidNumInput = false;
                        std::cout << "''Oh! Fantastic!''" << std::endl;
                        wait(500);
                        std::cout << "You hand them them and they take an egg.\nThey, in return, hand you a potion." << std::endl;
                        std::cout << "''Thank you!'' they say," << std::endl;
                        wait(500);
                        std::cout << "The adventurer goes on thier way." << std::endl;
                        snakeegg--;
                        wait(500);
                        std::cout << "You then drink the potion" << std::endl;
                        std::cout << "Your health has been restored!" << std::endl;
                        PLAYERHP = (PLAYERHP + (.5 * phpmax));
                        //makes sure players hp doesn't excceed max.
                        if (PLAYERHP >= phpmax) {
                            PLAYERHP = phpmax;
                        }

                    }
                }
            }
            else {
                std::cout << "You check your pockets. You don't have any eggs." << std::endl;
                wait(500);
                std::cout << "''Oh well! Well guess I'm off, nice meeting you!''" << std::endl;
                wait(500);
                std::cout << "The adventurer goes on thier way." << std::endl;
            }

        }

    }
    wait(1000);
    transtxt(3);
    wait(1000);
    //end of adven command
}



//this is a function I made that checks the users HP, and if they should game over.
void HPDISPLAY(int DMG)
{
    PLAYERHP = PLAYERHP - DMG;
    std::cout << std::endl << "Damage taken!";
    std::cout << std::endl;
    //prodedure for when you have the revival item
    if (PLAYERHP <= 0 && occult_doll >= 1) {
        std::cout << std::endl << "Oh no! It seems like you are going to die!!!" << std::endl;
        wait(500);
        occult_doll--;
        std::cout << std::endl << "You black out briefly, then feel a strange warmth from one of those strange dolls you picked up earier..." << std::endl;
        std::cout << "You feel your pocket, but the doll giving off the warmth is now tattered and in pieces!" << std::endl;
        std::cout << "You seem to somehow still be alive!" << std::endl;
        //revives you with between 13 and 20 HP
        PLAYERHP = ((rand() % 7) + 1) + 12;
        wait(500);
    }
    //regular game over procedure
    else if (PLAYERHP <= 0) {
        incombat = false;
        //this is the game over procedure
        std::cout << std::endl << "Oh no! It seems you have died..." << std::endl;
        wait(500);

        //punishment to score for getting a game over via death!
        score = (score / 3);
        std::cout << "GAME OVER!" << std::endl;
        playattempts++;
        scorescreen(playattempts);
    }
    else {
        //regular damage taken procedure
        remHP = PLAYERHP;
        if (remHP % 2 == 0) {
            for (remHP = (PLAYERHP / 2); remHP > 0; remHP--) {
                std::cout << "X";
            }

        }
        else {
            for (remHP = ((PLAYERHP - 1) / 2); remHP > 0; remHP--) {
                std::cout << "X";
            }
            std::cout << "x";

        }
        wait(500);
        std::cout << std::endl;
    }
}

//This is the score/game over procedure of the game
void scorescreen(int playattempts) {
    //final score bonuses
    //if I feel like it, i'll make these a seperate void function.
    // extra hp bonus
    if (phpmax > 20) {
        bscalc = (phpmax - 20);
        for ((bscalc = bscalc / 2); bscalc > 0; bscalc--) {
            score = score + (rand() % 150) + 50;
        }
    }
    // extra doll bonus
    if (occult_doll > 0) {
        bscalc = (occult_doll);
        for (bscalc; bscalc > 0; bscalc--) {
            score = score + (rand() % 50) + 27;
        }
    }
    // remaining hp bonus
    bscalc = PLAYERHP;
    for ((bscalc = bscalc / 2); bscalc > 0; bscalc--) {
        score = score + (rand() % 17) + 5;
    }



    //what did you chef up?
    if (PLAYERHP >= 0)
    {
        std::cout << "With what you found in the dungeon you where able to cook up ";
        //extra  heart piece bonus
        if (pieceofheart > 0) {
            std::cout << "hearty ";
            score = score + 200;
        }
        if (dragonheart) {
            std::cout << "dragon-fat sautéed ";
        }
        else {
            rngdice = (rand() % 4);
            if (rngdice == 0) {
                std::cout << "steamed ";
            }
            else if (rngdice == 1) {
                std::cout << "baked ";
            }
            else if (rngdice == 2) {
                std::cout << "fried ";
            }
            else if (rngdice == 3) {
                std::cout << "stewed ";
            }
            else if (rngdice == 4) {
                std::cout << "roasted ";
            }
        }

        //meat bonus
        if (sparemeat > 0) {
            bscalc = (sparemeat);
            for (bscalc; bscalc > 0; bscalc--) {
                score = score + (rand() % 90) + 10;
            }
            std::cout << "meat";
        }
        else {
            std::cout << "mushrooms";
        }

        //onion bonus
        if (onions > 0) {
            bscalc = (onions);
            for ((bscalc = bscalc * 10); bscalc > 0; bscalc--) {
                score = score + (rand() % 7);
            }
            std::cout << " with with chopped onions";
        }
        //egg bonus
        if (snakeegg > 0)
        {
            std::cout << " with a poached egg on top";
            bscalc = (snakeegg);
            for (bscalc; bscalc > 0; bscalc--) {
                score = score + (rand() % 20) + 80;
            }
        }
        //spice bonus
        if (rarespice > 0) {
            bscalc = (rarespice);
            for (bscalc; bscalc > 0; bscalc--) {
                score = score + (rand() % 50) + 50;
            }
            std::cout << " as well as delectable ancient spices";
        }
        //end
        //bowl bonus
        if (have_ancient_bowl) {
            score = (score * 2);
            std::cout << " all in an beautiful ancient bowl";
        }
        std::cout << "!" << std::endl;
    }
    wait(500);

    std::cout << "Your final score was " << score << "!";
    wait(500);
    if (score < 5000) {
        std::cout << std::endl << "Not super impressive..." << std::endl;
    }
    else if (score < 15000) {
        std::cout << std::endl << "A pretty average score..." << std::endl;
    }
    else {
        std::cout << std::endl << "An impressive score!" << std::endl;
    }
    if (playattempts != 1) {
        if (score > StoredScore) {
            std::cout << "It was even higher then your previous High Score of ";
            std::cout << StoredScore << "!" << std::endl;
            StoredScore = score;
        }
        else if (StoredScore >= score) {
            std::cout << "A score of " << score << " isn't higher then your previous High Score of ";
            std::cout << StoredScore << "..." << std::endl;
        }
    }
    else {
        StoredScore = score;
    }
    wait(500);


    std::cout << std::endl << "You have played the game " << playattempts << " time(s). Would you like to play again?" << std::endl;
    InvalidNumInput = true;
    std::cout << "\n1) Play Again\n2) Close Game";
    while (InvalidNumInput) {
        std::cout << "\nYour answer: ";
        std::cin >> UnoDos;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2..." << std::endl;
        }
        else if (UnoDos == 1) {
            InvalidNumInput = false;
            main();
        }
        else if (UnoDos == 2) {
            InvalidNumInput = false;
            std::cout << "\n\nClosing the Game!\n";
            //ask about this
            exit (0);
        }
        else {
            std::cin.clear();
            std::cin.ignore(1024, '\n');
            std::cout << "\nYou have failed to input a valid number. Please input a 1 or a 2..." << std::endl;

        }
    }
}

//this is for wait commands
void wait(int waittime) {
    std::this_thread::sleep_for(std::chrono::milliseconds(waittime));
}

//this is for random transistionary dialouge
void transtxt(int transtype) {
    if (transtype == 1) {
        //pre-enemy encounter
        rngdice = (rand() % 7) + 1;
        switch (rngdice) {
        case 1:
            std::cout << "You make your way through the dungeon..." << std::endl;
            wait(500);
            std::cout << "Then you notice something move in the corner of your vision!" << std::endl;
            break;
        case 2:
            std::cout << "You can hear something approaching from the left!" << std::endl;
            wait(500);
            std::cout << "The creature is now in front of you!" << std::endl;
            break;
        case 3:
            std::cout << "You can hear something approaching from the right!" << std::endl;
            wait(500);
            std::cout << "The creature is now in front of you!" << std::endl;
            break;
        case 4:
            std::cout << "As you make your way through the dungeon you can't help but feel you are being watched..." << std::endl;
            wait(500);
            std::cout << "Your suspicions are confirmed when...";
            break;
        case 5:
            std::cout << "The air feels colder around you...";
            wait(500);
            std::cout << "!";
            break;
        case 6:
            std::cout << "As you walk you can swear you heard the feint echoing of screams from deeper in the dungeon...";
            wait(500);
            std::cout << "But you have no time to linger on that thought because...";
            break;
        case 7:
            std::cout << "You swear you saw something move in the corner of your eye, but perhaps it's just the dungeons' atmosphere getting to you...";
            wait(500);
            std::cout << "You are quickly proven wrong as...";
            break;
        case 8:
            std::cout << "The dungeon often feels like it goes on forever, winding every-which. You wonder if you are even heading deeper.";
            wait(500);
            std::cout << "But you have no time to linger on that thought because...";
            break;

        }

    }
    else if (transtype == 2) {
        //treasure & post enemu encounter
        rngdice = (rand() % 5) + 1;
        switch (rngdice) {
        case 1:
            std::cout << "And so you continue on through the dungeon. One step at a time..." << std::endl;
            break;
        case 2:
            std::cout << "As you continue on you think you hear something approaching from the left!" << std::endl;
            wait(500);
            std::cout << "But your hearing must be going from being down here, as there is nothing there..." << std::endl;
            wait(500);
            std::cout << "So you continue onward." << std::endl;
            break;
        case 3:
            std::cout << "And so, your journey continues..." << std::endl;
            break;
        case 4:
            std::cout << "Through the dungeon you go, corridoor after corridorr, no end in sight..." << std::endl;
            wait(500);
            std::cout << "You find yourself wondering if your journey will end down here..." << std::endl;
            break;
        case 5:
            std::cout << "Then you find yourself walking down a tight spiral staircase...";
            wait(500);
            std::cout << "It reminds you of home for some reason.";
            break;
        case 6:
            std::cout << "As you walk on you catch the scent of something foul...";
            wait(500);
            std::cout << "You haven't the chance to attempt to deduce its origin point, for as quickly as it came it disapates...";
            break;
        }
        wait(1000);
    }
    else if (transtype == 3) {
        //adventurer
        rngdice = (rand() % 5) + 1;
        switch (rngdice) {
        case 1:
            std::cout << "And so you press onward through the dungeon. One step at a time..." << std::endl;
            break;
        case 2:
            std::cout << "As you walk on you thing to yourself, It's nice to know you aren't entirely isolated from the rest of humanity in here." << std::endl;
            break;
        case 3:
            std::cout << "What interesting people occupy the dungeon..." << std::endl;
            wait(500);
            std::cout << "You think to yourself as you walk on." << std::endl;
            break;
        case 4:
            std::cout << "Now that they're gone you are once again alone..." << std::endl;
            break;
        case 5:
            std::cout << "You wonder if you'll meet that adventurer ever again..." << std::endl;
            break;
        case 6:
            std::cout << "What interesting people the dungeon brings out..." << std::endl;
            break;
        }
        wait(1000);
    }
    else if (transtype == 4) {
        //empty branch so nothing happens
    }
    else {
        //fail case
        std::cout << "\nA horrible chill goes down your spine..." << std::endl;
        std::cout << "However you got here, you shouldn't be here." << std::endl;
        std::cerr << "This message displays when the call for transtype(x) fails, please report";
        wait(500);
    }

}
