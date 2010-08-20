//
//  SGLocationService.h
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

#import "SGLocationTypes.h"
#import "SGRecordAnnotation.h"
#import "SGAuthorization.h"

@class SGCommitLog, SGHistoryQuery, SGNearbyQuery;

@protocol SGLocationServiceDelegate;

/*!
* @class SGLocationService 
* @abstract ￼Update and retrieve records from SimpleGeo.
* @discussion ￼All methods that create an HTTP request return a response identifier. The response identifier is used
* to identify which response is being returned when either @link locationService:succeededForResponseId:responseObject: locationService:succeededForResponseId:responseObject: @/link
* or @link locationService:failedForResponseId:error: locationService:failedForResponseId:error: @/link is called for all delegates. The class
* allows multiple records to be registered using @link addDelegate: addDelegate: @/link. Delegates will continue recieving notifications
* so it is important to @link removeDelegate: removeDelegate: @/link when that object no longer wants to recieve notifications.
*
* The location service requires that @link HTTPAuthorizer HTTPAuthorizer @/link be set with a valid instance of @link //simplegeo/ooc/cl/SGOAuth SGOAuth @/link. Since
* the only type of authorization supported by SimpleGeo is OAuth, this property must be set or all HTTP requests will result in an error.
*
* All HTTP requests are sent as NSInvocationOperations. Currently, the SimpleGeo API only offers get/add/delete methods for only a single
* record. When using a method like @link updateRecordAnnotations: updateRecordAnnotations: @/link, a single NSInvocationOperation is used
* to update all of the records. It does not create a new operation for each @link //simplegeo/ooc/cl/SGRecordAnnotation SGRecordAnnotation @/link
* that is passed through.
*
* This interface provides two different avenues when it comes to storing and retrieving records. The first is using the standard
* NSString parameratized methods like @link retrieveRecord:layer: retrieveRecord:layer: @/link. Calling this method
* only requires the knowledge of the desired recordId and the layer to which it belongs to. The alternative method would be to use
* @link retrieveRecordAnnotation: retrieveRecordAnnotation: @/link which requires an NSObject that implements 
* @link //simplegeo/ooc/intf/SGRecordAnnotation SGRecordAnnotation @/link. @link //simplegeo/ooc/cl/SGRecord SGRecord @/link is provided
* as a standard class that implements the protocol. The advantage to using the second method is that everytime a response succeeds,
* the delegate can use the responseObject that is passed in via @link locationService:succeededForResponseId:responseObject: locationService:succeededForResponseId:responseObject: @/link
* to update the SGRecord by calling @link //simplegeo/ooc/instm/SGRecord/updateRecordWithGeoJSONObject: updateRecordWithGeoJSONObject: @/link.
*/
@interface SGLocationService : NSObject <CLLocationManagerDelegate> {

    NSOperationQueue* operationQueue;
 
    id<SGAuthorization> HTTPAuthorizer;

#if __IPHONE_4_0 >= __IPHONE_OS_VERSION_MAX_ALLOWED
    
    BOOL useGPS;
    BOOL useWiFiTowers;
    CLLocationManager* locationManager;

#endif

    @private
    NSMutableArray* requestIds;
    NSMutableArray* delegates;
    
#if __IPHONE_4_0 >= __IPHONE_OS_VERSION_MAX_ALLOWED

    UIBackgroundTaskIdentifier backgroundTask;
    SGCommitLog* commitLog;
    NSMutableArray* cachedResponseIds;
    
    NSArray* trackRecords;
    
    CLLocationAccuracy accuracy;
    
#endif

}

/*!
* @property
* @abstract The operation queue that is used to send HTTP requests.
*/
@property (nonatomic, retain) NSOperationQueue* operationQueue;

/*!
* @property
* @abstract The type of authorization to apply to all HTTP requests.
*/
@property (nonatomic, assign) id<SGAuthorization> HTTPAuthorizer;

