# Tutorials - Workshop

A hot topic that comes up: Workshop / UGC. There are a lot of moving parts and we'll probably miss quite a few in this tutorial.  Luckily some smart folks have provided some information based on their experiences that we can use.

---

## Before You Start

Before anything else, you'll want to read [Valve's write-up on Workshop / UGC which will cover a lot of steps that aren't covered in this tutorial.](https://partner.steamgames.com/doc/features/workshop) Once you get through that, you should also read through [Valve's write-up on the implementation of Workshop / UGC so you'll be ready to continue on.](https://partner.steamgames.com/doc/features/workshop/implementation)

And, when you're finally read both of those, we can start.

---

## Uploading / Downloading In Workshop / UGC

[KarpPaul of Forgotten Dream Games has given us this pretty great tutorial on uploading and downloading items in Workshop / UGC.](https://www.forgottendreamgames.com/post/godotsteam-how-to-upload-and-download-user-generated-content-ugc) Since he has written everything, with code examples, there isn't any reason to reiterate it here when you can just click the link and read it all yourself.

## Using Items In Workshop / UGC

Lyaaaaaaaaaaaaaaa submitted some code showing how they use items in Workshop / UGC:

```
extends Node

class_name Steam_Workshop

signal query_request_success

var published_items  : Array
var steam = SteamAutoload.steam    # I don't use the `Steam` directly to avoid scripts errors in non-steam builds.

var _app_id        : int =  ProjectSettings.get_setting("global/steam_app_id")
var _query_handler : int
var _page_number   : int = 1
var _subscribed_items : Dictionary

func _init() -> void:
    steam.connect("ugc_query_completed", self, "_on_query_completed")

    for item in steam.getSubscribedItems():
        var info : Dictionary
        info = get_item_install_info(item)
        if info["ret"] == true:
            _subscribed_items[item] = info


static func open_tos() -> void:
    var steam = SteamAutoload.steam
    var tos_url = "https://steamcommunity.com/sharedfiles/workshoplegalagreement"
    steam.activateGameOverlayToWebPage(tos_url)


func get_item_install_info(p_item_id : int) -> Dictionary:
    var info : Dictionary
    info = steam.getItemInstallInfo(p_item_id)

    if info["ret"] == false:
        var warning = "Item " + String(p_item_id) + " isn't installed or has no content"
        # Here your code to log/display errors

    return info


func get_published_items(p_page : int = 1, p_only_ids : bool = false) -> void:
    var user_id : int = steam.getSteamID()
    var list    : int = steam.USER_UGC_LIST_PUBLISHED
    var type    : int = steam.WORKSHOP_FILE_TYPE_COMMUNITY
    var sort    : int = steam.USERUGCLISTSORTORDER_CREATIONORDERDESC

    _query_handler = steam.createQueryUserUGCRequest(user_id,
                                                     list,
                                                     type,
                                                     sort,
                                                     _app_id,
                                                     _app_id,
                                                     p_page)
    steam.setReturnOnlyIDs(_query_handler, p_only_ids)
    steam.sendQueryUGCRequest(_query_handler)


func get_item_folder(p_item_id : int) -> String:
    return _subscribed_items[p_item_id]["folder"]


func fetch_query_result(p_number_results : int) -> void:
    var result : Dictionary
    for i in range(p_number_results):
        result = steam.getQueryUGCResult(_query_handler, i)
        published_items.append(result)

    steam.releaseQueryUGCRequest(_query_handler)


func _on_query_completed(p_query_handler    : int,
                         p_result           : int,
                         p_results_returned : int,
                         p_total_matching   : int,
                         p_cached           : bool) -> void:

    if p_result == steam.RESULT_OK:
        fetch_query_result(p_results_returned)
    else:
        var warning = "Couldn't get published items. Error: " + String(p_result)
        # Here your code to log/display errors

    if p_result == 50:
        _page_number ++ 1
        get_published_items(_page_number)

    elif p_result < 50:
        emit_signal("query_request_success",
                    p_results_returned,
                    _page_number)
```

## Workshop / UGC Items

Following along with Lyaaaaaaaaaaaaaaa's example, here is a class for your Workshop / UGC items.

```
extends Node

class_name UGC_Item

signal item_created
signal item_updated
signal item_creation_failed
signal item_update_failed

var steam = SteamAutoload.steam # I don't use the `Steam` directly to avoid scripts errors in non-steam builds.

var _app_id  : int = ProjectSettings.get_setting("global/steam_app_id")
var _item_id : int
var _update_handler

func _init(p_item_id   : int = 0,
           p_file_type : int = steam.WORKSHOP_FILE_TYPE_COMMUNITY) -> void:

    steam.connect("item_created", self, "_on_item_created")
    steam.connect("item_updated", self, "_on_item_updated")

    if p_item_id == 0:
        steam.createItem(_app_id, p_file_type)
    else:
        _item_id = p_item_id
        start_update(p_item_id)


func start_update(p_item_id : int) -> void:
    _update_handler = steam.startItemUpdate(_app_id, p_item_id)


func update(p_update_description : String = "Initial commit") -> void:
    steam.submitItemUpdate(_update_handler, p_update_description)


func set_title(p_title : String) -> void:
    if steam.setItemTitle(_update_handler, p_title) == false:
        # Here your code to log/display errors


func set_description(p_description : String = "") -> void:
    if steam.setItemDescription(_update_handler, p_description) == false:
        # Here your code to log/display errors


func set_update_language(p_language : String) -> void:
    if steam.setItemUpdateLanguage(_update_handler, p_language) == false:
        # Here your code to log/display errors


func set_visibility(p_visibility : int = 2) -> void:
    if steam.setItemVisibility(_update_handler, p_visibility) == false:
        # Here your code to log/display errors


func set_tags(p_tags : Array = []) -> void:
    if steam.setItemTags(_update_handler, p_tags) == false:
        # Here your code to log/display errors


func set_content(p_content : String) -> void:
    if steam.setItemContent(_update_handler, p_content) == false:
        # Here your code to log/display errors


func set_preview(p_image_preview : String = "") -> void:
    if steam.setItemPreview(_update_handler, p_image_preview) == false:
        # Here your code to log/display errors


func set_metadata(p_metadata : String = "") -> void:
    if steam.setItemMetadata(_update_handler, p_metadata) == false:
        # Here your code to log/display errors


func get_id() -> int:
    return _item_id


func _on_item_created(p_result : int, p_file_id : int, p_accept_tos : bool) -> void:
    if p_result == steam.RESULT_OK:
        _item_id = p_file_id
        # Here your code to log/display success
        emit_signal("item_created", p_file_id)
    else:
        var error = "Failed creating workshop item. Error: " + String(p_result)
        # Here your code to log/display errors
        emit_signal("item_creation_failed", error)

    if p_accept_tos:
        Steam_Workshop.open_tos()


func _on_item_updated(p_result : int, p_accept_tos : bool) -> void:
    if p_result == steam.RESULT_OK:
        var item_url = "steam://url/CommunityFilePage/" + String(_item_id)
        # Here your code to log/display success
        steam.activateGameOverlayToWebPage(item_url)
        emit_signal("item_updated")
    else:
        var error = "Failed updated workshop item. Error: " + String(p_result)
        # Here your code to log/display errors
        emit_signal("item_update_failed", error)

    if p_accept_tos:
        Steam_Workshop.open_tos()
```
