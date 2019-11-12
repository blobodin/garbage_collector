#include "eval_types.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>

#include "config.h"
#include "eval_dict.h"
#include "eval_list.h"
#include "eval_refs.h"
#include "exception.h"
#include "refs.h"

//// TYPE-SPECIFIC FUNCTIONS ////

const char *type_to_str(value_type_t type) {
    switch (type) {
        case VAL_NONE:    return "NoneType";
        case VAL_BOOL:    return "bool";
        case VAL_INTEGER: return "int";
        case VAL_STRING:  return "str";
        case VAL_LIST:    return "list";
        case VAL_DICT:    return "dict";
        default:          return "<unknown>";
    }
}

// SINGLETON FUNCTIONS //

static reference_t singleton_to_ref_borrow(SingletonType type) {
    switch (type) {
        case S_NONE:  return NONE_REF;
        case S_TRUE:  return TRUE_REF;
        case S_FALSE: return FALSE_REF;
    }

    exception_set(EXC_INTERNAL, "unknown singleton type");
    return NULL_REF;
}
reference_t singleton_to_ref(SingletonType type) {
    reference_t result = singleton_to_ref_borrow(type);
    incref(result);
    return result;
}

static bool singleton_bool(value_t *obj) {
    assert(obj->type == VAL_NONE || obj->type == VAL_BOOL);

    /* Of all the singletons, only True is truthy. */
    return obj == deref(TRUE_REF);
}

static uint64_t singleton_hash(value_t *obj) {
    return obj->type == VAL_NONE ? NONE_REF : bool_ref(singleton_bool(obj));
}

static int singleton_cmp(value_t *l, value_t *r) {
    assert((l->type == VAL_NONE || l->type == VAL_BOOL) && l->type == r->type);

    return singleton_bool(l) - singleton_bool(r);
}
static bool singleton_eq(value_t *l, value_t *r) {
    assert(l->type == VAL_NONE || l->type == VAL_BOOL);
    assert(r->type == VAL_NONE || r->type == VAL_BOOL);

    return l == r;
}

/*! Implements printing for singletons (None and boolean values). */
static void singleton_print(value_t *obj, FILE *stream, size_t depth) {
    assert(obj->type == VAL_NONE || obj->type == VAL_BOOL);

    /* If we have reached the maximum recursion depth, print a placeholder. */
    if (depth == 0) {
        fprintf(stream, "...");
        return;
    }

    /* Otherwise, print an appropriate textual representation. */
    if (obj == deref(NONE_REF)) {
        fprintf(stream, "None");
    } else if (obj == deref(TRUE_REF)) {
        fprintf(stream, "True");
    } else if (obj == deref(FALSE_REF)) {
        fprintf(stream, "False");
    } else {
        /* If we reach here then something is very wrong. Memory corruption? */
        UNREACHABLE();
    }
}

/*! Returns the appropriate reference to a bool value. */
reference_t bool_ref(bool value) {
    reference_t result = value ? TRUE_REF : FALSE_REF;
    incref(result);
    return result;
}

// INTEGER FUNCTIONS //

static inline integer_value_t *integer_coerce(value_t *obj) {
    assert(obj->type == VAL_INTEGER);
    return (integer_value_t *) obj;
}

static bool integer_bool(value_t *obj) {
    /* First ensure that this is actually and integer_value_t. */
    integer_value_t *integer = integer_coerce(obj);

    /* An integer is truthy if it is not zero. */
    return integer->integer_value != 0;
}

static uint64_t integer_hash(value_t *obj) {
    return integer_coerce(obj)->integer_value;
}

static reference_t integer_unaryop_negate(value_t *l, value_t *r) {
    (void) r;
    return make_reference_int(-integer_coerce(l)->integer_value);
}
static reference_t integer_unaryop_identity(value_t *l, value_t *r) {
    (void) r;
    return make_reference_int(+integer_coerce(l)->integer_value);
}

static int integer_cmp(value_t *l, value_t *r) {
    int64_t lval = integer_coerce(l)->integer_value,
            rval = integer_coerce(r)->integer_value;
    return (lval > rval) - (lval < rval);
}
static bool integer_eq(value_t *l, value_t *r) {
    return integer_cmp(l, r) == 0;
}

