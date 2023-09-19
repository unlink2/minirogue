#ifndef DEFAULTS_H_
#define DEFAULTS_H_

#include "idc.h"

#define MRG_DEFAULT_ROOM_W 40
#define MRG_DEFAULT_ROOM_H 30

// indicates that the idc file is the default
// and should not be freed
#define MRG_IDC_DEFAULT_VERSION (-1)
/**
 * Defines a default idc file
 * that can be loaded as a starting point or fallback
 */

struct mrg_idc_file mrg_default_idc(void);

#endif
