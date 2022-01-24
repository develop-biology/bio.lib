#pragma once

#include "RegisterableTypes.h"

class NonTemplateRegisterable;

//A Brain Register is a simple object responsible for creating new instances
//  of Registerable derived classes.
//T must be a child of BaseBrain

class BaseRegister
{
public:
    //Brain name is the name of the BaseBrain derived class
    BaseRegister(const char* brainName, RegisterableType type) :
        m_name(brainName),
        m_type(type)
    {}
    virtual ~BaseRegister() {}

    //RETURNS: a new Regsterable derived class
    virtual NonTemplateRegisterable* operator() () {return NULL;}

    const char* GetName() const {return m_name;}
    RegisterableType GetType() const {return m_type;}

protected:
    const char* m_name;
    RegisterableType m_type;
};

template <class T>
class Register : public BaseRegister
{
public:
    //Brain name is the name of the BaseBrain derived class
    Register(const char* name, RegisterableType type) :
        BaseRegister(name, type)
    {}
    virtual ~Register() {}

    //RETURNS: a new BaseBrain derived class
    //TODO: confirm T is a BaseBrain at compile time
    virtual NonTemplateRegisterable* operator() () {return new T();}
};