static reference_t integer_binop_add(value_t *l, value_t *r) {
    return make_reference_int(
            integer_coerce(l)->integer_value + integer_coerce(r)->integer_value);
}
static reference_t integer_binop_subtract(value_t *l, value_t *r) {
    return make_reference_int(
            integer_coerce(l)->integer_value - integer_coerce(r)->integer_value);
}
static reference_t integer_binop_multiply(value_t *l, value_t *r) {
    return make_reference_int(
            integer_coerce(l)->integer_value * integer_coerce(r)->integer_value);
}
static reference_t integer_binop_divide(value_t *l, value_t *r) {
    return make_reference_int(
            integer_coerce(l)->integer_value / integer_coerce(r)->integer_value);
}
static reference_t integer_binop_modulo(value_t *l, value_t *r) {
    return make_reference_int(
            integer_coerce(l)->integer_value % integer_coerce(r)->integer_value);
}

/*! Implements printing for integers. */
static void integer_print(value_t *obj, FILE *stream, size_t depth) {
    /* If we have reached the maximum recursion depth, print a placeholder. */
    if (depth == 0) {
        fprintf(stream, "...");
        return;
    }

    /* Otherwise, ensure that this is actually an integer_value_t. */
    integer_value_t *integer = integer_coerce(obj);

    /* If it is, print an appropriate textual representation. */
    fprintf(stream, "%" PRIi64, integer->integer_value);
}

// STRING FUNCTIONS //

static inline string_value_t *string_coerce(value_t *obj) {
    assert(obj->type == VAL_STRING);
    return (string_value_t *) obj;
}

static bool string_bool(value_t *obj) {
    return string_coerce(obj)->string_value[0] != '\0';
}

static uint64_t string_hash(value_t *obj) {
    /* Adapted from Java's String.hashCode(). */
    string_value_t *str = string_coerce(obj);

    uint64_t hash = 1125899906842597UL; // prime
    for (const char *sp = str->string_value; *sp; sp++) {
        hash = 31 * hash + *sp;
    }
    return hash;
}

static int64_t string_len(value_t *obj) {
    return strlen(string_coerce(obj)->string_value);
}

static int string_cmp(value_t *l, value_t *r) {
    return strcmp(string_coerce(l)->string_value, string_coerce(r)->string_value);
}
static bool string_eq(value_t *l, value_t *r) {
    return string_cmp(l, r) == 0;
}

static reference_t string_binop_add(value_t *l, value_t *r) {
    return make_reference_string_concat(
            string_coerce(l)->string_value,
            string_coerce(r)->string_value);
}

static void string_print_gen(const char *format, value_t *obj, FILE *stream, size_t depth) {
    /* If we have reached the maximum recursion depth, print a placeholder. */
    if (depth == 0) {
        fprintf(stream, "...");
        return;
    }

    /* Otherwise, ensure that this is actually a string_value_t. */
    string_value_t *string = string_coerce(obj);

    /* If it is, print it out. */
    fprintf(stream, format, string->string_value);
}

static void string_print_repr(value_t *obj, FILE *stream, size_t depth) {
    string_print_gen("\"%s\"", obj, stream, depth);
}
static void string_print(value_t *obj, FILE *stream, size_t depth) {
    string_print_gen("%s", obj, stream, depth);
}

//// TYPE FUNCTION LISTINGS ////

/*!
 * This is a union that holds information about the available builtin
 * operations for a particular type.
 */
typedef union builtin_table {
    struct {
        reference_t (*u_negate  )(value_t *l, value_t *r);
        reference_t (*u_identity)(value_t *l, value_t *r);

        reference_t (*b_add     )(value_t *l, value_t *r);
        reference_t (*b_subtract)(value_t *l, value_t *r);
        reference_t (*b_multiply)(value_t *l, value_t *r);
        reference_t (*b_divide  )(value_t *l, value_t *r);
        reference_t (*b_modulo  )(value_t *l, value_t *r);
    };
    reference_t (*f_table[OP_MODULO + 1])(value_t *l, value_t *r);
} builtin_table_t;

