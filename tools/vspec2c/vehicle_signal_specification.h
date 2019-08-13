// (C) 2016 Jaguar Land Rover
//
// All files and artifacts in this repository are licensed under the
// provisions of the license provided by the LICENSE file in this repository.
//
// Header file to inspect a vehicle signal specification encoded directly in C, with
// code generated by vspec2c.py
//
#ifndef __VEHICLE_SIGNAL_SPECIFICATION__
#define __VEHICLE_SIGNAL_SPECIFICATION__
#include <stdint.h>
#include <float.h>
#include <errno.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _vss_data_type_e {
    VSS_INT8 = 0,
    VSS_UINT8 = 1,
    VSS_INT16 = 2,
    VSS_UINT16 = 3,
    VSS_INT32 = 4,
    VSS_UINT32 = 5,
    VSS_DOUBLE = 6,
    VSS_FLOAT = 7,
    VSS_BOOLEAN = 8,
    VSS_STRING = 9,
    VSS_STREAM = 10,
    // Usd for VSS_BRANCH and VSS_RBRANCH etnries
    VSS_NA = 11,
} vss_data_type_e;
typedef enum _vss_element_type_e {
    VSS_ATTRIBUTE = 0,
    VSS_BRANCH = 1,
    VSS_SENSOR = 2,
    VSS_ACTUATOR = 3,
    VSS_RBRANCH = 4,
    VSS_ELEMENT = 5,
} vss_element_type_e;
// A single signal.
// The vss_signal[] array hosts all the signals from the specification.
// The array is pre-populated and initialized at build time, meaning
// that it can be accessed directly from a program without any
// further initialization.
//
typedef struct _vss_signal_t {
    // Unique index for a signal. Based on the signal's position
    // in the array.
    //
    const int index;
    // Pointer to parent signal. Null if this is root signal
    //
    struct _vss_signal_t* parent;
    // Pointer to null-termianted array of all children.
    // Traverse using children[index].
    // If there are no children, then children[0] == 0
    //
    struct _vss_signal_t** children;
    // Name of this signal or branch.
    // Use vss_get_signal_path() to get complete path to a signal.
    // Always set.
    //
    const char *name;
    // UUID of signal or branch.
    // Always set.
    //
    const char *uuid;
    // Element type of signal
    // Use this to determine if this is a signal or a branch.
    //
    const vss_element_type_e element_type;
    // Data type of signal.
    // Will be set to VSS_NA if this is a branch.
    //
    const vss_data_type_e data_type;
    // Unit type of signal.
    // Set to "" if not definedf.
    //
    const char *const unit_type;
    // Minimum allowed value for a signal.
    // Use min_val.d if signal.data_type is VSS_FLOAT or VSS_DOUBLE.
    // Use min_val.i if signal.data_type is one of the integer types.
    //
    const union  {
        int64_t i;
        double d;
    } min_val;
    // Maximum allowed value for a signal.
    // Use max_val.d if signal.data_type is VSS_FLOAT or VSS_DOUBLE.
    // Use max_val.i if signal.data_type is one of the integer types.
    //
    const union {
        int64_t i;
        double d;
    } max_val;
    // Signal description.
    // Set to "" if not specified.
    //
    const char* description;
    // Pointer to null-termianted array of values that signal can have
    // Traverse using enum_values[index].
    // If there are no enumerated values specifed, then enum_values[0] == 0
    //
    const char* const* enum_values;
    // Sensor specification of signal.
    // Set to "" if not specified.
    //
    const char* sensor;
    // Actuator specification of signal.
    // Set to "" if not specified.
    //
    const char* actuator;
    // User Data element that can be used to
    // connect the signal to application-managed structures.
    void* user_data;
} vss_signal_t;
// Return a signal struct pointer based on signal index.
//
// The index of a signal is available in its 'index'
// struct member and is guaranteed to have the same
// signal - index mapping across all vspec2c-generated
// code for a given signal specification.
//
// Use index 0 to get root signal ("Vehicle")
//
// The returned signal can have its members inspected (but not modified).
//
// The 'children' member will always be a null terminated array
// of vsd_signal_t pointers that can be traversed by accessing
// sig->children[index] untila null pointer is encountered.
// If there are no children sig->children[0] will be null.
//
// The 'enum_values' member will likewise be a null terminated
// array of char pointers with the enum values.
// If there are no enum values sig->enum_values[0] will be null.
//
// If the index argument is less than zero or greater to or equal
// than the size of the index array.
//
extern vss_signal_t* vss_get_signal_by_index(int index);
// Locate a signal by its path.
// Path is in the format "Branch.Branch.[...].Signal.
// If
extern int vss_get_signal_by_path(char* path,
                                   vss_signal_t ** result);
const char* vss_element_type_string(vss_element_type_e elem_type);
const char* vss_data_type_string(vss_data_type_e data_type);
// Populate the full path name to the given signal.
// The name will be stored in 'result'.
// No more than 'result_max_len' bytes will be copied.
// The copied name will always be null terminated.
// 'result' is returned.
// In case of error, an empty string is copiec into result.
extern char* vss_get_signal_path(vss_signal_t* signal,
                                 char* result,
                                 int result_max_len);
// A unique hash generated across all signals' combined
// UUID values.
// This signature can be used by two networked systems to
// verify that they are both using the same signal
// specification version.
//
extern const char* vss_get_sha256_signature(void);
// The number of signals in vss_signal array.
// The max value for vss_get_signal_by_index() is
// the return value of this function - 1.
extern const int vss_get_signal_count(void);

extern vss_signal_t vss_signal[];

// Tag to denote that a signal's min_value or max_value has
// not been specified.
#define VSS_LIMIT_UNDEFINED INT64_MIN

#ifdef __cplusplus
}
#endif

#endif
