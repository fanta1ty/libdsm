//
//   smbShare.m
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "smbShare.h"
#import "smbError.h"
#import "smbShare_Protected.h"
#import "smbFile_Protected.h"

#import "smb_file.h"
#import "smb_dir.h"
#import "smb_share.h"
#import "smb_stat.h"

@interface smbShare ()
@property (nonatomic) dispatch_queue_t  serialQueue;
@property (nonatomic) smb_tid           shareID;
@end

@implementation  smbShare
- (nullable instancetype)initWithName:(nonnull NSString *)name server:(nonnull  smbFileServer *)server {
    self = [super init];
    if (self) {
        NSString *queueName = [NSString stringWithFormat:@" smb_share_queue_%@", name];
        
        _name = name;
        _server = server;
        _shareID = 0;
        _serialQueue = dispatch_queue_create(queueName.UTF8String, DISPATCH_QUEUE_SERIAL);
    }
    return self;
}

- (void)dealloc {
}

- (NSString *)description {
    return [NSString stringWithFormat:@"%@ on %@", self.name, self.server];
}

- (void)open:(nullable void (^)(NSError *_Nullable))completion {
    dispatch_async(_serialQueue, ^{
        [self.server openShare:self.name completion:^( smb_tid shareID, NSError *error){
            if (error == nil) {
                _shareID = shareID;
            }
            if (completion) {
                completion(error);
            }
        }];
    });
}

- (void)close:(nullable void (^)(NSError *_Nullable))completion {
    dispatch_async(_serialQueue, ^{
        if (_shareID == 0) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion([ smbError notOpenError]);
            });
        } else {
            [self.server closeShare:_shareID completion:completion];
            _shareID = 0;
        }
    });
}

- (BOOL)isOpen {
    return _shareID > 0;
}

- (void)createDirectories:(nonnull NSString *)path completion:(nullable void (^)( smbFile *_Nullable, NSError *_Nullable))completion {
    dispatch_async(_serialQueue, ^{
         smbFile *file = nil;
        NSError *error = nil;
        
        if (self.server. smbSession) {
            if ([self isOpen]) {
                NSString *p = path;
                
                while ([p hasPrefix:@"/"]) {
                    p = [path substringFromIndex:1];
                }
                NSArray *directories = p.pathComponents;
                
                p = @"";
                
                for (NSUInteger i = 0; error == nil && i < directories.count; i++) {
                    p = [p stringByAppendingFormat:@"\\%@", [directories objectAtIndex:i]];
                    
                    const char *cpath = p.UTF8String;
                     smbStat *stat = [self _stat:cpath];
                    
                    if (!stat.exists) {
                        int dsm_error =  smb_directory_create(self.server. smbSession, _shareID, cpath);
                        
                        if (dsm_error != 0) {
                            error = [ smbError dsmError:dsm_error session:self.server. smbSession];
                        }
                    }
                    
                    if (error == nil && i == directories.count - 1) {
                        file = [[ smbFile alloc] initWithPath:path share:self];
                        
                        if (!stat.exists) {
                            stat = [self _stat:cpath];
                        }
                        file. smbStat = stat;
                    }
                }
                
            } else {
                error = [ smbError notOpenError];
            }
        } else {
            error = [ smbError notConnectedError];
        }
        
        if (completion) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(file, error);
            });
        }
    });
}

- (void)createDirectory:(nonnull NSString *)path completion:(nullable void (^)( smbFile *_Nullable, NSError *_Nullable))completion {
    dispatch_async(_serialQueue, ^{
         smbFile *file = nil;
        NSError *error = nil;
        
        if (self.server. smbSession) {
            if ([self isOpen]) {
                
                NSString * smbPath = [path stringByReplacingOccurrencesOfString:@"/" withString:@"\\"];
                const char *cpath =  smbPath.UTF8String;
                 smbStat *stat = [self _stat:cpath];
                
                if (!stat.exists) {
                    int dsm_error =  smb_directory_create(self.server. smbSession, _shareID, cpath);
                    
                    if (dsm_error != 0) {
                        error = [ smbError dsmError:dsm_error session:self.server. smbSession];
                    }
                }
                
                if (error == nil) {
                    file = [[ smbFile alloc] initWithPath:path share:self];
                    
                    if (!stat.exists) {
                        stat = [self _stat:cpath];
                    }
                    file. smbStat = stat;
                }
            } else {
                error = [ smbError notOpenError];
            }
        } else {
            error = [ smbError notConnectedError];
        }
        
        if (completion) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(file, error);
            });
        }
    });
}

