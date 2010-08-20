//
//  SGCommitLog.h
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

@class SGCacheHandler;
@protocol SGCommitLogReplayDelegate;

/*!
* @class SGCommitLog 
* @abstract A small implementation of a commit log
* that contains replay functionality.
* @discussion The directory format is stored as:
*
*  #{commit_log_name}/#{username}/#{key}/#{commit}-#{current_timestamp}
*
* Each commit is stored within the key of a username. Commits are stored in a 
* file called commit that has a timestamp appended to it.
*/
@interface SGCommitLog : NSObject {
    
    id<SGCommitLogReplayDelegate> delegate;

    NSTimeInterval flushTimeInterval;

    @private
    SGCacheHandler* cacheHandler;
    NSTimer* flushTimer;
    NSMutableDictionary* commitLog;
    
    NSLock* commitLogLock;
}

/*!
* @property
* @abstract The delegate that will recieve replay notifications.
*/
@property (nonatomic, assign) id<SGCommitLogReplayDelegate> delegate;

/*!
* @property
* @abstract The time interval for the flush timer that calls @link flush flush @/link.
*/
@property (nonatomic, assign) NSTimeInterval flushTimeInterval;

/*!
* @method initWithName:
* @abstract ￼Initializes a new commit log that will write
* all commits in a directory called name
* @param name ￼The directory to create for the commit log.
* @result ￼
*/
- (id) initWithName:(NSString*)name;

/*!
* @method startFlushTimer
* @abstract ￼Starts the flush timer which will write all commits in memory
* to a file.
*/
- (void) startFlushTimer;

/*!
* @method stopFlushTimer
* @abstract Stops the flush timer that is in charge of writing commits to
* a file.
*/
- (void) stopFlushTimer;

/*!
* @method flush
* @abstract ￼Saves all commits onto disk.
*/
- (void) flush;

/*!
* @method clear
* @abstract Deletes all commits that are loaded into memory.
*/
- (void) clear;

/*!
* @method reload
* @abstract ￼Reloads all commits from disk into memory.
*/
- (void) reload;

/*!
* @method deleteAllUsernames
* @abstract Deletes all of the user directories for this
* commit log.
*/
- (void) deleteAllUsernames;

/*!
* @method deleteUsername:
* @abstract Deletes all commits for a given user.
* @param username ￼The name of the user to delete.
*/
- (void) deleteUsername:(NSString*)username;

/*!
* @method deleteUsername:key:
* @abstract Deletes all commits for a given user and key.
* @param username ￼The name of the user to delete.
* @param key ￼The key to delete.
*/
- (void) deleteUsername:(NSString*)username key:(NSString*)key;

/*!
* @method replay:
* @abstract ￼Replays all of the commits made by a user.
* @discussion Commits for the user are loaded into memory
* and then sent of to the @link delegate delegate @/link for some
* replay action. Once the commits have been replayed, they are deleted
* from disk.
* @param username The username to load commits for.
*/
- (void) replay:(NSString*)username;

/*!
* @method addCommit:forUsername:andKey:
* @abstract ￼Adds a commit for a user and a specified key.
* @param data ￼The data to save.
* @param username ￼The username.
* @param key ￼The key.
*/
- (void) addCommit:(NSData*)data forUsername:(NSString*)username andKey:(NSString*)key;

/*!
* @method addError:forUsername:andKey:
* @abstract ￼Adds an error for a user and a specified key.
* @param data ￼The data to save as an error.
* @param username ￼The username.
* @param key ￼The key.
*/
- (void) addError:(NSData*)data forUsername:(NSString*)username andKey:(NSString*)key;

/*!
* @method getAllCommitsForUsername:
* @abstract ￼Retrieves all commits for a given user that are loaded into
* memory.
* @param username ￼The name of the user.
* @result ￼All commits for a given user.
*/
- (NSMutableDictionary*) getAllCommitsForUsername:(NSString*)username;

/*!
* @method getCommitsForUsername:key:
* @abstract ￼Retrieves all commits for a given user and key that are loaded
* into memory.
* @param username The name of the user.
* @param key ￼The key.
* @result ￼All commits for a given user and key.
*/
- (NSMutableDictionary*) getCommitsForUsername:(NSString*)username key:(NSString*)key;

/*!
* @method getCommitCountForUsername:key:
* @abstract Retreives the amount of commits a user has for a key that
* are currently loaded into memory.
* @param username ￼The name of the user.
* @param key ￼The key.
* @result ￼The amount of commits.
*/
- (int) getCommitCountForUsername:(NSString*)username key:(NSString*)key;

/*!
* @method getErrorCountForUsername:key:
* @abstract ￼Retreives the amount of errors a user has for a key that
* are currently loaded into memory.
* @param username ￼The name of the user.
* @param key ￼The key.
* @result ￼The amont of errors.
*/
- (int) getErrorCountForUsername:(NSString*)username key:(NSString*)key;

@end

/*!
* @protocol SGCommitLogReplayDelegates
* @abstract Deals with replay notifications that are sent out fomr @link SGCommitLog SGCommitLog @/link.
*/
@protocol SGCommitLogReplayDelegate

/*!
* @method commitLog:replay:username:key:
* @abstract ￼When @link replay replay @/link is called, this delegate is notified
* with every commit that is currenlty being replayed. All commits (including errors) are
* used.
* @param commitLog ￼The @link SGCommitLog SGCommitLog @/link that is in replay mode.
* @param data ￼The data contained by the commit.
* @param username ￼The name of the user.
* @param key ￼The key.s
*/
- (void) commitLog:(SGCommitLog*)commitLog replay:(NSData*)data username:(NSString*)username key:(NSString*)key;

@end



