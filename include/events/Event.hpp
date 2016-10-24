#pragma once

enum Event: short {
    ENTITY_IS_DEAD,
    ENTITY_IS_DYING,
    ENTITY_TOOK_DAMAGE,
    ENTITY_SHOT_ENTITY,
    FIRE,
    COLLISION_EVENT,
    MAP_OBJECT_IN_VIEW,
    GAME_OVER,
    PLAY_SOUND
};
