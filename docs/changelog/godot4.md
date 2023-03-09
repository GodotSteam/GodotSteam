# Godot 4.x Change-Log

### A history of all changes to the **godot4** branch.

**Note:** when Godot 4.x goes stable, this branch will become the master branch and current master branch will become godot3.

------

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