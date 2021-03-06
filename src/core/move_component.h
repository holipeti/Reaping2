#ifndef INCLUDED_CORE_MOVE_COMPONENT_H
#define INCLUDED_CORE_MOVE_COMPONENT_H

#include "i_move_component.h"
#include "core/property_loader.h"
#include "platform/export.h"

class MoveComponent : public IMoveComponent
{
public:
    virtual double const& GetHeading() const;
    virtual double const& GetHeadingModifier() const;
    virtual double GetSpeedX() const;
    virtual double GetSpeedY() const;
    virtual bool GetMoving() const;
    virtual bool IsMoving() const;
    virtual void SetSpeed( double Speed ); //dont use this
    virtual void SetHeading( double Heading );
    virtual void SetHeadingModifier( double HeadingModifier );
    virtual void SetMoving( bool moving );
    virtual Buffable<double>& GetSpeed();
    virtual void SetRooted( bool rooted );
    virtual bool IsRooted()const;
protected:
    MoveComponent();
    friend class ComponentFactory;
private:
    double mHeading;
    double mHeadingCos;
    double mHeadingSin;
    double mHeadingModifier;
    double mSpeedX;
    double mSpeedY;
    bool mMoving;
    Buffable<double> mSpeed;
    bool mRooted;
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void MoveComponent::serialize( Archive& ar, const unsigned int version )
{
    //NOTE: generated archive for this class
    ar& boost::serialization::base_object<IMoveComponent>( *this );
    ar& mHeading;
    ar& mHeadingCos;
    ar& mHeadingSin;
    ar& mHeadingModifier;
    ar& mSpeedX;
    ar& mSpeedY;
    ar& mMoving;
    ar& mSpeed;
    ar& mRooted;
}

class MoveComponentLoader: public ComponentLoader<MoveComponent>
{
public:
    DEFINE_COMPONENT_LOADER_BASE( MoveComponentLoader )
private:
    virtual void BindValues();
protected:
    MoveComponentLoader();
    friend class ComponentLoaderFactory;
};


REAPING2_CLASS_EXPORT_KEY2( MoveComponent, MoveComponent, "move_component" );
#endif//INCLUDED_CORE_MOVE_COMPONENT_H
