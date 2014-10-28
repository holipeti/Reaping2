#include "i_platform.h"

namespace Json {
bool GetStr( Json::Value const& V, std::string& O )
{
    if( !V.isString() )
    {
        return false;
    }
    O = V.asString();
    return !O.empty();
}
bool GetUInt( Json::Value const& V, uint32_t& O )
{
    if( !V.isNumeric() )
    {
        return false;
    }
    O = V.asUInt();
    return true;
}
bool GetInt( Json::Value const& V, int32_t& O )
{
    if( !V.isNumeric() )
    {
        return false;
    }
    O = V.asInt();
    return true;
}
bool GetDouble( Json::Value const& V, double& O )
{
    if( !V.isNumeric() )
    {
        return false;
    }
    O = V.asDouble();
    return true;
}
}

JsonReader::JsonReader( File& F )
    : mValid( false )
{
    Json::Reader Reader;
    std::string Contents;
    if( !F.ReadAll( Contents ) )
    {
        return;
    }
    if( !Reader.parse( Contents, mRoot, false ) )
    {
        assert( false );
    }
    mValid = true;
}

bool JsonReader::IsValid() const
{
    return mValid;
}

Json::Value& JsonReader::GetRoot()
{
    return mRoot;
}

