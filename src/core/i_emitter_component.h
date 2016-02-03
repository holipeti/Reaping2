#ifndef INCLUDED_CORE_I_EMITTER_COMPONENT_H
#define INCLUDED_CORE_I_EMITTER_COMPONENT_H

#include "component.h"

class IEmitterComponent : public Component
{
public:
    DEFINE_COMPONENT_BASE(IEmitterComponent)
    virtual std::vector<int32_t> GetEmitTypes() const = 0;
    virtual void Emitted(std::vector<int32_t> emitTypes) = 0; //inform component that it has emitted these types
    virtual void Update( double dt ) = 0;
};

#endif//INCLUDED_CORE_I_EMITTER_COMPONENT_H

//command:  "../../build/tools/classgenerator/classgenerator" -g "i_component" -c "i_emitter_component" -m "int32_t-emitType bool-isemitting"
