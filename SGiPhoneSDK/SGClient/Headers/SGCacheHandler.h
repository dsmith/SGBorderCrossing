//
//  SGCacheHandler.h
//  SGClient
//
//  Copyright (c) 2009-2010, SimpleGeo
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, 
//  this list of conditions and the following disclaimer. Redistributions 
//  in binary form must reproduce the above copyright notice, this list of
//  conditions and the following disclaimer in the documentation and/or 
//  other materials provided with the distribution.
//  
//  Neither the name of the SimpleGeo nor the names of its contributors may
//  be used to endorse or promote products derived from this software 
//  without specific prior written permission.
//   
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
//  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
//  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created by Derek Smith.
//

#import <Foundation/Foundation.h>

/*!
* @class SGCacheHandler 
* @abstract A wrapper around NSFileManager that traverses directory
* structures and mutates files.
*/
@interface SGCacheHandler : NSObject {

    NSTimeInterval ttl;
    NSString* cachePath;
    
    NSString* topLevelCachePath;
}

/*!
* @property
* @abstract The current path to which the handler is pointing to.
*/
@property (nonatomic, readonly) NSString* cachePath;

/*!
* @property
* @abstract The initial path of the handler.
*/
@property (nonatomic, readonly) NSString* topLevelCachePath;

/*!
* @property
* @abstract When @link clearStaleCacheFiles clearStaleCacheFiles @/link is called,
* this value is used to determine whether or not to delete the cached files.
*/
@property (nonatomic, assign) NSTimeInterval ttl;

/*!
* @method initWithDirectory:
* @abstract Initializes the cache handler to point to
* NSCachesDirectory/directory
* @param directory ￼The directory to create or point to.
* @result ￼A new cache handler.
*/
- (id) initWithDirectory:(NSString*)directory;

/*!
* @method changeDirectory:
* @abstract ￼Points the cache handler to the new directory,
* creating it if necessary.
* @param directory ￼The new directory.
*/
- (void) changeDirectory:(NSString*)directory;

/*!
* @method deleteDirectory:
* @abstract ￼Deletes a directory that is contained within
* the @link cachePath cachePath @/link.
* @param directory ￼The directory to delete.
*/
- (void) deleteDirectory:(NSString*)directory;

/*!
* @method  changeToParentDirectory
* @abstract ￼Changes directory to the parent directory
* if the current directory isn't already the top level
* directory.
*/
- (void) changeToParentDirectory;

/*!
* @method changeToTopLevelPath
* @abstract Changes the directory to the patch declared
* by @link topLevelCachePath topLevelCachePath @/link.
*/
- (void) changeToTopLevelPath;

/*!
* @method getFiles
* @abstract Returns all the files/directories listed within the
* current cache path.
* @result An array of NSStrings.
*/
- (NSArray*) getFiles;

/*!
* @method  clearStaleCacheFiles
* @abstract For all files within the current cache path,
* this method checks that last modified attribute and determines
* whether or not to delete it based on th
*/
- (void) clearStaleCacheFiles;

/*!
* @method deleteAllFiles
* @abstract ￼Deletes all files and directories under the top level 
* cache path.
*/
- (void) deleteAllFiles;

/*!
* @method updateFile:withContents:
* @abstract ￼Update the contents of the file with the data.
* @param file ￼The name of the file that will contain the data.
* @param data ￼The data to add to the file.
* @result YES if the update was successful; otherwise, NO.
*/
- (BOOL) updateFile:(NSString*)file withContents:(NSData*)data;

/*!
* @method getContentsOfFile:
* @abstract ￼Retrieves the contents of the file.
* @param file The file to read from.
* @result The data contents of the file.
*/
- (NSData*) getContentsOfFile:(NSString*)file;

@end
