// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HangPoint Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHangPoint *HangPoint;

void HangPoint_Update(void)
{
    RSDK_THIS(HangPoint);

    if (self->length > 0) {
        bool32 flag = true;
        if (self->activePlayers == 2) {
            EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
            if (player2->sidekick)
                flag = false;
        }

        for (int32 i = 0; i < 4; ++i) {
            if (self->moveDistance[i])
                flag = false;
        }

        if (self->pullPos > self->length) {
            if (self->pullPos <= 0) {
                self->activePlayers &= 0xF;
            }
            else {
                self->velocity.y -= 0x1800;
                if (self->velocity.y < -self->maxVel)
                    self->velocity.y = -self->maxVel;

                self->activePlayers |= 16 * self->activePlayers;
                self->pullPos += self->velocity.y;
            }
        }
        else {
            if (!self->activePlayers || !flag) {
                self->active = ACTIVE_BOUNDS;
                if (self->direction == FLIP_NONE) {
                    if (self->pullPos > 0) {
                        self->velocity.y -= 0x1000;
                        if (self->velocity.y < -self->maxVel)
                            self->velocity.y = -self->maxVel;

                        self->pullPos += self->velocity.y;
                    }
                }
                else if (self->pullPos < self->length) {
                    self->velocity.y += 0x1000;
                    if (self->velocity.y > self->maxVel)
                        self->velocity.y = self->maxVel;

                    self->pullPos += self->velocity.y;
                    if (self->pullPos > self->length) {
                        self->pullPos    = self->length;
                        self->velocity.y = 0;
                    }
                }
            }
            else {
                if (self->direction) {
                    if (self->pullPos <= 0) {
                        self->activePlayers &= 0xF;
                    }
                    else {
                        self->velocity.y -= 0x1800;
                        if (self->velocity.y < -self->maxVel)
                            self->velocity.y = -self->maxVel;

                        self->activePlayers |= 16 * self->activePlayers;
                        self->pullPos += self->velocity.y;
                    }
                }
                else if (self->pullPos >= self->length) {
                    self->activePlayers &= 0xF;
                }
                else {
                    self->velocity.y += 0x1800;
                    if (self->velocity.y > self->maxVel)
                        self->velocity.y = self->maxVel;

                    self->pullPos += self->velocity.y;
                    if (self->pullPos > self->length) {
                        self->pullPos    = self->length;
                        self->velocity.y = 0;
                    }
                    self->activePlayers |= (self->activePlayers << 4);
                }
            }
        }

        if (self->pullPos < 0) {
            self->pullPos    = 0;
            self->velocity.y = 0;
        }
    }

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    self->position.y += self->pullPos;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if (self->playerTimer[playerID] > 0)
            self->playerTimer[playerID]--;
        if (!((1 << playerID) & self->activePlayers)) {
            if (player->state != Player_State_None && !self->playerTimer[playerID]) {
                Hitbox *playerHitbox = Player_GetHitbox(player);

                Hitbox hitbox;
                hitbox.top    = playerHitbox->top;
                hitbox.left   = playerHitbox->left;
                hitbox.right  = playerHitbox->right;
                hitbox.bottom = hitbox.top + 4;
                if (RSDK.CheckObjectCollisionTouchBox(self, &HangPoint->hitbox, player, &hitbox)) {
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    player->groundVel  = 0;
                    player->onGround   = false;
                    player->angle      = 0;
                    player->rotation   = 0;
                    player->position.x = self->position.x;
                    player->position.y = self->position.y;
                    player->position.y += ((HangPoint->hitbox.top - playerHitbox->top) << 16)
                                          + (((HangPoint->hitbox.bottom - HangPoint->hitbox.top) << 15) & 0xFFFF0000);
                    player->tileCollisions       = false;
                    self->moveDistance[playerID] = 0;

                    if (!self->activePlayers) {
                        if (self->direction == FLIP_X) {
                            if (self->pullPos <= self->length)
                                self->velocity.y = 0x20000;
                        }
                        else {
                            self->velocity.y = 0;
                        }
                    }
                    self->activePlayers |= (1 << playerID);
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, false, 0);
                    player->nextAirState     = StateMachine_None;
                    player->nextGroundState  = StateMachine_None;
                    player->state            = Player_State_None;
                    player->abilityValues[0] = RSDK.Rand(0, 2);
                    RSDK.PlaySfx(Player->sfxGrab, false, 255);
                    self->active = ACTIVE_NORMAL;
                }
            }
        }
        else {
            if (player->state == Player_State_Hit) {
                player->tileCollisions = true;
                self->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + 4));
                self->moveDistance[playerID] = 0;
                if (player->left || player->right || player->down || player->state == Player_State_Hit)
                    self->playerTimer[playerID] = 64;
                else
                    self->playerTimer[playerID] = 16;
            }
            else {
                if (player->state != Player_State_None || self->playerTimer[playerID]) {
                    self->activePlayers &= ~(1 << playerID);
                    if (player->objectID == Player->objectID && Player_CheckValidState(player)) {
                        player->tileCollisions = true;
                    }
                }
                else {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    player->velocity.x   = 0;
                    player->velocity.y   = 0;
                    player->groundVel    = 0;
                    player->angle        = 0;
                    player->rotation     = 0;
                    player->position.x   = self->position.x;
                    player->position.y   = self->position.y;
                    player->position.y += (((HangPoint->hitbox.bottom - HangPoint->hitbox.top) << 15) & 0xFFFF0000)
                                          + ((HangPoint->hitbox.top - playerHitbox->top) << 16);
                    if (!self->moveDistance[playerID]) {
                        if (player->left)
                            player->direction = FLIP_X;
                        if (player->right)
                            player->direction = FLIP_NONE;

                        if (player->jumpPress) {
                            player->velocity.y = -0x40000;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            player->animator.speed = 48;
                            player->applyJumpCap            = false;
                            player->jumpAbilityState        = 1;
                            player->state                   = Player_State_Air;
                            player->tileCollisions          = true;
                            self->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + 4));
                            if (player->left || player->right || player->down || player->state == Player_State_Hit) {
                                self->playerTimer[playerID] = 64;
                                HangPoint_HandlePlayerMovement(self, player, playerID);
                            }
                            else {
                                self->playerTimer[playerID] = 16;
                                HangPoint_HandlePlayerMovement(self, player, playerID);
                            }
                        }
                        else if ((!((1 << (playerID + 4)) & self->activePlayers) && (self->direction != FLIP_NONE || self->pullPos >= self->length)
                                  && (self->direction != FLIP_X || self->pullPos <= 0))
                                 || player->sidekick) {
                            bool32 flag = false;
                            if (player->left) {
                                foreach_active(HangPoint, point)
                                {
                                    if (self != point) {
                                        int32 distance = self->position.x - point->position.x;
                                        if (distance < 0x220000 && distance > 0) {
                                            if (abs(self->position.y - point->position.y - point->pullPos) < 0x40000) {
                                                self->activePlayers &= ~(1 << playerID);
                                                self->playerTimer[playerID] = 16;
                                                point->activePlayers |= 1 << playerID;
                                                point->moveDistance[playerID] = 0x200000;
                                                point->playerTimer[playerID]  = 0;
#if RETRO_USE_PLUS
                                                if (player->characterID == ID_RAY) {
                                                    if (player->abilityValues[0])
                                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANGMOVE, &player->animator, false, 6);
                                                    else
                                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANGMOVE, &player->animator, false, 1);
                                                }
                                                else {
#endif
                                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANGMOVE, &player->animator, false, 3);
#if RETRO_USE_PLUS
                                                }
#endif

                                                player->position.x = point->position.x;
                                                if (RSDK.GetEntityID(point) < SceneInfo->entitySlot)
                                                    HangPoint_HandlePlayerMovement(point, player, playerID);
                                                HangPoint_HandlePlayerMovement(self, player, playerID);
                                                flag = true;
                                                foreach_break;
                                            }
                                        }
                                    }
                                }
                            }
                            else if (player->right) {
                                foreach_active(HangPoint, point)
                                {
                                    if (self != point) {
                                        int32 distance = point->position.x - self->position.x;
                                        if (distance < 0x220000 && distance > 0) {
                                            if (abs(self->position.y - point->position.y - point->pullPos) < 0x40000) {
                                                self->activePlayers &= ~(1 << playerID);
                                                self->playerTimer[playerID] = 16;
                                                point->activePlayers |= 1 << playerID;
                                                point->moveDistance[playerID] = -0x200000;
                                                point->playerTimer[playerID]  = 0;
#if RETRO_USE_PLUS
                                                if (player->characterID == ID_RAY) {
                                                    if (!player->abilityValues[0]) {
                                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANGMOVE, &player->animator, false, 0);
                                                    }
                                                    else {
                                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANGMOVE, &player->animator, false, 6);
                                                    }
                                                }
                                                else {
#endif
                                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANGMOVE, &player->animator, false, 3);
#if RETRO_USE_PLUS
                                                }
#endif

                                                player->position.x = point->position.x;
                                                if (RSDK.GetEntityID(point) < SceneInfo->entitySlot)
                                                    HangPoint_HandlePlayerMovement(point, player, playerID);
                                                HangPoint_HandlePlayerMovement(self, player, playerID);
                                                flag = true;
                                                foreach_break;
                                            }
                                        }
                                    }
                                }
                            }
                            if (!flag) {
#if RETRO_USE_PLUS
                                if (player->characterID == ID_RAY && player->abilityValues[0] == 1)
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_DROPDASH, &player->animator, false, 0);
                                else
