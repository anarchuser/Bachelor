#ifndef BACHELOR_LOG_H
#define BACHELOR_LOG_H

#define PRINT_PORT "\t" << port << ": "

// Ctor / Dtor
bool const kLogCDtor = false;
// Router
bool const kLogRoute = false;
// Send / Receive
bool const kLogSend = false;
bool const kLogRecv = false;
bool const kLogRecvPing =    !kLogRecv && false;
bool const kLogRecvConnect = !kLogRecv && false;
bool const kLogRecvAction =  !kLogRecv && false;
// Interconnecting
bool const kLogJoin = false;
bool const kLogTell = false;
// State
bool const kLogState = false;
//TODO: log actions being applied persistently

#endif //BACHELOR_LOG_H

/* Copyright © 2022 Aaron Alef */
