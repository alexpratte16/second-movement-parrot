#ifndef PARROT_CMDS_H
#define PARROT_CMDS_H

#define ENABLE_PARROT_CMDS 1

#if ENABLE_PARROT_CMDS
int parrot_on_cmd(int argc, char *argv[]);
int parrot_off_cmd(int argc, char *argv[]);    
int parrot_state_cmd(int argc, char *argv[]);
#endif // ENABLE_PARROT_CMDS

#endif // PARROT_CMDS_H