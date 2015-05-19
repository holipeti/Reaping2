#include "platform/i_platform.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include <string>
#include <ctype.h>

class Generator
{
public:
    typedef std::pair<std::string,std::string> Type_Member_Pair_t;
    typedef std::vector<Type_Member_Pair_t> Type_Member_Pairs_t;
    typedef std::pair<std::string,std::string> Namespace_Event_Pair_t;
    typedef std::vector<Namespace_Event_Pair_t> Namespace_Event_Pairs_t;
    std::string classUnderscore;
    std::string parentUnderscore;
    std::string namespaceLowerCase;
    std::string membersArg;
    std::string eventsArg;
    std::string targetUnderscore;
    std::string targetItemTypeUnderscore;
    std::string targetItemNameUnderscore;
protected:
    std::string classCamelCase;
    std::string classUpperCaseUnderscore;
    std::string classVariable;
    std::string parentCamelCase;
    std::string nameSpaceUpperCase;
    std::string targetCamelCase;
    std::string targetVariableName;
    std::string targetItemTypeCamelCase;
    std::string targetItemTypeVariableName;
    std::string targetItemNameCamelCase;
    std::string targetItemNameVariableName;
    std::string headerGuard;
    Type_Member_Pairs_t typeMemberPairs;
    Namespace_Event_Pairs_t namespaceEventPairs;
public:
    virtual void Generate()
    {
        L1("default generator implementation, do nothing\n");
    }
    virtual void Init()
    {
        classCamelCase=UnderscoreToCamelCase(classUnderscore);
        classVariable=classCamelCase;
        classVariable[0]=tolower(classCamelCase[0]);
        classUpperCaseUnderscore=boost::to_upper_copy(classUnderscore);
        parentCamelCase=UnderscoreToCamelCase(parentUnderscore);
        nameSpaceUpperCase=boost::to_upper_copy(namespaceLowerCase);
        if (!targetUnderscore.empty())
        {
            targetCamelCase=UnderscoreToCamelCase(targetUnderscore);
            targetVariableName=targetCamelCase;
            targetVariableName[0]=tolower(targetVariableName[0]);
        }

        if (!targetItemTypeUnderscore.empty())
        {
            targetItemTypeCamelCase=UnderscoreToCamelCase(targetItemTypeUnderscore);
            targetItemTypeVariableName=targetItemTypeCamelCase;
            targetItemTypeVariableName[0]=tolower(targetItemTypeVariableName[0]);
        }
        if (!targetItemNameUnderscore.empty())
        {
            targetItemNameCamelCase=UnderscoreToCamelCase(targetItemNameUnderscore);
            targetItemNameVariableName=targetItemNameCamelCase;
            targetItemNameVariableName[0]=tolower(targetItemNameVariableName[0]);
        }


        headerGuard="INCLUDED_"+nameSpaceUpperCase+"_"+classUpperCaseUnderscore+"_H";
        typeMemberPairs=SplitMemberPairs(membersArg);
        namespaceEventPairs=SplitEventPairs(eventsArg);
    }
    virtual ~Generator() {}
    std::string UnderscoreToCamelCase(std::string underscore)
    {
        if (underscore.empty())
        {
            return std::string();
        }
        boost::to_lower(underscore);
        std::vector<std::string> parts;
        boost::split(parts,underscore,boost::is_any_of("_"));
        for(std::vector<std::string>::iterator i=parts.begin(),e=parts.end();i!=e;++i)
        {
            (*i)[0]=toupper((*i)[0]);
        }
        std::string camelCase;
        camelCase=boost::join(parts,camelCase);
        return camelCase;
    }

    Type_Member_Pairs_t SplitMemberPairs(std::string membersArg)
    {
        if (membersArg.empty())
        {
            return Type_Member_Pairs_t();
        }
        std::vector<std::string> typeMemberSplit;
        boost::split(typeMemberSplit,membersArg,boost::is_any_of(" "));

        Type_Member_Pairs_t typeMemberPairs;
        for(std::vector<std::string>::iterator i=typeMemberSplit.begin(),e=typeMemberSplit.end();i!=e;++i)
        {
            std::vector<std::string> parts;
            boost::split(parts,*i,boost::is_any_of("-"));
            if(parts.size()==2)
            {
                typeMemberPairs.push_back(Type_Member_Pair_t(parts[0],parts[1]));
            }
            else
            {
                L1("members are not correctly separated: %s\n",i->c_str());
            }
        }
        return typeMemberPairs;
    }

