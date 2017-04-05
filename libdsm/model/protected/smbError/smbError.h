//
//  smbError.h
//  libdsm
//
//  Created by trekvn on 4/4/17.
//  Copyright © 2017 trekvn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "smb_session.h"

@interface smbError : NSObject
+(NSError *)unknownError;

+(NSError *)hostNotFoundError;

+(NSError *)noIPAddressError;

+(NSError *)notConnectedError;

+(NSError *)notOpenError;

+(NSError *)notSuchFileOrDirectory;

+(NSError *)writeError;

+(NSError *)readError;

+(NSError *)seekError;

+(NSError *)dsmError:(int)dsmError session:(smb_session *)session;
@end
