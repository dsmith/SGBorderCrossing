//
//  SGLayerMapView.h
//  SGMapKit
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

#import "SGLayer.h"
#import "SGRecord.h"
#import "SGGeoJSONEncoder.h"

/*!
* @class
* @abstract Displays records from a layer on a MKMapView.
* @discussion ￼This is just a simple subclass of MKMapView which uses the records 
* for each registered layer as annotations. SGLayerMapView will automatically
* load more records from SimpleGeo after the view is animated. In addition, the map will attempt to load more
* records based on the @link reloadTimeInterval reloadTimeInterval @/link if no movement is created.
* Just add @link //simplegeo/ooc/cl/SGLayer SGLayer @/link object to the map view. You can disable this feature
* by calling @link stopRetrieving stopRetrieving @/link since every record retrieval requires
* an HTTP request. Also, the map will remove annotation as they move out of view. 
*
* The map view will build records based on the SGLayers 
* @link //simplegeo/ooc/instm/SGLayer/recordAnnotationFromGeoJSONObject: recordAnnotationFromGeoJSONObject: @/link
* method. Records are not automatically added to their layer when they are retrieved. In order to add
* retrieved records to their SGLayer object,
* @link addRetrievedRecrodsToLayer addRetrievedRecrodsToLayer @/link should be set to YES.
*/
@interface SGLayerMapView : MKMapView <MKMapViewDelegate, SGLocationServiceDelegate> {
 
    NSInteger limit;
    
    NSTimeInterval reloadTimeInterval;
    BOOL addRetrievedRecordsToLayer;
    
    double requestStartTime;
    double requestEndTime;
 
    @private
    NSMutableDictionary* sgLayers;
    
    NSMutableArray* presentAnnotations;
    NSMutableArray* newRecordAnnotations;
    
    id<MKMapViewDelegate> trueDelegate;
    
    NSTimer* timer;

    NSMutableArray* layerResponseIds;
    
    NSString* containsResponseId;
    NSArray* validRegionTypes;
    NSMutableArray* boundaryResponseIds;
    NSMutableArray* regions;
}

/*!
* @property
* @abstract The amount of records to retrieve each time the map view
* detects it needs to refresh its annotations.
*/
@property (nonatomic, assign) NSInteger limit;

/*!
* @property
* @abstract The time interval (expressed in seconds) that tells the map to
* retrieve annotations when no movement has occurred.
* @discussion Set this value to a negative interval to not load any records
* when no animations have occurred.
*/
@property (nonatomic, assign) NSTimeInterval reloadTimeInterval;

/*!
* @property
* @abstract Determines whether or not to add retrieved records to
* each SGLayer object as they are loaded by the map.
*/
@property (nonatomic, assign) BOOL addRetrievedRecordsToLayer;

/*!
 * @property
 * @abstract An Epoch timestamp that will be used when obtain nearby records
 * the layers. The default is 0.
 */
@property (nonatomic, assign) double requestStartTime;

/*!
* @property
* @abstract An Epoch timestamp that will be used when obtain nearby records
* the layers. The default is 0.
*/
@property (nonatomic, assign) double requestEndTime;

/*!
* @method startRetrieving
* @abstract ￼ Enables fetching records for each layer from SimpleGeo when the map view's region changes.
*/
- (void) startRetrieving;

/*!
* @method stopRetrieving
* @abstract ￼ Disables fetching records for each layer from SimpleGeo when the map view's region changes.
*/
- (void) stopRetrieving;

/*!
* @method addLayers: 
* @abstract ￼Adds an array of layers to be displayed in the map view.
* @param layers ￼The array of layers.
*/
- (void) addLayers:(NSArray*)layers;

/*!
* @method removeLayers:
* @abstract ￼Removes an array of layers from the map view.
* @param layers ￼The array of layers.
*/
- (void) removeLayers:(NSArray*)layers;

/*!
* @method addLayer:
* @abstract ￼Adds a layer to the map view. 
* @param layer ￼The @link //simplegeo/ooc/cl/SGLayer SGLayer @/link to register
* with the map.
*/
- (void) addLayer:(SGLayer*)layer;

/*!
* @method removeLayer:
* @abstract ￼Removes a layer from the map view.
* @discussion Remvoing a layer will also remove any annotation that are present
* on the map.
* @param layer ￼The @link //simplegeo/ooc/cl/SGLayer SGLayer @/link to remove from the map.
*/
- (void) removeLayer:(SGLayer*)layer;

/*!
* @method retrieveLayers
* @abstract Creates a nearby request for the current viewport and registered layers.
*/
- (void) retrieveLayers;

#if __IPHONE_4_0 >= __IPHONE_OS_VERSION_MAX_ALLOWED

- (void) drawRegionsForLocation:(CLLocation*)location types:(NSArray*)types;

#endif

@end
