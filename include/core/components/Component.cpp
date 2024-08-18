#include "Component.h"
#include "../actors/Actor.h"
#include <memory>

namespace Core {
    Component::Component(std::unique_ptr<Core::Actor>&& owner, int order): m_Actor(std::move(owner)), m_Order(order){}

    Component::~Component(){}

    
}