- (uint32_t)_mod:( smbFileMode)mode {
    uint32_t mod = 0;
    
    if (mode &  smbFileModeRead) {
        mod |=  SMB_MOD_READ |  SMB_MOD_READ_EXT |  SMB_MOD_READ_ATTR |  SMB_MOD_READ_CTL;
    }
    if (mode &  smbFileModeWrite) {
        mod |=  SMB_MOD_WRITE |  SMB_MOD_WRITE_EXT |  SMB_MOD_WRITE_ATTR |  SMB_MOD_APPEND;
    }
    
    return mod;
}

- (void)openFile:(nonnull NSString *)path mode:( smbFileMode)mode completion:(nullable void (^)( smbFile *,  smb_fd, NSError *_Nullable))completion {
    
    dispatch_async(_serialQueue, ^{
        
         smb_fd fd = -1;
        NSError *error = nil;
         smbFile *file = nil;
        
        if (self.server. smbSession) {
            if ([self isOpen]) {
                NSString * smbPath = [path stringByReplacingOccurrencesOfString:@"/" withString:@"\\"];
                const char *cpath =  smbPath.UTF8String;
                uint32_t mod = [self _mod:mode];
                
                file = [[ smbFile alloc] initWithPath:path share:self];
                file. smbStat = [self _stat:cpath];
                
                int dsm_error =  smb_fopen(self.server. smbSession, self.shareID, cpath, mod, &fd);
                
                if (dsm_error != 0) {
                    error = [ smbError dsmError:dsm_error session:self.server. smbSession];
                }
            } else {
                error = [ smbError notOpenError];
            }
        } else {
            error = [ smbError notConnectedError];
        }
        
        if (completion) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(file, fd, error);
            });
        }
    });
}

- (void)closeFile:( smb_fd)fd path:(NSString *)path completion:(nullable void (^)( smbFile *_Nullable, NSError *_Nullable))completion {
    
    dispatch_async(_serialQueue, ^{
        
        NSError *error = nil;
         smbFile *file = nil;
        
        if (self.server. smbSession) {
            if ([self isOpen]) {
                
                 smb_fclose(self.server. smbSession, fd);
                
                NSString * smbPath = [path stringByReplacingOccurrencesOfString:@"/" withString:@"\\"];
                const char *cpath =  smbPath.UTF8String;
                
                file = [[ smbFile alloc] initWithPath:path share:self];
                file. smbStat = [self _stat:cpath];
                
            } else {
                error = [ smbError notOpenError];
            }
        } else {
            error = [ smbError notConnectedError];
        }
        
        if (completion) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(file, error);
            });
        }
    });
}

- (void)deleteFile:(nonnull NSString *)path completion:(nullable void (^)(NSError *_Nullable))completion {
    dispatch_async(_serialQueue, ^{
        NSError *error = nil;
        
        if (self.server. smbSession) {
            if ([self isOpen]) {
                
                NSString * smbPath = [path stringByReplacingOccurrencesOfString:@"/" withString:@"\\"];
                const char *cpath =  smbPath.UTF8String;
                 smbStat *stat = [self _stat:cpath];
                
                if (stat.exists) {
                    int dsm_error = 0;
                    
                    if (stat.isDirectory) {
                        dsm_error =  smb_directory_rm(self.server. smbSession, _shareID, cpath);
                    } else {
                        dsm_error =  smb_file_rm(self.server. smbSession, _shareID, cpath);
                    }
                    
                    if (dsm_error != 0) {
                        error = [ smbError dsmError:dsm_error session:self.server. smbSession];
                    }
                }
                
            } else {
                error = [ smbError notOpenError];
            }
        } else {
            error = [ smbError notConnectedError];
        }
        
        if (completion) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(error);
            });
        }
    });
}

