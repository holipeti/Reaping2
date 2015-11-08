#ifndef INCLUDED_MAP_RESPAWN_ACTOR_H
#define INCLUDED_MAP_RESPAWN_ACTOR_H

#include "core/map/map_element.h"
#include "platform/i_platform.h"
#include "core/actor_factory.h"
#include "base_input.h"

namespace map {

class RespawnActorMapElement : public MapElement, public BaseInput
{
public:
    DEFINE_MAP_ELEMENT_BASE(RespawnActorMapElement)
    RespawnActorMapElement();
    void Load(Json::Value& setters);
    void SetActorID(int32_t actorID);
    int32_t GetActorID()const;
    ActorCreator::ComponentLoaderMap_t const& GetComponentLoaders()const;
    void SetSecsToRespawn(double secsToRespawn);
    double GetSecsToRespawn()const;
    void SetSpawnedActorGUID(int32_t spawnedActorGUID);
    int32_t GetSpawnedActorGUID()const;
    void SetSecsToRespawnOriginal(double secsToRespawnOriginal);
    double GetSecsToRespawnOriginal()const;
    static const int32_t SpawnNodeId;
private:
    int32_t mActorID;
    ActorCreator::ComponentLoaderMap_t mComponentLoaders;
    double mSecsToRespawn;
    int32_t mSpawnedActorGUID;
    double mSecsToRespawnOriginal;
};

} // namespace map

#endif//INCLUDED_MAP_RESPAWN_ACTOR_H

//command:  "classgenerator.exe" -g "map_element" -c "respawn_actor" -m "int32_t-actorID ActorCreator::ComponentLoaderMap_t-componentLoaders double-secsToRespawn int32_t-spawnedActorGUID double-secsToRespawnOriginal"