/*!
* @property
* @abstract Use the GPS unit when tracking records or asking for
* notificaitons in the background.
*/
@property (nonatomic, assign) BOOL useGPS;

/*!
* @property
* @abstract Use WiFi triangulation when tracking records or asking
* for notifications in the background.
*/
@property (nonatomic, assign) BOOL useWiFiTowers;

/*!
* @property
* @abstract The array of records that are updated when @link startTracking startTracking @/link
* is called.
*/
@property (nonatomic, retain) NSArray* trackRecords;

/*!
* @property
* @abstract The CLLocationManager used to generate location updates.
*/
@property (nonatomic, readonly) CLLocationManager* locationManager;

/*!
* @property
* @abstract The accuracy that is specified for the @link locationManager locationManager @/link.
*/
@property (nonatomic, assign) CLLocationAccuracy accuracy;

#if __IPHONE_4_0 >= __IPHONE_OS_VERSION_MAX_ALLOWED

/*!
* @method becameActive
* @abstract Replays the @link //simplegeo/ooc/intf/SGCommitLog SGCommitLog @/link to allow any
* cached updates to take effect service-side.
* @discussion When the UIApplicationDelegate recieves the @link applicationDidBecomeActive: applicationDidBecomeActive: @/link
* callback notification, this method should also be called to allow the location service to cleanup/send whatever data it
* has collected while running in the background. 
*/
- (void) becameActive;

/*!
* @method enterBackground
* @abstract Notifies the location service that it has entered as a background process.
* @discussion When the UIApplicationDelegate recieves the @link applicationDidEnterBackground: applicationDidEnterBackground: @/link
* this method should be called to setup the location service to properly run on in the background. The location service
* first attempts to ask for more time to empty out its NSOperationQueue and then begins listening for
* location updates.
*/
- (void) enterBackground;

/*!
* @method leaveBackground
* @abstract ￼Notifies the location service that it has entered the foreground.
* @discussion ￼When the UIApplicationDelegate recieves the @link applicationWillEnterForeground: applicationWillEnterForeground: @/link
* this method should be to allow the location service to cleanup and commit anything in its managed memory space to disk.
*/
- (void) leaveBackground;

/*!
* @method willBeTerminated
* @abstract Notifies the location service that it will be terminated. ￼
* @discussion ￼When the UIApplicationDelegate recieves the @link applicationWillTerminate: applicationWillTerminate: @/link
* notification, this method should be called to allow the location service to cleanup and commit anything in its managed
* memory to disk.
*/
- (void) willBeTerminated;

/*!
* @method getBackgroundActivityInformation
* @abstract ￼Returns a few metrics that provide information about
* a background processes actions. (e.g duration, number of records cached).
* @result ￼
*/
- (NSDictionary*) getBackgroundActivityInformation;

#endif

/*!
* @method sharedLocationService
* @abstract The shared instance of @link SGLocationService SGLocationService @/link.
* @result The shared instance of @link SGLocationService SGLocationService @/link.
*/
+ (SGLocationService*) sharedLocationService;

/*!
* @method callbackOnMainThread:
* @abstract Determines whether or not to use the main thread to call delegate methods. Default is YES.
* @discussion Since NSOpertaionQueue spawns new threads in order to fulfill its purpose, there are instances
* where it might be benefical to extend the life of that thread (e.g. loading objects into CoreData).
*
* There are only two delegate methods defined by @link SGLocationServiceDelegate SGLocationServiceDelegate @/link.
* If you plan on updating any UIViews, Apple recomends to us the main thread. So be careful. If you plan on updating
* a UITableView within your implementation of the delegate methods, be sure that this value is set to YES.
* @param callback YES to have the delegate methods be called on the main thread; otherwise NO.
*/
+ (void) callbackOnMainThread:(BOOL)callback;

