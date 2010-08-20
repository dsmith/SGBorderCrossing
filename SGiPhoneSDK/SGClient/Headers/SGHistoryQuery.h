//
//  SGHistoryQuery.h
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
#import "SGQuery.h"
#import "SGRecordAnnotation.h"

/*!
* @class SGHistoryQuery 
* @abstract A query object that defines common properties in
* order to provide all search filters for a history request.
*/
@interface SGHistoryQuery : NSObject <SGQuery> {

    NSString* cursor;
    NSString* recordId;
    NSString* layer;
    
    NSString* requestId;
    int limit;
}

/*!
* @property
* @abstract The cursor that is obtained from a previous nearby query.
* @discussion This property is not required in order to fulfill a
* successful query. It is only used for paginiation.
*/
@property (nonatomic, retain) NSString* cursor;

/*!
* @property
* @abstract The record id to obtain the history from.
*/
@property (nonatomic, retain) NSString* recordId;

/*!
* @property
* @abstract The layer where the record lives.
*/
@property (nonatomic, retain) NSString* layer;

/*!
* @property
* @abstract The amount of historical events to be returned.
* The default is 10.
*/
@property (nonatomic, assign) int limit;

/*!
 * @property
 * @abstract The request identifier that was used to iniate
 * this request.
 * @discussion This value is set when the nearby request
 * is sent through the @link //simplegeo/ooc/cl/SGLocationService SGLocationService @/link.
 */
@property (nonatomic, retain) NSString* requestId;

/*!
* @method initWithRecord:
* @abstract ￼Initializes a history query from a record.
* @param record ￼
* @result ￼
*/
- (id) initWithRecord:(id<SGRecordAnnotation>)record;

@end
