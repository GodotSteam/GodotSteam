# Tutorials - Getting a list of lobbies friends are in

In this quick tutorial we will cover how to get a dictionary of lobbies to friends that are visible to the user. This was created as it's a pretty common feature for multiplayer games.

We will break down the different steam functions to call first before looking at the code samples.

---

## Functions explained

```gdscript
var number_of_friends: int = Steam.getFriendCount(Steam.FRIEND_FLAG_IMMEDIATE)
```
This must be ran before `Steam.getFriendByIndex` according to the Steamworks documentation, we'll be using this for our loop over each friend in our friends list.

```gdscript
var steam_id: int = Steam.getFriendByIndex(index, Steam.FRIEND_FLAG_IMMEDIATE)
```
Inside of the loop we'll call `Steam.getFriendByIndex` to get the Steam ID of our friend at this index.

Note: you must use the same flag as the flag used inside of `Steam.getFriendCount`.

```gdscript
var game_info: Dictionary = Steam.getFriendGamePlayed(steam_id)
```
With our friends Steam ID we can now get information about the game our friend may or may not be playing.

This dictionary will be empty if the friend is offline or not playing a game.

Here's the fields and possible values inside this dictionary:

- **id**: int
  - The app id of the game they are playing.
- **lobby**: int ***or*** String
  - If the friend is in a lobby visible to this user, it will be an int of the lobby ID
  - Otherwise, it will be a String equal to: `"No valid lobby"`
- **ip**: String
- **game_port**: int
- **query_port**: int
  - Depends on the lobby settings, but `ip`, `game_port`, and `query_port` will probably all just be zeros.

---

## Putting it all together

```gdscript
# get a dictionary of lobbies with friends
# key-value pair: lobby_id -> Array<steam_id>
func get_lobbies_with_friends() -> Dictionary:
  var results = {}
  var num_of_friends: int = Steam.getFriendCount()
  for i in range(0, num_of_friends):
    var steam_id: int = Steam.getFriendByIndex(i, Steam.FRIEND_FLAG_IMMEDIATE)
    var game_info: Dictionary = Steam.getFriendGamePlayed(steam_id)
    if game_info.has("lobby"):
      var app_id: int = game_info.id
      var lobby = game_info.lobby
      if app_id != Steam.getAppID() or lobby is String:
        continue
      if not results.has(lobby):
        results[lobby] = []
      results[lobby].append(steam_id)
  return results
```

If you would rather get back a dictionary of `friend_id -> lobby_id` you can replace the three lines after `continue` with `results[steam_id] = lobby`.

It's important to not run this every frame, but it is important to refresh periodically. (Probably, I'm not sure how expensive this is.)

From here you can create your UI however you like, and simply call `Steam.joinLobby(lobby_id)` when the user has made a selection.

---

## Checking if a friend is still in a lobby

In the case that you are not running `get_lobbies_with_friends()` every frame, there's a small chance a user might click on a lobby a friend has since left, or worse, a lobby that no longer exists.

This is a little check you can do before joining the lobby:

```gdscript
# check if a friend is in a lobby
func is_a_friend_still_in_lobby(steam_id: int, lobby_id: int) -> bool:
  var game_info: Dictionary = Steam.getFriendGamePlayed(steam_id)
  if not game_info.has("lobby"):
    return false
  var app_id: int = game_info.id
  var lobby = game_info.lobby
  if app_id != Steam.getAppID() or lobby is String or lobby != lobby_id:
    return false
  return true
```

---

## Troubleshooting

If you don't see any lobbies while testing, make sure to check the flags for how the lobby was created.

For instance, you won't be able to see a lobby with this flag:
```gdscript
SteamLobby.create_lobby(Steam.LOBBY_TYPE_INVISIBLE, 8)
```

If the lobby is configured in such a way that's it's invisible to you, or at max capacity, steam won't provide a lobby ID.