    Namespace_Event_Pairs_t SplitEventPairs(std::string eventsArg)
    {
        if (eventsArg.empty())
        {
            return Namespace_Event_Pairs_t();
        }
        std::vector<std::string> namespaceEventSplit;
        boost::split(namespaceEventSplit,eventsArg,boost::is_any_of(" "));

        Type_Member_Pairs_t namespaceEventPairs;
        for(std::vector<std::string>::iterator i=namespaceEventSplit.begin(),e=namespaceEventSplit.end();i!=e;++i)
        {
            std::vector<std::string> parts;
            boost::split(parts,*i,boost::is_any_of("-"));
            if(parts.size()==2)
            {
                namespaceEventPairs.push_back(Type_Member_Pair_t(parts[0],parts[1]));
            }
            else
            {
                L1("events are not correctly separated: %s\n",i->c_str());
            }
        }
        return namespaceEventPairs;
    }

    // from targetActor -> mTargetActor
    std::string CreateMemberName(std::string memberName)
    {
        memberName[0]=toupper(memberName[0]);
        std::string r="m"+memberName;
        return r;
    }

    // from damageTaken -> mOnDamageTaken
    std::string VariableToCamelCase(std::string variableName)
    {
        variableName[0]=toupper(variableName[0]);
        return variableName;
    }

    // from targetActor -> GetTargetActor
    std::string CreateGetMember(std::string memberType, std::string memberName)
    {
        memberName[0]=toupper(memberName[0]);
        std::string r=(memberType=="bool"?"Is":"Get")+memberName;
        return r;
    }
    // from targetActor -> SetTargetActor
    std::string CreateSetMember(std::string memberName)
    {
        memberName[0]=toupper(memberName[0]);
        std::string r="Set"+memberName;
        return r;
    }
    // from int32_t,targetActor -> virtual int32_t GetTargetActor()const
    std::string CreateGetMemberFull(std::string memberType, std::string memberName)
    {
        std::string r="virtual "+memberType+" "+CreateGetMember(memberType,memberName)+"()const";
        return r;
    }
    // from int32_t,targetActor -> virtual void SetTargetActor(int32_t targetActor)
    std::string CreateSetMemberFull(std::string memberType, std::string memberName)
    {
        std::string r="virtual void "+CreateSetMember(memberName)+"("+memberType+" "+memberName+")";
        return r;
    }

    /// from int32_t,targetActor -> virtual int32_t GetTargetActor()const=0
    std::string CreateAbstractGetMember(std::string memberType, std::string memberName)
    {
        std::string r="virtual "+memberType+" "+CreateGetMember(memberType,memberName)+"()const=0";
        return r;
    }

    /// from int32_t,targetActor -> virtual void SetTargetActor(int32_t targetActor)=0
    std::string CreateAbstractSetMember(std::string memberType, std::string memberName)
    {
        std::string r="virtual void "+CreateSetMember(memberName)+"("+memberType+" "+memberName+")=0";
        return r;
    }

    // from int32_t,targetActor -> int32_t mTargetActor
    std::string CreateMemberWithType(std::string memberType, std::string memberName)
    {
        std::string r=memberType+" "+CreateMemberName(memberName);
        return r;
    }

    // from int32_t,targetActor -> int32_t targetActor
    std::string CreateArgumentWithType(std::string memberType, std::string memberName)
    {
        std::string r=memberType+" "+memberName;
        return r;
    }

    // from int32_t,targetActor,TargetComponent -> 
    // int32_t GetTargetActor() const
    // {
    //     return mTargetActor;
    // }
    // 
    std::string CreateGetMemberCppDefiniton(std::string memberType, std::string memberName,std::string ClassCamelCase)
    {
        
        std::string r=memberType+" "+ClassCamelCase+"::"+CreateGetMember(memberType,memberName)+"()const\n";
        r=r+"{\n";
        r=r+"    return "+CreateMemberName(memberName)+";\n";
        r=r+"}\n";
        r=r+"\n";
        return r;
    }

    // from int32_t,targetActor,TargetComponent -> 
    // virtual void SetTargetActor(int32_t targetActor)
    // {
    //     mTargetActor=targetActor;
    // }
    // 
    std::string CreateSetMemberCppDefiniton(std::string memberType, std::string memberName,std::string ClassCamelCase)
    {
        std::string r="void "+ClassCamelCase+"::"+CreateSetMember(memberName)+"("+memberType+" "+memberName+")\n";
        r=r+"{\n";
        r=r+"    "+CreateMemberName(memberName)+"="+memberName+";\n";
        r=r+"}\n";
        r=r+"\n";
        return r;
    }
};