/*!
* @method addDelegate:
* @abstract Adds an instance of @link SGLocationServiceDelegate SGLocationServiceDelegate @/link to be notified
* when a HTTP request fails or succeeds.
* @param delegate The delegate to add.
*/
- (void) addDelegate:(id<SGLocationServiceDelegate>)delegate;

/*!
* @method removeDelegate:
* @abstract Removes the @link SGLocationServiceDelegate SGLocationServiceDelegate @/link from the group
* of delegates.
* @param delegate The delegate to remove.
*/
- (void) removeDelegate:(id<SGLocationServiceDelegate>)delegate;

#pragma mark -
#pragma mark Tracking 

/*!
* @method  startTrackingRecords
* @abstract Starts updating records that are registeredd with @link trackRecords trackRecords @/link.
* Anytime a location update is recieved, the records will be updated and the proper request will
* be pushed out to SimpleGeo.
*/
- (void) startTrackingRecords;

/*!
* @method stopTrackingRecords
* @abstract ￼Stop updated records that are registered with @link trackRecords trackRecords @/link.
*/
- (void) stopTrackingRecords;

#pragma mark -
#pragma mark Record Information 

/*!
* @method retrieveRecordAnnotation:
* @abstract Retrieves information about a @link //simplegeo/ooc/intf/SGRecordAnnotation SGRecordAnnotation @/link from 
* SimpleGeo.
* @discussion See @link retrieveRecord:layer: retrieveRecord:layer: @/link
* @param record The record.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) retrieveRecordAnnotation:(id<SGRecordAnnotation>)record;

/*!
* @method retrieveRecordAnnotations:
* @abstract Retreives information about an array of @link //simplegeo/ooc/intf/SGRecordAnnotation SGRecordAnnotation @/link
* from Simplegeo.
* @discussion See @link retrieveRecord:layer: retrieveRecord:layer: @/link
* @param records ￼
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) retrieveRecordAnnotations:(NSArray*)records;

/*!
* @method retrieveRecord:layer:
* @abstract Retrieves information about a record within a layer.
* @discussion Use this method to retrieve information about a single record that is already stored in SimpleGeo. 
* @param recordId The id of the record.
* @param layer The layer in which the record is located in.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) retrieveRecord:(NSString*)recordId layer:(NSString*)layer;

/*!
* @method updateRecordAnnotation:
* @abstract Updates a record a @link //simplegeo/ooc/intf/SGRecordAnnotation SGRecordAnnotation @/link in
* SimpleGeo.
* @discussion See @link updateRecord:layer:properites: updateRecord:layer:properites: @/link
* @param record The record to update.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) updateRecordAnnotation:(id<SGRecordAnnotation>)record;

/*!
* @method updateRecordAnnotations:
* @abstract Updates an array of @link //simplegeo/ooc/intf/SGRecordAnnotation SGRecordAnnotations @/link in
* SimpleGeo.
* @discussion See @link updateRecord:layer:coord:properties: updateRecord:layer:coord:properties: @/link
* @param records ￼An array of records.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) updateRecordAnnotations:(NSArray*)records;

/*!
* @method updateRecord:layer:properties:
* @abstract Updates a record with the given properties.
* @discussion Use this method to update records in SimpleGeo. If the record id is not found in the layer
* then a new record will be created.
* @param recordId The id of the record.
* @param layer The layer in which the record is located in.
* @param properties The new properties for the record. Every record is required to contain
* a type. The type of the record needs to be declared in the properties dictionary. If no
* type is declared, the value is assumed to be of type "object".
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) updateRecord:(NSString*)recordId layer:(NSString*)layer coord:(CLLocationCoordinate2D)coord properties:(NSDictionary*)properties;

/*!
* @method deleteRecordAnnotation:
* @abstract ￼Deletes a @link //simplegeo/ooc/intf/SGAnnotationRecord SGAnnotationRecord @/link from SimpleGeo.
* @discussion ￼See @link deleteRecord:layer: deleteRecord:layer: @/link
* @param record ￼The record to delete.
* @result ￼A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) deleteRecordAnnotation:(id<SGRecordAnnotation>)record;

/*!
* @method deleteRecordAnnotations:
* @abstract ￼Deletes an array of @link //simplegeo/ooc/intf/SGAnnotationRecord SGAnnotationRecord @/link from SimpleGeo.
* @discussion ￼See @link deleteRecord:layer: deleteRecord:layer: @/link
* @param records ￼The records to delete.
* @result ￼A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) deleteRecordAnnotations:(NSArray*)records;

/*!
* @method deleteRecord:layer:
* @abstract￼Deletes a single record from SimpleGeo.
* @discussion￼Use this method to delete a record from SimpleGeo.
* @param recordId The id of the record to delete.
* @param layer The layer in which the record is located.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) deleteRecord:(NSString*)recordId layer:(NSString*)layer;

/*!
* @method history:
* @abstract￼ Retrieves the history of a single record.
* @discussion Return a reverse chronological list of where a record has been over time.
* Currently only returns the last 10 places a record has been. The response is a 
* GeoJSON GeometryCollection containing a list of Point objects, each with a created field containing the timestamp.
*
* This type of query supports pagination. If a limit is reached server-side but there are still more records that can be 
* returned, the key "next_cursor" will be set as a top-level value in the return object. The key is 
* an encripted string that can be used to return the next set of records in the queried sequence. 
* @param query ￼The @link //simplegeo/ooc/cl/SGHistoryQuery SGHistoryQuery @/link object
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @linkidentify SGLocationServiceDelegate delegate @/link.
*/
- (NSString*) history:(SGHistoryQuery*)query;