typedef struct func_table {
    bool        (*f_bool      )(value_t *obj);
    int64_t     (*f_len       )(value_t *obj);
    uint64_t    (*f_hash      )(value_t *obj);

    int         (*f_cmp       )(value_t *l, value_t *r);
    bool        (*f_eq        )(value_t *l, value_t *r);

    builtin_table_t f_builtins;

    reference_t (*f_subscr_get)(value_t *obj, reference_t subscript);
    void        (*f_subscr_set)(value_t *obj, reference_t subscript, reference_t value);
    void        (*f_subscr_del)(value_t *obj, reference_t subscript);

    void        (*f_print_repr)(value_t *obj, FILE *stream, size_t depth);
    void        (*f_print     )(value_t *obj, FILE *stream, size_t depth);
} func_table_t;

static func_table_t table[NUM_TYPES];

/*!
 * This function setups the function tables used to handle operations that vary
 * between the different types of values supported by Subpython.
 *
 * Note that the singleton references are actually set by `eval_init`, not
 * this function.
 */
void eval_types_init() {
    memset(table, 0, sizeof(table));

    table[VAL_NONE] = (func_table_t) {
        .f_bool       = &singleton_bool,
        .f_hash       = &singleton_hash,
        .f_cmp        = &singleton_cmp,
        .f_eq         = &singleton_eq,
        .f_print_repr = &singleton_print,
        .f_print      = &singleton_print
    };
    table[VAL_BOOL] = table[VAL_NONE];
    table[VAL_INTEGER] = (func_table_t) {
        .f_bool         = &integer_bool,
        .f_hash         = &integer_hash,
        .f_cmp          = &integer_cmp,
        .f_eq           = &integer_eq,
        .f_builtins     = (builtin_table_t) {
            .u_negate   = &integer_unaryop_negate,
            .u_identity = &integer_unaryop_identity,
            .b_add      = &integer_binop_add,
            .b_subtract = &integer_binop_subtract,
            .b_multiply = &integer_binop_multiply,
            .b_divide   = &integer_binop_divide,
            .b_modulo   = &integer_binop_modulo,
        },
        .f_print_repr = &integer_print,
        .f_print      = &integer_print,
    };
    table[VAL_STRING] = (func_table_t) {
        .f_bool       = &string_bool,
        .f_len        = &string_len,
        .f_hash       = &string_hash,
        .f_cmp        = &string_cmp,
        .f_eq         = &string_eq,
        .f_builtins   = (builtin_table_t) {
            .b_add    = &string_binop_add
        },
        .f_print_repr = &string_print_repr,
        .f_print      = &string_print
    };
    table[VAL_LIST] = (func_table_t) {
        .f_bool       = &list_bool,
        .f_len        = &list_len,
        .f_cmp        = &list_cmp,
        .f_eq         = &list_eq,
        .f_subscr_get = &list_subscr_get,
        .f_subscr_set = &list_subscr_set,
        .f_subscr_del = &list_subscr_del,
        .f_print_repr = &list_print,
        .f_print      = &list_print
    };
    table[VAL_DICT] = (func_table_t) {
        .f_bool       = &dict_bool,
        .f_len        = &dict_len,
        .f_eq         = &dict_eq,
        .f_subscr_get = &dict_subscr_get,
        .f_subscr_set = &dict_subscr_set,
        .f_subscr_del = &dict_subscr_del,
        .f_print_repr = &dict_print,
        .f_print      = &dict_print
    };
}

//// GENERIC DISPATCH FUNCTIONS ////

bool ref_is_none(reference_t r) {
    return r == NONE_REF;
}
bool ref_is_true(reference_t r) {
    return r == TRUE_REF;
}
bool ref_is_false(reference_t r) {
    return r == FALSE_REF;
}

/*! Return the type of the value pointed to by the provided reference. */
value_type_t ref_type(reference_t r) {
    return deref(r)->type;
}

/*! Return the result of coercing the reference to a bool. */
bool ref_bool(reference_t r) {
    /* Attempt to dereference the provided reference. */
    value_t *obj = deref(r);

    /* If the bool function is not set, then error out. */
    if (table[obj->type].f_bool == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "object of type '%s' has no bool()", type_to_str(obj->type));
        return NULL_REF;
    }

    /* Otherwise, dispatch to function. */
    return table[obj->type].f_bool(obj);
}

