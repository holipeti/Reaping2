#ifndef INCLUDED_CORE_FLASH_NORMAL_ITEM_H
#define INCLUDED_CORE_FLASH_NORMAL_ITEM_H

#include "normal_item.h"
#include "core/property_loader.h"
#include "platform/export.h"

class FlashNormalItem : public NormalItem
{
public:
    FlashNormalItem( int32_t id );
protected:
    FlashNormalItem();
private:
public:
    friend class ::boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version );
};

template<class Archive>
void FlashNormalItem::serialize( Archive& ar, const unsigned int version )
{
    ar& boost::serialization::base_object<NormalItem>( *this );
}


REAPING2_CLASS_EXPORT_KEY2( FlashNormalItem, FlashNormalItem, "flash_normal_item" );
#endif//INCLUDED_CORE_FLASH_NORMAL_ITEM_H