#pragma mark -
#pragma mark Layer

/*!
* @method layerInformation:
* @abstract ￼Retrieves information for a given layer.
* @param layerName ￼The layer.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link. 
*/
- (NSString*) layerInformation:(NSString*)layerName;

#pragma mark -
#pragma mark Nearby

/*!
* @method nearby:
* @abstract ￼Retieves records based on the @link //simplegeo/ooc/cl/SGNearbyQuery SGNearbyQuery @/link
* properties that are set.
* @discussion ￼There are two forms of nearby queries: @link //simplegeo/ooc/cl/SGGeohashNearbyQuery SGGeohashNearbyQuery @/link
* and @link //simplegeo/ooc/cl/SGLatLonNearbyQuery SGLatLonNearbyQuery @/link. To see the common filter properties, take a look
* at @link //simplegeo/ooc/cl/SGNearbyQuery SGNearbyQuery @/link.
*
* This type of query supports pagination. If a limit is reached server-side but there are still more records that can be 
* returned, the key "next_cursor" will be set as a top-level value in the return object. The key is 
* an encripted string that can be used to return the next set of records in the queried sequence.
* 
* @param query ￼The @link //simplegeo/ooc/cl/SGNearbyQuery SGNearbyQuery @/link object
* that will be used to produce the proper request.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link. 
*/
- (NSString*) nearby:(SGNearbyQuery*)query;
 
/*!
* @method reverseGeocode:
* @abstract Returns resource information for a given pair lat/lon coordinate.
* @discussion This method does not use MapKit's reverse geocoding methods. SimpleGeo
* offers its own reverse geocoding endpoint. The response object is a GeoJSON dictionary
* where the properties key contains a dictionary with the following keys:
*
*  street_number, country, street, postal_code, county_name, county_code, state_code, place_name
*
* Reverse geocoding is only supported within the US; however, other countries will
* be offered in the coming months.
* @param coord The coordinate to use for reverse geocoding.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link. 
*/
- (NSString*) reverseGeocode:(CLLocationCoordinate2D)coord;

#pragma mark -
#pragma mark SpotRank

