//
//  smbShare_Protected.h
//  libdsm
//
//  Created by trekvn on 4/5/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "smbShare.h"
#import "smbFile_Protected.h"
#import "smbFileServer_Protected.h"
#import "smb_session.h"

@interface smbStat : NSObject
@property (nonatomic, readonly) BOOL                                exists;
@property (nonatomic, readonly, getter=isDirectory) BOOL            directory;
@property (nonatomic, readonly) unsigned long long                  size;
@property (nonatomic, readonly, nullable) NSDate                    *creationTime;
@property (nonatomic, readonly, nullable) NSDate                    *modificationTime;
@property (nonatomic, readonly, nullable) NSDate                    *accessTime;
@property (nonatomic, readonly, nullable) NSDate                    *writeTime;
@property (nonatomic, readonly, nullable) NSDate                    *statTime;
@property (nonatomic, readonly, nullable) NSString                  *smbName;

+ (nullable instancetype)statForNonExistingFile;

+ (nullable instancetype)statForRoot;

+ (nullable instancetype)statWithStat:(nonnull smb_stat)stat;

- (nullable instancetype)initWithStat:(nonnull smb_stat)stat;

- (nullable instancetype)initForRoot;

- (nullable instancetype)initForNonExistingFile;
@end

@interface smbShare ()

- (nullable instancetype)initWithName:(nonnull NSString *)name
                               server:(nonnull smbFileServer *)server;

- (void)listFiles:(nonnull NSString *)path
           filter:(nullable BOOL (^)(smbFile *_Nonnull file))filter
       completion:(nullable void (^)(NSArray<smbFile *> *_Nullable files, NSError *_Nullable error))completion;

- (void)getStatusOfFile:(nonnull NSString *)path
             completion:(nullable void (^)(smbStat *_Nullable status, NSError *_Nullable error))completion;

- (void)createDirectory:(nonnull NSString *)path
             completion:(nullable void (^)(smbFile *_Nullable file, NSError *_Nullable error))completion;

- (void)createDirectories:(nonnull NSString *)path
               completion:(nullable void (^)(smbFile *_Nullable file, NSError *_Nullable error))completion;

- (void)moveFile:(nonnull NSString *)oldPath
              to:(nonnull NSString *)newPath
      completion:(nullable void (^)(smbFile *_Nullable file, NSError *_Nullable error))completion;

- (void)deleteFile:(nonnull NSString *)path
        completion:(nullable void (^)(NSError *_Nullable error))completion;

- (void)openFile:(nonnull NSString *)path
            mode:(smbFileMode)mode
      completion:(nullable void (^)(smbFile *_Nullable file, smb_fd fd, NSError *_Nullable error))completion;

- (void)closeFile:(smb_fd)fd path:(nonnull NSString *)path
       completion:(nullable void (^)(smbFile *_Nullable file, NSError *_Nullable error))completion;
@end
