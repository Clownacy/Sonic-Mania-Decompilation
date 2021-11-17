#ifndef OBJ_RING_H
#define OBJ_RING_H

#include "SonicMania.h"

typedef enum {
    RING_TYPE_NORMAL,
    RING_TYPE_BIG,
}RingTypes;

typedef enum {
    RING_MOVE_NONE,
    RING_MOVE_NORMAL,
    RING_MOVE_CIRCLE,
    RING_MOVE_PATH,
    RING_MOVE_TRACK,
}RingMoveTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    int32 pan;
    uint16 aniFrames;
    uint16 sfxRing;
} ObjectRing;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    RingTypes type;
    PlaneFilterTypes planeFilter;
    int32 ringAmount;
    int32 timer;
    int32 maxFrameCount;
    int32 sparkleType;
    EntityPlayer *storedPlayer;
    RingMoveTypes moveType;
    Vector2 amplitude;
    int32 speed;
    Vector2 offset;
    Animator animator;
} EntityRing;

// Object Struct
extern ObjectRing *Ring;

// Standard Entity Events
void Ring_Update(void);
void Ring_LateUpdate(void);
void Ring_StaticUpdate(void);
void Ring_Draw(void);
void Ring_Create(void* data);
void Ring_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Ring_EditorDraw(void);
void Ring_EditorLoad(void);
#endif
void Ring_Serialize(void);

// Extra Entity Functions
void Ring_DebugSpawn(void);
void Ring_DebugDraw(void);
void Ring_StateDraw_Normal(void);
void Ring_StateDraw_Oscillating(void);
void Ring_StateDraw_Sparkle(void);
void Ring_State_Attract(void);
void Ring_State_Big(void);
void Ring_State_Bounce(void);
void Ring_State_Circular(void);
void Ring_State_Grow(void);
void Ring_State_Move(void);
void Ring_State_Normal(void);
void Ring_State_Path(void);
void Ring_State_Sparkle(void);
void Ring_State_Track(void);
void Ring_CheckObjectCollisions(int32 offsetX, int32 offsetY);
uint8 Ring_CheckPlatformCollisions(EntityPlatform *platform);
void Ring_Collect(void);
void Ring_FakeLoseRings(Vector2 *entity, int32 ringCount, uint8 drawOrder);

#endif //!OBJ_RING_H
