# Godot 4.x Change-Log

### A history of all changes to the **godot4** branch.

---

**Version 4.3.1 Changes**

- Fixed: wrong variant type for join_requested

**Version 4.3 Changes**

- Added: full GodotSteam documentation into the editor
- Added: steamShutdown to allow Steamworks to be manually shutdown
- Added: requestEquippedProfileItems function and equipped_profile_items callback
- Added: Steam Deck as Steam Input type
- Changed: all enums are now directly linked to their SDK counterparts
- Changed: getDigitalActionData returned keys are now state and active
- Changed: names of some Steam enums to be cleaner and leaner
- Changed: getAppInstallDir now returns dictionary with absolute path and install size
- Fixed: some missing enum binds
- Fixed: missing function argument binds
- Fixed: get_ticket_for_web_api sending back strings
- Removed: enums that are not in the SDK but Valve's docs

**Version 4.2.2 Changes**

- Added: new Input callback _input_gamepad_slot_change_
- Added: new User callback _get_ticket_for_web_api_
- Added: new User function _getAuthTicketForWebApi_
- Changed: getAuthSessionTicket argument is now optional, defaults to NULL

**Version 4.2.1 Changes**

- Added: new return values for _overlay_toggled_; this will break compatibility with this
- Added: new Input and Parental Settings enums
- Added: new UGC Content Descriptor ID enums
- Added: new UGC functions _removeContentDescriptor_, _addContentDescriptor_, and _getQueryUGCContentDescriptors_
- Added: new signal _filter_text_dictionary_changed_
- Changed: getAuthSessionTicket now uses networking identities
- Changed: gamepad_text_input_dismissed now passes back the app ID
- Changed: Steam Input max analog and digital actions values
- Removed: ERegisterActivationCodeResult due to removal in SDK

**Version 4.2 Changes**

* Added: various backports from Godot 3.x branch
* Fixed: options array size for new Networking classes and memory leaks, _thanks to profour_
* Fixed: need for godotsteam.sh file on some Linux systems, _thanks to mikix_

**Version 4.1.5 Changes**

* Added: networking type message constants
* Added: more descriptions and tutorial links to in-editor docs
* Added: avatar_image_loaded callback to get raw response from Steamworks
* Changed: brought 4.x branch up-to-speed with master / 3.x branch
* Changed: enums now bound with BIND_ENUM_CONSTANT, _thanks to raulsntos_
* Changed: bitwise enums now bound with BIND_ENUM_BITWISE_CONSTANT, _thanks to raulsntos_
* Changed: platform of 'osx' to new 'macos'
* Fixed: platform of 'osx' not being recognized so the module doesn't get added
* Fixed: microtransaction_auth_response spelling for callback
* Fixed: getLobbyData not returning UTF-8 encoded string
* Fixed: sendLobbyChatMsg truncating non-English strings
* Fixed: filterText truncating input; thanks to _tcoxon_
* Removed: MarketingMessageFlags as they don't exist in the header files

**Version 4.1.4 Changes**

* Changed: layout to make Git cloning easier
* Changed: submitItemUpdate to use null if no notes are passed, thanks to mashumafi
* Fixed: incorrect use of io failure
* Fixed: DEFVAL type mismatch, thanks to raulsntos
* Fixed: getSessionConnectionInfo using old networking struct
* Removed: unused networking stricts

**Version 4.1.3 Changes**

* Fixed: config.py and SCsub so that Linux compiling will link Steam correctly

**Version 4.1.2 Changes**

* Changed: brought branch up to speed with GodotSteam 3.17.4

**Version 4.1.1 Changes**

* Fixed: wrong arch in SCsub file
* Fixed: registering class in register_types.cpp

**Version 4.1 Changes**

* Changed: brought branch up to speed with GodotSteam 3.17.1
* Fixed: various small issues to get it running with Godot alpha 1

**Version 4.0 Changes**

* Added: Initial build, highly experimental!