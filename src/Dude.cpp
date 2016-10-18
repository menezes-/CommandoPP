#include "Dude.hpp"


Dude::Dude(EntityConfig config, EventDispatcher &eventDispatcher)
    : Entity(DUDE, config, eventDispatcher) {

}


Dude::Dude(EventDispatcher &eventDispatcher)
    : Dude(EntityConfig{1, 150, false, true}, eventDispatcher) {

}