class IComponentGenerator : public Generator
{
    virtual void Generate()
    {
        L1("IComponentGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="component";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="core";
        }
        Init();

        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"component.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_COMPONENT_BASE(%s)\n",classCamelCase.c_str());
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateAbstractSetMember(i->first,i->second).c_str());
                fprintf(file.mFile, "    %s;\n",CreateAbstractGetMember(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());
        }
        L1("IComponentGenerator ended\n");
    }
};

class ComponentGenerator : public Generator
{
    virtual void Generate()
    {
        L1("ComponentGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="i_"+classUnderscore;
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="core";
        }

        Init();
        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"%s.h\"\n",parentUnderscore.c_str());
            fprintf(file.mFile, "#include \"core/property_loader.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateSetMemberFull(i->first,i->second).c_str());
                fprintf(file.mFile, "    %s;\n",CreateGetMemberFull(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    friend class ComponentFactory;\n");
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateMemberWithType(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "class %sLoader : public ComponentLoader<%s>\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    virtual void BindValues();\n");
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    %sLoader();\n",classCamelCase.c_str());
            fprintf(file.mFile, "    friend class ComponentLoaderFactory;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "//TODO: to component_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateComponent<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to component_loader_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateComponentLoader<%sLoader>);\n",classUnderscore.c_str(),classCamelCase.c_str());
        }
        

        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"core/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            bool isFirst=true;
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s %s(_fill_me_)\n",
                    isFirst?":":",",CreateMemberName(i->second).c_str());
                isFirst=false;
            }
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "%s",CreateSetMemberCppDefiniton(i->first,i->second,classCamelCase).c_str());
                fprintf(file.mFile, "%s",CreateGetMemberCppDefiniton(i->first,i->second,classCamelCase).c_str());
            }

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %sLoader::BindValues()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%sLoader::%sLoader()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
        }

        L1("ComponentGenerator ended\n");
    }
};

class NormalItemGenerator : public Generator
{
    virtual void Generate()
    {
        L1("NormalItemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="normal_item";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="core";
        }

        Init();
        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"normal_item.h\"\n");
            fprintf(file.mFile, "#include \"core/property_loader.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    %s( int32_t id );\n",classCamelCase.c_str());
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateSetMemberFull(i->first,i->second).c_str());
                fprintf(file.mFile, "    %s;\n",CreateGetMemberFull(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "protected:\n");
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateMemberWithType(i->first,i->second).c_str());
            }
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to item_factory.cpp:\n");
            fprintf(file.mFile, "Bind<%s>(AutoId(\"%s\"));\n",classCamelCase.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"core/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s( int32_t id )\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : NormalItem(id)\n");
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s %s(_fill_me_)\n",
                    ",",CreateMemberName(i->second).c_str());
            }
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "%s",CreateSetMemberCppDefiniton(i->first,i->second,classCamelCase).c_str());
                fprintf(file.mFile, "%s",CreateGetMemberCppDefiniton(i->first,i->second,classCamelCase).c_str());
            }

            fprintf(file.mFile, "\n");
        }

        L1("NormalItemGenerator ended\n");
    }
};