/*!
 * Return the result of calling len() on the provided reference.
 */
int64_t ref_len(reference_t r) {
    /* Attempt to dereference the provided reference. */
    value_t *obj = deref(r);

    /* If the function table is not set for this type or the print function
     * is not set, then error out. */
    if (table[obj->type].f_len == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "object of type '%s' has no len()", type_to_str(obj->type));
        return NULL_REF;
    }

    /* Otherwise, dispatch to function. */
    return table[obj->type].f_len(obj);
}

/*!
 * Return the result of calling hash() on the provided reference.
 */
uint64_t ref_hash(reference_t r) {
    /* Attempt to dereference the provided reference. */
    value_t *obj = deref(r);

    /* If the function table is not set for this type or the print function
     * is not set, then error out. */
    if (table[obj->type].f_hash == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "unhashable type: '%s'", type_to_str(obj->type));
        return NULL_REF;
    }

    /* Otherwise, dispatch to function. */
    return table[obj->type].f_hash(obj);
}

static const char *builtin_to_str(NodeExprBuiltinType type) {
    switch (type) {
        case UOP_NEGATE:   return "-";
        case UOP_IDENTITY: return "+";

        case COMP_EQUALS:  return "==";
        case COMP_LT:      return "<";
        case COMP_GT:      return ">";
        case COMP_LE:      return "<=";
        case COMP_GE:      return ">=";

        case OP_ADD:       return "+";
        case OP_SUBTRACT:  return "-";
        case OP_MULTIPLY:  return "*";
        case OP_DIVIDE:    return "/";
        case OP_MODULO:    return "%";

        default:           return "<unk>";
    }
}

/*!
 * Return the result of executing the specified builtin operation.
 */
reference_t ref_builtin(NodeExprBuiltinType type, reference_t l, reference_t r) {
    /* Attempt to dereference the provided references. */
    value_t *lobj = deref(l);
    value_t *robj = deref(r);

    /* First, we make the simplifying assumption that the two values must have
     * the same type, which holds for all operations current available in
     * Subpython. Then, if the appropriate builtin function is not set for this
     * type, error out. */
    if (lobj->type != robj->type || table[lobj->type].f_builtins.f_table[type] == NULL) {
        if (is_unary_builtin(type)) {
            exception_set_format(EXC_TYPE_ERROR,
                    "bad operand type for unary %s: '%s'",
                    builtin_to_str(type), type_to_str(lobj->type));
        } else {
            exception_set_format(EXC_TYPE_ERROR,
                    "unsupported operand type(s) for %s: '%s' and '%s'",
                    builtin_to_str(type), type_to_str(lobj->type), type_to_str(robj->type));
        }
        return NULL_REF;
    }

    /* Otherwise, dispatch to function. */
    return table[lobj->type].f_builtins.f_table[type](lobj, robj);
}

/*!
 * Compares the values at two references.
 * Return value is negative if the first is less,
 * positive if the first is greater, and 0 if they are equal.
 */
int compare(reference_t l, reference_t r) {
    /* Attempt to dereference the provided references. */
    value_t *lobj = deref(l);
    value_t *robj = deref(r);

    /* If the operands have different types or can't be compared, error out. */
    int (*f_cmp)(value_t *, value_t *) = table[lobj->type].f_cmp;
    if (f_cmp == NULL || lobj->type != robj->type) {
        exception_set_format(EXC_TYPE_ERROR,
                "type(s) are not comparable: '%s' and '%s'",
                type_to_str(lobj->type), type_to_str(robj->type));
        return 0;
    }

    return f_cmp(lobj, robj);
}

/*! Returns the result of executing the builtin compare operation. */
reference_t ref_compare(NodeExprBuiltinType type, reference_t l, reference_t r) {
    int comparison = compare(l, r);
    if (exception_occurred()) {
        return NULL_REF;
    }

    switch (type) {
        case COMP_EQUALS:
            return bool_ref(comparison == 0);
        case COMP_LT:
            return bool_ref(comparison < 0);
        case COMP_GT:
            return bool_ref(comparison > 0);
        case COMP_LE:
            return bool_ref(comparison <= 0);
        case COMP_GE:
            return bool_ref(comparison >= 0);
        default:
            UNREACHABLE();
    }
}

