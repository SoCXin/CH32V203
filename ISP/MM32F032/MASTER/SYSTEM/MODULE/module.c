#include "sys.h"
#include "module.h"

//发送事件
void set_event(event_t *ptEvent)
{
    if (NULL != ptEvent)
    {
        ptEvent->bIsSet = true;
    }
}

//等待事件，成功等到返回true，未等到或指针为空返回false
//若事件属性为AUTO，等到事件后自动复位事件
bool wait_event(event_t *ptEvent)
{
    if (NULL != ptEvent)
    {
        if (false != ptEvent->bIsSet)
        {
            if (false != ptEvent->bAutoReset)
            {
                ptEvent->bIsSet = false;
            }
            return true;
        }
    }
    return false;
}

//复位事件
void reset_event(event_t *ptEvent)
{
    if (NULL != ptEvent)
    {
        ptEvent->bIsSet = false;
    }
}

//初始化事件，事件初始状态为bValue，事件属性为bAuto，
void init_event(event_t *ptEvent, bool bValue, bool bAuto)
{
    if (NULL != ptEvent)
    {
        ptEvent->bAutoReset = bAuto;
        ptEvent->bIsSet = bValue;
    }
}

//初始化邮箱，邮件为空，邮件属性为自动复位（仅可以被打开一次）
void init_mail(mailbox_t *ptMail)
{
    if (NULL != ptMail)
    {
        INIT_EVENT(&ptMail->tSealed, RESET, AUTO);  //初始化Sealed事件为AUTO属性，RESET
        ptMail->pTarget = NULL;
    }
}

//打开邮箱，成功则返回邮箱内容（指针），失败则返回空指针
void *open_mail(mailbox_t *ptMail)
{
    if (NULL != ptMail)
    {
        if (WAIT_EVENT(&ptMail ->tSealed))
        {
            return ptMail->pTarget;
        }
    }
    return NULL;
}

//发送邮件，邮件内容为pTarget
void post_mail(mailbox_t *ptMail, void *pTarget)
{
    if (NULL != ptMail && NULL != pTarget)
    {
        SET_EVENT(&ptMail ->tSealed);
        ptMail->pTarget = pTarget;
    }
}

//进入临界区，若临界区已上锁返回false，未上锁则给临界区上锁并返回true
bool enter_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical)
    {
        if (false == ptCritical ->bLocked)
        {
            ptCritical ->bLocked = true;
            return true;
        }
    }
    return false;
}

//离开临界区，解锁
void leave_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical)
    {
        ptCritical ->bLocked = false;
    }
}

//初始化临界区为未上锁状态
void init_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical)
    {
        ptCritical ->bLocked = false;
    }
}
