#ifndef SMOLLIB_H
#define SMOLLIB_H

#ifndef SL_MAX_RULES 
#define SL_MAX_RULES 10
#endif

#ifndef SL_MAX_ARGUMENTS
#define SL_MAX_ARGUMENTS 5
#endif

struct rule_t
{
  unsigned char flags;
  unsigned char condition_id;
  unsigned char command_id;
  int arguments[SL_MAX_ARGUMENTS];
};

struct object_t
{
  
};

#endif
