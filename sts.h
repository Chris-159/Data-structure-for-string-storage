#pragma once
#ifndef STS_H_INCLUDED
#define STS_H_INCLUDED

#include "sts_type.h"

TableStore TSCreate(ULONG_t table_count);
STATUS_t TSDestroy(TableStore *ts_obj);
ULONG_t TSGetTableSize(TableStore ts_obj);
ULONG_t TSGetTableCount(TableStore ts_obj);

STATUS_t TSSearchAndSaveEntry(TableStore ts_obj, char *word_);

#endif // STS_H_INCLUDED