#include "flag_spawn_target.h"
#include "spawn_actor_map_element.h"
#include "map_system.h"
#include "map_element_factory.h"
#include "engine/engine.h"
#include "input/mouse.h"
#include "../position_component.h"
#include "editor_target_system.h"
#include "../i_collision_component.h"
#include "editor_grid_system.h"
#include "respawn_actor_map_element.h"
#include "../pickup_collision_component.h"
#include "../item_type.h"
#include "../i_position_component.h"
#include "../i_team_component.h"
#include "ctf_flag_spawn_point_map_element.h"
#include "ctf_spawn_flags_map_element_system.h"
namespace map {

FlagSpawnTarget::FlagSpawnTarget(int32_t Id)
    : ITarget( Id )
    , mCursorId( -1 )
    , mActorId( Id )
    , mTeam( Team::None )
    , mScene( Scene::Get() )
{

}

FlagSpawnTarget::FlagSpawnTarget( int32_t Id, int32_t curosrId, Team::Type team )
    : ITarget( Id )
    , mCursorId( curosrId )
    , mActorId( Id )
    , mTeam( team )
    , mScene( Scene::Get() )
{

}

void FlagSpawnTarget::Update( double DeltaTime )
{

}

int32_t FlagSpawnTarget::GetCursorId() const
{
    return mCursorId;
}

int32_t FlagSpawnTarget::GetActorId() const
{
    return mActorId;
}

void FlagSpawnTarget::PutTarget( glm::vec2 position )
{
    std::auto_ptr<MapElement> mapElement( MapElementFactory::Get()( AutoId( "ctf_flag_spawn_point" ) ) );
    Opt<ctf::CtfFlagSpawnPointMapElement> ctfFlagSpawnPoint( static_cast<ctf::CtfFlagSpawnPointMapElement*>( mapElement.get() ) );
    ctfFlagSpawnPoint->SetTeam( mTeam );
    ctfFlagSpawnPoint->SetX( position.x );
    ctfFlagSpawnPoint->SetY( position.y );
    MapSystem::Get()->GetMapElementList().insert( Opt<MapElement>( mapElement.release() ) );
    ctf::CtfSpawnFlagsMapElementSystem::Spawn( ctfFlagSpawnPoint );
}

std::auto_ptr<Actor> FlagSpawnTarget::GetCursor()
{
    std::auto_ptr<Actor> player( ActorFactory::Get()( mCursorId ) );
    Opt<ITeamComponent> teamC( player->Get<ITeamComponent>() );
    if ( teamC.IsValid() )
    {
        teamC->SetTeam( mTeam );
    }
    return player;
}

bool FlagSpawnTarget::Load( const Json::Value& setters )
{
    std::string cursor_id;
    if( !Json::GetStr(setters["cursor_id"], cursor_id))
    {
        L1("Error retrieving 'cursor_id' \n" );
        return false;
    }
    std::string team;
    if( !Json::GetStr(setters["team"], team))
    {
        L1("Error retrieving 'team' \n" );
        return false;
    }
    if ( team == "blue" )
    {
        mTeam = Team::Blue;
    }
    else if ( team == "red" )
    {
        mTeam = Team::Red;
    }
    else
    {
        L1("Sytnax error. Read %s as team, valid values are 'red' or 'blue'\n", team.c_str() );
        return false;
    }
    mCursorId = AutoId( cursor_id );
    return true;
}
} // namespace map
