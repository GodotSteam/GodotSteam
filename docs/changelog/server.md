# Server Change-Log

### A history of all changes to the **server** branch.

------

**Version 2.0.1 Changes**

* Changed: layout to make Git cloning easier
* Fixed getSessionConnectionInfo using old networking struct
* Removed: unused networking stricts

**Version 2.0 Changes**

* Changed: separated server back into it's own module / branch
* Changed: brought server branch in line with related master branch functions

**Version 1.2.4 Changes**

* Fixed: lots of compiler warnings on Linux, thanks to gregcsokas

**Version 1.2.3 Changes**

* Added: missing functions to Apps class
* Added: new functions and callbacks to UGC class
* Changed: EnableHeartbeats was renamed to SetAdvertiseServerActive in SDK
* Changed: various internal variable names
* Fixed: some memory allocation issues
* Removed: setHeartbeatInterval and forceHeartbeat; was removed from SDK

**Version 1.2.2 Changes**

* Added: Added: ability to provide different locations for custom modules, thanks to dsnopek

**Version 1.2.1 Changes**

* Fixed: getNumSubscribedItems was mislabeled as getSubscribedItems

**Version 1.2 Changes**

* Added: various notations and spacing
* Added: addRequiredTagGroup, getQueryUGCTag, getQueryUGCTagDisplayName, and getQueryUGCNumTags functions from UGC
* Changed: setCookie to setHTTPCookie
* Fixed: various delete statements
* Fixed: converting user ID in createQueryUserUGCRequest
* Fixed: metadata length in setItemMetadata
* Fixed: library paths in config.py
* Removed: compiling flag in config.py for OSX
* Removed: ticket struct as it causes craches

**Version 1.1.1 Changes**

* Changed: includes Godot header file to allow more than 5 arguments in a function
* Fixed: filterText for Steamworks SDK 1.50

**Version 1.1.0 Changes**

* Added: Apps, HTTP, Inventory, Networking, UGC, and Utils classes
* Added: related callbacks, call results, constants, and enums
* Added: headless server platform for linux
* Fixed: OSX compiling config rules