/*!
* @method densityForCoordinate:day:hour:
* @abstract Returns a GeoJSON Feature that contains SpotRank data for a specific point.
* @discussion ￼See @link http://www.skyhookwireless.com/spotrank/index.php SpotRank @/link for
* information about the data set. If @link hour hour @/link is not specified, then a collection of
* of tiles, bounding boxes with density data, will be returned for the entire day.
* @param coord ￼The desired location.
* @param day See the defined SpotRank days in @link //simplegeo/ooc/intf/SGLocationTypes SGLocationTypes @/link.
* (e.g. @"mon")￼. Default is nil.s
* @param hour ￼An integer value between 0 and 24. The timezone depends on the location of the coord. Deafault is 12.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link. 
*/
- (NSString*) densityForCoordinate:(CLLocationCoordinate2D)coord day:(NSString*)day hour:(int)hour;

/*!
* @method densityForCoordinate:day:hour:
* @abstract Returns a GeoJSON FeatureCollection that contains SpotRank data for a specific point. 
* @discussion ￼See @link http://www.skyhookwireless.com/spotrank/index.php SpotRank @/link for
* information about the data set. The data returned is a collection of tiles, bounding boxes with density data.
* @param coord ￼The desired location.
* @param day See the defined SpotRank days in @link //simplegeo/ooc/intf/SGLocationTypes SGLocationTypes @/link.
* (e.g. @"mon")￼. Default is nil.s
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link. 
*/
- (NSString*) densityForCoordinate:(CLLocationCoordinate2D)coord day:(NSString*)day;

#pragma mark -
#pragma mark PushPin

/*!
* @method contains:
* @abstract ￼Does a "pushpin" query through a series of polygon layers and identifies the "cone" of
* administrative and other boundaries in which the point lies.
* @discussion ￼Returns a NSArray where the elements contains these key fields: id, name, 
* abbr, type and bounds.
*
* id: A string that uniquely identifies the feature in the SimpleGeo gazetteer. This ID can be used to query the exact shape of the polygon itself via the `boundary` API call. 
* name: A well-known name English-language name for the entity. This may change in the future.
* abbr: If present, an official abbreviation for the entity, e.g. an ISO code, postal code, or similar.
* bounds: A list containing the (west, south, east, north) bounds of the polygon, in that order. 
* type: A string describing the type of feature identified. May or may not be one of:
* Country: A national boundary.
* Province: A state or province.
* County: A county, parish, or similar sub-provincial administrative entity. US only at present.
* City: An incorporated city, town, village, hamlet, etc. US only at present.
* Urban Area: An approximate metropolitan region, from the 1:10M NaturalEarth cultural dataset. May be useful for places outside the US where no incorporated place boundary is (yet) available.
* Neighborhood: A usually approximate boundary for a (usually informal) sub-division of a city. US only at present.
* Postal Code: A postal delivery region. In the US, a ZIP code. US only at present. 
* Legislative District: In the US, a congressional district for the currently sitting US Congress.
* Census Tract: As defined by the US Census Bureau in the most recently published census (2000). 
*
* Other values are possible, so this list will be updated from time to time.
* @param coord ￼The desired coordinate.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link. 
*/
- (NSString*) contains:(CLLocationCoordinate2D)coord;

/*!
* @method boundary:
* @abstract Returns a feature object from the SimpleGeo gazetteer, @link contains: contains: @/link, along with the geometry
* of the feature in GeoJSON format in the geometry field. 
* @param featureId ￼A string that uniquely identifies the feature in the SimpleGeo gazetteer.
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link. 
*/
- (NSString*) boundary:(NSString*)featureId;

