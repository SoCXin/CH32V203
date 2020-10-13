#ifndef __MODULE_H
#define __MODULE_H

#include "stdbool.h"

//! \name finit state machine state
//! @{
typedef enum {
   fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
   fsm_rt_cpl          = 0,     //!< fsm complete
   fsm_rt_on_going     = 1,     //!< fsm on-going
} fsm_rt_t;
//! @}

//event
typedef struct {
    bool bAutoReset;    //  reset type
    bool bIsSet;        //  initial state 
} event_t;

//#define SET     true        //设置
//#define RESET   false       //重置
#define AUTO    true        //自动
#define MANUAL  false       //手动
    
#define SET_EVENT(__EVENT)      set_event(__EVENT)      //发送事件
#define WAIT_EVENT(__EVENT)     wait_event(__EVENT)     //等待事件，等到事件返回true，未等到返回false
#define RESET_EVENT(__EVENT)    reset_event(__EVENT)    //复位事件
#define INIT_EVENT(__EVENT, __INIT_VALUE,__AUTO)  init_event(__EVENT, __INIT_VALUE,__AUTO)   //初始化事件

extern void set_event(event_t *ptEvent);
extern bool wait_event(event_t *ptEvent);
extern void reset_event(event_t *ptEvent);
extern void init_event(event_t *ptEvent,bool bValue,bool bAuto);
//mailbox
typedef struct {
    event_t tSealed;
    void *pTarget;
}mailbox_t;

#define POST_MAIL(__MAIL,__TARGET)  post_mail(__MAIL,__TARGET)      //发送邮件，邮件内容为指针（__TARGET）
#define OPEN_MAIL(__MAIL)           open_mail(__MAIL)               //打开邮箱（只能被打开一次），返回值为邮件内容
#define INIT_MAIL(__MAIL)           init_mail(__MAIL)               //初始化邮箱为空

extern void init_mail(mailbox_t *ptMail);
extern void *open_mail(mailbox_t *ptMail);
extern void post_mail(mailbox_t *ptMail,void *pTarget);
//critical sector
typedef struct {
    bool bLocked;
}critical_sector_t;
//typedef critical_sector_t mutex_t;

#define ENTER_CRITICAL_SECTOR(__CRITICAL)   enter_cricital_sector(__CRITICAL)   //进入临界区，成功返回true，失败返回false
#define LEAVE_CRITICAL_SECTOR(__CRITICAL)   leave_cricital_sector(__CRITICAL)   //离开临界区
#define INIT_CRITICAL_SECTOR(__CRITICAL)    init_cricital_sector(__CRITICAL)    //初始化临界区

extern bool enter_cricital_sector(critical_sector_t *ptCritical);
extern void leave_cricital_sector(critical_sector_t *ptCritical);
extern void init_cricital_sector(critical_sector_t *ptCritical);

#endif
