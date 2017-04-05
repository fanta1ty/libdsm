//
//  smbFile.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "smbShare.h"

typedef NS_OPTIONS(NSUInteger, smbFileMode) {
    smbFileModeRead = 1 << 0,
    smbFileModeWrite = 1 << 1,
    smbFileModeReadAndWrite = smbFileModeRead | smbFileModeWrite
};

@interface smbFile : NSObject
@property (nonatomic, readonly, nonnull) smbShare           *share;
@property (nonatomic, readonly, nullable) smbFile           *parent;

@property (nonatomic, readonly, nonnull) NSString           *path;
@property (nonatomic, readonly, nonnull) NSString           *name;
@property (nonatomic, readonly, nullable) NSDate            *creationTime;
@property (nonatomic, readonly, nullable) NSDate            *modificationTime;
@property (nonatomic, readonly, nullable) NSDate            *accessTime;
@property (nonatomic, readonly, nullable) NSDate            *writeTime;
@property (nonatomic, readonly, nullable) NSDate            *statusTime;

@property (nonatomic, readonly) BOOL                        exists;
@property (nonatomic, readonly) BOOL                        isDirectory;
@property (nonatomic, readonly) BOOL                        hasStatus;
@property (nonatomic, readonly) BOOL                        isOpen;

@property (nonatomic, readonly) unsigned long long          size;

+ (nullable instancetype)rootOfShare:(nonnull smbShare *)share;

+ (nullable instancetype)fileWithPath:(nonnull NSString *)path
                                share:(nonnull smbShare *)share;

+ (nullable instancetype)fileWithPath:(nonnull NSString *)path
                       relativeToFile:(nonnull smbFile *)file;

- (nullable instancetype)initWithPath:(nonnull NSString *)path
                                share:(nonnull smbShare *)share;

- (nullable instancetype)initWithPath:(nonnull NSString *)path
                       relativeToFile:(nonnull smbFile *)file;

- (void)open:(smbFileMode)mode
  completion:(nullable void (^)(NSError *_Nullable error))completion;

- (void)close:(nullable void (^)(NSError *_Nullable error))completion;

- (void)write:(nonnull NSData *_Nullable (^)(unsigned long long))dataHandler
     progress:(nullable void (^)(unsigned long long bytesWrittenTotal, long bytesWrittenLast,
                                 BOOL complete, NSError *_Nullable error))progress;

- (void)read:(NSUInteger)bufferSize
    progress:(nullable BOOL (^)(unsigned long long bytesReadTotal, NSData *_Nullable data,
                                BOOL complete, NSError *_Nullable error))progress;

- (void)read:(NSUInteger)bufferSize
    maxBytes:(unsigned long long)maxBytes
    progress:(nullable BOOL (^)(unsigned long long bytesReadTotal, NSData *_Nullable data,
                                BOOL complete, NSError *_Nullable error))progress;

- (void)seek:(unsigned long long)offset
    absolute:(BOOL)absolute
  completion:(nullable void (^)(unsigned long long position, NSError *_Nullable error))completion;

- (void)listFiles:(nullable void (^)(NSArray<smbFile *> *_Nullable files,
                                     NSError *_Nullable error))completion;

- (void)listFilesUsingFilter:(nullable BOOL (^)(smbFile *_Nonnull file))filter
                  completion:(nullable void (^)(NSArray<smbFile *> *_Nullable files,
                                                NSError *_Nullable error))completion;

- (void)updateStatus:(nullable void (^)(NSError *_Nullable error))completion;

- (void)createDirectory:(nullable void (^)(NSError *_Nullable error))completion;

- (void)createDirectories:(nullable void (^)(NSError *_Nullable error))completion;

- (void)deleteFile:(nullable void (^)(NSError *_Nullable error))completion;

- (void)moveTo:(nonnull NSString *)path
    completion:(nullable void (^)(NSError *_Nullable error))completion;

@end
