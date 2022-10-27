#ifndef BACHELOR_LOG_H
#define BACHELOR_LOG_H

#include <atomic>

#define PRINT_PORT "\t" << port << ": "

// Ctor / Dtor
static bool const kLogCDtor = true;
// Router
static bool const kLogRoute = true;
// Send
static bool const kLogSend = false;
static bool const kLogSendPing    = !kLogSend && false;
static bool const kLogSendConnect = !kLogSend && false;
static bool const kLogSendAction  = !kLogSend && false;
static bool const kLogSendVote    = kLogSend && true;
// Receive
static bool const kLogRecv = false;
static bool const kLogRecvPing    = !kLogRecv && false;
static bool const kLogRecvConnect = !kLogRecv && false;
static bool const kLogRecvVote    = !kLogRecv && true;
// Interconnecting
static bool const kLogJoin = false;
static bool const kLogTell = false;
// State
static bool const kLogState = false;
static bool const kLogConsistent = false;
static std::atomic <bool> kLogPeerDtorState = false;
static bool const kLogInvalidAction = false;
//TODO: log actions being applied persistently

#endif //BACHELOR_LOG_H

/* Copyright © 2022 Aaron Alef */
