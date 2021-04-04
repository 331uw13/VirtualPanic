#ifndef VIRTUAL_PANIC__MESSAGES_H
#define VIRTUAL_PANIC__MESSAGES_H

#define VMSG_OK 32
#define VMSG_INFO 34
#define VMSG_WARNING 33
#define VMSG_ERROR 31
#define VMSG_DEBUG 35

#define VMESSAGES_BUFFER_SIZE 1024


void VMessage(int type, const char* msg, ...);
void VMessageSameLine(const char* msg);


#endif
