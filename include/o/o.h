#ifndef O_O_H
#define O_O_H

/**
 * @file o.h
 *
 * Includes most of the "o" library.
 *
 * The "o" library contains a way for
 * object oriented and allocator tree based programming.
 * As well as standard library stuff like string functions.
 */

//
// base + base object
//

#include "allocator.h"
#include "common.h"
#include "OObj.h"
#include "OObjRoot.h"

//
// library functions, may use objects...
//

#include "clipboard.h"
#include "endian.h"
#include "file.h"
#include "img.h"
#include "log.h"
#include "str.h"
#include "tar.h"
#include "terminalcolor.h"
#include "timer.h"


//
// some useful objects
//

#include "OArray.h"
#include "ODelcallback.h"
#include "OJoin.h"
#include "OJson.h"
#include "OMap.h"
#include "OPattern.h"
#include "OPtr.h"
#include "OStream.h"
#include "OStreamArray.h"
#include "OStreamBuffered.h"
#include "OStreamMem.h"
#include "OTask.h"
#include "OWeakjoin.h"

//
// threading stuff
//
#ifdef MIA_OPTION_THREAD
#include "OCondition.h"
#include "OFuture.h"
#include "OQueue.h"
#include "OThread.h"
#include "OThreadpool.h"
#endif


//
// fetch http with GET or POST
//
#ifdef MIA_OPTION_FETCH
#include "o/OFetch.h"
#endif

//
// socket stuff
//
#ifdef MIA_OPTION_SOCKET
#include "OSocketserver.h"
#include "OStreamSocket.h"
#include "socket.h"
#endif


#endif //O_O_H
