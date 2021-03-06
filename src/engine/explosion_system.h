#ifndef INCLUDED_ENGINE_EXPLOSION_SYSTEM_H
#define INCLUDED_ENGINE_EXPLOSION_SYSTEM_H

#include "core/scene.h"
#include "engine/system.h"

namespace engine {

class ExplosionSystem : public System
{
public:
    DEFINE_SYSTEM_BASE( ExplosionSystem )
    ExplosionSystem();
protected:
    virtual void Init();
    virtual void Update( double DeltaTime );
private:
    Scene& mScene;
};

} // namespace engine

#endif//INCLUDED_ENGINE_EXPLOSION_SYSTEM_H

