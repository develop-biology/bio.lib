#include "registration/Registrar.h"
#include "registration/Register.h"
#include "registration/Registerable.h"

#include <cstring>
#include <iostream>
#include <sstream>

Registrar::Registrar() {}

/*static*/ Registrar& Registrar::Instance()
{
    static Registrar instance;
    return instance;
}

void Registrar::AddRegister(BaseRegister* newRegister) {
    if (GetRegister(newRegister->GetName(), newRegister->GetType())) {
        //TODO: Throw error
        return;
    }
    m_registry.push_back(newRegister);
    // std::cout << "done" << std::endl;
}


BaseRegister* Registrar::GetRegister(const char* name, RegisterableType type) {
    Registry::iterator reg = GetIteratorFor(name, type);
    if (reg != m_registry.end())
        return *reg;
    return NULL;
}

//Find the register of the given name and instantiate its brain
NonTemplateRegisterable* Registrar::Create(
    const char* name,
    RegisterableType type)
{
    Registry::iterator reg = GetIteratorFor(name, type);
    if (reg != m_registry.end())
        return (**reg)();
    return NULL;
}

Registry::iterator Registrar::GetIteratorFor(
    const char* name,
    RegisterableType type)
{
    for (Registry::iterator reg = m_registry.begin();
        reg != m_registry.end(); ++reg)
    {
        if ((*reg)->GetType() == type && strcmp(name, (*reg)->GetName()) == 0)
            return reg;
    }
    return m_registry.end();
}

//RETURNS: all available Registers
StdStrings Registrar::GetFullRegistry() const
{
    StdStrings ret;
    std::ostringstream buffer;
    for (Registry::const_iterator reg = m_registry.begin();
        reg != m_registry.end(); ++reg)
    {
        buffer
            << (*reg)->GetName()
            << " : "
            << RegisterableTypeToString((*reg)->GetType());
        ret.push_back(buffer.str());
        buffer.str("");
        buffer.clear();
    }
    return ret;
}

//RETURNS: all available registers of the given type
StdStrings Registrar::GetRegistryFor(RegisterableType type) const
{
    StdStrings ret;
    std::ostringstream buffer;
    for (Registry::const_iterator reg = m_registry.begin();
        reg != m_registry.end(); ++reg)
    {
        if ((*reg)->GetType() != type)
            continue;

        buffer
            << (*reg)->GetName()
            << " : "
            << RegisterableTypeToString((*reg)->GetType());
        ret.push_back(buffer.str());
        buffer.str("");
        buffer.clear();
    }
    return ret;
}
