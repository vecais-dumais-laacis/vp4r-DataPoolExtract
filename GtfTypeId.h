////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Elektrobit Automotive GmbH
// Alle Rechte vorbehalten. All Rights Reserved.
//
// Information contained herein is subject to change without notice.
// Elektrobit retains ownership and all other rights in the software and each
// component thereof.
// Any reproduction of the software or components thereof without the prior
// written permission of Elektrobit is prohibited.
////////////////////////////////////////////////////////////////////////////////

#ifndef GTF_INCLUDED_GTF_TYPE_ID_H
#define GTF_INCLUDED_GTF_TYPE_ID_H

#include <GtfTypes/GtfStdDef.h>

/**
 * \brief Optimize GtfTypes.
 *
 * GtfTypeIds are used to optimize GtfTypes. Use GtfTypeManager to get a GtfType object
 * for a GtfTypeId.
 */
class GtfTypeId
{
    public:
        typedef uint32_t typeId_t;

        enum ETypeId
        {
            // basic scalar types
            eTypeId_bool    = 0,
            eTypeId_uint8   = 1,
            eTypeId_uint16  = 2,
            eTypeId_uint32  = 3,
            eTypeId_uint64  = 4,
            eTypeId_int8    = 5,
            eTypeId_int16   = 6,
            eTypeId_int32   = 7,
            eTypeId_int64   = 8,
            eTypeId_float   = 9,
            eTypeId_double  = 10,
            eTypeId_string  = 11,
            eTypeId_data    = 12,
            eTypeId_uintptr = 13,

            // basic list types
            eTypeId_list_bool    = 14,
            eTypeId_list_uint8   = 15,
            eTypeId_list_uint16  = 16,
            eTypeId_list_uint32  = 17,
            eTypeId_list_uint64  = 18,
            eTypeId_list_uintptr = 19,  // not defined in adapter
            eTypeId_list_int8    = 20,
            eTypeId_list_int16   = 21,
            eTypeId_list_int32   = 22,
            eTypeId_list_int64   = 23,
            eTypeId_list_float   = 24,  // not defined in adapter
            eTypeId_list_double  = 25,  // not defined in adapter
            eTypeId_list_string  = 26,
            eTypeId_list_data    = 27, // not defined in adapter

            // misc types
            // not defined in adapter
            eTypeId_color           = 28,
            eTypeId_list_color      = 29,
            eTypeId_dp_id           = 30,
            eTypeId_view_id         = 31,
            eTypeId_statemachine_id = 32,
            eTypeId_popupstack_id   = 33,
            eTypeId_template_id     = 34,
            eTypeId_event_id        = 35,
            eTypeId_state_id        = 36,

            // resource types
            eTypeId_font       = 37,
            eTypeId_list_font  = 38,
            eTypeId_image      = 39,
            eTypeId_list_image = 40,

            // invalid type
            eTypeId_invalid = 41,

            // void type
            eTypeId_void      = 42,
            eTypeId_list_void = 43,

            // advanced types
            eTypeId_dp_list_element_const = 44,
            eTypeId_dp_list_element_item  = 45,

            eTypeId_animation = 46,

            // even more resource types, which cannot be put above
            eTypeId_model      = 47,
            eTypeId_list_model = 48,
            eTypeId_bytearray  = 49, // for internal use only

            eTypeId_propertyContainer = 50,
            eTypeId_widget            = 51,
            eTypeId_event             = 52,
            eTypeId_closure           = 53,

            eTypeId_autoassign = 54, // only for adding types to the TypeManager

            // still more resources types
            eTypeId_srgs_grammar = 55,
            eTypeId_list_srgs_grammar = 56,
            eTypeId_speech_hmi_slot = 57,
            eTypeId_speech_static_slot = 58,


            _NUM_builtin,

            _first_dynamic_distributed_TypeId = _NUM_builtin + 64
        };
};

// static associations of typeIds with POD C data types. Feel free to extend when necessary
template<typename T>
struct GtfTypeIdOf
{
    // no field typeId here, to generate a compile error when trying to use with not-yet defined type
};

template<>
struct GtfTypeIdOf<int32_t>
{
    static const GtfTypeId::typeId_t typeId = GtfTypeId::eTypeId_int32;
};

template<>
struct GtfTypeIdOf<uint32_t>
{
    static const GtfTypeId::typeId_t typeId = GtfTypeId::eTypeId_uint32;
};

template<>
struct GtfTypeIdOf<float>
{
    static const GtfTypeId::typeId_t typeId = GtfTypeId::eTypeId_float;
};

template<>
struct GtfTypeIdOf<bool>
{
    static const GtfTypeId::typeId_t typeId = GtfTypeId::eTypeId_bool;
};

template<int X>
struct GtfTypeIdOf<char[X]>
{
    static const GtfTypeId::typeId_t typeId = GtfTypeId::eTypeId_string;
};

#endif // GTF_INCLUDED_GTF_TYPE_ID_H