/*!
* @method overlapsType:inPolygon:withLimit:
* @abstract Queries a series of polygon layers and identifies the "cone" of administrative and
* other boundaries that overlap with the given bounding box.
* @discussion ￼The results are returned in the same form as the @link contains: contains: @/link.
* @param type If only one feature type is desired, the results can be constrained to that feature type.
* @param envelope ￼The bounds to use when querying the polygon layers.
* @param limit The amount of features to return. Note that this is not a strict count of how many 
* features will be returned, but rather the maximum amount that will be returned. The maximum 
* number of features returned is 1000.  The results are not paginated, so if you need more than
* 1000 results, consider breaking down your bounding box into multiple queries. 
* @result A response id that is used to identify the return value from SimpleGeo. 
* You can use this value in @link SGLocationServiceDelegate delegate @/link. 
*/
- (NSString*) overlapsType:(NSString*)type inPolygon:(SGEnvelope)envelope withLimit:(int)limit;

@end

/*!
* @protocol SGLocationServiceDelegate
* @abstract Recieves notifications when @link SGLocationService SGLocationService @/link returns from 
* sending an HTTP request.
* @discussion This is a simple delegate that allows notifications for when a request to SimpleGeo's API
* succeeds or fails. In the case that a request succeeds a responseObject is returned. The responseObject
* can take on the identity of either NSDictionary or NSArray. 
*
* If the responseObject is of type NSDictionary, then the dictoinary is the GeoJSON representation of the 
* of a record. If the responseObject is of type NSArray, then each element within the array will be
* a dictionary that is aGeoJSON representation of a record. See @link //simplegeo/ooc/cl/SGGeoJSONEncoder SGGeoJSONEncoder @/link
* for a specific guide to the GeoJSON objects that returned from SimpleGeo.
*/
@protocol SGLocationServiceDelegate <NSObject>

/*!
* @method locationService:succeededForResponseId:responseObject:
* @abstract In the case that an HTTP request succeeds, this method is called.
* @discussion It is up to the delegate to decide how to interpret the responseObject. The object can take on two identities, either
* as a NSDictionary or as a NSArray. If the responseObject matches an instance of NSDictionary then the key/values of
* the NSDictionary will match that of the GeoJSON representation of the record that was either created, updated or retrieved.
* 
* If the responseObject matches an instance of NSArray, then every element in the array will be a NSDictionary with its
* key-value pair matching the GeoJSON representaiton of a particular record. 
* @param service￼The @link SGLocationService SGLocationService @/link recieved a successful HTTP response.
* @param requestId￼The response id that was used to create the request.
* @param responseObject The response object.
*/
- (void) locationService:(SGLocationService*)service succeededForResponseId:(NSString*)requestId responseObject:(NSObject*)responseObject;

/*!
* @method locationService:failedForResponseId:error:
* @abstract In the case that an HTTP request fails, this method is called.
* @param service The @link SGLocationService SGLocationService @/link that produced the error.
* @param requestId￼The request id that was used to generate the error.
* @param error The error that was generated.
*/
- (void) locationService:(SGLocationService*)service failedForResponseId:(NSString*)requestId error:(NSError*)error;

#if __IPHONE_4_0 >= __IPHONE_OS_VERSION_MAX_ALLOWED

@optional

/*!
* @method locationService:recordsForBackgroundLocationUpdate:
* @abstract ￼Asks the delegate for all the records for the new location. The location service will not update
* the records with the new location. It assumes that the returned records have already been properly modified.
* @param service ￼The @link SGLocationService SGLocationService @/link
* @param newLocation ￼
* @result ￼
*/
- (NSArray*) locationService:(SGLocationService*)service recordsForBackgroundLocationUpdate:(CLLocation*)newLocation;

/*!
* @method locationService:shouldCacheRecord:
* @abstract After collecting all the @link SGRecordAnnotation SGRecordAnnotations @/link for a new location,
* this callback asks whether or not the subsequent request should be saved for a later time.
* @param service ￼The @link SGLocationService SGLocationService @/link
* @param record
* @result ￼
*/
- (BOOL) locationService:(SGLocationService*)service shouldCacheRecord:(id<SGRecordAnnotation>)record;

#endif

@end