#pragma once

#include "RegisterableTypes.h"
#include <vector>
#include <string>

class BaseRegister;
class NonTemplateRegisterable;

typedef std::vector<BaseRegister*> Registry;
typedef std::vector<std::string> StdStrings;

//The Registrar is a singleton that holds all Registers.
//Registerable derived classes will register themselves with *this and can be
//  instantiated by calling Create(...)
//For how to create a Registerable class, see Registerable.h
class Registrar
{
public:
    static Registrar& Instance();

    //Add a BrainRegister to the registry
    //This method checks for uniqueness among the name and type of the Register,
    //  thus you cannot register multiple classes of the same name and type.
    //You should not have to worry about this method, as a Registerable class
    //  will call this for you automatically.
    void AddRegister(BaseRegister* newRegister);

    //RETURNS: a register of the given name and type (which are unique)
    BaseRegister* GetRegister(const char* name, RegisterableType type);

    //Find the register of the given name and instantiate it
    NonTemplateRegisterable* Create(const char* name, RegisterableType type);

    //RETURNS: all available Registers
    StdStrings GetFullRegistry() const;

    //RETURNS: all available registers of the given type
    StdStrings GetRegistryFor(RegisterableType type) const;

private:
    Registry m_registry;

    Registry::iterator GetIteratorFor(const char* name, RegisterableType type);

    //Make this a singleton
    Registrar();

    // Override default copy constructor and assignment operator so that nobody
    // can make a copy of the singleton (otherwise it wouldn't be a singleton).
    // We don't define them, so these methods will give a link error if used.
    Registrar(Registrar const &);
    void operator=(Registrar const &);

};
