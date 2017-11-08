/*
@(#)File:           $RCSfile: debug.h,v $
@(#)Version:        $Revision: 3.6 $
@(#)Last changed:   $Date: 2008/02/11 06:46:37 $
@(#)Purpose:        Definitions for the debugging system
@(#)Author:         J Leffler
@(#)Copyright:      (C) JLSS 1990-93,1997-99,2003,2005,2008
@(#)Product:        :PRODUCT:
*/

#ifndef ESTACAO_DEBUG_H
#define ESTACAO_DEBUG_H

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define TRACE(fmt, ...) \
        do { if (DEBUG_TEST) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__); } while (0)
#define debug_printf(fmt, ...) \
            do { if (DEBUG_TEST) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

#endif //ESTACAO_DEBUG_H