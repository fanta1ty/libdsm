//
//  smbShare.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import <Foundation/Foundation.h>

@class smbFile;
@class smbFileServer;

@interface smbShare : NSObject
@property (nonatomic, readonly, nonnull) smbFileServer      *server;
@property (nonatomic, readonly, nonnull) NSString           *name;
@property (nonatomic, readonly) BOOL                        isOpen;

- (void)open:(nullable void (^)(NSError *_Nullable error))completion;

- (void)close:(nullable void (^)(NSError *_Nullable error))completion;

- (void)listFiles:(nullable void (^)(NSArray<smbFile *> *_Nullable files,
                                     NSError *_Nullable error))completion;

- (void)listFilesUsingFilter:(nullable BOOL (^)(smbFile *_Nonnull file))filter
                  completion:(nullable void (^)(NSArray<smbFile *> *_Nullable files,
                                                NSError *_Nullable error))completion;
@end
