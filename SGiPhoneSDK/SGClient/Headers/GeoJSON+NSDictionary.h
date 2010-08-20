//
//  SGGeoJSONNSDictionary.h
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
#import "SGRecordAnnotation.h"

/*!
* @category NSDictionary(GeoJSONObject)
* @abstract Ease the pain of accessing SimpleGeo GeoJSON values.
*/
@interface NSDictionary (GeoJSONObject)

/*!
* @method type
* @abstract Retrieves the type value for this GeoJSONObject. ￼
* @result The type value for the object.
*/
- (NSString*) type;

/*!
* @method geometry
* @abstract Returns the GeoJSONObject associated with the key at geometry. ￼
* @result ￼The geometry GeoJSONObject.
*/
- (NSDictionary*) geometry;

/*!
* @method coordinates
* @abstract Returns the GeoJSONObject associated with the key coordinates. ￼
* @result ￼The coordinates GeoJSONObject.
*/
- (NSArray*) coordinates;

/*!
* @method properties
* @abstract Returns the GeoJSONObject associated with the key properties.
* @result ￼The properties GeoJSONObject.
*/
- (NSDictionary*) properties;

/*!
* @method features
* @abstract Returns the GeoJSONObject associated with the key features.
* @result ￼The features GeoJSONObject.
*/
- (NSArray*) features;

/*!
 * @method geometries
 * @abstract Returns the GeoJSONObject associated with the key geometries.
 * @result ￼The geometries GeoJSONObject.
 */
- (NSArray*) geometries;

/*!
* @method isFeature
* @abstract Determines whether this GeoJSON object is a feature. 
* @result YES if the object is a feature; otherwise NO.
*/
- (BOOL) isFeature;

/*!
* @method isFeatureCollection
* @abstract Determines whether this GeoJSON object is a collection
* of features.￼
* @result ￼YES if the object is a collection of features; otherwise
* NO.
*/
- (BOOL) isFeatureCollection;

/*!
* @method Determines whether this GeoJSON object is a geometry collection.
* @result YES if the object is a collection of geometries; otherwise NO.
*/
- (BOOL) isGeometryCollection;

/*!
* @method isPoint
* @abstract Determines whether this GeoJSON object is a point. ￼
* @result ￼YES if the object is a point; otherwise NO.
*/
- (BOOL) isPoint;

/*!
* @method isPolygon
* @abstract Determines whether this GeoJSON object is a polygon. ￼
* @result ￼YES if the object is a polygon; otherwise NO.
*/
- (BOOL) isPolygon;

/*!
* @method isMultiPolygon
* @abstract Determines whether this GeoJSON object is a multipolygon. ￼
* @result ￼YES if the object is a multipolygon; otherwise NO.
*/
- (BOOL) isMultiPolygon;

@end

/*!
* @category NSMutableDictionary(GeoJSONObject)
* @abstract Ease the pain of storing SimpleGeo GeoJSON values.
*/
@interface NSMutableDictionary (GeoJSONObject)

/*!
* @method setType:
* @abstract Sets the value stored at key type.
* @param type ￼The value to store.
*/
- (void) setType:(NSString*)type;

/*!
* @method setGeometry:
* @abstract Sets the GeoJSONObject stored at key geometry.
* @param geometry ￼The GeoJSONObject to store.
*/
- (void) setGeometry:(NSDictionary*)geometry;

/*!
* @method setCoordinates:
* @abstract Sets the GeoJSONObject stored at key coordinates.
* @param coordinates ￼The GeoJSONObject to store.
*/
- (void) setCoordinates:(NSArray*)coordinates;

/*!
* @method setProperties:
* @abstract Sets the GeoJSONObject stored at key properties.
* @param properties The GeoJSONObject to store.
*/
- (void) setProperties:(NSDictionary*)properties;

/*!
* @method setFeatures:
* @abstract Sets the GeoJSONObject stored at key features.
* @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
* allows key/values to be defined at the top-level of the object.
* @param features ￼The value to store.
*/
- (void) setFeatures:(NSArray*)features;

/*!
 * @method setGeometries:
 * @abstract Sets the GeoJSONObject stored at key geometries.
 * @discussion This is a value specific to SimpleGeo. The GeoJSON spec (http://geojson.org/geojson-spec.html)
 * allows key/values to be defined at the top-level of the object.
 * @param geometries ￼The value to store.
 */
- (void) setGeometries:(NSArray*)geometries;

/*!
* @method addGeometry:
* @abstract ￼Appends a new geometry object to the geometry collection.
* @param geometry ￼
*/
- (void) addGeometry:(NSDictionary*)geometry;

@end

