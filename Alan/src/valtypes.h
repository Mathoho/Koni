/**
 * @file    valtypes.h
 * @brief   Value types for ALAN-2022 type checking.
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2022-08-03
 */

#ifndef VALTYPES_H
#define VALTYPES_H

typedef enum valtype {
	TYPE_NONE     = 0,
	TYPE_ARRAY    = 1,
	TYPE_BOOLEAN  = 2,
	TYPE_INTEGER  = 4,
	TYPE_CALLABLE = 8
} ValType;

#define IS_ARRAY(type)          (IS_ARRAY_TYPE(type) && !IS_CALLABLE_TYPE(type))
#define IS_ARRAY_TYPE(type)     ((type & TYPE_ARRAY) != 0) //TODO
#define IS_BOOLEAN_TYPE(type)   ((type & TYPE_BOOLEAN) != 0)//TODO
#define IS_CALLABLE_TYPE(type)  ((type & TYPE_CALLABLE) != 0)//TODO
#define IS_FUNCTION(type)       (IS_CALLABLE_TYPE(type) && !IS_PROCEDURE(type))
#define IS_INTEGER_TYPE(type)   ((type & TYPE_INTEGER) != 0)//TODO
#define IS_PROCEDURE(type)      (IS_CALLABLE_TYPE(type))//TODO
#define IS_VARIABLE(type)       (!IS_CALLABLE_TYPE(type))

#define SET_AS_ARRAY(type)      /* TODO */((type) |= TYPE_ARRAY)
#define SET_AS_CALLABLE(type)   ((type) |= TYPE_CALLABLE)
#define SET_BASE_TYPE(type)     /* TODO */
#define SET_RETURN_TYPE(type)   ((type) &= ~TYPE_CALLABLE)

/**
 * Returns a string representation of the specified value type.
 *
 * @param[in]   type
 *     the type for which to return a string representation
 * @return      a string representation of the specified value type
 */
const char *get_valtype_string(ValType type);

#endif /* VALTYPES_H */