- (void)listFiles:(void (^)(NSArray< smbFile *> *, NSError *))completion {
    [self listFiles:@"/" filter:nil completion:completion];
}

- (void)listFilesUsingFilter:(nullable BOOL (^)( smbFile *_Nonnull))filter completion:(void (^)(NSArray< smbFile *> *, NSError *))completion {
    [self listFiles:@"/" filter:filter completion:completion];
}

- (void)listFiles:(NSString *)path filter:(nullable BOOL (^)( smbFile *_Nonnull file))filter completion:(void (^)(NSArray< smbFile *> *, NSError *))completion {
    
    dispatch_async(_serialQueue, ^{
        
        NSMutableArray *fileList = nil;
        NSError *error = nil;
        
        if (self.server. smbSession) {
            if ([self isOpen]) {
                
                NSString * smbPath = [path stringByReplacingOccurrencesOfString:@"/" withString:@"\\"];
                
                if (![ smbPath hasSuffix:@"\\"]) {
                     smbPath = [ smbPath stringByAppendingString:@"\\"];
                }
                 smbPath = [ smbPath stringByAppendingString:@"*"];
                
                //Query for a list of files in this directory
                smb_stat_list statList =  smb_find(self.server. smbSession, _shareID,  smbPath.UTF8String);
                
                if (statList != NULL) {
                    size_t listCount =  smb_stat_list_count(statList);
                    
                    fileList = [NSMutableArray array];
                    
                    for (NSInteger i = 0; i < listCount; i++) {
                         smb_stat item =  smb_stat_list_at(statList, i);
                        const char *name =  smb_stat_name(item);
                        
                        NSString *filePath = [path stringByAppendingPathComponent:[NSString stringWithUTF8String:name]];
                        
                         smbFile *file = [[ smbFile alloc] initWithPath:filePath share:self];
                        
                        file. smbStat = [[ smbStat alloc] initWithStat:item];
                        
                        if (!(file.isDirectory && ([file.name isEqualToString:@".."] || [file.name isEqualToString:@"."]))) {
                            if (filter == nil || filter(file)) {
                                [fileList addObject:file];
                            }
                        }
                    }
                     smb_stat_list_destroy(statList);
                } else {
                    /*
                     uint32_t nt_status =  smb_session_get_nt_status(self.server. smbSession);
                     if (nt_status != NT_STATUS_SUCCESS) {
                     error = [ smbError dsmError:DSM_ERROR_NT session:self.server. smbSession];
                     }
                     */
                }
                
            } else {
                error = [ smbError notOpenError];
            }
        } else {
            error = [ smbError notConnectedError];
        }
        
        if (completion) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(fileList, error);
            });
        }
    });
}

- (void)moveFile:(NSString *)oldPath to:(NSString *)newPath completion:(void (^)( smbFile *, NSError *))completion {
    dispatch_async(_serialQueue, ^{
        NSError *error = nil;
         smbFile *file = nil;
        
        if (self.server. smbSession) {
            if ([self isOpen]) {
                NSString * smbOldPath = [oldPath stringByReplacingOccurrencesOfString:@"/" withString:@"\\"];
                NSString * smbNewPath = [newPath stringByReplacingOccurrencesOfString:@"/" withString:@"\\"];
                
                int res =  smb_file_mv(self.server. smbSession, _shareID,  smbOldPath.UTF8String,  smbNewPath.UTF8String);
                
                if (res != 0) {
                    error = [ smbError notSuchFileOrDirectory];
                } else {
                    file = [ smbFile fileWithPath:newPath share:self];
                    file. smbStat = [self _stat: smbNewPath.UTF8String];
                }
            } else {
                error = [ smbError notOpenError];
            }
        } else {
            error = [ smbError notConnectedError];
        }
        
        if (completion) {
            dispatch_async(dispatch_get_main_queue(), ^{
                completion(file, error);
            });
        }
    });
}

