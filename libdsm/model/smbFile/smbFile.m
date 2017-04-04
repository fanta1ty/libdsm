//
//  smbFile.m
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import "smbFile.h"

@implementation smbFile

+ (nullable instancetype)rootOfShare:(nonnull smbShare *)share {
    return nil;
}

+ (nullable instancetype)fileWithPath:(nonnull NSString *)path
                                share:(nonnull smbShare *)share {
    return nil;
}

+ (nullable instancetype)fileWithPath:(nonnull NSString *)path
                       relativeToFile:(nonnull smbFile *)file {
    return nil;
}

- (nullable instancetype)initWithPath:(nonnull NSString *)path
                                share:(nonnull smbShare *)share {
    return nil;
}

- (nullable instancetype)initWithPath:(nonnull NSString *)path
                       relativeToFile:(nonnull smbFile *)file {
    return nil;
}

- (void)open:(smbFileMode)mode
  completion:(nullable void (^)(NSError *_Nullable error))completion {
    
}

- (void)close:(nullable void (^)(NSError *_Nullable error))completion {
    
}

//- (void)write:(nonnull NSData *)data completion:(nullable void (^)(long bytesWritten, NSError *_Nullable error))completion;

- (void)write:(nonnull NSData *_Nullable (^)(unsigned long long))dataHandler
     progress:(nullable void (^)(unsigned long long bytesWrittenTotal, long bytesWrittenLast,
                                 BOOL complete, NSError *_Nullable error))progress {
    
}

- (void)read:(NSUInteger)bufferSize
    progress:(nullable BOOL (^)(unsigned long long bytesReadTotal, NSData *_Nullable data,
                                BOOL complete, NSError *_Nullable error))progress {
    
}

- (void)read:(NSUInteger)bufferSize
    maxBytes:(unsigned long long)maxBytes
    progress:(nullable BOOL (^)(unsigned long long bytesReadTotal, NSData *_Nullable data,
                                BOOL complete, NSError *_Nullable error))progress {
    
}

- (void)seek:(unsigned long long)offset
    absolute:(BOOL)absolute
  completion:(nullable void (^)(unsigned long long position, NSError *_Nullable error))completion {
    
}

- (void)listFiles:(nullable void (^)(NSArray<smbFile *> *_Nullable files,
                                     NSError *_Nullable error))completion {
    
}
- (void)listFilesUsingFilter:(nullable BOOL (^)(smbFile *_Nonnull file))filter
                  completion:(nullable void (^)(NSArray<smbFile *> *_Nullable files,
                                                NSError *_Nullable error))completion {
    
}

- (void)updateStatus:(nullable void (^)(NSError *_Nullable error))completion {
    
}

- (void)createDirectory:(nullable void (^)(NSError *_Nullable error))completion {
    
}

- (void)createDirectories:(nullable void (^)(NSError *_Nullable error))completion {
    
}

- (void)deleteFile:(nullable void (^)(NSError *_Nullable error))completion {
    
}

- (void)moveTo:(nonnull NSString *)path
    completion:(nullable void (^)(NSError *_Nullable error))completion {
    
}
@end
