#include "sys.h"
#include "module.h"

//�����¼�
void set_event(event_t *ptEvent)
{
    if (NULL != ptEvent)
    {
        ptEvent->bIsSet = true;
    }
}

//�ȴ��¼����ɹ��ȵ�����true��δ�ȵ���ָ��Ϊ�շ���false
//���¼�����ΪAUTO���ȵ��¼����Զ���λ�¼�
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

//��λ�¼�
void reset_event(event_t *ptEvent)
{
    if (NULL != ptEvent)
    {
        ptEvent->bIsSet = false;
    }
}

//��ʼ���¼����¼���ʼ״̬ΪbValue���¼�����ΪbAuto��
void init_event(event_t *ptEvent, bool bValue, bool bAuto)
{
    if (NULL != ptEvent)
    {
        ptEvent->bAutoReset = bAuto;
        ptEvent->bIsSet = bValue;
    }
}

//��ʼ�����䣬�ʼ�Ϊ�գ��ʼ�����Ϊ�Զ���λ�������Ա���һ�Σ�
void init_mail(mailbox_t *ptMail)
{
    if (NULL != ptMail)
    {
        INIT_EVENT(&ptMail->tSealed, RESET, AUTO);  //��ʼ��Sealed�¼�ΪAUTO���ԣ�RESET
        ptMail->pTarget = NULL;
    }
}

//�����䣬�ɹ��򷵻��������ݣ�ָ�룩��ʧ���򷵻ؿ�ָ��
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

//�����ʼ����ʼ�����ΪpTarget
void post_mail(mailbox_t *ptMail, void *pTarget)
{
    if (NULL != ptMail && NULL != pTarget)
    {
        SET_EVENT(&ptMail ->tSealed);
        ptMail->pTarget = pTarget;
    }
}

//�����ٽ��������ٽ�������������false��δ��������ٽ�������������true
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

//�뿪�ٽ���������
void leave_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical)
    {
        ptCritical ->bLocked = false;
    }
}

//��ʼ���ٽ���Ϊδ����״̬
void init_cricital_sector(critical_sector_t *ptCritical)
{
    if (NULL != ptCritical)
    {
        ptCritical ->bLocked = false;
    }
}