class SystemGenerator : public Generator
{
    virtual void Generate()
    {
        L1("SystemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="system";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }

        if (targetUnderscore.empty())
        {
            targetUnderscore="some_target";
        }
        Init();

        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"core/scene.h\"\n");
            fprintf(file.mFile, "#include \"engine/%s.h\"\n",parentUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SYSTEM_BASE(%s)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( double DeltaTime );\n");
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "    Scene& mScene;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSystem<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "Eng.AddSystem(AutoId(\"%s\"));\n",classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"platform/i_platform.h\"\n");
            fprintf(file.mFile, "#include \"%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "#include \"core/i_%s_component.h\"\n",targetUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : mScene( Scene::Get() )\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    for( ActorList_t::iterator it = mScene.GetActors().begin(), e = mScene.GetActors().end(); it != e; ++it )\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "       Actor& actor = **it;\n");
            fprintf(file.mFile, "       Opt<I%sComponent> %sC=actor.Get<I%sComponent>();\n",targetCamelCase.c_str(),targetVariableName.c_str(),targetCamelCase.c_str());
            fprintf(file.mFile, "       if (!%sC.IsValid())\n",targetVariableName.c_str());
            fprintf(file.mFile, "       {\n");
            fprintf(file.mFile, "           continue;\n");
            fprintf(file.mFile, "       }\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("SystemGenerator ended\n");
    }
};


class SubSystemGenerator : public Generator
{
protected:
    virtual void Init()
    {
        if (targetUnderscore.empty())
        {
            targetUnderscore="some_target";
        }
        Generator::Init();
    }

    virtual void Generate()
    {
        L1("SubSystemGenerator started\n");
        L1("SubSystemGenerator ended\n");
    }
};



class CollisionSubSystemGenerator : public SubSystemGenerator
{
    virtual void Generate()
    {
        L1("CollisionSubSystemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="collision_sub_system";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }

        Init();

        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"engine/collisions/collision_sub_system.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n");
            fprintf(file.mFile, "    virtual void ClipScene( Actor& actor );\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to sub_system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "collisionSS->AddSubSystem(AutoId(\"%s_collision_component\"),AutoId(\"%s\"));\n",targetUnderscore.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"platform/i_platform.h\"\n");
            fprintf(file.mFile, "#include \"engine/engine.h\"\n");
            fprintf(file.mFile, "#include \"engine/collisions/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "#include \"core/%s_collision_component.h\"\n",targetUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : CollisionSubSystem()\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    if (!mOther)\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        return;\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "    Opt<%sCollisionComponent> %sCC=actor.Get<%sCollisionComponent>();\n",targetCamelCase.c_str(),targetVariableName.c_str(),targetCamelCase.c_str());
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::ClipScene(Actor& actor)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    CollisionSubSystem::ClipScene(actor);\n");
            fprintf(file.mFile, "    Opt<%sCollisionComponent> %sCC=actor.Get<%sCollisionComponent>();\n",targetCamelCase.c_str(),targetVariableName.c_str(),targetCamelCase.c_str());
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("CollisionSubSystemGenerator ended\n");
    }
};

class ControllerSubSystemGenerator : public SubSystemGenerator
{
    virtual void Generate()
    {
        L1("ControllerSubSystemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="sub_system";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }

        Init();

        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"engine/sub_system.h\"\n");
            fprintf(file.mFile, "#include \"core/scene.h\"\n");
            fprintf(file.mFile, "#include \"core/program_state.h\"\n");
            fprintf(file.mFile, "using core::ProgramState;\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n");
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "    Scene& mScene;\n");
            fprintf(file.mFile, "    ProgramState& mProgramState;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to sub_system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "controllserSystem->AddSubSystem(AutoId(\"%s_controller_component\"),AutoId(\"%s\"));\n",targetUnderscore.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"platform/i_platform.h\"\n");
            fprintf(file.mFile, "#include \"engine/engine.h\"\n");
            fprintf(file.mFile, "#include \"engine/controllers/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "#include \"core/%s_controller_component.h\"\n",targetUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : mScene(Scene::Get())\n");
            fprintf(file.mFile, "    , mProgramState(core::ProgramState::Get())\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    Opt<%sControllerComponent> %sCC=actor.Get<%sControllerComponent>();\n",targetCamelCase.c_str(),targetVariableName.c_str(),targetCamelCase.c_str());
            fprintf(file.mFile, "    if (!%sCC.IsValid()||!%sCC->IsEnabled())\n",targetVariableName.c_str(),targetVariableName.c_str());
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        return;\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("ControllerSubSystemGenerator ended\n");
    }
};

class NormalItemSubSystemGenerator : public Generator
{
    virtual void Generate()
    {
        L1("NormalItemSubSystemGenerator started\n");
        if (parentUnderscore.empty())
        {
            parentUnderscore="sub_system";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }
        if (targetItemTypeUnderscore.empty())
        {
            targetItemTypeUnderscore="normal_item";
        }
        if (targetItemNameUnderscore.empty())
        {
            targetItemNameUnderscore="item_name";
        }
        Init();

        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"engine/items/common_sub_system_includes.h\"\n");
            fprintf(file.mFile, "#include \"%s_sub_system.h\"\n",targetItemTypeUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public %s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%s)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update( Actor& actor, double DeltaTime );\n");
            fprintf(file.mFile, "private:\n");
            fprintf(file.mFile, "    Scene& mScene;\n");
            fprintf(file.mFile, "    Opt<%sSubSystem> m%sSubSystem;\n",targetItemTypeCamelCase.c_str(),targetItemTypeCamelCase.c_str());
            fprintf(file.mFile, "    ActorFactory& mActorFactory;\n");
            fprintf(file.mFile, "    int32_t mProjectileId;\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "//TODO: to sub_system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s\"), &CreateSubSystem<%s>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "%sSS->AddSubSystem(AutoId(\"%s_%s\"),AutoId(\"%s\"));\n",targetItemTypeVariableName.c_str(),targetItemNameUnderscore.c_str(),targetItemTypeUnderscore.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"engine/items/%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "#include \"%s_sub_system.h\"\n",targetItemTypeUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : mScene(Scene::Get())\n");
            fprintf(file.mFile, "    , m%sSubSystem(%sSubSystem::Get())\n",targetItemTypeCamelCase.c_str(),targetItemTypeCamelCase.c_str());
            fprintf(file.mFile, "    , mActorFactory(ActorFactory::Get())\n");
            fprintf(file.mFile, "    , mProjectileId(AutoId(\"_fill_me_\"))\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %s::Update(Actor& actor, double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    Opt<IInventoryComponent> inventoryC = actor.Get<IInventoryComponent>();\n");
            fprintf(file.mFile, "    Opt<%s> %s = inventoryC->GetSelected%s();\n",targetItemTypeCamelCase.c_str(),targetItemTypeVariableName.c_str(),targetItemTypeCamelCase.c_str());
            fprintf(file.mFile, "    if (%s->IsUse())\n",targetItemTypeVariableName.c_str());
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        //TODO: do some stuff with it\n");
            fprintf(file.mFile, "        %s->SetConsumed(true);\n",targetItemTypeVariableName.c_str());
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("NormalItemSubSystemGenerator ended\n");
    }
};

class EventGenerator : public Generator
{
    virtual void Generate()
    {
        L1("%s started\n",__FUNCTION__);
        if (parentUnderscore.empty())
        {
            parentUnderscore="event";
        }
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="engine";
        }
        Init();

        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"platform/event.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "struct %s : public platform::%s\n",classCamelCase.c_str(),parentCamelCase.c_str());
            fprintf(file.mFile, "{\n");

            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateMemberWithType(i->first,i->second).c_str());
            }

            fprintf(file.mFile, "    %s(",classCamelCase.c_str());
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                if (e-1!=i)
                {
                    fprintf(file.mFile, "%s, ",CreateArgumentWithType(i->first,i->second).c_str());
                }
                else
                {
                    fprintf(file.mFile, "%s)\n",CreateArgumentWithType(i->first,i->second).c_str());
                }
            }
            fprintf(file.mFile, "        :");
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                if (e-1!=i)
                {
                    fprintf(file.mFile, "%s(%s),",CreateMemberName(i->second).c_str(), i->second.c_str());
                }
                else
                {
                    fprintf(file.mFile, "%s(%s){}\n",CreateMemberName(i->second).c_str(), i->second.c_str());
                }
            }

            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

        }

        L1("%s ended\n",__FUNCTION__);
    }
};

class MessageGenerator : public Generator
{
    virtual void Generate()
    {
        L1("%s started\n",__FUNCTION__);
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="network";
        }

        Init();
        {
            AutoNormalFile file((classUnderscore+"_message.h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"network/message.h\"\n");
            fprintf(file.mFile, "#include \"network/message_handler_sub_system.h\"\n");
            fprintf(file.mFile, "#include \"network/message_sender_system.h\"\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %sMessage : public Message\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    friend class ::boost::serialization::access;\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_MESSAGE_BASE(%sMessage)\n",classCamelCase.c_str());
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    %s;\n",CreateMemberWithType(i->first,i->second).c_str());
            }

            fprintf(file.mFile, "    %sMessage()\n",classCamelCase.c_str());
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                if (i==typeMemberPairs.begin())
                {
                    fprintf(file.mFile, "        : %s(_fill_me_)\n",CreateMemberName(i->second).c_str());
                }
                else
                {
                    fprintf(file.mFile, "        , %s(_fill_me_)\n",CreateMemberName(i->second).c_str());
                }
            }
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "    template<class Archive>\n");
            fprintf(file.mFile, "    void serialize(Archive& ar, const unsigned int version);\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "template<class Archive>\n");
            fprintf(file.mFile, "void %sMessage::serialize(Archive& ar, const unsigned int version)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    ar & boost::serialization::base_object<Message>(*this);\n");
            for(Type_Member_Pairs_t::iterator i=typeMemberPairs.begin(),e=typeMemberPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    ar & %s;\n",CreateMemberName(i->second).c_str());
            }
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "class %sMessageHandlerSubSystem : public MessageHandlerSubSystem\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SUB_SYSTEM_BASE(%sMessageHandlerSubSystem)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %sMessageHandlerSubSystem();\n",classCamelCase.c_str());
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Execute(Message const& message );\n");
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "class %sMessageSenderSystem : public MessageSenderSystem\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            for(Type_Member_Pairs_t::iterator i=namespaceEventPairs.begin(),e=namespaceEventPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    AutoReg mOn%s;\n",VariableToCamelCase(i->second).c_str());
                fprintf(file.mFile, "    void On%s(%s::%sEvent const& Evt);\n"
                    ,VariableToCamelCase(i->second).c_str(),i->first.c_str(),VariableToCamelCase(i->second).c_str());
            }
            fprintf(file.mFile, "public:\n");
            fprintf(file.mFile, "    DEFINE_SYSTEM_BASE(%sMessageSenderSystem)\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %sMessageSenderSystem();\n",classCamelCase.c_str());
            fprintf(file.mFile, "    virtual void Init();\n");
            fprintf(file.mFile, "    virtual void Update(double DeltaTime);\n");
            fprintf(file.mFile, "};\n");

            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());

            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "//TODO: to message_order.h\n");
            fprintf(file.mFile, "BOOST_CLASS_EXPORT_GUID(%s::%sMessage, \"%s\")\n",namespaceLowerCase.c_str(),classCamelCase.c_str(),classUnderscore.c_str());
            fprintf(file.mFile, "//TODO: to message_handler_sub_system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s_message_handler_sub_system\"), &CreateSubSystem<%sMessageHandlerSubSystem>);\n",classUnderscore.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to system_factory.cpp:\n");
            fprintf(file.mFile, "Bind( AutoId(\"%s_message_sender_system\"), &CreateSystem<%s::%sMessageSenderSystem>);\n",classUnderscore.c_str(),namespaceLowerCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "//TODO: to main.cpp:\n");
            fprintf(file.mFile, "Eng.AddSystem(AutoId(\"%s_message_sender_system\"));\n",classUnderscore.c_str());
            fprintf(file.mFile, "messageHandlerSSH->AddSubSystem(%s::%sMessage::GetType_static(),AutoId(\"%s_message_handler_sub_system\"));\n",
                namespaceLowerCase.c_str(),classCamelCase.c_str(),classUnderscore.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+"_message.cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"platform/i_platform.h\"\n");
            fprintf(file.mFile, "#include \"network/%s_message.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%sMessageSenderSystem::%sMessageSenderSystem()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : MessageSenderSystem()\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            L1("%s ended1\n",__FUNCTION__);

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %sMessageSenderSystem::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    MessageSenderSystem::Init();\n");
            for(Type_Member_Pairs_t::iterator i=namespaceEventPairs.begin(),e=namespaceEventPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "    mOn%s=EventServer<%s::%sEvent>::Get().Subscribe( boost::bind( &%sMessageSenderSystem::On%s, this, _1 ) );\n"
                    ,VariableToCamelCase(i->second).c_str(),i->first.c_str(),VariableToCamelCase(i->second).c_str(),classCamelCase.c_str(),VariableToCamelCase(i->second).c_str());
            }
            L1("%s ended2\n",__FUNCTION__);
           fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %sMessageSenderSystem::Update(double DeltaTime)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    MessageSenderSystem::Update(DeltaTime);\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            L1("%s ended3\n",__FUNCTION__);
            for(Type_Member_Pairs_t::iterator i=namespaceEventPairs.begin(),e=namespaceEventPairs.end();i!=e;++i)
            {
                fprintf(file.mFile, "void %sMessageSenderSystem::On%s(%s::%sEvent const& Evt)\n"
                    ,classCamelCase.c_str(),VariableToCamelCase(i->second).c_str(),i->first.c_str(),VariableToCamelCase(i->second).c_str());
                fprintf(file.mFile, "{\n");
                fprintf(file.mFile, "    std::auto_ptr<%sMessage> %sMsg(new %sMessage);\n",classCamelCase.c_str(),classVariable.c_str(),classCamelCase.c_str());
                for(Type_Member_Pairs_t::iterator itTypeMember=typeMemberPairs.begin(),eitTypeMember=typeMemberPairs.end();itTypeMember!=eitTypeMember;++itTypeMember)
                {
                    fprintf(file.mFile, "    %sMsg->%s=Evt.%s;\n",classVariable.c_str(),CreateMemberName(itTypeMember->second).c_str(),CreateMemberName(itTypeMember->second).c_str());
                }
                fprintf(file.mFile, "    mMessageHolder.AddOutgoingMessage(%sMsg);\n",classVariable.c_str());
                fprintf(file.mFile, "}\n");
                fprintf(file.mFile, "\n");
            }
            L1("%s ended4\n",__FUNCTION__);


            fprintf(file.mFile, "%sMessageHandlerSubSystem::%sMessageHandlerSubSystem()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    : MessageHandlerSubSystem()\n");
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "void %sMessageHandlerSubSystem::Init()\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            L1("%s ended5\n",__FUNCTION__);
           fprintf(file.mFile, "void %sMessageHandlerSubSystem::Execute(Message const& message)\n",classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    %sMessage const& msg=static_cast<%sMessage const&>(message);\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "    Opt<Actor> actor=mScene.GetActor(msg.mActorGUID);\n");
            fprintf(file.mFile, "    if (!actor.IsValid())\n");
            fprintf(file.mFile, "    {\n");
            fprintf(file.mFile, "        L1(\"cannot find actor with GUID: (%%s) %%d \\n\",__FUNCTION__,msg.mActorGUID );\n");
            fprintf(file.mFile, "        return;\n");
            fprintf(file.mFile, "    }\n");
            fprintf(file.mFile, "    \n");
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("%s ended\n",__FUNCTION__);
    }
};


