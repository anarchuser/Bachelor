#ifndef BACHELOR_LOG_H
#define BACHELOR_LOG_H

#include <atomic>

#define PRINT_PORT "\t" << port << ": "

// Ctor / Dtor
static bool const kLogCDtor = false;
// Router
static bool const kLogRoute = false;
// Send
static bool const kLogSend = false;
static bool const kLogSendPing    = !kLogSend && false;
static bool const kLogSendConnect = !kLogSend && false;
static bool const kLogSendAction  = !kLogSend && false;
static bool const kLogSendVote    = !kLogSend && false;
// Receive
static bool const kLogRecv = false;
static bool const kLogRecvPing    = !kLogRecv && false;
static bool const kLogRecvConnect = !kLogRecv && false;
static bool const kLogRecvAction  = !kLogRecv && false;
static bool const kLogRecvVote    = !kLogRecv && false;
// Interconnecting
static bool const kLogJoin = false;
static bool const kLogTell = false;
// State
static bool const kLogState = false;
static std::atomic <bool> kLogPeerDtorState = false;
static bool const kLogInvalidAction = true;
//TODO: log actions being applied persistently

#endif //BACHELOR_LOG_H

/* Copyright © 2022 Aaron Alef */