#endif
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, false, 0);
                                HangPoint_HandlePlayerMovement(self, player, playerID);
                            }
                        }
                        else {
#if RETRO_USE_PLUS
                            if (player->characterID == ID_RAY && player->abilityValues[0] == 1)
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_DROPDASH, &player->animator, false, 0);
                            else
#endif
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, false, 0);
                            HangPoint_HandlePlayerMovement(self, player, playerID);
                        }
                    }
                    else {
                        HangPoint_HandlePlayerMovement(self, player, playerID);
                    }
                }
            }
        }
    }
    self->position.x = storeX;
    self->position.y = storeY;
}

void HangPoint_LateUpdate(void) {}

void HangPoint_StaticUpdate(void) {}

void HangPoint_Draw(void)
{
    RSDK_THIS(HangPoint);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.y += self->pullPos;
    RSDK.DrawSprite(&HangPoint->animator, &drawPos, false);
    if (self->length) {
        HangPoint->animator.frameID = 1;
        int32 length                = (self->pullPos >> 16) & 0xFF00;

        SpriteFrame *frame = RSDK.GetFrame(HangPoint->aniFrames, 0, HangPoint->animator.frameID);

        int32 extraLength = (self->pullPos >> 16) & 0x00FF;
        frame->pivotY     = -(self->pullPos >> 16);
        frame->height     = extraLength;
        frame->sprY       = 257 - extraLength;
        RSDK.DrawSprite(&HangPoint->animator, &drawPos, false);

        frame->height = 256;
        frame->sprY   = 1;
        while (length >= 256) {
            frame->pivotY = -length;
            RSDK.DrawSprite(&HangPoint->animator, &drawPos, false);
            length -= 256;
        }
        HangPoint->animator.frameID = 0;
    }
}