- (void)getStatusOfFile:(NSString *)path completion:(void (^)( smbStat *, NSError *))completion {
    
    if (path.length == 0 || [path isEqualToString:@"/"]) {
        if (completion) {
            completion([ smbStat statForRoot], nil);
        }
    } else {
        dispatch_async(_serialQueue, ^{
            NSError *error = nil;
             smbStat * smbStat = nil;
            
            if (self.server. smbSession) {
                if ([self isOpen]) {
                    NSString * smbPath = [path stringByReplacingOccurrencesOfString:@"/" withString:@"\\"];
                     smbStat = [self _stat: smbPath.UTF8String];
                } else {
                    error = [ smbError notOpenError];
                }
            } else {
                error = [ smbError notConnectedError];
            }
            
            if (completion) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    completion( smbStat, error);
                });
            }
        });
    }
}

#pragma mark - Private methods

- (smbStat *)_stat:(const char *)path {
    smb_stat stat =  smb_fstat(self.server. smbSession, _shareID, path);
    
    smbStat *tempStat = [smbStat statForNonExistingFile];
    
    // This is a workaround because the above doesn't seem to work on directories
    // See https://github.com/videolabs/libdsm/issues/79
    
    if (stat == NULL) {
        smb_stat_list statList =  smb_find(self.server. smbSession, _shareID, path);
        
        if (statList != NULL) {
            size_t listCount =  smb_stat_list_count(statList);
            
            if (listCount == 1) {
                stat =  smb_stat_list_at(statList, 0);
                tempStat = [smbStat statWithStat:stat];
            } else {
                NSLog(@"Unexpectedly got multiple stat entries for %s", path);
            }
            
             smb_stat_list_destroy(statList);
        }
    } else {
         tempStat = [smbStat statWithStat:stat];
         smb_stat_destroy(stat);
    }
    // end of workaround
    
    return  tempStat;
}
@end

@implementation smbStat
+ (nullable instancetype)statForNonExistingFile {
    return [[self alloc] initForNonExistingFile];
}

+ (nullable instancetype)statForRoot {
    return [[self alloc] initForRoot];
}

+ (nullable instancetype)statWithStat:( smb_stat)stat {
    return [[self alloc] initWithStat:stat];
}

- (nullable instancetype)initForNonExistingFile {
    self = [super init];
    if (self) {
        _statTime = [NSDate new];
    }
    return self;
}

- (instancetype)initForRoot {
    self = [super init];
    if (self) {
        _directory = YES;
        _exists = YES;
        _statTime = [NSDate new];
        _smbName = @"\\";
    }
    return self;
}

- (instancetype)initWithStat:(smb_stat)stat {
    self = [super init];
    if (self) {
        if (stat != NULL) {
            uint64_t modificationTimestamp =  smb_stat_get(stat, SMB_STAT_MTIME);
            uint64_t creationTimestamp =  smb_stat_get(stat, SMB_STAT_CTIME);
            uint64_t accessTimestamp =  smb_stat_get(stat, SMB_STAT_ATIME);
            uint64_t writeTimestamp =  smb_stat_get(stat, SMB_STAT_WTIME);
            
            _smbName = [NSString stringWithUTF8String: smb_stat_name(stat)];
            
            _size = smb_stat_get(stat, SMB_STAT_SIZE);
            _directory = (smb_stat_get(stat, SMB_STAT_ISDIR) != 0);
            
            _modificationTime = [self _dateFromSMBTime:modificationTimestamp];
            _creationTime = [self _dateFromSMBTime:creationTimestamp];
            _accessTime = [self _dateFromSMBTime:accessTimestamp];
            _writeTime = [self _dateFromSMBTime:writeTimestamp];
            
            _exists = YES;
        }
        
        _statTime = [NSDate new];
    }
    return self;
}

- (NSString *)description {
    return [NSString stringWithFormat:@"Status of %@ %@ as of %@: Size: %llu, Created: %@, Modified: %@, Last opened: %@", self.isDirectory ? @"directory" : @"file", self. smbName, self.statTime, self.size, self.creationTime, self.modificationTime, self.accessTime];
}

#pragma mark - Private methods

- (NSDate *)_dateFromSMBTime:(uint64_t) smbTime {
    // If you really want some explanation, search for
    // 'SystemTimeLow and SystemTimeHigh' at http://ubiqx.org/cifs/ smb.html
    
    NSTimeInterval timestamp  = (NSTimeInterval)(( smbTime/10000000.) - 11644473600);
    
    return [NSDate dateWithTimeIntervalSince1970:timestamp];
}
@end
