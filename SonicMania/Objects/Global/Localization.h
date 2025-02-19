#ifndef OBJ_LOCALIZATION_H
#define OBJ_LOCALIZATION_H

#include "Game.h"

typedef enum {
    STR_TESTSTR,
    STR_FEATUREUNIMPLIMENTED,
    STR_RESTARTWARNING,
    STR_QUITWARNINGLOSEPROGRESS,
#if GAME_VERSION != VER_100
    STR_QUITWARNING,
#endif
    STR_DELETEPOPUP,
    STR_CONNECTING,
    STR_LOADINGFROMSERVER,
    STR_COMMERROR,
    STR_NOWIFI,
    STR_EXITCOMP,
    STR_COMPTOTAL,
    STR_TAUNLOCKED,
    STR_COMPUNLOCKED,
    STR_PEELOUTUNLOCKED,
    STR_INSTASHIELDUNLOCKED,
    STR_ANDKNUXUNLOCKED,
    STR_DEBUGMODEUNLOCKED,
    STR_BLUESPHERESUNLOCKED,
    STR_MBMUNLOCKED,
    STR_DAGARDENUNLOCKED,
    STR_CONNECTXBOX,
    STR_LOADXBOX,
    STR_CONNECTPSN,
    STR_LOADPSN,
    STR_CONNECTSTEAM,
    STR_LOADSTEAM,
    STR_CONNECTNINTENDO,
    STR_LOADNINTENDO,
    STR_PLACEHOLDERLOGIN,
    STR_SAVELOADFAIL,
    STR_CORRUPTSAVE,
    STR_NOSAVESPACE,
    STR_NOXBOXPROFILE,
    STR_RECONNECTCONTROLLER,
    STR_RECONNECTWIRELESSCONTROLLER,
    STR_SIGNOUTDETECTED,
    STR_AUTOSAVENOTIF,
#if GAME_VERSION != VER_100
    STR_KEYALREADYBOUND,
    STR_KEYALREADYBOUNDP1,
    STR_KEYALREADYBOUNDP2,
    STR_APPLYCHANGEDSETTINGS,
    STR_VIDEOCHANGESAPPLIED,
    STR_STEAMOVERLAYUNAVALIABLE,
#endif
#if MANIA_USE_PLUS
    STR_SAVEREPLAY,
    STR_NOREPLAYSPACE,
    STR_DELETEREPLAY,
    STR_ERRORLOADINGREPLAY,
    STR_CANNOTLOADREPLAY,
    STR_LOADING,
    STR_NOREPLAYS,
    STR_AREYOUSURE,
    STR_AREYOUSURESAVE,
    STR_UPTO4PLAYERS,
    STR_RESETTIMESWARNING,
    STR_MIGHTYRAYPLUS,
    STR_ENCOREREQUIRED,
    STR_RETRURNINGTOTITLE,
    STR_DEFAULT,
    STR_SELECTATRACK,
    STR_SELECTDATATOERASE,
    STR_RPC_PLAYING,
    STR_RPC_MENU,
    STR_RPC_MANIA,
    STR_RPC_ENCORE,
    STR_RPC_TA,
    STR_RPC_COMP,
    STR_RPC_TITLE,
#if MANIA_USE_EGS
    STR_CONNECTEGS,
    STR_LOADEGS,
    STR_OPENBROWSER,
    STR_ACHIEVEMENT,
    STR_NOWAYNOWAY,
    STR_FULLMEDALJACKET,
    STR_MAGNIFICENTSEVEN,
    STR_SEEYOUNEXTGAME,
    STR_SUPERSTAR,
    STR_THATSATWOFER,
    STR_NOWITCANTHURTYOU,
    STR_TRIPLETROUBLE,
    STR_FAMOUSHEDGEHOG,
    STR_WINDOWSHOPPING,
    STR_CRATEEXPECTATIONS,
    STR_KINGOFSPEED,
    STR_BOATENTHUSIAST,
    STR_THEPASSWORDIS,
    STR_SECRETSUB,
    STR_WITHOUTATRACE,
    STR_COLLECTEMALL,
    STR_PROFESSIONALHEDGEHOG,
#endif
#else
#if GAME_VERSION != VER_100
    STR_DEFAULT,
#endif
    STR_SELECTATRACK,
    STR_RPC_PLAYING,
    STR_RPC_MENU,
    STR_RPC_MANIA,
    STR_RPC_TA,
    STR_RPC_COMP,
    STR_RPC_TITLE,
#endif
    STR_STRING_COUNT,
} GameStrings;

// Object Class
struct ObjectLocalization {
    RSDK_OBJECT
    String text;
    String strings[STR_STRING_COUNT];
    bool32 loaded;
    uint8 language;
};

// Entity Class
struct EntityLocalization {
    RSDK_ENTITY
};

// Object Struct
extern ObjectLocalization *Localization;

// Standard Entity Events
void Localization_Update(void);
void Localization_LateUpdate(void);
void Localization_StaticUpdate(void);
void Localization_Draw(void);
void Localization_Create(void *data);
void Localization_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Localization_EditorDraw(void);
void Localization_EditorLoad(void);
#endif
void Localization_Serialize(void);

// Extra Entity Functions
void Localization_LoadStrings(void);
void Localization_GetString(String *string, uint8 id);
void Localization_GetZoneName(String *string, uint8 zone);
void Localization_GetZoneInitials(String *string, uint8 zone);

#endif //! OBJ_LOCALIZATION_H
