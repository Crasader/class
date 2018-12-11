//
//  Common.h
//  GameDotJungle
//
//  Created by Hoàng Thảo on 3/21/18.
//

#ifndef Common_h
#define Common_h


using namespace std;
using namespace cocos2d;

static int POINT_FOR_PLUS_TIME = 5;
static int POINT_FOR_PLUS_MOVE = 5;


static const std::string PlusForTime = "PlusForTime";
static const std::string PlusForExpand = "PlusForExpand";
static const std::string PlusForShin = "PlusForShin";
static const std::string MoneyShrin = "MoneyShrin";
static const std::string MoneyExpand = "MoneyExpand";
static const std::string MoneyTime = "MoneyTime";
static const std::string ITEM_TIME = "ItemTime";
static const std::string ITEM_SHRINKER = "ItemShrinker";
static const std::string ITEM_EXPANDER = "ItemExpander";
static const std::string FILE_NAME_ITEM = "Item.plist";

static const std::string HIGH_SCORE_FREE = "HighScoreFree";
static const std::string HIGH_SCORE_MOVES = "HighScoreMoves";
static const std::string HIGH_SCORE_TIMED = "HighScoreTimed";
static Color3B COLOR_YELLOW = Color3B(255,179,44);

static Color3B COLOR_FONT_THEMES = Color3B(255,179,44);

static const std::string MONEY = "Money";
static const std::string ID_THEME = "ID_THEME";

static const std::string FIRST_OPEN = "FirstOpenGame";


static const std::string COLOR_ARCHIE = "colorAchie";
static const std::string COLOR_ARCHIE_UNLOCK = "colorAchieUnlock";
static const std::string COLOR_MOVE = "colorBestMove";
static const std::string COLOR_TIME = "colorBestTime";
static const std::string COLOR_PANEL = "colorPanel";
static const std::string BACKGROUND_COLOR = "background3B";
static const std::string COLOR_FONT = "ColorFont";
static const std::string BACKGROUND = "background";
static const std::string LIST_ANIMAL = "listanimal";
static const std::string SQUARE = "square";

static const std::string VIBRATION = "StatusVibration";
static const std::string SOUND = "StatusSound";

///NOTI
static const std::string NOTI_ITEM_SHINKER = "Please purchase more item Shrinker";
static const std::string NOTI_ITEM_EXPANDER = "Please purchase more item Expander";
static const std::string NOTI_ITEM_TIMER = "Please purchase more item Time";

static const std::string NOTI_NOT_MONEY = "You aren't enough money.";

static const std::string NO_INTERNET = "Please check internet.";

static const std::string TRY_AGAIN = "Can not load advertising, please try again.";

static const std::string UNLOCK_ENDLESS = "unlockEndless";

//BUY ITEM
static const std::string NOTI_SHRINKER = "Shrink a animal off the board. \n Double tap a dot to quickly shrink.";
static const std::string NOTI_EXPANDER = "Remove all of one animal.";
static const std::string NOTI_TIME = "Add the time for round.";
//
//
//static const std::string NOTI_NOT_MONEY = "You aren't enough money";
//static const std::string NOTI_NOT_MONEY = "You aren't enough money";
//static const std::string NOTI_NOT_MONEY = "You aren't enough money";



static const std::string FONT_JUNGBN = "res/NewFolder/Font/JUNGBN__.TTF";

static const std::string FONT_RAVE = "res/NewFolder/Virtual Rave.ttf";
static const std::string FONT_ROBOTO_LIGHT = "res/NewFolder/Font/Roboto-Light.ttf";
static const std::string FONT_ROBOTO_MEDIUM = "res/NewFolder/Font/Roboto-Medium.ttf";

//#define ITEM_TIME "ItemTime";
//#define ITEM_SHRINKER "ItemShrinker";
//#define ITEM_EXPANDER "ItemExpander";
//#define FILE_NAME_ITEM "Item.plist";
//
//#define HIGH_SCORE_MOVES "HighScoreMoves";
//#define HIGH_SCORE_TIMED "HighScoreTimed";
//
//
//#define MONEY "Money";


#define TRANSITION_DURATION 0.2

enum LAYER_MAIN_ID
{
    LAYER_BOTTOM = 100,
    LAYER_TOP = 101,
    LAYER_HOME = 99
};


enum LAYER_ID
{
    GAMEOVER_LAYER = 200,
    MAIN_GAME_LAYER = 201,
};

enum GAME_ID
{
    GAME_FREE = 999,
    GAME_MOVES = 998,
    GAME_TIME = 997
};



#endif /* Common_h */
