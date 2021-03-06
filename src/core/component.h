#ifndef INCLUDED_CORE_COMPONENT_H
#define INCLUDED_CORE_COMPONENT_H

#include "platform/auto_id.h"
#include <boost/ptr_container/ptr_map.hpp>
#include "core/opt.h"
#include "json/json.h"
#include "property_loader.h"
#include <portable_oarchive.hpp>
#include <portable_iarchive.hpp>
#include "platform/export.h"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include "platform/export.h"
#include <unordered_map>

#define DEFINE_COMPONENT_BASE( ComponentType ) \
    static int GetType_static() \
    { \
        static int const typ = platform::AutoId( #ComponentType ); \
        return typ; \
    } \
    virtual int GetType() const \
    { \
        return ComponentType::GetType_static(); \
    } \

#define DEFINE_COMPONENT_LOADER_BASE( ComponentLoader ) \
    virtual ComponentLoader* clone() const \
    { \
        return new ComponentLoader( *this ); \
    } \

extern const double PRECISION;

class Actor;

class Component
{
public:
    DEFINE_COMPONENT_BASE( Component )
    virtual ~Component();
    virtual void SetActorGUID( int32_t actorGUID );
    void SetId( int32_t id );
    int32_t GetId() const;
    virtual void Save( Json::Value& component );

    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
    Component();
protected:
    int32_t mActorGUID;
    int32_t mId;
};

template<class Archive>
void Component::serialize( Archive& ar, const unsigned int version )
{
    ar& mActorGUID;
    ar& mId;
}

class ComponentFactory;

class ComponentHolder
{
protected:
    typedef std::unordered_map<int32_t, Component*> ComponentList_t;
    ComponentFactory& mComponentFactory;
    ComponentList_t mComponents;

public:
    ComponentHolder();
    virtual void AddComponent( std::auto_ptr<Component> Comp  );
    void DropComponent( int32_t id );

    template<typename Component_t>
    Opt<Component_t> Get() const;
    template<typename Component_t>
    Opt<Component_t> Get();
    virtual ~ComponentHolder();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void ComponentHolder::serialize( Archive& ar, const unsigned int version )
{
    ar& mComponents;
}

template<typename Component_t>
Opt<Component_t> ComponentHolder::Get() const
{
    auto i = mComponents.find( Component_t::GetType_static() );
    return Opt<Component_t>( i == mComponents.end() ? nullptr 
        : static_cast<Component_t*> ( const_cast<Component*> ( i->second ) ) );
}

template<typename Component_t>
Opt<Component_t> ComponentHolder::Get()
{
    return ( ( const ComponentHolder* )this )->Get<Component_t>();
}


class DefaultComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE( DefaultComponent )
    DefaultComponent();
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void DefaultComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<Component>( *this );
}


template<typename COMPONENT>
class ComponentLoader: public PropertyLoader<COMPONENT, Component>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( ComponentLoader )
    virtual void FillProperties( ComponentHolder& actor )const;
    virtual void BindValues();
};

template<typename COMPONENT>
void ComponentLoader<COMPONENT>::BindValues()
{
}

template<typename COMPONENT>
void ComponentLoader<COMPONENT>::FillProperties( ComponentHolder& actor ) const
{
    if ( this->mBase.get() )
    {
        static_cast<const ComponentLoader<Component> *>( this->mBase.get() )->FillProperties( actor );
    }
    if ( this->mSetterFuncList.empty() )
    {
        return;
    }
    Opt<COMPONENT> castedTarget = actor.Get<COMPONENT>();
    if ( !castedTarget.IsValid() )
    {
        return;
    }
    for( auto const& fun : this->mSetterFuncList )
    {
        fun( castedTarget.Get() );
    }
}

REAPING2_CLASS_EXPORT_KEY2( DefaultComponent, DefaultComponent, "default_component" );
#endif//INCLUDED_CORE_COMPONENT_H
