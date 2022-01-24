#pragma once

//In the interest of avoiding RTTI issues, the types available to be
//  registered must be declared before hand.
//  This makes it easier to get the right instance from the Registrar.
//The Name:Type pair is unique within the Registrar,
//  so if you want to have 2 classes of the same name,
//  they must be different types.
//  However, as Name is the name of your class,
//  the only time this would be an issue is if
//  you are using the same name in multiple namespaces.
//  In otherwords, don't try to register classes of the same name and,
//  if you must, make sure they are different types.
typedef enum {
    BRAIN=0,
    REGISTERABLE_TYPES_TOTAL
} RegisterableType;

//When adding types to RegisterableTypes, you should also add the string
//  equivalent to g_registerableTypeNames. This enables the
//  RegisterableType[To/From]String functions.
static const char* const g_registerableTypeNames[REGISTERABLE_TYPES_TOTAL] = {
    "BRAIN"
};

const char* RegisterableTypeToString(RegisterableType type);
RegisterableType RegisterableTypeFromString(const char* typeAsString);