class FactoryGenerator : public Generator
{
    virtual void Generate()
    {
        L1("%s started\n",__FUNCTION__);
        if (namespaceLowerCase.empty())
        {
            namespaceLowerCase="core";
        }

        if (targetUnderscore.empty())
        {
            targetUnderscore="some_target";
        }
        Init();

        {
            AutoNormalFile file((classUnderscore+".h").c_str(),"w" );
            fprintf(file.mFile, "#ifndef %s\n",headerGuard.c_str());
            fprintf(file.mFile, "#define %s\n",headerGuard.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#include \"platform/factory.h\"\n");
            fprintf(file.mFile, "#include \"platform/singleton.h\"\n");
            fprintf(file.mFile, "#include \"%s.h\"\n",targetUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "class %s : public platform::Factory<%s>, public platform::Singleton<%s>\n",classCamelCase.c_str(),targetCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    friend class platform::Singleton<%s>;\n",classCamelCase.c_str());
            fprintf(file.mFile, "    %s();\n",classCamelCase.c_str());
            fprintf(file.mFile, "protected:\n");
            fprintf(file.mFile, "    template<typename Elem_T>\n");
            fprintf(file.mFile, "    static std::auto_ptr<%s> Create%s( int32_t Id );\n",targetCamelCase.c_str(),targetCamelCase.c_str());
            fprintf(file.mFile, "};\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "template<typename Elem_T>\n");
            fprintf(file.mFile, "std::auto_ptr<%s> %s::Create%s( int32_t Id )\n",targetCamelCase.c_str(),classCamelCase.c_str(),targetCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    return std::auto_ptr<%s>( new Elem_T() );\n",targetCamelCase.c_str());
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "#endif//%s\n",headerGuard.c_str());
        }


        {
            AutoNormalFile file((classUnderscore+".cpp").c_str(),"w" );
            fprintf(file.mFile, "#include \"platform/i_platform.h\"\n");
            fprintf(file.mFile, "#include \"%s.h\"\n",classUnderscore.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "using platform::AutoId;\n");
            fprintf(file.mFile, "namespace %s {\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
            fprintf(file.mFile, "%s::%s()\n",classCamelCase.c_str(),classCamelCase.c_str());
            fprintf(file.mFile, "{\n");
            fprintf(file.mFile, "    Bind( AutoId(\"default_%s\"), &Create%s<Default%s> );\n",targetUnderscore.c_str(),targetCamelCase.c_str(),targetCamelCase.c_str());
            fprintf(file.mFile, "    SetDefault( AutoId(\"default_%s\") );\n",targetUnderscore.c_str());
            fprintf(file.mFile, "}\n");
            fprintf(file.mFile, "\n");

            fprintf(file.mFile, "} // namespace %s\n",namespaceLowerCase.c_str());
            fprintf(file.mFile, "\n");
        }

        L1("%s ended\n",__FUNCTION__);
    }
};

class GeneratorFactory : public platform::Factory<Generator>, public platform::Singleton<GeneratorFactory>
{
    friend class platform::Singleton<GeneratorFactory>;
    template<typename Elem_T>
    static std::auto_ptr<Generator> CreateGenerator( int32_t Id );

    GeneratorFactory()
    {
        Bind( AutoId( "default_generator" ), &CreateGenerator<Generator> );
        SetDefault( AutoId( "default_generator" ) );
        Bind( AutoId( "i_component" ), &CreateGenerator<IComponentGenerator> );
        Bind( AutoId( "component" ), &CreateGenerator<ComponentGenerator> );
        Bind( AutoId( "normal_item" ), &CreateGenerator<NormalItemGenerator> );
        Bind( AutoId( "system" ), &CreateGenerator<SystemGenerator> );
        Bind( AutoId( "collision_sub_system" ), &CreateGenerator<CollisionSubSystemGenerator> );
        Bind( AutoId( "controller_sub_system" ), &CreateGenerator<ControllerSubSystemGenerator> );
        Bind( AutoId( "normal_item_sub_system" ), &CreateGenerator<NormalItemSubSystemGenerator> );
        Bind( AutoId( "event" ), &CreateGenerator<EventGenerator> );
        Bind( AutoId( "message" ), &CreateGenerator<MessageGenerator> );
        Bind( AutoId( "factory" ), &CreateGenerator<FactoryGenerator> );
    }
};

template<typename Elem_T>
std::auto_ptr<Generator> GeneratorFactory::CreateGenerator( int32_t Id )
{
    std::auto_ptr<Generator> generator( new Elem_T() );
    return generator;
}

int main(int argc, char* argv[])
{
	namespace po=boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	std::string classUnderscore;
	std::string generatorType;
    std::string parentUnderscore;
    std::string targetUnderscore;
    std::string targetItemTypeUnderscore;
    std::string targetItemNameUnderscore;
    std::string namespaceLowerCase;
    std::string membersArg;
    std::string eventsArg;
	desc.add_options()
        ("help", "produce help message")
		("-c", po::value<std::string>(&classUnderscore), "class_name_underscore")
		("-g", po::value<std::string>(&generatorType), "type of generator")
		("-p", po::value<std::string>(&parentUnderscore), "parent_name_underscore")
        ("-t", po::value<std::string>(&targetUnderscore), "target_name_underscore")
        ("target_item_type", po::value<std::string>(&targetItemTypeUnderscore), "target_item_type")
        ("target_item_name", po::value<std::string>(&targetItemNameUnderscore), "target_item_name")
        ("-n", po::value<std::string>(&namespaceLowerCase), "namespacelowercase")
        ("-m", po::value<std::string>(&membersArg), "optional: members: \"double-radius int32_t-targetId\"")
        ("-e", po::value<std::string>(&eventsArg), "optional: events to subscribe: \"core-damageTaken\" -> AutoReg mOnDamageTaken; void OnDamageTaken (core::DamageTakenEvent const& Evt);")
		("generators:", 
        "\n*** default_generator ***\n does nothing\n" 
        "\n*** i_component ***\n class_name shall be in \"i_{the_name_underscore}_component\" format. generates a class_name_underscore.h with abstract member getters setters. guesses the parent to Component.\n" 
        "\n*** component ***\n class_name shall be in \"{the_name_underscore}_component\" format. generates a class_name_underscore.h and class_name_underscore.cpp with getters setters and member variables. guesses the parent to i_class_name_underscore if not set \n" 
        "\n*** system ***\n class_name shall be in \"{the_name_underscore}_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -t \"target_component_name_without_component\" (e.g. for drop_on_death_component: \"drop_on_death\")\n" 
        "\n*** collision_sub_system ***\n class_name shall be in \"{the_name_underscore}_collision_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -t \"target_component_name_without_collision_component\" (e.g. for shot_collision_component \"shot\")\n" 
        "\n*** controller_sub_system ***\n class_name shall be in \"{the_name_underscore}_controller_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -t \"target_component_name_without_controller_component\" (e.g. for random_controller_component \"random\")\n" 
        "\n*** normal_item ***\n class_name shall be in \"{the_name_underscore}_normal_item\" format. generates a class_name_underscore.h and class_name_underscore.cpp with getters setters and member variables. guesses the parent to normal_item if not set \n" 
        "\n*** normal_item_sub_system ***\n class_name shall be in \"{the_name_underscore}_normal_item_sub_system\" format. generates a class_name_underscore.h and class_name_underscore.cpp with overridden methods.\n  uses: -target_item_type, -target_item_name (e.g. for grenade_normal_item -target_item_type \"narmal_item\" -target_item_name \"grenade\")\n" 
        "\n*** event ***\n class_name shall be in \"{the_name_underscore}_event\" format. generates a class_name_underscore.h with constructor for memebers.\n  uses: -m members)\n" 
        "\n*** message ***\n class_name shall be in \"{the_name_underscore}\" format. generates a class_name_message_underscore.h class_name_message_underscore.cpp with message, messagehandler, messagesender.\n  uses: -m members -e events - subscribing to event)\n" 
        "\n*** factory ***\n class_name shall be in \"{the_name_underscore}_factory\" format. generates a class_name_underscore.h class_name_underscore.cpp.\n  uses: -t \"target_generated_class\" - base of the genereted classes by this factory)\n" 
        //"\n\n\n"
           )
        ;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    if (!vm.count("-g")) {
        L1("need to specify a valid generator!");
        return 2;
    }

    std::auto_ptr<Generator> generator(GeneratorFactory::Get()(AutoId(generatorType)));
    generator->classUnderscore=classUnderscore;
    generator->parentUnderscore=parentUnderscore;
    generator->namespaceLowerCase=namespaceLowerCase;
    generator->membersArg=membersArg;
    generator->eventsArg=eventsArg;
    generator->targetUnderscore=targetUnderscore;
    generator->targetItemTypeUnderscore=targetItemTypeUnderscore;
    generator->targetItemNameUnderscore=targetItemNameUnderscore;
    generator->Generate();
	return 0;
}