/*! Returns the result of comparing two objects for equality. */
bool ref_eq(reference_t l, reference_t r) {
    /* Attempt to dereference the provided references. */
    value_t *lobj = deref(l);
    value_t *robj = deref(r);

    /* If the operands have different types, then return false. */
    if (lobj->type != robj->type) {
        return false;
    }

    /* If the object type doesn't support equality comparison, then error
     * out. */
    if (table[lobj->type].f_eq == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "unsupported operand types for '==': '%s' and '%s'",
                type_to_str(lobj->type), type_to_str(robj->type));
        return false;
    }

    return table[lobj->type].f_eq(lobj, robj);
}

/*!
 * Return the result of a subscript access to an object.
 */
reference_t ref_subscr_get(reference_t r, reference_t subscr) {
    /* Attempt to dereference the provided reference. */
    value_t *obj = deref(r);

    /* If the subscript get function is not set, then error out. */
    if (table[obj->type].f_subscr_get == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "'%s' object is not subscriptable", type_to_str(obj->type));
        return NULL_REF;
    }

    /* Otherwise, dispatch to function. */
    return table[obj->type].f_subscr_get(obj, subscr);
}

/*!
 * Return the result of a subscript assignment to an object.
 */
void ref_subscr_set(reference_t r, reference_t subscr, reference_t value) {
    /* Attempt to dereference the provided reference. */
    value_t *obj = deref(r);

    /* If the subscript set function is not set, then error out. */
    if (table[obj->type].f_subscr_set == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "'%s' object does not support item assignment", type_to_str(obj->type));
        return;
    }

    /* Otherwise, dispatch to function. */
    table[obj->type].f_subscr_set(obj, subscr, value);
}

/*!
 * Execute the deletion of an item from an object.
 */
void ref_subscr_del(reference_t r, reference_t subscr) {
    /* Attempt to dereference the provided reference. */
    value_t *obj = deref(r);

    /* If the subscript del function is not set, then error out. */
    if (table[obj->type].f_subscr_del == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "'%s' object does not support item deletion", type_to_str(obj->type));
        return;
    }

    /* Otherwise, dispatch to function. */
    table[obj->type].f_subscr_del(obj, subscr);
}

/*!
 * Print the provided reference to the provided stream, recursing to some
 * limited depth.
 */
void ref_print_repr(reference_t r, FILE *stream, size_t depth) {
    /* Attempt to dereference the provided reference. */
    value_t *obj = deref(r);

    /* If the function table is not set for this type or the print function
     * is not set, then error out. */
    if (table[obj->type].f_print_repr == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "cannot print value of type '%s'", type_to_str(obj->type));
        return;
    }

    /* Otherwise, dispatch to function. */
    table[obj->type].f_print_repr(obj, stream, depth);
}

/*!
 * Print the provided reference to the provided stream, recursing to some
 * limited depth.
 */
void ref_print(reference_t r, FILE *stream, size_t depth) {
    /* Attempt to dereference the provided reference. */
    value_t *obj = deref(r);

    /* If the function table is not set for this type or the print function
     * is not set, then error out. */
    if (table[obj->type].f_print == NULL) {
        exception_set_format(EXC_TYPE_ERROR,
                "cannot print value of type '%s'", type_to_str(obj->type));
        return;
    }

    /* Otherwise, dispatch to function. */
    table[obj->type].f_print(obj, stream, depth);
}

/*! Print as in ref_print_repr but with an additional newline. */
void ref_println_repr(reference_t r, FILE *stream, size_t depth) {
    /* First print the value. */
    ref_print_repr(r, stream, depth);

    /* Then a newline. */
    fprintf(stream, "\n");
}
/*! Print as in ref_print but with an additional newline. */
void ref_println(reference_t r, FILE *stream, size_t depth) {
    /* First print the value. */
    ref_print(r, stream, depth);

    /* Then a newline. */
    fprintf(stream, "\n");
}
