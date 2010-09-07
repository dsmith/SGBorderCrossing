//
//  SGMainViewController.m
//  SGBorderCrossing
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

#import "SGMainViewController.h"

static BOOL updated = NO;

@interface SGMainViewController (Private) <MKMapViewDelegate>

- (UIColor*) randomColor;

@end

@implementation SGMainViewController
@synthesize recordOverlay;

- (id) initWithLayer:(NSString*)layerName
{
    if(self = [super init]) {        
        SGLocationService* locationService = [SGLocationService sharedLocationService];
        [locationService addDelegate:self];        

        SGRecord* record = [[SGRecord alloc] init];
        record.recordId = @"border-crossing-record-3";
        record.layer = layerName;
        
        recordOverlay = [[SGRecordLine alloc] initWithRecordAnnoation:record];
        polylineView = nil;

        historyQuery = [[SGHistoryQuery alloc] initWithRecord:record];
        historyQuery.limit = 100;
    }
    
    return self;
}

- (void) loadView
{
    [super loadView];
    
    self.title = @"SGBorderCrossing";
    
    mapView = [[SGLayerMapView alloc] initWithFrame:self.view.bounds];
    mapView.showsUserLocation = YES;
    mapView.delegate = self;
    [self.view addSubview:mapView];
    
    UIButton* locateMeButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [locateMeButton addTarget:self action:@selector(locateMe:) forControlEvents:UIControlEventTouchUpInside];
    UIImage* locateMeImage = [UIImage imageNamed:@"LocateMe.png"];
    locateMeButton.frame = CGRectMake(0.0, 0.0, locateMeImage.size.width, locateMeImage.size.height);
    [locateMeButton setImage:locateMeImage forState:UIControlStateNormal];
    UIBarButtonItem* locateMeBarButton = [[UIBarButtonItem alloc] initWithCustomView:locateMeButton];
    [self setToolbarItems:[NSArray arrayWithObject:locateMeBarButton] animated:NO];
    [self.navigationController setToolbarHidden:NO animated:NO];
    [locateMeBarButton release];
    
    UIBarButtonItem* refreshButton = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh 
                                                                                   target:self
                                                                                   action:@selector(refresh:)];
    self.navigationItem.rightBarButtonItem = refreshButton;
    [refreshButton release];
}

////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark UIButton actions 
//////////////////////////////////////////////////////////////////////////////////////////////// 

- (void) refresh:(id)button
{
    SGLocationService* locationService = [SGLocationService sharedLocationService];
    [locationService deleteRecordAnnotation:recordOverlay.recordAnnotation];
    
    [mapView removeOverlays:[NSArray arrayWithArray:mapView.overlays]];
    
    CLLocation* location = mapView.userLocation.location;
    [mapView drawRegionsForLocation:mapView.userLocation.location types:nil];
    SGRecord* record = recordOverlay.recordAnnotation;
    record.history = nil;

    record.latitude = location.coordinate.latitude;
    record.longitude = location.coordinate.longitude;
    [locationService updateRecordAnnotation:record];

    recordOverlay = [[SGRecordLine alloc] initWithRecordAnnoation:record];
    [mapView addOverlay:recordOverlay];
    
    [[SGLocationService sharedLocationService] stopTrackingRecords];
    [[SGLocationService sharedLocationService] startTrackingRecords];
}

- (void) locateMe:(id)button
{
    CLLocation* userLocation = mapView.userLocation.location;
    MKCoordinateSpan span = MKCoordinateSpanMake(0.0001, 0.0001);
    [mapView setRegion:MKCoordinateRegionMake(userLocation.coordinate, span) animated:YES]; 
}

////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark SGLocationService delegate methods 
//////////////////////////////////////////////////////////////////////////////////////////////// 
     
- (void) locationService:(SGLocationService*)service succeededForResponseId:(NSString*)requestId responseObject:(NSObject*)responseObject
{
    if([requestId isEqualToString:historyQuery.requestId]) {
        if(historyQuery.cursor)
            [[SGLocationService sharedLocationService] history:historyQuery];
        
        [recordOverlay.recordAnnotation updateHistory:(NSDictionary*)responseObject];
        [recordOverlay reloadAnnotation];
        [mapView addOverlay:recordOverlay];
    }
}
     
- (void) locationService:(SGLocationService*)service failedForResponseId:(NSString*)requestId error:(NSError*)error
{
    ;
}

////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark MKMapView delegate methods 
//////////////////////////////////////////////////////////////////////////////////////////////// 

- (MKOverlayView*) mapView:(MKMapView*)mv viewForOverlay:(id<MKOverlay>)overlay
{
    MKOverlayView* overlayView = nil;
    if([overlay isKindOfClass:[MKPolygon class]]) {
        MKPolygonView* polygonView = [[MKPolygonView alloc] initWithOverlay:overlay];
        polygonView.fillColor = [self randomColor];
        overlayView = polygonView;
    }
    
    if([overlay isKindOfClass:[SGRecordLine class]]) {
        polylineView = [[SGDynamicPolylineView alloc] initWithOverlay:overlay];
        polylineView.fillColor = [UIColor redColor];
        polylineView.strokeColor = polylineView.fillColor;
        overlayView = polylineView;
    }    
        
    return overlayView;
}

- (void) mapView:(MKMapView*)mv didUpdateUserLocation:(MKUserLocation*)userLocation
{    
    if(!updated) {
        [mapView drawRegionsForLocation:userLocation.location types:nil];
        
        SGLocationService* locationService = [SGLocationService sharedLocationService];
        [locationService history:historyQuery];
        [locationService startTrackingRecords];          
        
        SGRecord* record = recordOverlay.recordAnnotation;
        record.latitude = userLocation.coordinate.latitude;
        record.longitude = userLocation.coordinate.longitude;
        
        updated = YES;
    } else
        [((SGRecord*)recordOverlay.recordAnnotation) updateCoordinate:userLocation.coordinate];
    
    [recordOverlay reloadAnnotation];
    if(polylineView)
        [polylineView setNeedsDisplay];
    
}

////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Utility methods 
//////////////////////////////////////////////////////////////////////////////////////////////// 

- (UIColor*) randomColor
{
    float alpha = 0.16;
    int i = (rand() % 10);
    UIColor* color = nil;
    switch (i) {
        case 0:
            color = [UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:alpha];
            break;
        case 1:
            color = [UIColor colorWithRed:0.0 green:0.0 blue:0.0 alpha:alpha];
            break;
        case 2:
            color = [UIColor colorWithRed:1.0 green:0.0 blue:0.0 alpha:alpha];
            break;
        case 3:
            color = [UIColor colorWithRed:1.0 green:1.0 blue:0.0 alpha:alpha];
            break;
        case 4:
            color = [UIColor colorWithRed:0.0 green:1.0 blue:0.0 alpha:alpha];
            break;
        case 5:
            color = [UIColor colorWithRed:0.0 green:1.0 blue:1.0 alpha:alpha];
            break;
        case 6:
            color = [UIColor colorWithRed:1.0 green:0.0 blue:1.0 alpha:alpha];
            break;
        case 7:
            color = [UIColor colorWithRed:1.0 green:0.5 blue:1.0 alpha:alpha];
            break;
        case 8:
            color = [UIColor colorWithRed:0.7 green:1.0 blue:1.0 alpha:alpha];
            break;
        case 9:
            color = [UIColor colorWithRed:0.1 green:0.5 blue:1.0 alpha:alpha];
            break;
            
        default:
            break;
    }
    
    return color;
}

- (void) dealloc
{
    [mapView release];
    [recordOverlay release];
    [super dealloc];
}

@end
