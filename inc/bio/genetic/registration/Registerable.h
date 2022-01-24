#pragma once

#include "Register.h"
#include "Registrar.h"
#include "RegisterableTypes.h"
#include <iostream>

//A Registerable is a class that can be registered with the Registrar
//These contain static objects that will be constructed and added to the
//  Registrar prior to the execution of main(...)
//NOTE: the derived class will not be constructed!

//Example Registerable class:
/*
class MyRegisterable : public Registerable<MyRegisterable>
{
public:
    MyRegisterable() {}
    virtual ~MyRegisterable() {}

    //MyType is a type that has been added to RegisterableType in
    //  RegisterableTypes.h
    REGISTRATION_METHODS(MyRegisterable, MY_TYPE)
};
*/

//Grants general access to templated Registerables.
class NonTemplateRegisterable
{
public:
    NonTemplateRegisterable() {}
    virtual ~NonTemplateRegisterable() {}
};

template <class T>
class Registerable : public NonTemplateRegisterable
{
public:
    Registerable() {}
    virtual ~Registerable() {}

    struct DoRegistration {
        DoRegistration() {
            // std::cout << "Registering " << T::Name() << "...";
            BaseRegister* newRegister = new Register<T>(T::Name(), T::Type());
            Registrar::Instance().AddRegister(newRegister);
        }

        //Confirming the registration is only necessary for msvc
        //This must be accessed from a compilation unit.
        bool ConfirmRegistration() {return true;}
    };

    template<DoRegistration&> struct RegReference {};

    static DoRegistration doesRegistration;
    static RegReference<doesRegistration> reffersToRegistration;
};

template<class T> typename Registerable<T>::DoRegistration
    Registerable<T>::doesRegistration;

//Add this macro to your Registerable derived class,
//  specifying name as the name of the derived class and type as the
//  RegisterableType in RegisterableTypes.h
#define REGISTRATION_METHODS(name, type) \
    static const char* Name() {return #name;} \
    static RegisterableType Type() {return type;}

//Confirming the registration is only necessary for msvc
//This must be added to a compilation unit.
//This will create yet another global variable that will access the
//  Registerable derived class, thus ensuring its registation.
//Specifically, this accesses DoRegistration::ConfirmRegistration()
#define CONFIRM_REGISTRATION(name) \
    struct Confirm_##name { \
        Confirm_##name() { \
            m_registrationConfirmed = \
                name::doesRegistration.ConfirmRegistration(); \
        } \
        bool m_registrationConfirmed; \
    };

//For more information on how this design was inspired, see:
//  https://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
//  and
//  https://stackoverflow.com/questions/401621/best-way-to-for-c-types-to-self-register-in-a-list/401801#401801
