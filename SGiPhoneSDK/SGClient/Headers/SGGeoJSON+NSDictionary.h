//
//  SGGeoJSON+NSDictionary.h
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

#import "GeoJSON+NSDictionary.h"

/*!
* @class NSDictionary (SGGeoJSONObject) 
* @abstract A category used to access SimpleGeo
* values easier within a GeoJSON dictionary.
*/
@interface NSDictionary (SGGeoJSONObject) <SGRecordAnnotation>

/*!
* @method created
* @abstract ￼Returns the value associated with the key "created".
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @result ￼The created value.
*/
- (double) created;

/*!
* @method expires
* @abstract Returns the value associated with the key "expires"
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @result ￼The expires value.
*/
- (double) expires;

/*!
* @method recordId
* @abstract Returns the value associated ￼with the key "id".
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @result ￼The id value.
*/
- (NSString*) recordId;

/*!
* @method layerLink
* @abstract Use this URL string to obtain information about the layer
* this GeoJSON object resides in.
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @result ￼The URL string of this object's layer.
*/
- (NSString*) layerLink;

/*!
* @method selfLink
* @abstract ￼The URL used to retrieve information about this GeoJSON
* object.
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object. 
* @result A URL string. ￼
*/
- (NSString*) selfLink;

/*!
* @method layer
* @abstract Returns the value associated ￼with the key "layer".
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @result ￼The layer value.
*/
- (NSString*) layer;

/*!
* @method coordinates
* @abstract Returns the GeoJSONObject associated with the key "coordinates" within
* the geometry object. ￼
* @result ￼The coordinates GeoJSONObject.
*/
- (CLLocationCoordinate2D) coordinate;

@end

@interface NSMutableDictionary (SGGeoJSONObject)

/*!
* @method setCreated:
* @abstract Sets the value stored at key created.
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @param created The value to store.
*/
- (void) setCreated:(double)created;

/*!
* @method setRecordId:
* @abstract Sets the value stored at key id.
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @param id ￼The value to store.
*/
- (void) setRecordId:(NSString*)recordId;

/*!
* @method setLayer:
* @abstract Sets the value stored at key layer.
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @param id ￼The value to store.
*/
- (void) setLayer:(NSString*)layer;

/*!
* @method setExpires:
* @abstract Sets the value stored at key expires.
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @param expires ￼The value to store.
*/
- (void) setExpires:(double)expires;

@end