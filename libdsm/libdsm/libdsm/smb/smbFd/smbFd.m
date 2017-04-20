//
//  smbFd.m
//  test
//
//  Created by trekvn on 4/13/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "config.h"
#import "smbFd.h"

@implementation smbFd
#pragma mark - smbSessionShareAdd
void smb_session_share_add(smb_session *s, smb_share *share)
{
    smb_share *iter;
    
    assert(s != NULL && share != NULL);
    
    if (s->shares == NULL)
    {
        s->shares = share;
        return;
    }
    
    iter = s->shares;
    while (iter->next != NULL)
        iter = iter->next;
    iter->next = share;
}

#pragma mark - smbSessionShareGet
smb_share *smb_session_share_get(smb_session *s, smb_tid tid) {
    smb_share *iter;
    
    assert(s != NULL);
    
    iter = s->shares;
    while (iter != NULL && iter->tid != tid)
        iter = iter->next;
    
    return iter;
}

#pragma mark - smbSessionShareRemove
smb_share *smb_session_share_remove(smb_session *s, smb_tid tid)
{
    smb_share *iter, *keep;
    
    assert(s != NULL);
    
    iter = s->shares;
    
    if (iter == NULL)
        return NULL;
    if (iter->tid == tid)
    {
        s->shares = s->shares->next;
        return iter;
    }
    
    while (iter->next != NULL && iter->next->tid != tid)
        iter = iter->next;
    
    if (iter->next != NULL) // We found it
    {
        keep = iter->next;
        iter->next = iter->next->next;
        return keep;
    }
    return NULL;
}

#pragma mark - smbSessionShareClear
void smb_session_share_clear(smb_session *s)
{
    smb_share   *iter, *tmp;
    smb_file    *fiter, *ftmp;
    
    assert(s != NULL);
    
    iter = s->shares;
    while(iter != NULL)
    {
        fiter = iter->files;
        while(fiter != NULL)
        {
            ftmp = fiter;
            fiter = fiter->next;
            
            free(ftmp->name);
            free(ftmp);
        }
        
        tmp = iter;
        iter = iter->next;
        free(tmp);
    }
}

#pragma mark - smbSessionFileAdd
int smb_session_file_add(smb_session *s, smb_tid tid, smb_file *f) {
    smb_share *share;
    smb_file  *iter;
    
    assert(s != NULL && f != NULL);
    
    if ((share = smb_session_share_get(s, tid)) == NULL)
        return 0;
    
    if (share->files == NULL)
        share->files = f;
    else
    {
        iter = share->files;
        while (iter->next != NULL)
            iter = iter->next;
        iter->next = f;
    }
    
    return 1;
}

#pragma mark - smbSessionFileGet
smb_file  *smb_session_file_get(smb_session *s, smb_fd fd) {
    smb_share *share;
    smb_file  *iter;
    
    assert(s != NULL && fd);
    
    if ((share = smb_session_share_get(s, SMB_FD_TID(fd))) == NULL)
        return NULL;
    
    iter = share->files;
    while (iter != NULL && iter->fid != SMB_FD_FID(fd))
        iter = iter->next;
    
    return iter;
}

#pragma mark - smbSessionFileRemove
smb_file  *smb_session_file_remove(smb_session *s, smb_fd fd) {
    smb_share *share;
    smb_file  *iter, *keep;
    
    assert(s != NULL && fd);
    
    if ((share = smb_session_share_get(s, SMB_FD_TID(fd))) == NULL)
        return NULL;
    
    iter = share->files;
    
    if (iter == NULL)
        return NULL;
    if (iter->fid == SMB_FD_FID(fd))
    {
        share->files = iter->next;
        return iter;
    }
    
    while (iter->next != NULL && iter->next->fid != SMB_FD_TID(fd))
        iter = iter->next;
    if (iter->next != NULL)
    {
        keep = iter->next;
        iter->next = iter->next->next;
        return keep;
    }
    else
        return NULL;
}
@end
