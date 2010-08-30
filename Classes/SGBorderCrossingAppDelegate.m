//
//  SGBorderCrossingAppDelegate.m
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

#import "SGBorderCrossingAppDelegate.h"

#import "SGMainViewController.h"

@implementation SGBorderCrossingAppDelegate
@synthesize window;

////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Application lifecycle 
//////////////////////////////////////////////////////////////////////////////////////////////// 

- (BOOL) application:(UIApplication*)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions 
{    
    NSString* path = [[NSBundle mainBundle] pathForResource:@"Token" ofType:@"plist"];
    NSDictionary* token = [NSDictionary dictionaryWithContentsOfFile:path];
    
    NSString* key = [token objectForKey:@"key"];
    NSString* secret = [token objectForKey:@"secret"];
    NSString* layer = [token objectForKey:@"layer"];
    
    if([key isEqualToString:@"my-secret"] || [secret isEqualToString:@"my-secret"] || [layer isEqualToString:@"my-layer"]) {
        NSLog(@"ERROR!!! - Please change the credentials in Resources/Token.plist");
        exit(1);
    }   
    
    SGOAuth* oauthToken = [[SGOAuth alloc] initWithKey:key secret:secret];
    SGLocationService* locationService = [SGLocationService sharedLocationService];
    locationService.HTTPAuthorizer = oauthToken;

    SGMainViewController* mainViewController = [[SGMainViewController alloc] initWithLayer:layer];
    locationService.trackRecords = [NSArray arrayWithObject:mainViewController.recordOverlay.recordAnnotation];
    [locationService startTrackingRecords];
    
    locationManager = [[SGLocationManager alloc] init];
    locationManager.delegate = self;
    
    UINavigationController* navigationController = [[UINavigationController alloc] initWithRootViewController:mainViewController];
    [window addSubview:navigationController.view];
    [window makeKeyAndVisible];
	
	return YES;
}

- (void) applicationDidEnterBackground:(UIApplication *)application 
{
    [[SGLocationService sharedLocationService] enterBackground];
    [locationManager startMonitoringSignificantLocationChanges];
}

- (void) applicationWillEnterForeground:(UIApplication*)application 
{
    [[SGLocationService sharedLocationService] leaveBackground];
    [locationManager stopMonitoringSignificantLocationChanges];
}

- (void) applicationDidBecomeActive:(UIApplication*)application
{
    [[SGLocationService sharedLocationService] becameActive];
}

- (void) applicationWillTerminate:(UIApplication*)application 
{
    [[SGLocationService sharedLocationService] willBeTerminated];    
}

- (void) application:(UIApplication*)application didReceiveLocalNotification:(UILocalNotification*)notification
{
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:notification.alertBody
                                                    message:nil
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
}

////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark SGLocationManager delegate methods 
//////////////////////////////////////////////////////////////////////////////////////////////// 

- (void) locationManager:(SGLocationManager*)locationManager didEnterRegions:(NSArray*)regions
{
    UILocalNotification* localNotification = [[UILocalNotification alloc] init];
    localNotification.alertBody = [NSString stringWithFormat:@"Entered %i new regions", [regions count]];
    localNotification.fireDate = nil;
    [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
    [localNotification release];
}

- (void) locationManager:(SGLocationManager*)locationManager didLeaveRegions:(NSArray*)regions
{
    UILocalNotification* localNotification = [[UILocalNotification alloc] init];
    localNotification.alertBody = [NSString stringWithFormat:@"Exited %i old regions", [regions count]];
    localNotification.fireDate = nil;
    [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
    [localNotification release];    
}

- (void) dealloc
{
    [window release];
    [locationManager release];
    [super dealloc];
}

@end