void HangPoint_Create(void *data)
{
    RSDK_THIS(HangPoint);
    if (!self->speed)
        self->speed = 2;

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->visible       = true;
    self->updateRange.y = (self->length + 64) << 16;
    self->drawOrder     = Zone->drawOrderLow;
    if (!SceneInfo->inEditor) {
        self->length = self->length << 16;
        self->maxVel = self->speed << 15;
        if (self->direction == FLIP_X)
            self->pullPos = self->length;
    }
}

void HangPoint_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        HangPoint->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangPoint.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(HangPoint->aniFrames, 0, &HangPoint->animator, true, 0);
    Hitbox *hitbox           = RSDK.GetHitbox(&HangPoint->animator, 0);
    HangPoint->hitbox.top    = hitbox->top;
    HangPoint->hitbox.left   = hitbox->left;
    HangPoint->hitbox.bottom = hitbox->bottom;
    HangPoint->hitbox.right  = hitbox->right;
}

void HangPoint_HandlePlayerMovement(EntityHangPoint *self, void *p, int32 playerID)
{
    EntityPlayer *player = (EntityPlayer *)p;
    if (self->moveDistance[playerID]) {
        if (self->moveDistance[playerID] < 0) {
            self->moveDistance[playerID] += 0x15556;
            if (self->moveDistance[playerID] >= 0) {
                self->moveDistance[playerID] = 0;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                player->abilityValues[0] ^= 1;
            }
        }
        else {
            self->moveDistance[playerID] -= 0x15556;
            if (self->moveDistance[playerID] <= 0) {
                self->moveDistance[playerID] = 0;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                player->abilityValues[0] ^= 1;
            }
        }

        player->position.x += self->moveDistance[playerID] & 0xFFFF0000;
        if (player->jumpPress) {
            player->velocity.y = -0x40000;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            player->applyJumpCap            = false;
            player->animator.speed = 48;
            player->jumpAbilityState        = 1;
            player->state                   = Player_State_Air;
            player->tileCollisions          = true;
            self->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + 4));
            self->moveDistance[playerID] = 0;
            if (player->left || player->right || player->down || player->state == Player_State_Hit)
                self->playerTimer[playerID] = 64;
            else
                self->playerTimer[playerID] = 16;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void HangPoint_EditorDraw(void)
{
    RSDK_THIS(HangPoint);

    int len = self->length;
    self->length <<= 16;

    if (self->direction == FLIP_X)
        self->pullPos = self->length;
    else
        self->pullPos = 0;

    HangPoint_Draw();

    if (showGizmos()) {
        self->inkEffect = INK_BLEND;

        if (self->direction != FLIP_X)
            self->pullPos = self->length;
        else
            self->pullPos = 0;

        HangPoint_Draw();

        self->inkEffect = INK_NONE;
    }

    self->length = len;
}

void HangPoint_EditorLoad(void)
{
    HangPoint->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangPoint.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(HangPoint->aniFrames, 0, &HangPoint->animator, true, 0);

    RSDK_ACTIVE_VAR(HangPoint, direction);
    RSDK_ENUM_VAR("Drop Down", FLIP_NONE);
    RSDK_ENUM_VAR("Pull Up", FLIP_X);
}
#endif

void HangPoint_Serialize(void)
{
    RSDK_EDITABLE_VAR(HangPoint, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(HangPoint, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(HangPoint, VAR_UINT8, direction);
}
