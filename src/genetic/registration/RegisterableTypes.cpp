#include "registration/RegisterableTypes.h"

#include <cstring>

const char* RegisterableTypeToString(RegisterableType type)
{
    if (type >= REGISTERABLE_TYPES_TOTAL)
        return "UNKNOWN";
    return g_registerableTypeNames[type];
}
RegisterableType RegisterableTypeFromString(const char* typeAsString)
{
    for (int rtp = 0; rtp < REGISTERABLE_TYPES_TOTAL; ++rtp) {
        if (strcmp(typeAsString, g_registerableTypeNames[rtp]) == 0)
            return static_cast<RegisterableType>(rtp);
    }
    return REGISTERABLE_TYPES_TOTAL;
}
