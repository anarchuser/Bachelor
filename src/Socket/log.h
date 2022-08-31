#ifndef BACHELOR_LOG_H
#define BACHELOR_LOG_H

#define PRINT_PORT "\t" << port << ": "

// Ctor / Dtor
bool const kLogCDtor = false;
// Router
bool const kLogRoute = false;
// Send / Receive
bool const kLogSend = false;
bool const kLogRecv = true;
// Interconnecting
bool const kLogJoin = false;
bool const kLogTell = false;
// Initialisation
//TODO: log actions being applied persistently

#endif //BACHELOR_LOG_H

/* Copyright © 2022 Aaron Alef */
